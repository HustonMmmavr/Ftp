#ifndef __WINDOWS_SOCKET__H_
#define __WINDOWS_SOCKET__H_
#ifdef _WIN32 
//#include <windows.h>
//#include <WinSock2.h>
#pragma comment(lib,"Ws2_32.lib")
//#include <winsock2.h>
//#include <ws2tcpip.h>
#include "socket.h"
#include "exeption.h"
#define SIZE_FIRST_MESSAGE sizeof(long)
class WindowsSocket : public ISocket
{
	 SOCKET sock;
	WSADATA wsaData;
	sockaddr_in sAddr;
	FD_SET set;
	 static FD_SET& SetSocketEpoll()
	 {
		 static FD_SET *set = new FD_SET;
		 static ULONG g = FD_ZERO(set);
		 return *set;
	 }


public:
	WindowsSocket()
	{
		sock = INVALID_SOCKET;
	}

	WindowsSocket(SOCKET descriptor)
	{
		sock = descriptor;
	}

	WindowsSocket(short port, char *ip, int aFamily = AF_INET, int type = SOCK_STREAM, int protocol = IPPROTO_TCP)
	{
		try
		{
			sock = INVALID_SOCKET;
			InitSocket(aFamily, type, protocol);
			SetSocketPort(port, ip);
		}
		catch (Exception &e)
		{
			throw e;
		}
	}

	WindowsSocket(short port, int aFamily = AF_INET, int type = SOCK_STREAM, int protocol = IPPROTO_TCP)
	{
		try
		{
			sock = INVALID_SOCKET;
			InitSocket(aFamily, type, protocol);
			SetSocketPort(port, 0);
		}
		catch (Exception &e)
		{
			throw e;
		}
	}


	void InitSocket(int aFamily, int type, int protocol)
	{
		int iRes = WSAStartup(0x202, &wsaData);
		if (iRes)
		{
			char message[128];
			sprintf(message, "%s %d", "WsaStartup failed. WsaGetLastError:", WSAGetLastError());
			ThrowException(message);
		}	
	
		sock = socket(aFamily, type, protocol);
		if (sock == INVALID_SOCKET)
		{
			char message[128];
			sprintf(message, "%s %d", "WsaStartup failed. WsaGetLastError:", WSAGetLastError());
			ThrowException(message);
		}
	}

	void SetSocketPort(short port, char* srvAddr, int aFamily = AF_INET)
	{
		sAddr.sin_family = aFamily;
		sAddr.sin_port = htons(port);
		sAddr.sin_addr.s_addr = inet_addr(srvAddr);
		//Connect()
	}

	void SetDirectPort(short port)
	{
		sAddr.sin_port = htons(port);
		//Connect()
	}

	void SetSocketPort(short port, int srvAddr, int aFamily = AF_INET)
	{
		sAddr.sin_family = aFamily;
		sAddr.sin_port = htons(port);
		sAddr.sin_addr.s_addr = srvAddr;
		//Connect()
	}


	int Select()
	{
		static FD_SET set;
		static ULONG c = FD_ZERO(&set);
		return 0;
	}


	int RecvAll(char *buf)
	{
		int sizeBuffer = 0;
		int lenght = 0;
		int init = 1;

		while (sizeBuffer < SIZE_FIRST_MESSAGE && init)
		{
			int e = recv(sock, (char*)&lenght + sizeBuffer, SIZE_FIRST_MESSAGE - sizeBuffer, 0);
			if (e < 0)
			{
				char message[128];
				sprintf(message, "%s %d", "Error Recieve. WSAGetLastError:", WSAGetLastError());
				ThrowException(message);
			}
			sizeBuffer += e;
			if (!sizeBuffer) init = 0;
		}

		if (!lenght) return -1;

		sizeBuffer = 0;
		while (sizeBuffer < lenght)
		{

			int e = recv(sock, buf + sizeBuffer, lenght - sizeBuffer, 0);
			sizeBuffer += e;
			if (e < 0)
			{
				char message[128];
				sprintf(message, "%s %d", "Error Recieve. WSAGetLastError:", WSAGetLastError());
				ThrowException(message);
			}
		}
		return sizeBuffer;
	}

