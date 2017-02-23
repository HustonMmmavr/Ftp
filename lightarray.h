#ifndef __LIGHT_ARRAY__H_
#define __LIGHT_ARRAY__H_
#include <stdio.h>
#include <string.h>
#include <memory.h>
#include <stdlib.h>
#include "exeption.h"
typedef unsigned char byte;
template <typename T>
class LightArray
{
	T* ptr;
	int size;
	int position;
	int resizingCoefficient;
public:
	LightArray() : ptr(NULL), size(0) {}

	LightArray(int size)
	{
		if (size <= 0) size = 1;
		position = 0;
		this->size = size;
		ptr = new T[size];
		resizingCoefficient = 2;
	}

//	void Resize()
//	{
//		T* newPtr = ptr;
//		newPtr = (T*)realloc(ptr, resizingCoefficient);
//		if (!newPtr)
//			ThrowException("Cant Alloc memory for array");
//		ptr = newPtr;
//		size = resizingCoefficient;
//		resizingCoefficient *= 2;
//	}

//	void Resize(int size)
//	{
//		int rCoeff = resizingCoefficient;

//		while (this->size <= position + size)
//			rCoeff *= 2;

//		T* newPtr = ptr;
//		newPtr = (T*)realloc(ptr, rCoeff);
//		if (!newPtr)
//			ThrowException("Cant Alloc memory for array");
//		ptr = newPtr;
//		size = rCoeff;
//		resizingCoefficient = rCoeff;
//	}

//	void AddElement(T element)
//	{
//		if (this->size == position)
//			Resize();
//		ptr[position++] = element;
//	}

	void AddBuffer(T *buffer, int size)
	{
		//while (this->size < position + size)
//		if (this->size <= position + size)
//			Resize(size);
		memcpy(buffer + position, buffer, size);
	}
};

#endif
