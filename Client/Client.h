#pragma once
#include"../WindowsNetwork/WinNet.h"

namespace WinNet
{
	class Client
	{
	public:
		Client(void) :Properties{}
		{
			NetworkEnvironment::InitializeEnvironment();
		}
		void SetConnectionParameters(const char* Address,
			unsigned int Port,
			const int AF = AF_INET,
			const int SockType = SOCK_STREAM,
			const int Protocol = IPPROTO_TCP)
		{
			this->Properties.SetAddressFamily(AF);
			this->Properties.SetSockType(SockType);
			this->Properties.SetProtocol(Protocol);
			this->Properties.SetAddress(Address);
			this->Properties.SetDecimalPort(Port);
			this->Properties.SetSocket(socket(this->Properties.GetAddressFamily(),
				this->Properties.GetSockType(),
				this->Properties.GetProtocol()));
		}
		void Connect(void)
		{
			CheckSuccess(connect(this->Properties.GetSocket(),
				this->Properties.GetSockAddrIn(),
				this->Properties.GetSockAddrInSize()),
				"Connection error.");
		}
		int Send(std::string Contents)
		{
			int bytesent = send(this->Properties.GetSocket(), Contents.c_str(), Contents.size(), 0);
			std::cout << "Byte sent:" << bytesent << std::endl;

			return bytesent;
		}
		int Receive(void)
		{
			char recvbuffer[1024]{};
			int byterecived = recv(this->Properties.GetSocket(), recvbuffer, sizeof(recvbuffer), 0);
			std::cout << "Received message:" << recvbuffer << " Receive count:" << byterecived << std::endl;

			return byterecived;

		}
		~Client()
		{
			closesocket(this->Properties.GetSocket());
		}
	private:
		NetworkProperties Properties;
	};
}