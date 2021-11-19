#pragma once
/*
void game_loop() //thread
{
	while (1 / 20sec)
	{
		if (queue)
		{
			depue_packet;
			process_packet;
		}
	}

	send_to_everyone;
}
*/

enum class SESSION_STATE : int8
{
	disconnected,
	connected,
	ingame,
};

class Session
{
public:
	void new_session(SOCKET s, NetID id)
	{
		UNIQUE_LOCK lck{ connection_lock_ };
		socket_ = s;

		set_option();
		net_id_ = id;
		prerecv_size_ = 0;
		net_buf_.fill({});
		state_ = SESSION_STATE::connected;
	}

	void set_option()
	{
		//	BOOL opt = 1; => 두시간짜리,,
		//	setsockopt(socket_, SOL_SOCKET, SO_KEEPALIVE, (char*)&opt, sizeof(opt));
		//DWORD time = 1000;
		//setsockopt(socket_, SOL_SOCKET, SO_RCVTIMEO, (char*)&time, sizeof(time));
	}

	void disconnect()
	{
		cout << "diss" << net_id_ << endl;
		UNIQUE_LOCK lck{ connection_lock_ };
		cout << "disconnect" << net_id_ << endl;
		state_ = SESSION_STATE::disconnected;
		::closesocket(socket_);
	}

	bool check_client_alive()
	{
		// 접속 끊길경우, recv가 리턴하지 않는 경우가 있음, 
		// 패킷을 주고받으면서 체킹 해야함,, 하트비트 필요함. => 어차피 매번 1/20으로 보내는데 그걸 활용해서 5초정도마다 패킷 돌려받으면 될 듯?
		// 아래는 select 를 써서 이 문제를 해결할 경우에 쓸 예시임
		//fd_set read;
	//	read.fd_array[0] = socket_;
	//	read.fd_count = 1;
	//	fd_set write{};
	//	fd_set except{};
	//	timeval t; t.tv_sec = 10;
	//	auto ret = select(0, &read, &write, &except, &t);
		return false;
	}

	void do_recv();

	void do_send(void* packet, size_t packet_len)
	{
		TRY_LOCK_SHARED_OR_RETURN(connection_lock_);
		auto res = ::send(socket_, reinterpret_cast<const char*>(packet), packet_len, NULL);
		CHECK_ERR_DISPLAY(res, "send");
	}

public:
	GET(net_id);
	GET(prerecv_size);
	bool check_state(const SESSION_STATE state)const { return  state_ == state; }

private:
	array<char, MAX_PACKET_SIZE> net_buf_{};
	SOCKET socket_{ INVALID_SOCKET };
	size_t prerecv_size_{ 0 };
	NetID net_id_{};
	shared_mutex connection_lock_;
private:
	atomic<SESSION_STATE> state_{ SESSION_STATE::disconnected };
};

