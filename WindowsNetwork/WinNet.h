#pragma once
#include<WinSock2.h>
#include<WS2tcpip.h>
#include<string>
#include<exception>
#include<vector>
#include<iostream>

#pragma comment(lib,"WS2_32")

namespace WinNet {
	class NetworkEnvironment
	{
	public:
		static void InitializeEnvironment(void)
		{
			if (!Initialized)
			{
				MakeVersion(2, 0);
				ConfigEnvironment();
			}
			Initialized = true;
		}
		~NetworkEnvironment()
		{
			WSACleanup();
		}
	private:
		NetworkEnvironment(void) {}
		static void MakeVersion(int LowByte, int HighByte)
		{
			Version = MAKEWORD(LowByte, HighByte);
		}
		static void ConfigEnvironment(void)
		{
			ErrorCode = WSAStartup(Version, &(WSAData));
			isStartUpSuccess();
		}
		static void isStartUpSuccess(void)
		{
			if (NO_ERROR != ErrorCode)
			{
				throw std::exception{ "Network initialization error." };
			}
		}
	private:
		static bool Initialized;
		static WSADATA WSAData;
		static WORD Version;
		static int ErrorCode;
	};
	bool NetworkEnvironment::Initialized{ false };
	WSADATA NetworkEnvironment::WSAData{};
	WORD NetworkEnvironment::Version{};
	int NetworkEnvironment::ErrorCode{};

	void CheckSuccess(int RetValue, const char* ErrorMessage)
	{
		if (NO_ERROR != RetValue)
		{	
			std::cout << "Last error:" << WSAGetLastError() << std::endl;
			throw std::exception{ ErrorMessage };
		}
	}

	struct NetworkProperties
	{
		NetworkProperties(void) :Socket{},
			SockAddrIn{},
			SockProperties{}
		{}
		void SetSocket(const SOCKET Socket)
		{
			this->Socket = Socket;
		}
		void SetAddressFamily(const int AddressFamily)
		{
			this->SockAddrIn.sin_family = AddressFamily;
		}
		void SetSockType(const int SockType)
		{
			this->SockProperties.SockType = SockType;
		}
		void SetProtocol(const int Protocol)
		{
			this->SockProperties.Protocol = Protocol;
		}
		void SetAddress(unsigned long Address)
		{
			this->SockAddrIn.sin_addr.S_un.S_addr = Address;
		}
		void SetAddress(const char* Address)
		{
			inet_pton(this->GetAddressFamily(),
				Address,
				(void*)&this->SockAddrIn.sin_addr.S_un.S_addr);
		}
		void SetDecimalPort(const unsigned short DecimalPort)
		{
			this->SockAddrIn.sin_port = htons(DecimalPort);
		}
		int GetSocket(void) const
		{
			return this->Socket;
		}
		int GetAddressFamily(void) const
		{
			return this->SockAddrIn.sin_family;
		}
		int GetSockType(void) const
		{
			return this->SockProperties.SockType;
		}
		int GetProtocol(void) const
		{
			return this->SockProperties.Protocol;
		}
		SOCKADDR* GetSockAddrIn(void)
		{
			return (SOCKADDR*)&this->SockAddrIn;
		}
		int GetSockAddrInSize(void) const
		{
			return sizeof(this->SockAddrIn);
		}
		SOCKET Socket;
		sockaddr_in SockAddrIn;
		struct
		{
			int SockType;
			int Protocol;
		}SockProperties;
	};
}