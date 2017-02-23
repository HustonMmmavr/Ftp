#ifndef __SOCKET__H_
#define __SOCKET__H_
#include "Isocket.h"
#include "windowssocket.h"
#include "unixsocket.h"
class Socket : public ISocket
{
	ISocket *sock;
public:
	Socket()
	{
#ifdef _WIN32 
		sock = new WindowsSocket();
#elif  __unix__
		sock = new UnixSocket();
#endif
	}

	Socket(short port)
	{
#ifdef _WIN32 
		sock = new WindowsSocket(port);
#elif  __unix__
        sock = new UnixSocket(port);
#endif
	}

	Socket(int port, char* ip)
	{
#ifdef _WIN32 
		sock = new WindowsSocket(port, ip);
#elif  __unix__
        sock = new UnixSocket(port, ip);
#endif
	}

    ~Socket()
    {
        delete sock;
    }

	int RecvAll(char *buf)
	{
		return sock->RecvAll(buf);
	}

	int SendAll(char *buf, int size)
	{
		int sended = sock->SendAll(buf, size);
		return sended;
	}

	void Connect()
	{
		sock->Connect();
	}

	void Listen(int len)
	{
		sock->Listen(len);
	}

	Socket* AcceptSocket()
	{
		return (Socket*)Accept();
	}

	ISocket* Accept()
	{
		try
		{
			return sock->Accept();
		}
		catch (Exception &e)
		{
			throw e;
		}
	}

	void Close()
	{
		sock->Close();
	}
	
	void Bind()
	{
		sock->Bind();
	}

    //int Select()
    //{
    //	return sock->Select();
    //}

	int Recv(char *buf, int size)
	{
		return sock->Recv(buf, size);
	}

	int Send(char *buf, int size)
	{
		return sock->Send(buf, size);
	}

	void ShutDown(How shutHow)
	{
		sock->ShutDown(shutHow);
	}

	void SetDirectPort(short port)
	{
		sock->SetDirectPort(port);
	}

	int RecvFrom()
	{
		sock->RecvFrom();
		return 0;
	}
	int SendTo()
	{
		sock->SendTo();
		return 0;
	}

};


#endif
