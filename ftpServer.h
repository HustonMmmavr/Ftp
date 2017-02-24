#ifndef __FTP_SERVER__H_
#define __FTP_SERVER__H_
#include "socket.h"
#include "file.h"
#include "Dir.h"
#include <vector>
#define REQUEST_SIZE 32768
#define MAX_PATH 4096
#ifdef _WIN32
#define DEFAULT_PATH "D:\\"//"C:\\Users\\ho\\Documents\\Visual Studio 2015\\Projects\\ConsoleApplication1\\ConsoleApplication1\\Debug"//"C:\\users\\ho\\downloads"
#elif __unix__
#define DEFAULT_PATH "/home/cybe"
#endif
#undef unicode

#ifdef _WIN32
#define DIRECTORY_DIVIDER '\\'
#define ROOT_DIR "D:\\"
#elif __unix__
#defint DIRECTORY_DIVIDER '/'
#deifne ROOT_DIR "/"
#endif


///
//char ServerResponserArray[][] =
//{
/*
{"120 	?????? ????? ?????? ?????"},
{"125 	?????????? ?????? ??????? : ???????? ?????? ?????? ????????" },
{"150 	????????? ????? ???????(OK).?????????? ????? ?????? ???????" },

{ "	200 	??????? ? ???????(OK)"},
{"	211 	????????? ??????? ??? ?????????? ??????"},
{"	212 	????????? ??????????"},
{"	213 	????????? ?????"},
{"214 	?????????? ?????????"},
{"215 	??????????? ???? ???????(???????????? ???????)"},
{"220 	?????? ?????"},
{"221 	?????? ??????"},
{"225 	?????????? ??? ???????? ?????? ???????"},
{"226 	???????? ?????????? ??? ???????? ??????"},
{"227 	???????? ?????????? ??????; ?????? ???????? ???? IP - ????? ? ????? ?????"},
{"230 	???????????????? ???? ?????????(OK)"},
{"250 	?????? ????? ????????? ?????????(OK)"},

{"331 	??? ???????????? ?????????(OK).????????? ??????"},
{"332 	????????????? ????? ??? ???????????"},
{"350 	???????? ????? ??????????????? : ?????????? ?????? ??????????"},

{"425 	?????? ??????? ?????????? ??? ???????? ??????"},
{"426 	?????????? ??????????? : ???????? ????????"},
{"450 	???????? ? ?????? ?? ????????????, ???? ??????????"},
{"451 	???????? ????????; ????????? ??????"},
{"452 	???????? ????????; ???????????? ??????"},

{ "500 	?????????????? ??????; ??????????? ??????? "},
{ "501 	?????????????? ?????? ? ?????????? ??? ?????????"},
{ "502 	??????? ???????????"},
{ "503 	????????? ?????????????????? ??????"},
{ "504 	???????? ??????? ??????????"},
{ "530 	??????? ?? ???????????????"},
{ "532 	?????????? ??????? ?????? ??? ?????????????? ?????"},
{ "550 	???????? ?? ????????? : ???? ??????????"},
{ "552 	?????? ?? ?????????? ????????; ????????? ?????????? ??????"},
{ "553 	?????? ?? ??????????? ????????; ?? ??????????? ??? ?????"},
};*/
typedef enum TypeFile_ { ASCII, IMAGE } TypeFile;

typedef enum FtpCommand_
{
    LIST, USER, QUIT, CWD, CDUP, EPSV, ABOR,
    HELP, PASS, PORT, RETR, STOR, TYPE, DELE,
    MDTM, NLST, MKD, PASV, SYST, NOT, PWD
} FtpCommand;
class NetObject
{

};

class Server : public NetObject
{

};

class FtpServer : public Server
{
private:
    Socket *commandSocket;
    Socket *senderSocket;
	char directoryDivider;
public:
    FtpServer()
    {
        commandSocket = new Socket(21);
        commandSocket->SetDirectPort(21);
        commandSocket->Bind();
        commandSocket->Listen(10);
		directoryDivider = DIRECTORY_DIVIDER;
    }

    ~FtpServer()
    {
        //	delete co
    }

    static FtpServer& GetServer()
    {
        static FtpServer *server = new FtpServer();
        return *server;
    }


