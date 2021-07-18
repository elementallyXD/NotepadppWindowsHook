#include "KeyboardHook.h"
#include "ProcessTools.hpp"

int main(void)
{
	DWORD threadID = 0;

	try
	{
		const std::wstring process = L"notepad++.exe";
		std::wcout << "Searching " << process << " process..." << std::endl;
		ProcessSearch(process, &threadID);
	}
	catch (const std::exception& exception)
	{
		std::cout << exception.what() << std::endl;
	}
	
	try
	{
		const std::wstring dllName = L"Notepad++Hook.dll";
		const std::unique_ptr<KeyboardHook> hook = std::make_unique<KeyboardHook>(dllName, threadID);
		BOOL bRet = -1;
		MSG	 msg;
		
		while ((bRet = GetMessage(&msg, g_processTools_HWND, WM_INPUT, WM_INPUT)) != 0)
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
