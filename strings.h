#ifndef __STRING__H_
#define __STRING__H_
#include <string.h>
#include "lightarray.h"
#define MEM_STEP 100
#define MIN_STRING_SIZE 128
#define MIN_STRING_ARRAY_SIZE 32

void ResizeCharArray(char **charArr, int *size, int rCoef)
{
	int oldSize = *size;
	int newSize = oldSize * rCoef;
	char *ptr = new char[newSize];
	if (!ptr) ThrowException("Error Alloc arr for resize");
	memcpy(ptr, *charArr, oldSize);
	delete[] * charArr;
	*charArr = ptr;
	*size = newSize;
}

class String
{
	char *charString;
	int lenght;
	int realSize;
public:
	String()
	{

	}

	String(char *string)
	{
		lenght = strlen(string);
		realSize = lenght;
		charString = (char*)malloc((lenght + 1) * sizeof(char));
		memcpy(charString, string, lenght);
		charString[lenght] = '\0';
	}

	~String()
	{
		free(charString);
	}

	void Resize(int size)
	{
		char* ptr = charString;
		ptr = (char*)realloc(charString, size * sizeof(char));
		if (!ptr) return;
		charString = ptr;
		lenght = size;
	}

	void Append(char* str)
	{
		int oldLen = lenght;
		int len = strlen(str) + 1; // \0
		if (realSize < lenght + len)
			Resize(lenght + len);

		memcpy(charString + oldLen, str, len);
		charString[lenght + 1] = '\0';
	}

	void SetSize(int size)
	{
		lenght = size;
		charString = new char[lenght + 1];
	}

	void Concat()
	{
		//if 
	}
};

#endif