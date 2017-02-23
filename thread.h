#ifndef THREAD_H
#define THREAD_H


class IThread
{

};

class Thread : public IThread

{
	IThread * thread;
public:
    Thread();

	~Threa

	void InitThread();

	void Interrupt();

	void Suspen();
	
	void Start();

	void Resume();

};

#include <Windows.h>

class WinThread : public IThread
{
	HANDLE threadHandle;
	LPTHREAD_START_ROUTINE threadFunc;
	LPVOID threadArgs;
	DWORD threadFlags;
	DWORD threadId;
	LPSECURITY_ATTRIBUTES threadSecurityAttributes;
	SIZE_T threadStackSize;

	WinThread()
	{

	}

	long ThreadId()
	{
		return threadId;
	}

	void InitThread(void *func, void* args, long stackSize, void* securityAttr, long flags)
	{
		threadFunc = (LPTHREAD_START_ROUTINE)func;
		threadArgs = (LPVOID)args;
		threadFlags = flags;
		threadSecurityAttributes = (LPSECURITY_ATTRIBUTES)securityAttr;
		threadStackSize = stackSize;

		//thread = CreateThread(//0, 0, (LPTHREAD_START_ROUTINE)func, args, 0, 0);
	}

	void Start()
	{
		threadHandle = CreateThread(threadSecurityAttributes, threadStackSize, threadFunc, threadArgs, threadFlags, &threadId);
	}

	static void Wait()
	{
		WaitForMultipleObjects();
	}

	void Suspend()
	{

	}

	void interrupt()
	{

	}

	void Resume()
	{

	}



};

class UnixThread : public IThread
{
	pthread threadPointer;

};

#endif // THREAD_H
