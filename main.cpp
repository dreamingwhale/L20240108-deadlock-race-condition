#include <process.h>
#include <windows.h>

int main()
{												//�����ڵ� ���α׷��� helloworld��� �̸����� ����ڴ�. ���� �̸����� ����� message�� ������. 
	HANDLE OnceMutex = CreateMutex(NULL, FALSE, L"HelloWorld");

	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		MessageBox(nullptr, L"�ѹ��� �����ض�.", L"���", MB_OK);
		return 0;
	}
	while (true);

	CloseHandle(OnceMutex);
	
	return 0;
}