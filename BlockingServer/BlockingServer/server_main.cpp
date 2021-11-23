#include "stdafx.h"
#include "Server.h"


int main()
{
	cout << "!" << endl;
	auto& s = Server::get();
	
	s.do_accept();
}