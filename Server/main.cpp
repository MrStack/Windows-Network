#include"Server.h"
#include<iostream>

int main(int argc, char* argv[])
{
	try
	{
		WinNet::Server Server{};
		Server.SetConnectionParameters(20002);
		Server.Listen();
		Server.Accept();
		while (true)
		{
			Server.Receive();
			Server.Send("Hello client.");
			Sleep(500);
		}
	}
	catch (std::exception& excep)
	{
		std::cout << excep.what() << std::endl;
	}
	
	

	return 0;
}