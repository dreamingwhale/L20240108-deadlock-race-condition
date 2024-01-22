#include <process.h>
#include <windows.h>
#include <iostream>

using namespace std;

int money = 0;
int cash = -100;

//임계영역 : 함수를 실행하는 동안 다른 스레드 실행하지마!
CRITICAL_SECTION MoneySection;
CRITICAL_SECTION MoneySection2;

unsigned Increase(void* Arg)
{
	for (int i = 0; i < 1000000; i++)
	{
		EnterCriticalSection(&MoneySection);	//임계영역 설정
		EnterCriticalSection(&MoneySection2);
		money++;
		cash++;
		LeaveCriticalSection(&MoneySection2);
		LeaveCriticalSection(&MoneySection);
	}


	return 0;
}
unsigned Decrease(void* Arg)
{
	for (int i = 0; i < 1000000; i++)
	{
		EnterCriticalSection(&MoneySection);
		EnterCriticalSection(&MoneySection2);	//임계영역 설정
		cash--;
		money--;
		LeaveCriticalSection(&MoneySection2);
		LeaveCriticalSection(&MoneySection);
	}

	return 0;
}


//main thread
int main()
{
	InitializeCriticalSection(&MoneySection);	//임계영역 초기화
	InitializeCriticalSection(&MoneySection2);
	DWORD StartTime = timeGetTime();
	HANDLE ThreadHandle1 = (HANDLE)_beginthreadex(
		nullptr
		, 0
		, Increase
		, nullptr
		, 0
		, nullptr
	);

	HANDLE ThreadHandle2 = (HANDLE)_beginthreadex(
		nullptr
		, 0
		, Decrease
		, nullptr
		, 0
		, nullptr
	);

	//메인스레드가 끝나면 그 안의 스레드가 실행되지 않고 그대로 끝나버린다. 따라서 내부의 스레드가 끝나고 난 후에 메인 스레드를 끝내도록 해야한다.
	//위의 문제 때문에 race condition이라는 문제가 생겨버린다. 같은 메모리에 접근한 경우 한 스레드를 돌리다 다른 스레드를 실행, 그후 다시 기존 스레드를 실행, 이를 반복하면서 실제 원하던 값과 다른 값을 return한다.
	Sleep(1000);

	DeleteCriticalSection(&MoneySection);	//임계영역 해제


	cout << money << endl;
	cout << cash << endl;

	return 0;
}