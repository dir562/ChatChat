#include "stdafx.h"


constexpr short SERVER_PORT{ 8888 };
constexpr size_t BUFSIZE{ 256 };


void Disconnect(int);

enum COMP_OP { OP_RECV = 1, OP_SEND, OP_ACCEPT };
class EXP_OVER {
public:
	WSAOVERLAPPED	_wsa_over;
	COMP_OP			_comp_op;
	WSABUF			_wsa_buf;
	unsigned char	_net_buf[BUFSIZE];
public:
	EXP_OVER(COMP_OP comp_op, char num_bytes, void* mess) : _comp_op(comp_op)
	{
		ZeroMemory(&_wsa_over, sizeof(_wsa_over));
		_wsa_buf.buf = reinterpret_cast<char*>(_net_buf);
		_wsa_buf.len = num_bytes;
		memcpy(_net_buf, mess, num_bytes);
	}

	EXP_OVER(COMP_OP comp_op) : _comp_op(comp_op) {}

	EXP_OVER()
	{
		EXP_OVER(OP_RECV);
	}

	~EXP_OVER()
	{
	}
};

enum class STATE { ST_FREEE, ST_ACCEPT, ST_INGAME };
class CLIENT {
private:
public:
	/* data race */
	char _name[MAX_NAME_SIZE];	// => logout login 시에 바뀜, _in_use 먼저 체크하고 사용하자.
	short  x = 0, y = 0;		// => 성능을 위해서 보호하지 말자. => 위치랙
	SOCKET _socket;				// => reuse // => _state 추가, accept 했지만 login_in_game 하지 않은 경우. {free, in_lobby, in_game}
	STATE _state{ STATE::ST_FREEE };
	mutex _state_lock;

	/* no data race */
	int	   _id;
	EXP_OVER _recv_over;
	int		_prev_size = 0;

public:
	CLIENT() = default;

	CLIENT(int id, SOCKET s) : _id(id), _socket(s)
	{
		cout << "clients[" << id << "]created\n";
	}

	~CLIENT()
	{
		closesocket(_socket);
	}

	void do_recv()
	{
		DWORD recv_flag = 0;
		ZeroMemory(&_recv_over._wsa_over, sizeof(_recv_over._wsa_over));
		_recv_over._wsa_buf.buf = reinterpret_cast<char*>(_recv_over._net_buf) + _prev_size;
		_recv_over._wsa_buf.len = sizeof(_recv_over._net_buf) - _prev_size;
		int res = WSARecv(_socket, &_recv_over._wsa_buf, 1, 0, &recv_flag, &_recv_over._wsa_over, NULL);
		if (SOCKET_ERROR == res) {
			if (WSA_IO_PENDING != WSAGetLastError())
			{
				cout << "RECV ERRROR :: dorecv" << endl;
				Disconnect(_id);

			}
		}
	}

	void do_send(int num_bytes, void* mess)
	{
		EXP_OVER* ex_over = new EXP_OVER(OP_SEND, num_bytes, mess);
		WSASend(_socket, &ex_over->_wsa_buf, 1, 0, 0, &ex_over->_wsa_over, NULL);
		cout << "sended= " << (int)ex_over->_net_buf[0] << endl;
	}

	void disconnect()
	{
		scoped_lock lock{ _state_lock };
		_state = STATE::ST_FREEE;
		closesocket(_socket);
	}
};

array <CLIENT, MAX_USER> clients;

size_t get_new_id()
{
	static size_t g_id = 0;
	for (int i = 0; i < MAX_USER; i++)
	{
		scoped_lock lock{ clients[i]._state_lock };
		if (STATE::ST_FREEE == clients[i]._state)
		{
			clients[i]._state = STATE::ST_ACCEPT;
			return i;
		}
	}
	cout << "overflow!!\n";
	return -1;
}

void send_login_ok_packet(int c_id)
{
	sc_packet_login_ok p;
	p.id = c_id;
	p.size = sizeof(p);
	p.type = SC_PACKET_LOGIN_OK;
	p.x = clients[c_id].x;
	p.y = clients[c_id].y;
	cout << "logined:" << c_id << " bytes:" << (int)p.size << " x:" << p.x << ", y:" << p.y << endl;
	clients[c_id].do_send(sizeof(p), &p);
}

void send_move_packet(int c_id, int mover)
{
	sc_packet_move p;
	p.id = mover;
	p.size = sizeof(p);
	p.type = SC_PACKET_MOVE;
	p.x = clients[mover].x;
	p.y = clients[mover].y;
	clients[c_id].do_send(sizeof(p), &p);

}