	int Recv(char *buf, int size)
	{

			int e = recv(sock, buf , size, 0);
			//sizeBuffer += e;
			if (e < 0)
			{
				char message[128];
				sprintf(message, "%s %d", "Error Recieve. WSAGetLastError:", WSAGetLastError());
				ThrowException(message);
			}
		//}
		return e;
	}

	int Send(char *buf, int size)
	{
		int e = send(sock, buf, size, 0);
		if (e < 0)
		{
			char message[128];
			sprintf(message, "%s %d", "Error Send. WSAGetLastError:", WSAGetLastError());
			ThrowException(message);
		}
		return e;
	}


	int SendAll(char *buf, int bufLen)
	{
		int sizeBuffer = 0;
		int lenght = bufLen;

		while (sizeBuffer < SIZE_FIRST_MESSAGE)
		{
			int e = send(sock, (char*)&lenght + sizeBuffer, SIZE_FIRST_MESSAGE - sizeBuffer, 0);
			if (e < 0)
			{
				char message[128];
				sprintf(message, "%s %d", "Error Send. WSAGetLastError:", WSAGetLastError());
				ThrowException(message);
			}
			sizeBuffer += e;
		}

		sizeBuffer = 0;

		while (sizeBuffer < lenght)
		{
			int e = send(sock, buf + sizeBuffer, bufLen - sizeBuffer, 0);
			if (e < 0)
			{
				char message[128];
				sprintf(message, "%s %d", "Error Send. WSAGetLastError:", WSAGetLastError());
				ThrowException(message);
			}
			sizeBuffer += e;
		}

		return sizeBuffer;
	}

	void Connect()
	{
		int len = sizeof(sAddr);
		if (connect(sock, (sockaddr*)&sAddr, len))
		{
			char message[128];
			sprintf(message, "%s %d", "Error Connect. WSAGetLastError:", WSAGetLastError());
			Close();
			WSACleanup();
			ThrowException(message);
		}

	}


	void ShutDown(How shutHow)
	{
		if (shutdown(sock, shutHow))
		{
			char message[128];
			sprintf(message, "%s %d", "Error Connect. WSAGetLastError:", WSAGetLastError());
			Close();
			WSACleanup();
			ThrowException(message);
		}
	}

	void Close()
	{
		closesocket(sock);
	}

	void Bind()
	{
		if (bind(sock, (sockaddr *)& sAddr, sizeof(sAddr)))
		{
			char message[128];
			sprintf(message, "%s %d", "Error Bind. WSAGetLastError:", WSAGetLastError());
			Close();
			WSACleanup();
			ThrowException(message);
		}
	}

	void Listen(int len)
	{
		if (listen(sock, len))
		{
			char message[128];
			sprintf(message, "%s %d", "Error Lisen. WSAGetLastError:", WSAGetLastError());
			Close();
			WSACleanup();
			ThrowException(message);
		}
	}

	ISocket* Accept()
	{
		int sizeSAddr = sizeof(sAddr);
		SOCKET newS = accept(sock, (sockaddr*)&sAddr, &sizeSAddr);
		if (!newS) return NULL;
		if (newS == INVALID_SOCKET)
		{
			char message[128];
			sprintf(message, "%s %d", "Error Accept. WSAGetLastError:", WSAGetLastError());
			Close();
			WSACleanup();
			ThrowException(message);
		}
		WindowsSocket *newSocket = new WindowsSocket(newS);
		newSocket->sAddr = sAddr;
		newSocket->wsaData = wsaData;
		return (ISocket*)newSocket;
	}


	int RecvFrom()
	{
		return NoError;
	}
	int SendTo()
	{
		return NoError;
	}
};
#endif

#endif