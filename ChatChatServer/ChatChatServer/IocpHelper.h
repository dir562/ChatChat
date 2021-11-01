#pragma once

constexpr int MAX_BUFFER_SIZE = MAX_PACKET_SIZE;
static_assert(MAX_PACKET_SIZE <= MAX_BUFFER_SIZE);

BETTER_ENUM(COMP_OP, int8, OP_NONE, OP_RECV, OP_SEND, OP_ACCEPT);

struct EXP_OVER
{
	WSAOVERLAPPED	wsa_over{};
	COMP_OP			comp_op{ COMP_OP::OP_NONE };
	WSABUF			wsa_buf{};
	unsigned char	net_buf[MAX_BUFFER_SIZE]{};


	EXP_OVER(COMP_OP op, size_t packet_len, void* packet) : comp_op(op)
	{
		ZeroMemory(&wsa_over, sizeof(wsa_over));
		wsa_buf.buf = reinterpret_cast<char*>(net_buf);
		wsa_buf.len = static_cast<ULONG>(packet_len);
		memcpy(net_buf, packet, packet_len);
	}

	EXP_OVER(COMP_OP op) : comp_op(op) {}
	EXP_OVER() = default;
};
