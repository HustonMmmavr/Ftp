#ifndef __UNIX_SOCKET__H_
#define __UNIX_SOCKET__H_
#ifdef __unix__
#include <sys/socket.h>
#include "socket.h"
#include "exeption.h"
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SIZE_FIRST_MESSAGE sizeof(long)
class UnixSocket : public ISocket
{
    int sock;
    sockaddr_in sAddr;
public:
   UnixSocket()
   {
       sock = -1;
   }

   UnixSocket(int descriptor)
   {
       sock = descriptor;
   }

   UnixSocket(short port, char *ip, int aFamily = AF_INET, int type = SOCK_STREAM, int protocol = IPPROTO_TCP)
   {
       sock = -1;
       InitSocket(aFamily, type, protocol);
       SetSocketPort(port, ip);
   }

   UnixSocket(short port, int aFamily = AF_INET, int type = SOCK_STREAM, int protocol = IPPROTO_TCP)
   {
       sock = -1;
       InitSocket(aFamily, type, protocol);
       SetSocketPort(port, 0);
   }


   void InitSocket(int aFamily, int type, int protocol)
   {
       sock = socket(aFamily, type, protocol);
       if (sock < 0)
           ThrowExceptionWithCode("erroe socket, errno", errno);
   }

   void SetSocketPort(short port, char* srvAddr, int aFamily = AF_INET)
   {
       sAddr.sin_family = aFamily;
       sAddr.sin_port = htons(port);
       sAddr.sin_addr.s_addr = inet_addr(srvAddr);
   }

   void SetDirectPort(short port)
   {
       sAddr.sin_port = htons(port);
   }

   void SetSocketPort(short port, int srvAddr, int aFamily = AF_INET)
   {
       sAddr.sin_family = aFamily;
       sAddr.sin_port = htons(port);
       sAddr.sin_addr.s_addr = srvAddr;
       //Connect()
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
               ThrowExceptionWithCode("Error recv, errno ", errno);
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
               ThrowExceptionWithCode("Error recv, errno ", errno);
       }
       return sizeBuffer;
   }

   int Recv(char *buf, int size)
   {
       int e = recv(sock, buf , size, 0);
       if (e < 0)
           ThrowExceptionWithCode("Error recv, errno ", errno);

       return e;
   }

   int Send(char *buf, int size)
   {
       int e = send(sock, buf, size, 0);
       if (e < 0)
           ThrowExceptionWithCode("Error recv, errno ", errno);

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
               ThrowExceptionWithCode("Error recv, errno ", errno);
           sizeBuffer += e;
       }

       sizeBuffer = 0;
       while (sizeBuffer < lenght)
       {
           int e = send(sock, buf + sizeBuffer, bufLen - sizeBuffer, 0);
           if (e < 0)
               ThrowExceptionWithCode("Error recv, errno ", errno);

           sizeBuffer += e;
       }

       return sizeBuffer;
   }

   void Connect()
   {
       int len = sizeof(sAddr);
       if (connect(sock, (sockaddr*)&sAddr, len))
           ThrowExceptionWithCode("Error connect, errno ", errno);
   }


   void ShutDown(How shutHow)
   {
       if (shutdown(sock, shutHow) == -1)
           ThrowExceptionWithCode("Error shutdown, errno ", errno);
   }

   void Close()
   {
       close(sock);
   }

   void Bind()
   {
       if (bind(sock, (sockaddr *)& sAddr, sizeof(sAddr)) == -1)
           ThrowExceptionWithCode("Error bind, errno ", errno);

   }

   void Listen(int len)
   {
       if (listen(sock, len) == -1)
           ThrowExceptionWithCode("Error listen, errno ", errno);

   }

   ISocket* Accept()
   {
       socklen_t sizeSAddr = sizeof(sAddr);
       int newS = accept(sock, (sockaddr*)&sAddr, &sizeSAddr);
       if (!newS) return NULL;
       if (newS < 0)
           ThrowExceptionWithCode("Error accept, errno ", errno);

       UnixSocket *newSocket = new UnixSocket(newS);
       newSocket->sAddr = sAddr;
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

