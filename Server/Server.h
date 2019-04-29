#pragma once
#include"../WindowsNetwork/WinNet.h"

namespace WinNet {
	class Server
	{
	public:
		Server(void) :Properties{}
		{
			NetworkEnvironment::InitializeEnvironment();
		}
		void SetConnectionParameters(unsigned int Port,
			const int AF = AF_INET,
			const int SockType = SOCK_STREAM,
			const int Protocol = IPPROTO_TCP)
		{
			this->Properties.SetAddressFamily(AF);
			this->Properties.SetSockType(SockType);
			this->Properties.SetProtocol(Protocol);
			this->Properties.SetAddress(INADDR_ANY);
			this->Properties.SetDecimalPort(Port);
			this->Properties.SetSocket(socket(this->Properties.GetAddressFamily(),
				this->Properties.GetSockType(),
				this->Properties.GetProtocol()));
			CheckSockValid(this->Properties.GetSocket());
			CheckSuccess(bind(this->Properties.GetSocket(),
				this->Properties.GetSockAddrIn(),
				this->Properties.GetSockAddrInSize()),
				"Bind function faild.");
			std::cout << "Server initialized." << std::endl;
		}
		void Listen(const int ConnectionCounts = SOMAXCONN)
		{
			CheckSuccess(listen(this->Properties.GetSocket(),
				SOMAXCONN_HINT(ConnectionCounts)),
				"Listen function faild.");
			std::cout << "Server is listening." << std::endl;
		}
		void Accept(void)
		{
			SOCKET ClientSock{};
			sockaddr_in ClientAddress{};
			int AddressLength{ sizeof(ClientAddress) };
			ClientSock = accept(this->Properties.GetSocket(),
				(SOCKADDR*)&ClientAddress,
				&AddressLength);
			CheckSockValid(ClientSock);
			SaveClient(ClientSock, &ClientAddress);
			std::cout << "Client " << inet_ntoa(ClientAddress.sin_addr)
				<< " connected. " << "Socket: " << ClientSock << std::endl;
		}
		int Receive(void)
		{
			char recvbuffer[1024]{};
			int byterecived = recv(this->ClientsAccepted.back(), recvbuffer, sizeof(recvbuffer), 0);
			std::cout << "Received message:" << recvbuffer << " Receive count:" << byterecived << std::endl;

			return byterecived;
		}
		int Send(std::string Contents)
		{
			int bytesent = send(this->ClientsAccepted.back(), Contents.c_str(), Contents.size(), 0);
			std::cout << "Byte sent:" << bytesent << std::endl;

			return bytesent;
		}
		~Server()
		{
			closesocket(this->Properties.GetSocket());
		}
	private:
		void CheckSockValid(SOCKET Socket) const
		{
			if (INVALID_SOCKET == Socket)
			{
				throw std::exception{ "Invalid client socket." };
			}
		}
		void SaveClient(SOCKET ClientSock, const sockaddr_in* const ClientAddress)
		{
			ClientsAccepted.push_back(ClientSock);
			ClientsAddress.push_back(*ClientAddress);
		}
	private:
		NetworkProperties Properties;
		std::vector<SOCKET> ClientsAccepted;
		std::vector<sockaddr_in> ClientsAddress;
	};
}