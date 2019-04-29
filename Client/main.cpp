#include"Client.h"
#include<iostream>

int main(int argc, char* argv[])
{
	try
	{
		WinNet::Client Client{};
		Client.SetConnectionParameters("127.0.0.1", 20002);
		Client.Connect();
		while (true)
		{
			Client.Send("This is a test.");
			Client.Receive();
			Sleep(500);
		}
	}
	catch (std::exception& excep)
	{
		std::cout << excep.what() << std::endl;
	}

	return 0;
}