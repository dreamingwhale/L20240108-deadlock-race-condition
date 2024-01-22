#include <process.h>
#include <windows.h>
#include <iostream>

#pragma comment(lib,"winmm")

using namespace std;

int money = 0;
int cash = -100;

//임계영역 : 함수를 실행하는 동안 다른 스레드 실행하지마!
//CRITICAL_SECTION MoneySection;
//CRITICAL_SECTION MoneySection2;

DWORD StartTime = timeGetTime();	//프로그램이 실행되고 흘러간 시간

HANDLE MoneyMutex;//스레드가 시그널 상태인지를 설정함.
HANDLE ThreadHandle[2];

unsigned Increase(void* Arg)
{
	for (int i = 0; i < 1000000; i++)
	{
		//시그널 상태가 될때까지 기다린다.
		WaitForSingleObject(MoneyMutex, INFINITE);	//시그널 상태를 on
		money++;
		cash++;
		ReleaseMutex(MoneyMutex);					//시그널 상태를 off

	}


	return 0;
}
unsigned Decrease(void* Arg)
{
	for (int i = 0; i < 1000000; i++)
	{

		WaitForSingleObject(MoneyMutex, INFINITE);
		cash--;
		money--;
		Sleep(0);	//이렇게 하면 무조건 문맥교환이 발생한다.
		ReleaseMutex(MoneyMutex);

	}

	return 0;
}


//main thread
int main()
{
	//Thread Pooling, IOCP
	//Kernel Mode Lock object
	MoneyMutex = CreateMutex(nullptr, FALSE, nullptr);

	ThreadHandle[0] = (HANDLE)_beginthreadex(
		nullptr
		, 0
		, Increase
		, nullptr
		, CREATE_SUSPENDED			//스레드 실행을 멈춰
		, nullptr
	);

	ThreadHandle[1] = (HANDLE)_beginthreadex(
		nullptr
		, 0
		, Decrease
		, nullptr
		, CREATE_SUSPENDED
		, nullptr
	);

	ResumeThread(ThreadHandle[0]);	//스레드 다시 실행해
	ResumeThread(ThreadHandle[1]);

	//SuspendThread(ThreadHandle[0]);	//스레드 멈춰
	//SuspendThread(ThreadHandle[1]);

	WaitForMultipleObjects(2, ThreadHandle, TRUE, INFINITE);

	cout << "Process Time : " << timeGetTime() - StartTime<<endl;

	//WaitForSingleObject(ThreadHandle, INFINITE);//스레드가 끝나길 기다려라
	//WaitForSingleObject(ThreadHandle2, INFINITE);

	CloseHandle(MoneyMutex);
	CloseHandle(ThreadHandle[0]);
	CloseHandle(ThreadHandle[1]);

	cout << money << endl;
	cout << cash << endl;

	return 0;
}