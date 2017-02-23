#ifndef __CONVERTER__H_
#define __CONVERTER__H_
#include <string.h>
#include <memory.h>
#include "exeption.h"
#include <string>
using namespace std;
typedef unsigned char byte;

class IConverter
{

};

template <int countToCovert, typename Type1, typename Type2>
class LightConverter : public IConverter
{
public:
	static int ConvertCharStringArrayTobyte(byte** byteArr, char **str, int cnt)
	{
		int countBytes = 0;
		for (int i = 0; i < cnt; i++)
			countBytes += strlen(str[i]);
		int pos = 0;
		byte* buf = new byte[countBytes];
		if (!buf) ThrowException("Cant alloc memory for buf");
		for (int i = 0; i < cnt; i++)
		{
			int len = strlen(str[i]);
			memcpy(buf + pos, str[i], len);//str[i]));
			pos += len;
		}
		*byteArr = buf;
		return countBytes;
	}

	static int ConvertStringArrayTobyte(byte** byteArr, string *str, int cnt)
	{
		int countBytes = 0;
		for (int i = 0; i < cnt; i++)
			countBytes += str[i].length();

		int pos = 0;
		byte* buf = new byte[countBytes];
		if (!buf) ThrowException("Cant alloc memory for buf");
		for (int i = 0; i < cnt; i++)
		{
			int len = str[i].length();
			memcpy(buf + pos, str[i].c_str(), len);//str[i]));
			pos += len;
		}
		*byteArr = buf;
		return countBytes;
	}
};

#endif