    FtpCommand ParseCommand(char **args, char *request, int size)
    {
        int posRN = 0;
        int notFound = 1;
        char command[20];
        for (int i = 0; i < size && notFound; i++)
        {
            if (*(request + posRN) == ' ' || *(request + posRN) == '\r')
            {
                memcpy(command, request, posRN);
                command[posRN] = '\0';
                notFound = 0;
            }
            posRN++;
        }
        *args = request + posRN;
        if (!strcmp("ABOR", command)) return ABOR;
        if (!strcmp("LIST", command)) return LIST;
        if (!strcmp("DELE", command)) return DELE;
        if (!strcmp("QUIT", command)) return QUIT;
        if (!strcmp("USER", command)) return USER;
        if (!strcmp("CDUP", command)) return CDUP;
        if (!strcmp("CWD", command)) return  CWD;
        if (!strcmp("EPSV", command)) return EPSV;
        if (!strcmp("NLST", command)) return NLST;
        if (!strcmp("MKD", command)) return  MKD;
        if (!strcmp("HELP", command)) return HELP;
        if (!strcmp("PASS", command)) return PASS;
        if (!strcmp("PASV", command)) return PASV;
        if (!strcmp("PORT", command)) return PORT;
        if (!strcmp("RETR", command)) return RETR;
        if (!strcmp("STOR", command)) return STOR;
        if (!strcmp("TYPE", command)) return TYPE;
        if (!strcmp("MDTM", command)) return MDTM;
        if (!strcmp("SYST", command)) return SYST;
        if (!strcmp("PWD", command)) return PWD;

        return NOT;
    }


    typedef struct ServerThreadParams_
    {
        Socket *s;
        char *args;
    } ServerThreadParam;

    //	DWORD WINAPI ServerThread(LPVOID buf)

    void GenerateResp()
    {

    }

    void Pwd(Socket *des, char *path)
    {
        char response[MAX_PATH];
        printf("%s", path);
        sprintf(response, "%s \"%s\" %s\r\n", "257", path, "dir");
        des->Send(response, strlen(response));
    }

    void Commander()
    {
        Socket *newSocket = NULL;
        Socket *dataSocket = NULL;
        char *request = new char[REQUEST_SIZE];
        char response[256];
        strcpy(response, "200 Welcome\r\n");

        char command[20];
        {
            while (newSocket = (Socket*)commandSocket->Accept())
            {
                int size;
                char sbuffer[1024];
                sprintf(sbuffer, "200 Welcome \r\n\r\n");
                char path[MAX_PATH] = DEFAULT_PATH;
                TypeFile tF = ASCII;
                size = newSocket->Send(sbuffer, strlen(sbuffer));

                while (1)
                {
                    try
                    {
                        memset(request, 0, REQUEST_SIZE);
                        int requestSize = newSocket->Recv(request, REQUEST_SIZE);
                        printf("%s\n", request);
                        char *args = request;

                        FtpCommand com = ParseCommand(&args, request, requestSize);
						ClearArgs(args);

                        switch (com)
                        {
                        case FtpCommand::ABOR:
                            Abor(newSocket, dataSocket);
                            break;
                        case FtpCommand::LIST:
                            List(newSocket, dataSocket, path);
                            delete dataSocket;
                            break;
                        case FtpCommand::DELE:
                            Dele(newSocket, path, args);
                            break;
                        case FtpCommand::QUIT:
                            Quit(newSocket);
                            break;
                        case FtpCommand::USER:
                            User(newSocket, args);
                            break;
                        case FtpCommand::CDUP:
                            Cdup(newSocket, path);
                            break;
                        case FtpCommand::CWD:
                            Cwd(newSocket, path, args);
                            break;
                        case FtpCommand::EPSV:
                            Epsv();
                            break;
                        case FtpCommand::NLST:
                            Nlst();
                            break;
                        case FtpCommand::MKD:
                            Mkd(newSocket, path, args);
                            break;
                        case FtpCommand::HELP:
                            Help(newSocket);
                            break;
                        case FtpCommand::PASS:
                            Pass(newSocket, args);
                            break;
                        case FtpCommand::PASV:
                            dataSocket = Pasv(newSocket);
                            break;
                        case FtpCommand::PORT:
                            dataSocket = Port(newSocket, args);
                            break;
                        case FtpCommand::RETR:
                            Retr(newSocket, dataSocket, path, args, tF);
                            break;
                        case FtpCommand::STOR:
                            Stor(newSocket, dataSocket, path, args, tF);
                            break;
                        case FtpCommand::TYPE:
                            tF = Type(newSocket, args);
                            break;
                        /*case FtpCommand::MDTM:
                            //Mdtm(newSocket, args);
                            break;*/
                        case FtpCommand::SYST:
                            Syst(newSocket);
                            break;
                        case FtpCommand::PWD:
                            Pwd(newSocket, path);
                            break;
                        default:
                            newSocket->Send("500 No\r\n", 9);
                        }
                    }
                    catch (FileException e)
                    {
                        printf("%s \n%s", e.Message(), ParseException(e.GetErrorCode()));
                    }
                    catch(Exception e)
                    {
                        printf("%s", e.Message());
                        break;
                    }
                }
            }
        }
    }