void send_put_obj_packet(int c_id, int new_client)
{
	sc_packet_put_object p;
	p.id = new_client;
	strcpy_s(p.name, clients[new_client]._name);
	p.size = sizeof(p);
	p.object_type = 0;
	p.x = clients[new_client].x;
	p.y = clients[new_client].y;
	p.type = SC_PACKET_PUT_OBJECT;
	cout << "->send _ putobj_ logined:" << new_client << " bytes:" << p.size << " x:" << p.x << ", y:" << p.y << endl;
	clients[c_id].do_send(sizeof(p), &p);
}

void send_remove_object(int c_id, int victim)
{
	sc_packet_remove_object p;
	p.id = victim;
	p.size = sizeof(p);
	p.type = SC_PACKET_REMOVE_OBJECT;
	clients[c_id].do_send(sizeof(p), &p);
}

void process_packet(int client_id, unsigned char* packet_start)
{
	unsigned char packet_type = packet_start[1];
	CLIENT& cl = clients[client_id];

	switch (packet_type)
	{
	case CS_PACKET_LOGIN:
	{
		cout << "@@@@@logined";
		cs_packet_login* p = reinterpret_cast<cs_packet_login*>(packet_start);
		strcpy_s(cl._name, p->name);
		cout << cl._name << " = > logined" << endl;
		/* login_ok */
		{
			scoped_lock lock{ cl._state_lock };
			cl._state = STATE::ST_INGAME;
		}
		send_login_ok_packet(client_id);
		cout << "sent ok packet\n";

		/* update old objs to new obj */
		for (auto& other : clients)
		{
			if (other._id == client_id)continue;
			{
				scoped_lock lock{ other._state_lock };
				if (other._state != STATE::ST_INGAME)continue;
			}
			send_put_obj_packet(client_id, other._id);
		}

		/* broad cast new obj */
		for (auto& other : clients)
		{
			if (other._id == client_id)continue;
			{
				scoped_lock lock{ other._state_lock };
				if (other._state != STATE::ST_INGAME)continue;
			}
			send_put_obj_packet(other._id, client_id);

		}
	}
	break;
	case CS_PACKET_MOVE:
	{
		cs_packet_move* p = reinterpret_cast<cs_packet_move*>(packet_start);
		int x = cl.x;
		int y = cl.y;
		switch (p->direction) // 0 : up,  1: down, 2:left, 3:right
		{
		case 0: if (0 < y)y--; break;
		case 1: if (y < MAX_MAP_H - 1)y++; break;
		case 2: if (0 < x)x--; break;
		case 3: if (x < MAX_MAP_W - 1)x++; break;
		default: cout << "INvalid move directopn from cients" << client_id << endl; exit(-1);
		}
		cl.x = x;
		cl.y = y;
		for (int i = 0; i < MAX_USER; i++)
		{
			{
				scoped_lock lock{ clients[i]._state_lock };
				if (clients[i]._state != STATE::ST_INGAME)continue;
			}
			send_move_packet(i, client_id);
		}
	}
	break;
	}
}

void Disconnect(int c_id)
{
	cout << "DIsconnect::" << c_id << endl;
	clients[c_id].disconnect();
	for (auto& cl : clients)
	{
		{
			scoped_lock lock{ cl._state_lock };
			if (STATE::ST_INGAME != cl._state)continue;
		}
		send_remove_object(cl._id, c_id);
	}
}


HANDLE g_h_iocp;
SOCKET g_s_socket;

