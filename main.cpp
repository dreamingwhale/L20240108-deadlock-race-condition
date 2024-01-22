#include <process.h>
#include <windows.h>

int main()
{												//유니코드 프로그램을 helloworld라는 이름으로 만들겠다. 같은 이름으로 만들면 message를 던져라. 
	HANDLE OnceMutex = CreateMutex(NULL, FALSE, L"HelloWorld");

	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		MessageBox(nullptr, L"한번만 실행해라.", L"경고", MB_OK);
		return 0;
	}
	while (true);

	CloseHandle(OnceMutex);
	
	return 0;
}