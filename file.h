#ifndef __FILE__H_
#define __FILE__H_
#include "Ifile.h"
#include "winfile.h"
#include "unixfile.h"
#include "exeption.h"
#include "convert.h"

void ResizeStringArray(char ***arrayStrings, int *oldSize, int rCoeff)
{
	int newSize = *oldSize * rCoeff;
	char **oldPtr = *arrayStrings;
	char **newPtr = new char*[newSize];//(char**)realloc(oldPtr, newSize * sizeof(*oldPtr));
	if (!newPtr) ThrowException("Cant resize string array");
	memcpy(newPtr, oldPtr, *oldSize*sizeof(*newPtr));
	delete[] oldPtr;
	*oldSize = newSize;
	*arrayStrings = newPtr;
}

void ResizeString(char **string, int *oldSize, int rCoeff)
{
	int newSize = *oldSize * rCoeff;
	char *oldPtr = *string;
	char *newPtr = new char[newSize];// (char*)realloc(oldPtr, newSize * sizeof(*oldPtr));
	if (!newPtr) ThrowException("Cant resize string");
	memcpy(newPtr, oldPtr, *oldSize*sizeof(*newPtr));
	delete[] oldPtr;
	*oldSize = newSize;
	*string = newPtr;
}

void Resize(char **string, int *oldSize, int newSize)
{
	char *oldPtr = *string;
	char *newPtr = new char[newSize];// (char*)realloc(oldPtr, newSize * sizeof(*oldPtr));
	if (!newPtr) ThrowException("Cant resize string");
	memcpy(newPtr, oldPtr, *oldSize);
	delete[] oldPtr;
	*oldSize = newSize;
	*string = newPtr;
}

class File : public IFile
{
private:
	IFile *file;

	static int ReadStringFromBuffer(char **string, byte *buf, int *startPos, int sizeBuf)
	{
		int stringNotReaded = 1;
		int sizeString = MIN_STRING_SIZE;
		char *str = new char[sizeString];
		int idx = 0;
		int pos = *startPos;
		int rCoef = 2;

		if (!str) ThrowException("Cant alloc memory for str");
			
        for (int i = pos; i < sizeBuf && stringNotReaded; i++)
		{
			if (idx == sizeString)
				Resize(&str, &sizeString, sizeString * rCoef);
				//ResizeString(&str, &sizeString, 2);

			str[idx++] = buf[i];

            if (buf[i] == '\n' || buf[i] == '\0' || i == sizeBuf - 1)
			{
				stringNotReaded = 0;
				*startPos = i + 1;
			}
		}
		if (idx == sizeString)
            Resize(&str, &sizeString, sizeString + 1);
		str[idx] = '\0';
		*string = str;
		return sizeString;
	}
public:
	File()
	{
#ifdef __unix__
		file = new UnixFile();
#elif _WIN32 || _WIN64
		file = new WindowsFile();
#endif
	}

	~File()
	{
		delete file;
	}

    void Open(const char *fileName, FileMode mode)
	{
		return file->Open(fileName, mode);
	}

	void Close()
	{
		file->Close();
	}

	void Rename(const char *newFileName)
	{
		file->Rename(newFileName);
	}

	bool Exist()
	{
        return file->Exist();
	}

    static bool Exist(const char *fileName)
    {
#ifdef _WIN32
        WindowsFile::Exist(fileName);
#elif __unix__
        UnixFile::Exist(fileName);
#endif
    }

	void Delete()
	{

	}

	int ReadByte()
	{
		int b = file->ReadByte();
		return b;
	}

	unsigned long long ReadFromFile(byte *block, unsigned long long sizeBlock)
	{
		unsigned long long size = file->ReadFromFile(block, sizeBlock);
		return size;
	}

	void WriteToFile(byte *block, unsigned long long sizeBlock)
	{
		file->WriteToFile(block, sizeBlock);
	}

