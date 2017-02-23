#ifndef __I_SOCKET__H_
#define __I_SOCKET__H_
#include <stdio.h>
typedef unsigned char byte;
typedef enum End { ShutRecv, ShutSend, ShutBoth } How;
typedef enum ReturnValue_ { ErrorBind, ErrorConnect, ErrorAccept, ErrorCreate, NoError } ReturnValue;

class ISocket
{
public:
	virtual int RecvAll(char *buf) = 0;
	virtual int SendAll(char *buf, int size) = 0;
    virtual ~ISocket() {}
	virtual void Connect() = 0;
	virtual void Close() = 0;
	virtual void Bind() = 0;
	virtual void Listen(int len) = 0;
	virtual ISocket* Accept() = 0;
	virtual void SetDirectPort(short port) = 0;
	virtual int Recv(char *buf, int size) = 0;
	virtual int Send(char *buf, int size) = 0;
	virtual int RecvFrom() = 0;
	virtual int SendTo() = 0;
	virtual void ShutDown(How shutHow) = 0;
};
#endif