    void Nlst()
    {
        //TODO
    }

    void FillResponse(char *response, int code, const char *message)
    {
        sprintf(response, "%d %s\r\n", code, message);
    }


    ////
    void Syst(Socket *des)
    {
        char response[1024];
        FillResponse(response, 200, "Linux");
        //sprintf(response, "%s %s %s", "200", "Linux", "\r\n");
        des->Send(response, strlen(response));
    }

    void GetPathFromArgs(char *args)
    {
        ///int len = strlen(args);
        //if (len
        //args[len - 2] = 0;
    }

    void CheckArgsPath(char *path)
    {
#ifdef _WIN32

#elif __unix__

#endif
    }

	inline void ClearArgs(char *args)
	{
		args[strlen(args) - 2] = 0;
	}

	/*
	void Mkd(Socket *commandSocket, char *path, char *args)
	{
		char dirName[MAX_PATH];
		char response[128];
		FillResponse(response, 200, "Directory Created");
		//sprintf(dirName, "%s%c", path, '\\');

		//char directoryDivider = DIRECTORY_DIVIDER;
		if (args[0] == directoryDivider)
			strcpy(dirName, args);
		else
			sprintf(dirName, "%s/%s", path, args);

		Dir::MkDir(dirName);
		commandSocket->Send(response, strlen(response));
	}
	*/

    void Mkd(Socket *commandSocket, char *path, char *args)
    {
        char dirName[MAX_PATH];
        char response[128];
        FillResponse(response, 200, "Directory Created");
        //sprintf(dirName, "%s%c", path, '\\');

        if (args[0] == '/')
            strcpy(dirName, args);
        else
            sprintf(dirName, "%s/%s", path, args);

        Dir::MkDir(dirName);
        commandSocket->Send(response, strlen(response));
    }

    void Abor(Socket *commandSocket, Socket *dataSocket)
    {
        char response[128];
        FillResponse(response, 257, "Operation abor");
        dataSocket->ShutDown(How::ShutSend);
        delete dataSocket;
        commandSocket->Send(response, strlen(response));
    }


    /*
    char* Mdtm(Socket *commandSocket, Socket *dataSocket, char*args)
    {
        char response[128];
        FillResponse(response, 200, "Modifcation readed");
        char modification[128];


        dataSocket->Send(modification, strlen(modification));

        commandSocket->Send(response, strlen(response));
        return NULL;
    }*/


	inline char *LastChar(char *str)
	{
		return &str[strlen(str) - 1];
	}

/*
	void Cwd(Socket *commandSocket, char *path, char* args)
	{
		char response[128];
		//char directoryDivider = DIRECTORY_DIVIDER;
		//args[strlen(args) - 2] = 0;
#ifdef _WIN32
		int isAlpha = ((args[0] > 64) && (args[0] < 91)) || ((args[0] > 96) && (args[0] < 123));
		if (isAlpha && args[1] == ':' &&  args[2] == directoryDivider)
		{
			strcpy(path, args);
			if (*LastChar(path) != directoryDivider)
				strcat(path, "\\");
		}
		else
		{
			if (*LastChar(path) != directoryDivider)
				strcat(path, "\\");
			strcat(path, args);
		}
#elif __unix__
		printf("%s\n", args);

		if (args[0] == directoryDivider)
			strcpy(path, args);
		else
		{
			if (*LastChar(path) != directoryDivider)
				strcat(path, "/");
			strcat(path, args);
		}
#endif
		printf("%s\n", path);
		FillResponse(response, 200, "Directory changed");
		commandSocket->Send(response, strlen(response));
	}
*/

