// ConsoleApplication9.cpp : Defines the entry point for the console application.
//


#include "file.h"
#include "socket.h"
#include "windir.h"
#include "ftpServer.h"
#include "unixdir.h"

#define _DBG
#define MAX 1024000
#define MIN 32678

int main()
{
    try
    {
		setlocale(LC_CTYPE, ".1251");
		
		//printf("%s", WindowsDir::ListFiles("D:\\военк"));
		
		//getchar();
        FtpServer *ser = &FtpServer::GetServer();// *serv = new FtpServer();
		//serv->Commander();
        ser->Commander();
		File *f = new File();
//		FileState sd = f->Open("C:\\Users\\ho\\Downloads\\AtomSe=", FileMode::OpenRead);
        Socket s(2345);
		s.Bind();
//        s.Bind();
        s.Listen(12);
		char buf[2048];
		byte *data;
        //write(1, buf, 2048);
        //printf("%s", begin);
        char *response = new char[MAX];
        char httpResponse[] = "HTTP/1.0 200 Welcome \r\n\r\n";
        char error[] = "HTTP/1.0 404 Not Foun \r\n\r\n";
        // UnixDir dir;
        //printf("%s", dir.ListFiles("/home/cub"));
		//Socket *n = (Socket*)s.Accept();

		//x_(n+1) = (n+1) * x_n + n*x_n-1 / (2n+1)
		// производна  от произв функ 
		int len = strlen(httpResponse);
		for (;;)
		{
			Socket *n = (Socket*)s.Accept();
			try
			{

				int size1 = n->Recv(buf, 2048);
				char*begin;
				if (size1)
				{
					begin = strchr(buf, '/') + 1;
					*strchr(begin, ' ') = '\0';

					int size = File::ReadAllBytes(begin, &data);
					data[size] = '\0';
					memcpy(response, httpResponse, len);
					memcpy(response + len, data, size);
					response[len + size] = '\0';

		//			sprintf(response, "%s %s", httpResponse, (char*)data);
					n->Send(response, strlen(response));
					delete[] data;

				}
				memset(response, 0, MAX);
                memset(buf, 0, 2048);
				//len = 0;
				//size = 0;
			}
			catch (Exception &e)
			{
				// if (e.GetErrcode() == ENOENT)
				//{
				sprintf(response, "%s %s", error, "404\r\n Not Found");
				n->Send(response, strlen(response));
				memset(buf, 2048, 0);

			}
			n->ShutDown(How::ShutBoth);
			n->Close();
			delete n;
		}
        
       // s.Close();
        //UnixDir dir;
        //dir.ListFiles("/home/cub/");

        return 0;
    }
	catch (FileException &e)
	{
#ifdef _DBG
		printf("%s", e.Message());
#endif
		printf("%s", ParseException(e.GetErrorCode()));
	}
    catch (Exception &e)
    {
//#ifdef _DBG
        printf("%s\n%s", e.Message(), ParseException(e.GetErrcode()));
		getchar();
		getchar();
//#endif
   //     printf("%s", e.Message());
    }
}

//UnixDir dir;
//dir.ListFiles("/home/cub/");
//        File f;
//        byte l[128];
//        Socket client(2345, "127.0.0.1");
//        client.Connect();
//        char data[65536];
//        int size = client.RecvAll(data);
//        File::WriteAllBytes("/home/cub/s", (byte*)data, size);
//f.Open("/home/cub/convert.h", FileMode::OpenRead);
//f.ReadFromFile(l, 128);
//char **strings;
//int size = File::ReadAllCharStrings("/home/cub/convert.h", &strings);
// File::WriteAllCharStrings("/home/cub/tr", strings, size);
//f.Delete();
//		FtpServer::GetServer();
//		File *f  = new File();
//f->Open("D:\\1.jpg", FileMode::OpenRead);
//byte cc[256];
//f->ReadFromFile(cc, 256);
//f->Close();
//byte* arr;
//int size = File::ReadAllBytes("D:\\1.jpg", &arr);
//int size = Fil
//File::WriteAllBytes("D:\\1111.jpg", arr, size);
//Socket server(21);
//server.SetDirectPort(21);
//server.Bind();
//server.Listen(10);
//Socket *n = (Socket*)server.Accept();
//char buf[256];

//while (strcmp(buf, "quit") != 0)
//{
    //char he[] = "Input NameFile: ";
    //n->SendAll(he, strlen(he) + 1);
    //server.RecvAll(buf);

/*int size;
char sbuffer[256];
//Respond with welcome message, FTP client requires those
sprintf(sbuffer, "200 Welcome \r\n");
size = n->Send(sbuffer, strlen(sbuffer));
//sprintf(sbuffer, "530 Log in \r\n");
//size = n->Send(sbuffer, strlen(sbuffer));
//sprintf(sbuffer, "530 Log in \r\n");
//size = n->Send(sbuffer, strlen(sbuffer));


 size = n->Recv(buf, 256);//n->RecvAll(buf);
 size = n->Send(sbuffer, strlen(sbuffer));
 size = n->Send(sbuffer, strlen(sbuffer));
 size = n->Recv(buf, 256);//n->RecvAll(buf);


 size = n->Send(sbuffer, strlen(sbuffer));
 size = n->Recv(buf, 256);//n->RecvAll(buf



 size = n->Send(sbuffer, strlen(sbuffer));
 size = n->Recv(buf, 256);//n->RecvAll(buf

 size = n->Send(sbuffer, strlen(sbuffer));
 size = n->Recv(buf, 256);//n->RecvAll(buf

 size = n->Send(sbuffer, strlen(sbuffer));
 size = n->Recv(buf, 256);//n->RecvAll(buf
size = n->Send("blablabl", 8);
n->ShutDown(How::ShutBoth);

    //size = recv(			f->Open(buf, FileMode::OpenRead);
    int fSize = f->FileSize();

    byte buff[256];
    int len = 0;



    while (len = f->ReadFromFile(buff, 256))
        n->SendAll((char*)buff, len);



    n->SendAll(NULL, 0);
    n->ShutDown(How::ShutBoth);

//}*/


/*//char **strings;
string* srtrs;
byte *arr;
int size = File::ReadAllBytes("D:\\new1.jpg", &arr);
File::WriteAllBytes("D:\\new2.jpg", arr, size);
Socket *sock = new Socket();


File *f = new File();
f->Open("D:\\I.jg", FileMode::OpenRead);
//f->Close();
//byte arr[] = { 1,2,3,4,5 };
byte arr1[10];
//f->FileSize();
int c = f->ReadFromFile(arr1, 10);

f->SetOffset(0, MoveMethod::Start);
byte arr[1000000];
f->ReadFromFile(arr, 1000000);
//f->WriteToFile(arr, 5);
//f->WriteByte(5);
f->SetOffset(0, MoveMethod::End);
f->FileSize();
///f->Close();
f->Rename("D:\111.jg");
f->Seek();
//f->Seek();
f->SetOffset(0, MoveMethod::End);

//// TODO
////// GENERIC READWIRTE ....

////f->Close();
////f->Open("D:\\trat.jpg", FileMode::OpenRead);
//f->FileSize();
//f->Seek();
//f->FileSize();
//f->Seek();
//f->Rename("D:\\tratata.txt");
//int c = f->ReadByte();
//c = f->ReadByte();
//c = f->ReadByte();
*/

//c = f->ReadByte();
//c = f->ReadByte();
//c = f->ReadByte();
привет