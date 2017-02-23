#ifndef __IFILE__H_
#define __IFILE__H_

#include <string>
#include <iostream>
#include "strings.h"
#include "lightarray.h"
#define MIN_BUFFER_SIZE 128
using namespace std;
typedef unsigned char byte;
typedef enum MoveMethod_ { Start, Current, End } MoveMethod;
typedef enum FileMode_ { OpenRead, OpenWriteIfExist, OpenWrite, OpenReadWrite } FileMode;
typedef enum ReturnRes_ { FileNotFound, FileOk, FileCantWrite } FileState;



class IFile
{
public:
	// read write and other params
    virtual void Open(const char *fileName, FileMode mode) = 0;
	virtual void Close() = 0; //+
    virtual ~IFile() {};

	virtual void Rename(const char *newFileName) = 0;
    virtual bool Exist() = 0;
	virtual void Delete() = 0;

	virtual unsigned long long FileSize() = 0; //+
	virtual unsigned long long Seek() = 0; // +
	virtual void SetOffset(unsigned long long distance, MoveMethod move) = 0; //+

	// ReadFile
	virtual int ReadByte() = 0; //+
	virtual unsigned long long ReadFromFile(byte* arr, unsigned long long size) = 0; //+

	// WriteFile
	virtual void WriteToFile(byte *data, unsigned long long size) = 0; //+
	virtual void WriteByte(byte b) = 0; //+
};
#endif