void worker()
{
	SOCKET c_socket;
	EXP_OVER accept_ex;
	char accept_buf[sizeof(SOCKADDR_IN) * 2 + 32 + 100];

	auto do_accept = [&] {
		c_socket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, 0, 0, WSA_FLAG_OVERLAPPED);
		ZeroMemory(&accept_ex._wsa_over, sizeof(accept_ex._wsa_over));
		accept_ex._comp_op = OP_ACCEPT;
		cout << "wait_for_accept" << endl;
		auto x = AcceptEx(g_s_socket, c_socket, accept_buf, 0, sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16, NULL, &accept_ex._wsa_over);
		*reinterpret_cast<SOCKET*>(accept_ex._net_buf) = c_socket;

		cout << x << "accepted" << endl;
	};

	do_accept();

	for (int i = 0; i < MAX_USER; i++)
	{
		clients[i]._id = i;
	}

	for (;;)
	{
		DWORD num_bytes;
		LONG64 iocp_key{};
		WSAOVERLAPPED* p_over = new WSAOVERLAPPED;
		BOOL res = GetQueuedCompletionStatus(g_h_iocp, &num_bytes, (PULONG_PTR)&iocp_key, &p_over, INFINITE);
		cout << "\n\nGQCS returned\n";
		int client_id = static_cast<int>(iocp_key);
		cout << "id::" << client_id << " ";
		auto ex_over = reinterpret_cast<EXP_OVER*>(p_over);
		if (ex_over == nullptr)
		{
			cout << "?";
		}
		if (res == FALSE)
		{
			cout << "missconnect" << endl;
			Disconnect(client_id);
			if (ex_over->_comp_op == OP_SEND)
			{
				continue;
			}
			continue;
		}
		switch (ex_over->_comp_op)
		{
		case OP_RECV:
		{
			cout << "RECV ";

			CLIENT& cl = clients[client_id];
			int remain_data = num_bytes + cl._prev_size;
			unsigned char* packet_start = ex_over->_net_buf;
			int packet_size = packet_start[0];
			cout << num_bytes << "nsize:prev size" << cl._prev_size << " ! ";
			cout << packet_size << "psize:remain_data";
			cout << remain_data << endl;
			if (num_bytes == 0) //  0 바이트가  recv 들어오는 경우는 접속이 종료되었을 경우다. 잊지 말자.
			{
				cout << "recved 0 bytes. disconnect." << endl;
				Disconnect(client_id);
			}
			while (packet_size <= remain_data)
			{
				cout << "[r]";
				process_packet(client_id, packet_start);
				remain_data -= packet_size;
				packet_start += packet_size;
				if (0 < remain_data)packet_size - packet_start[0];
				else break;
			}
			if (0 < remain_data)
			{
				cout << "{r}";
				cl._prev_size = remain_data;
				memcpy(&ex_over->_net_buf, packet_start, remain_data);
			}
			{
				scoped_lock lock{ cl._state_lock };
				if (STATE::ST_FREEE != cl._state)
					cl.do_recv();
			}
		}
		break;
		case OP_SEND:
		{
			cout << "SEND ";

			if (num_bytes != ex_over->_wsa_buf.len)
			{
				Disconnect(client_id);
			}
			delete ex_over;
		}
		break;
		case OP_ACCEPT:
		{
			cout << "ACCEPT ";

			SOCKET c_socket = *(reinterpret_cast<SOCKET*>(ex_over->_net_buf));

			size_t new_id = get_new_id();
			CLIENT& cl = clients[new_id];
			cl.x = 0;
			cl.y = 0;
			cl._id = new_id;
			cl._prev_size = 0;
			cl._recv_over._comp_op = OP_RECV;
			cl._recv_over._wsa_buf.buf = reinterpret_cast<char*>(cl._recv_over._net_buf);
			cl._recv_over._wsa_buf.len = sizeof(cl._recv_over._net_buf);
			ZeroMemory(&cl._recv_over._wsa_over, sizeof(cl._recv_over._wsa_over));
			cl._socket = c_socket;

			cout << new_id << "E ";
			CreateIoCompletionPort(reinterpret_cast<HANDLE>(cl._socket), g_h_iocp, new_id, 0);
			cout << new_id << "E ";
			cl.do_recv();

			do_accept();
		}
		break;
		default:
			cout << "EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE";
			break;
		}
	}
}

int main()
{
	WSADATA WSAData;
	WSAStartup(MAKEWORD(2, 2), &WSAData);
	g_s_socket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, 0, 0, WSA_FLAG_OVERLAPPED);
	SOCKADDR_IN server_addr;
	ZeroMemory(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(SERVER_PORT);
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	bind(g_s_socket, reinterpret_cast<sockaddr*>(&server_addr), sizeof(server_addr));
	listen(g_s_socket, SOMAXCONN);

	g_h_iocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 0);
	CreateIoCompletionPort(reinterpret_cast<HANDLE>(g_s_socket), g_h_iocp, 0, 0);

	vector<thread> workers;
	for (int i = 0; i < 6; i++)
	{
		workers.emplace_back(worker);
	}

	for (auto& w : workers)
	{
		w.join();
	}

	for (auto& cl : clients)
	{
		if (STATE::ST_FREEE != cl._state)
			cl.disconnect();
	}

	closesocket(g_s_socket);
	WSACleanup();
}