	void WriteByte(byte b)
	{
		file->WriteByte(b);
	}

	unsigned long long Seek()
	{
		unsigned long long seek = file->Seek();
		return seek;
	}

	unsigned long long FileSize()
	{
		return file->FileSize();
	}

	void SetOffset(unsigned long long distance, MoveMethod move)
	{
		file->SetOffset(distance, move);
	}

	/////// static Maybe

	static int ReadAllBytes(const char *fileName, byte **byteArr)
	{
		unsigned long long size;
		byte *buf;
#ifdef _WIN32
		size = WindowsFile::ReadAllBytes(&buf, fileName);
#elif __unix__
		size = UnixFile::ReadAllBytes(&buf, fileName);
#endif
		*byteArr = buf;
		return size;
	}

	static LightArray<byte>& ReadAllBytes(const char *fileName)
	{
		byte* buf;
		int size = File::ReadAllBytes(fileName, &buf);
		LightArray<byte> *arr = new LightArray<byte>(size);
		//arr->AddBuffer()
        return *arr;
	}

	static int ReadAllCharStrings(const char *fileName, char*** charStrings)
	{
		byte *buf;
		int sizeBuf = File::ReadAllBytes(fileName, &buf);
		int sizeStringArray = MIN_STRING_ARRAY_SIZE;
			
		char **stringArray = new char*[sizeStringArray];
		if (!stringArray) ThrowException("Cant alloc mem for string");

        int posInBuf = 0;

		int countStrings = 0;
		int rCoef = 2;
		while ( posInBuf < sizeBuf)
		{
			char *str;
            ReadStringFromBuffer(&str, buf, &posInBuf, sizeBuf);
			if (countStrings == sizeStringArray)
				ResizeStringArray(&stringArray, &sizeStringArray, rCoef);
			stringArray[countStrings++] = str;
		}
		*charStrings = stringArray;
		delete[] buf;
		return countStrings;
	}
	
		static char* LastModified(const char *fileName)
	{
#ifdef _WIN32
		return WindowsFile::LastModified(fileName);
#elif __unix__
        //return UnixFile::LastModified(fileName);
#endif
	}


	static int ReadAllStrings(const char *fileName, string** stringArray)
	{
		char **charStrings;
		int cnt = File::ReadAllCharStrings(fileName, &charStrings);
		string *strings = new string[cnt];
		if (!strings) ThrowException("Cant Alloc strings");

		for (int i = 0; i < cnt; i++)
			strings[i] = charStrings[i];
		*stringArray = strings;
		return cnt;
	}

	static unsigned long long FileSize(const char *fileName)
	{
		unsigned long long size;
#ifdef _WIN32
		size = WindowsFile::FileSize(fileName);
#elif __unix__
		size = UnixFile::FileSize(fileName);
#endif
		return size;
	}

	static void Delete(const char *fileName)
	{
#ifdef _WIN32
		WindowsFile::Delete(fileName);
#elif __unix__
		UnixFile::Delete(fileName);
#endif
	}

	static void WriteAllBytes(const char *fileName, byte* data, int size, FileMode mode = OpenWriteIfExist)
	{
#ifdef _WIN32
		WindowsFile::WriteAllBytes(data, size, fileName, mode);
#elif __unix__
        UnixFile::WriteAllBytes(data, size, fileName, mode);
#endif
	}

	static void WriteAllCharStrings(const char *fileName, char** charStrings, int countStrings)
	{
		byte* data;
		int size = LightConverter<0, char**, byte>::ConvertCharStringArrayTobyte(&data, charStrings, countStrings);
		File::WriteAllBytes(fileName, data, size);
	}
	
	static void WriteAllStrings(const char *fileName, string *strings, int countStrings)
	{
		byte* data;
		int size = LightConverter<0, char**, byte>::ConvertStringArrayTobyte(&data, strings, countStrings);
		File::WriteAllBytes(fileName, data, size);
	}

};
#endif
