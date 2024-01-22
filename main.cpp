#include <process.h>
#include <windows.h>
#include <iostream>

#pragma comment(lib,"winmm")

using namespace std;

int money = 0;
int cash = -100;

//�Ӱ迵�� : �Լ��� �����ϴ� ���� �ٸ� ������ ����������!
//CRITICAL_SECTION MoneySection;
//CRITICAL_SECTION MoneySection2;

DWORD StartTime = timeGetTime();	//���α׷��� ����ǰ� �귯�� �ð�

HANDLE MoneyMutex;//�����尡 �ñ׳� ���������� ������.
HANDLE ThreadHandle[2];

unsigned Increase(void* Arg)
{
	for (int i = 0; i < 1000000; i++)
	{
		//�ñ׳� ���°� �ɶ����� ��ٸ���.
		WaitForSingleObject(MoneyMutex, INFINITE);	//�ñ׳� ���¸� on
		money++;
		cash++;
		ReleaseMutex(MoneyMutex);					//�ñ׳� ���¸� off

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
		Sleep(0);	//�̷��� �ϸ� ������ ���Ʊ�ȯ�� �߻��Ѵ�.
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
		, CREATE_SUSPENDED			//������ ������ ����
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

	ResumeThread(ThreadHandle[0]);	//������ �ٽ� ������
	ResumeThread(ThreadHandle[1]);

	//SuspendThread(ThreadHandle[0]);	//������ ����
	//SuspendThread(ThreadHandle[1]);

	WaitForMultipleObjects(2, ThreadHandle, TRUE, INFINITE);

	cout << "Process Time : " << timeGetTime() - StartTime<<endl;

	//WaitForSingleObject(ThreadHandle, INFINITE);//�����尡 ������ ��ٷ���
	//WaitForSingleObject(ThreadHandle2, INFINITE);

	CloseHandle(MoneyMutex);
	CloseHandle(ThreadHandle[0]);
	CloseHandle(ThreadHandle[1]);

	cout << money << endl;
	cout << cash << endl;

	return 0;
}