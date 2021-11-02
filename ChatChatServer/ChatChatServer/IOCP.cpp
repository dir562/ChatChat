#include "stdafx.h"
#include "IocpHelper.h"
#include "ListenSocket.h"
#include "Session.h"
#include "GameData.h"
#include "IOCP.h"

// =============================================== // 

IOCP::IOCP()
{
	iocp_ = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 0);
	ListenSocket::get().Init(iocp_);
	ListenSocket::get().do_accept();
}

// =============================================== // 

void IOCP::ProcessQueuedCompleteOperationLoop()
{
	while (true)
	{
		DWORD returned_bytes;
		NetID net_id;
		WSAOVERLAPPED* p_over;
		BOOL res = GetQueuedCompletionStatus(iocp_, &returned_bytes, (PULONG_PTR)&net_id, &p_over, INFINITE);
		auto ex_over = reinterpret_cast<EXP_OVER*>(p_over);
		cerr << "GQCS returned. net_id::" << net_id << endl;

		if (FALSE == res) [[unlikely]]
		{
			cerr << "GQCS::missconnect" << endl;
		};

		switch (ex_over->comp_op)
		{
		case COMP_OP::OP_RECV: OnRecvComplete(net_id, returned_bytes, ex_over); break;
		case COMP_OP::OP_SEND: OnSendComplete(net_id, returned_bytes, ex_over); break;
		case COMP_OP::OP_ACCEPT: OnAcceptComplete(ex_over); break;
		default: cerr << "couldn't be here!! COMPO_OP ERROR ::" << (int)ex_over->comp_op << "::" << endl;
		}
	}
}

// =============================================== // 

void IOCP::OnRecvComplete(NetID net_id, DWORD returned_bytes, EXP_OVER* ex_over)
{
	cerr << "RECV " << endl;

	Session& client = sessions_[net_id];

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

	// remain_bytes�� ���������� �̿ϼ���Ŷ�� ��������. prerecv ���ֱ�.
	if (0 != remain_bytes)
	{
		cerr << "prerecv_packet";
		client.set_prerecv_size(remain_bytes);
		memcpy(&ex_over->net_buf, pck_start, remain_bytes);
	}

	if (SESSION_STATE::ST_FREE != client.state_)
		client.do_recv();
}


void IOCP::OnSendComplete(NetID net_id, DWORD returned_bytes, EXP_OVER* ex_over)
{
	cerr << "SEND " << endl;

	cout << "send :: " << *(packet_size_t*)ex_over->net_buf << " bytes" << endl;

	if (returned_bytes != ex_over->wsa_buf.len)
	{
		Session& client = sessions_[net_id];
		client.do_disconnect();
	}

	delete ex_over;
}


void IOCP::OnAcceptComplete(EXP_OVER* ex_over) // ���ϼ� ���� �Լ�.
{
	//scoped_lock accept_lock{ connection_lock_ };
	cerr << "ACCEPT " << endl;
	SOCKET new_socket = *(reinterpret_cast<SOCKET*>(ex_over->net_buf));
	NetID new_id = get_new_net_id();
	Session& new_client = sessions_[new_id];
	new_client.NewSession(new_socket, new_id);

	CreateIoCompletionPort(reinterpret_cast<HANDLE>(new_socket), iocp_, new_id, 0);
	cout << "new id::" << (int)new_id << ". accept. ";
	new_client.do_recv();

	ListenSocket::get().do_accept();
}

// =============================================== // 


NetID IOCP::get_new_net_id() // ���ϼ� ���� �Լ�.
{
	for (int net_id = 1; net_id < sessions_.size(); net_id++)
	{
		if (CAS(sessions_[net_id].state_, ST_FREE, ST_ACCEPT))
		{
			return net_id;
		}
	}
	cerr << "SESSION_FULL!!!!!" << sessions_.size() << MAX_PLAYER << endl;
}
