#include "stdafx.h"
#include "IocpHelper.h"
#include "ListenSocket.h"
#include "Session.h"
#include "IocpThread.h"

// net_id 는 소켓 마다 유일,
void IocpThread::DoQueuedCompletionStatus()
{
	for (;;)
	{
		DWORD returned_bytes;
		LONG64 iocp_key;
		WSAOVERLAPPED* p_over;
		BOOL res = GetQueuedCompletionStatus(iocp_, &returned_bytes, (PULONG_PTR)&iocp_key, &p_over, INFINITE);
		int net_id = static_cast<int>(iocp_key);
		auto ex_over = reinterpret_cast<EXP_OVER*>(p_over);
		cerr << "GQCS returned. net_id::" << net_id << endl;

		if (res == FALSE) [[unlikely]]
		{
			cerr << "GQCS::missconnect" << endl;
		};

		switch (ex_over->comp_op)
		{
		case COMP_OP::OP_RECV:
		{
			cerr << "RECV " << endl;

			Session& client = sessions[net_id];

			if (0 == returned_bytes) [[unlikely]]
			{
				cout << "ID::" << net_id << ". recved 0 bytes. disconnect." << endl;
				client.do_disconnect();
				// clientlist -> delete client.
			};

			auto pck_start = ex_over->net_buf;
			packet_size_t pck_size = *reinterpret_cast<packet_size_t*>(pck_start);
			auto prerecved_size = client.get_prerecv_size();
			auto need_bytes = pck_size - prerecved_size;
			auto remain_bytes = returned_bytes;

			cerr << " return_bytes::" << returned_bytes;
			cerr << " prerecv_len::" << prerecved_size;
			cerr << " expected_pck_size::" << pck_size << endl;

			// process completed packets.
			while (need_bytes <= remain_bytes)
			{
				cerr << "completed_packet..";
				process_packet(net_id, pck_start);
				pck_start += need_bytes;
				remain_bytes -= need_bytes;
				if (0 != remain_bytes)
				{
					need_bytes = *reinterpret_cast<packet_size_t*>(pck_start);
				}
				else break;
			}

			// remain_bytes가 남아있으면 미완성패킷의 데이터임. prerecv 해주기.
			if (0 != remain_bytes)
			{
				cerr << "prerecv_packet";
				client.set_prerecv_size(remain_bytes);
				memcpy(&ex_over->net_buf, pck_start, remain_bytes);
			}

			if (+SESSION_STATE::ST_FREE != client.get_state())
				client.do_recv();
		}
		break;
		case  COMP_OP::OP_SEND:
		{
			cerr << "SEND " << endl;

			Session& client = sessions[net_id];

			if (returned_bytes != ex_over->wsa_buf.len)
			{
				client..do_disconnect();
			}
			delete ex_over;
		}
		break;
		case  COMP_OP::OP_ACCEPT:
		{
			cerr << "ACCEPT " << endl;
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
			CreateIoCompletionPort(reinterpret_cast<HANDLE>(cl._socket), iocp_, new_id, 0);
			cout << new_id << "E ";
			cl.do_recv();

			ServerNetwork::ListenSocket::get().do_accept();
		}
		default:
			cerr << "cou;dn'tbe here!! COMPO_OP ERROR ::" << ex_over->comp_op << "::" << endl;
			break;
		}
	}
}
