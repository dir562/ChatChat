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
		//	BOOL opt = 1; => �νð�¥��,,
		//	setsockopt(socket_, SOL_SOCKET, SO_KEEPALIVE, (char*)&opt, sizeof(opt));
		DWORD time = 1000;
		setsockopt(socket_, SOL_SOCKET, SO_RCVTIMEO, (char*)&time, sizeof(time));
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
		// ���� ������, recv�� �������� �ʴ� ��찡 ����, 
		// ��Ŷ�� �ְ�����鼭 üŷ �ؾ���,, ��Ʈ��Ʈ �ʿ���. => ������ �Ź� 1/20���� �����µ� �װ� Ȱ���ؼ� 5���������� ��Ŷ ���������� �� ��?
		// �Ʒ��� select �� �Ἥ �� ������ �ذ��� ��쿡 �� ������
		//fd_set read;
	//	read.fd_array[0] = socket_;
	//	read.fd_count = 1;
	//	fd_set write{};
	//	fd_set except{};
	//	timeval t; t.tv_sec = 10;
	//	auto ret = select(0, &read, &write, &except, &t);
		return false;
	}

	void do_recv(concurrent_queue<pair<void*, NetID>>& packet_queue)
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
				if (false == check_client_alive())
				{
					disconnect();
					return;
				};
				cout << "ok" << endl;
				continue;
			};

			auto pck_start = net_buf_.data();
			auto remain_bytes = ret + prerecv_size_;

			for (auto need_bytes = reinterpret_cast<packet_base<void>*>(pck_start)->size;
				need_bytes <= remain_bytes;)
			{
				auto new_packet = std::malloc(need_bytes * 8);
				memcpy(new_packet, pck_start, need_bytes);
				packet_queue.push(make_pair(new_packet, net_id_));
				cout << "push" << (int)need_bytes << net_id_ << endl;

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