    void Cwd(Socket *commandSocket, char *path, char* args)
    {
        char response[128];
		char directoryDivider = DIRECTORY_DIVIDER;
        //args[strlen(args) - 2] = 0;
#ifdef _WIN32
		int isAlpha = ((args[0] > 64) && (args[0] < 91)) || ((args[0] > 96) && (args[0] < 123));
		if (isAlpha && args[1] == ':' &&  args[2] == directoryDivider)
		{
			strcpy(path, args);
			if (path[strlen(path) - 1] != directoryDivider)
				strcat(path, "\\");
		}
		else
		{
			if (path[strlen(path) - 1] != directoryDivider)
				strcat(path, "\\");
			strcat(path, args);
		}
#elif __unix__
        printf("%s\n", args);

        if (args[0] == directoryDivider)
            strcpy(path, args);
        else
        {
            if (path[strlen(path) - 1] != directoryDivider)
                strcat(path, "/");
            strcat(path, args);
        }
#endif
        printf("%s\n", path);
        FillResponse(response, 200, "Directory changed");
        commandSocket->Send(response, strlen(response));
    }

/*
	void Cdup(Socket *commandSocket, char *oldPath)
	{

		//char directoryDivider = DIRECTORY_DIVIDER;
		char response[128];
		FillResponse(response, 200, "Directory changed");
		int len = strlen(oldPath);
		bool flag = 1;
		if (len == 1)
		{
			commandSocket->Send(response, strlen(response));
			return;
		}
		oldPath[len] = '\0';
		for (int i = len - 1; i >= 0 && flag; i--)
		{
			if (oldPath[i] == directoryDivider)
				flag = 0;
			else
				oldPath[i] = 0;
		}
		*LastChar(oldPath) = '\0';
		if (strlen(oldPath) == 0)
			*LastChar(oldPath) = '/';
		commandSocket->Send(response, strlen(response));
	}
	*/
    void Cdup(Socket *commandSocket, char *oldPath)
    {

		//char directoryDivider = DIRECTORY_DIVIDER;
        char response[128];
        FillResponse(response, 200, "Directory changed");
        int len = strlen(oldPath);
        bool flag = 1;
        if (len == 1)
        {
            commandSocket->Send(response, strlen(response));
            return;
        }
        oldPath[len] = '\0';
        for (int i = len - 1; i >= 0 && flag; i--)
        {
			if (oldPath[i] == directoryDivider)
				flag = 0;
			else
				oldPath[i] = 0;
        }
        oldPath[strlen(oldPath) - 1] = '\0';
        if (strlen(oldPath) == 0)
            oldPath[0] = '/';
        commandSocket->Send(response, strlen(response));
    }

    void Dele(Socket* commandSocket, char *path, char *file)
    {
        char response[128];
        FillResponse(response, 200, "File deleted");
        //file[strlen(file) - 2] = 0;
        char delFile[MAX_PATH];

		//char directoryDivider = DIRECTORY_DIVIDER;


		///////////////////////////////////
        if (file[0] == directoryDivider)//'/')
            strcpy(delFile, file);
        else
            sprintf(delFile, "%s/%s", path, file);

        File::Delete(delFile);
        commandSocket->Send(response, strlen(response));
    }

    void Epsv()
    {
        char response[REQUEST_SIZE];
    }

    // pasv
    Socket* SetServerPort(char* port)
    {
        int h1 = 127, h2 = 0, h3 = 0, h4 = 1;
        int iPort = 2345;
        int p1 = iPort / 256, p2 = iPort % 256;
        Socket *dataSocket = new Socket(iPort, "127.0.0.1");
        sprintf(port, "%d, %d, %d, %d, %d, %d", h1, h2, h3, h4, p1, p2);

        return dataSocket;
    }

    ///
    Socket* Pasv(Socket *commandSocket)
    {
        char response[1024];
        char port[100];
        Socket *dataSocket = SetServerPort(port);
        FillResponse(response, 227, port);
        dataSocket->Bind();
        dataSocket->Listen(1);
        commandSocket->Send(response, strlen(response));
        Socket* news = (Socket*)dataSocket->Accept();
        delete dataSocket;

        return news;
    }

    void Help(Socket *commandSocket)
    {
        char response[REQUEST_SIZE];
        char serverFunctions[] = "LIST\r\nUSER\r\nQUIT\r\nCWD\r\nCDUO\r\nEPSV\r\nABOR\r\n\
                                  HELP\r\nPASS\r\nPORT\r\nRETR\r\nSTOR\r\nTYPE\r\nDELE\r\n\
                                  MDTM\r\nNLST\r\nMKD\r\nPASV\r\n";

        FillResponse(response, 200, serverFunctions);
        commandSocket->Send(response, strlen(response));
    }

    void List(Socket *commandSocket, Socket *dataSocket, char* path)
    {
        char response[128];
        char *listFiles = Dir::ListFiles(path);
        printf("%s\n", path);
        printf("%s\n", listFiles);

        //FillResponse(response, 200, listFiles);
        dataSocket->Send(listFiles, strlen(listFiles));
        dataSocket->Close();

        char resp[128];
        FillResponse(resp, 200, "listFiles");
        commandSocket->Send(resp, strlen(resp));

        delete[] listFiles;
    }

