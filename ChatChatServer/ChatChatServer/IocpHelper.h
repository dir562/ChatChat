#pragma once

constexpr int MAX_BUFFER_SIZE = MAX_PACKET_SIZE;
static_assert(MAX_PACKET_SIZE <= MAX_BUFFER_SIZE);

BETTER_ENUM(COMP_OP, int8, OP_NONE, OP_RECV, OP_SEND, OP_ACCEPT);

struct EXP_OVER
{
	WSAOVERLAPPED	wsa_over{};
	COMP_OP			comp_op{ COMP_OP::OP_NONE };
	unsigned char	net_buf[MAX_BUFFER_SIZE]{};
	WSABUF			wsa_buf{ sizeof(net_buf), reinterpret_cast<char*>(net_buf) };


	// maybe send_over
	EXP_OVER(COMP_OP op, size_t packet_len, void* packet) : comp_op(op)
	{
		wsa_buf.len = static_cast<ULONG>(packet_len);
		memcpy(net_buf, packet, packet_len);
	}

	EXP_OVER(COMP_OP op) : comp_op(op) {}

	EXP_OVER() = default;
};
