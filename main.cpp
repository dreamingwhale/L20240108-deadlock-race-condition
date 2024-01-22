#include <process.h>
#include <windows.h>
#include <iostream>

using namespace std;

int money = 0;
int cash = -100;

//�Ӱ迵�� : �Լ��� �����ϴ� ���� �ٸ� ������ ����������!
CRITICAL_SECTION MoneySection;
CRITICAL_SECTION MoneySection2;

unsigned Increase(void* Arg)
{
	for (int i = 0; i < 1000000; i++)
	{
		EnterCriticalSection(&MoneySection);	//�Ӱ迵�� ����
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
		EnterCriticalSection(&MoneySection2);	//�Ӱ迵�� ����
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
	InitializeCriticalSection(&MoneySection);	//�Ӱ迵�� �ʱ�ȭ
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

	//���ν����尡 ������ �� ���� �����尡 ������� �ʰ� �״�� ����������. ���� ������ �����尡 ������ �� �Ŀ� ���� �����带 �������� �ؾ��Ѵ�.
	//���� ���� ������ race condition�̶�� ������ ���ܹ�����. ���� �޸𸮿� ������ ��� �� �����带 ������ �ٸ� �����带 ����, ���� �ٽ� ���� �����带 ����, �̸� �ݺ��ϸ鼭 ���� ���ϴ� ���� �ٸ� ���� return�Ѵ�.
	Sleep(1000);

	DeleteCriticalSection(&MoneySection);	//�Ӱ迵�� ����


	cout << money << endl;
	cout << cash << endl;

	return 0;
}