    void User(Socket *commandSocket, char *args)
    {
        char response[128];
        if (!strcmp(args, "anonymous\r\n"))
            FillResponse(response, 230, "User correct");

        FillResponse(response, 331, "User correct");
        commandSocket->Send(response, strlen(response));
    }

    void Pass(Socket *commandSocket, char *args)
    {
        char response[128];
        // TODO check Passw
        FillResponse(response, 230, "OK");
        commandSocket->Send(response, strlen(response));
    }

    void ParsePort(char *ip, int* port, char *args)
    {
        int count = 0;
        int i = 0;

        while (count < 4)
        {
            if (args[i] == ',') count++;
            i++;
        }

        memcpy(ip, args, i);
        ip[i - 1] = 0;
        for (unsigned int k = 0; k < strlen(ip); k++)
            if (args[k] == ',') ip[k] = '.';

        unsigned char p1, p2;
        char data[4];
        memset(data, 0, 4);
        int k = 0;
        for (; args[i] != '\0'; i++)
        {
            if (args[i] == ',')
            {
                p1 = atoi(data);
                memset(data, 0, 4);
                k = 0;
            }
            else data[k++] = args[i];
        }

        p2 = atoi(data);
        *port = p1 * 256 + p2;
    }

    Socket* Port(Socket *commandSocket, char *args)
    {
        char response[128];
        FillResponse(response, 226, "Connected");
        char ip[16];
        int port;
        ParsePort(ip, &port, args);
        senderSocket = new Socket(port, ip);
        senderSocket->Connect();
        commandSocket->Send(response, strlen(response));
        return senderSocket;
    }

	void FillAbsolutePath(char *absPath, char *path, char *fileName)
	{
		//char div = DIRECTORY_DIVIDER;
		sprintf(absPath, "%s%c%s", path, directoryDivider, fileName);
	}

	/*
    void FillAbsolutePath(char *absPath, char *path, char *fileName)
    {
        char div = D;
#ifdef _WIN32
        div = '\\';
#elif __unix__
        div = '/';
#endif
        sprintf(absPath, "%s%c%s", path, div, fileName);
    }
	*/

    void Retr(Socket *commandSocket, Socket *dataSocket, char *path, char *args, TypeFile tF)
    {
        char response[124];
        char fileName[MAX_PATH];
        FillResponse(response, 200, "File sent");
        File *f = new File();// = new File;
        //args[strlen(args) - 2] = 0;

        FillAbsolutePath(fileName, path, args);
        printf("%s", fileName);
        f->Open(fileName, FileMode::OpenRead);
        int blockSize = 256*1024;
        byte buf[256*1024];
        int readed = 0;
        while(readed = f->ReadFromFile(buf, blockSize))
            dataSocket->Send((char*)buf, readed);
        //dataSocket->Send("\r\n", 2);
        dataSocket->Close();
        //f->Close();
        delete f;
        commandSocket->Send(response, strlen(response));
    }

    void Stor(Socket *commandSocket, Socket *dataSocket, char *path, char *args, TypeFile tF)
    {
        char response[128];
        char fileName[MAX_PATH];

        FillResponse(response, 200, "File sended");
        //args[strlen(args) - 2] = 0;

        FillAbsolutePath(fileName, path, args);
        File *f = new File();
        f->Open(fileName, FileMode::OpenWriteIfExist);
		int size = 2 << 18;//1024 * 256;
        char buf[256*1024];
        int recieved = 0;
        while(recieved = dataSocket->Recv(buf, size))
            f->WriteToFile((byte*)buf, recieved);
        //f->Close();
        delete f;
        commandSocket->Send(response, strlen(response));
    }

    TypeFile Type(Socket *commandSocket, char *args)
    {
        char response[128];
        char tMessage[64];
        char cType[6];
        TypeFile type;
        if (args[0] == 'I')
        {
            type = IMAGE;
            strcpy(cType, "IMAGE");
        }
        if (args[0] == 'A')
        {
            type = ASCII;
            strcpy(cType, "ASCII");
        }

        sprintf(tMessage, "%s %s", "Type Set", cType);
        FillResponse(response, 200, tMessage);
        commandSocket->Send(response, strlen(response));
        return type;
    }

    void Quit(Socket *commandSocket)
    {
        char response[REQUEST_SIZE];
        commandSocket->Close();
    }
};



#endif
