#include "stdafx.h"
#include "Server.h"


int main()
{
	cout << "!" << endl;
	auto& s = Server::get();
	thread Networker{ [&] { s.do_accept(); } };
	while (true)
	{
	//	cout << s.get_packet_queue().unsafe_size() << endl;
	}
}