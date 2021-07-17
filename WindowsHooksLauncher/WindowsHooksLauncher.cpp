#include "KeyboardHook.h"
#include "ProcessTools.hpp"

int main(void)
{
	const std::wstring    process = L"notepad++.exe";
	const std::wstring    dllName = L"Notepad++Hook.dll";
	MSG				 	  msg;
	BOOL				  bRet = -1;
	DWORD				  threadID = 0;

	try
	{
		std::wcout << "Searching " << process << " process..." << std::endl;

		ProcessSearch(process, &threadID);
		if (threadID <= 0)
		{
			throw std::runtime_error("Thread id not found!");
		}
	}
	catch (const std::exception& exception)
	{
		std::cout << exception.what() << std::endl;
	}
	
	try
	{
		KeyboardHook hook(dllName, threadID);

		while ((bRet = GetMessage(&msg, g_HWND, WM_INPUT, WM_INPUT)) != 0)
		{
			if (bRet == -1)
			{
				throw std::runtime_error("Unable to GetMessage");
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
	}
	catch (const std::exception& exception)
	{
		std::cout << exception.what() << std::endl;
	}
			
	return 0;
}
