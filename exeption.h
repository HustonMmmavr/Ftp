#ifndef __EXCEPTION__H_
#define __EXCEPTION__H_
#define ThrowException(msg) throw Exception(msg, __FILE__, __FUNCTION__, __LINE__)// Exception *e = new Exception(msg, __FILE__, __FUNCTION__, __LINE__); throw *e;}

#define ThrowExceptionWithCode(msg, code) {	\
    char message[128];\
    sprintf(message, "%s %d",msg, code);\
    throw Exception(message, __FILE__, __FUNCTION__, __LINE__, code); \
}

#define ThrowFileException(msg, code) {	\
	char message[128];\
	sprintf(message, "%s %d",msg, code);\
	throw FileException(message,  __FILE__, __FUNCTION__, __LINE__, code);\
}

#ifdef _WIN32
#include <Windows.h>
inline const char *ParseException(int errCode)
{
	char *errMessage = new char[1024];
	LPSTR messageBuffer = NULL;
	size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, errCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);
	//wcstombs(errMessage, messageBuffer, size);
	strcpy(errMessage, messageBuffer);
	//errMessage[size] = '\0';
	LocalFree(messageBuffer);
	return errMessage;
}
#elif __unix__
inline const char *ParseException(int errCode)
{
	char *errMessage = new char[1024];
    strcpy(errMessage, strerror(errCode));
    return errMessage;
}
#endif
//ThrowException(message);\
}

#include <stdio.h>
#include <string>
using namespace std;
class Exception
{
	char message[1024];
	int errCode;
public:
	Exception() {}

    Exception(const char* message, const char *file, const char *function, int line)
    {
        sprintf(this->message, "In file = %s function = %s line = %d %s", file, function, line, message);
    }

    Exception(const char* message, const char *file, const char *function, int line, int code)
	{
        this->errCode = code;
        sprintf(this->message, "In file = %s function = %s line = %d %s", file, function, line, message);
	}

	~Exception()
	{
	}
	//	Exception(char *message);
	const char* Message() const
	{
		return message;
	}

	void SetMessage(const char* message)
	{
		strcpy(this->message, message);
	}

    int GetErrcode()
    {
        return errCode;
    }
};

class FileException : public Exception
{
	char message[1024];
	int errCode;
public:
    FileException(const char* message, const char *file, const char *function, int line, int errCode)
	{
		sprintf(this->message, "In file %s function %s line %d %s", file, function, line, message);
		this->errCode = errCode;
	}

	const char* Message() const
	{
		return message;
	}

	void operator = (const FileException &fE)
	{
		strcpy(message, fE.Message());
		errCode = fE.GetErrorCode();
	}

	int GetErrorCode() const
	{
		return errCode;
	}

	void SetErrorCode(int errCode)
	{
		this->errCode = errCode;
	}
};



//inline void ThrowException(const char *msg)
//{
//    throw Exception(msg, __FILE__, __FUNCTION__, __LINE__);
//}


//inline void ThrowExceptionWithCode(const char* msg, const char* code)
//{
//    char message[128];
//    sprintf(message, "%s %d",msg, code);
//    ThrowException(message);
//}
#endif
