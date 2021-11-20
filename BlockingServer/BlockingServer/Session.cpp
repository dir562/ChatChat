#include "stdafx.h"
#include "Server.h"
#include "Session.h"

void Session::disconnect()
{
	sc_disconnect disconnect_packet; disconnect_packet.DisconnectID = net_id_;
	auto& sessions = Server::get().get_sessions();
	for (auto& s : sessions)
	{
		if (s.check_state(SESSION_STATE::disconnected))
		{
			continue;
		}

		if (s.get_net_id() == net_id_)
		{
			continue;
		}
		else
		{
			s.do_send(&disconnect_packet, sizeof(disconnect_packet));
		}
	}

	UNIQUE_LOCK lck{ connection_lock_ };
	cout << "disconnect" << net_id_ << endl;
	state_ = SESSION_STATE::disconnected;
	::closesocket(socket_);
}

void Session::do_recv()
{
	cout << "do_recv" << net_id_ << endl;
	while (state_ != SESSION_STATE::disconnected)
	{
		TRY_LOCK_SHARED_OR_RETURN(connection_lock_);
		auto buf_len = net_buf_.size() - prerecv_size_;
		auto buf_start = net_buf_.data() + prerecv_size_;
		UNLOCK_SHARED(connection_lock_);
		auto ret = ::recv(socket_, buf_start, buf_len, NULL);
		cout << "recv" << net_id_ << "::" << ret << endl;

		if (0 == ret || SOCKET_ERROR == ret) [[unlikely]]
		{
			disconnect();
			return;
		};

		auto pck_start = net_buf_.data();
		auto remain_bytes = ret + prerecv_size_;

		for (auto need_bytes = reinterpret_cast<packet_base<void>*>(pck_start)->size;
			need_bytes <= remain_bytes;)
		{
			//auto new_packet = std::malloc(need_bytes * 8);
			//memcpy(new_packet, pck_start, need_bytes);

			Server::get().process_packet(net_id_, pck_start);
			//packet_queue.push(make_pair(new_packet, net_id_));
			//cout << "push" << (int)need_bytes << net_id_ << endl;
			pck_start += need_bytes;
			remain_bytes -= need_bytes;
			need_bytes = reinterpret_cast<packet_base<void>*>(pck_start)->size;

			if (0 == remain_bytes)
			{
				break;
			}
		}

		prerecv_size_ = remain_bytes;

		if (0 != remain_bytes)
		{
			memmove(net_buf_.data(), pck_start, remain_bytes);
		}
	}
}
