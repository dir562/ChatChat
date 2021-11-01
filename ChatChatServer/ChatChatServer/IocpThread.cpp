#include "stdafx.h"
#include "IocpHelper.h"
#include "ListenSocket.h"
#include "Session.h"
#include "IocpThread.h"


// =============================================== // 

void IocpThread::ProcessQueuedCompleteOperationLoop()
{
	while (true)
	{
		DWORD returned_bytes;
		LONG64 iocp_key;
		WSAOVERLAPPED* p_over;
		BOOL res = GetQueuedCompletionStatus(iocp_, &returned_bytes, (PULONG_PTR)&iocp_key, &p_over, INFINITE);
		int net_id = static_cast<int>(iocp_key);
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
		default: cerr << "couldn't be here!! COMPO_OP ERROR ::" << ex_over->comp_op << "::" << endl;
		}
	}
}

// =============================================== // 


void IocpThread::OnRecvComplete(int net_id, DWORD returned_bytes, EXP_OVER* ex_over)
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



void IocpThread::OnSendComplete(int netid, DWORD returned_bytes, EXP_OVER* ex_over)
{
	cerr << "SEND " << endl;

	if (returned_bytes != ex_over->wsa_buf.len)
	{
		Session& client = sessions[net_id];
		client.do_disconnect();
	}

	delete ex_over;
}



void IocpThread::OnAcceptComplete(EXP_OVER* ex_over)
{
	cerr << "ACCEPT " << endl;
	SOCKET new_socket = *(reinterpret_cast<SOCKET*>(ex_over->net_buf));
	size_t new_id = get_new_net_id();
	new_client = sessions.emplace(new_id, new_socket);

	CreateIoCompletionPort(reinterpret_cast<HANDLE>(new_socket), iocp_, new_id, 0);
	cout << "new id::" << new_id << ". accept. ";
	new_client.do_recv();

	ListenSocket::get().do_accept();
}
