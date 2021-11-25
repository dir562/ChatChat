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
		socket_ = s;
		set_option();
		net_id_ = id;
		prerecv_size_ = 0;
		net_buf_.fill({});
		state_ = SESSION_STATE::connected;
	}

	void set_option()
	{
		DWORD time = 3000;
		setsockopt(socket_, SOL_SOCKET, SO_RCVTIMEO, (char*)&time, sizeof(time));
	}

	void disconnect();

	void do_recv();

	void do_send_heart_beat()
	{
		clk::time_point prev_time = clk::now();
		sc_heart_beat heart_beat;
		while (state_ != SESSION_STATE::disconnected)
		{
			if (1s < duration_cast<milliseconds>(clk::now() - prev_time))
			{
				do_send(&heart_beat, sizeof(heart_beat));
				prev_time = clk::now();
			}
		}
	}

	void do_send(const void* packet, size_t packet_len)
	{
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
private:
	volatile atomic<SESSION_STATE> state_{ SESSION_STATE::disconnected };
};

