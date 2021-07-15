#include "KeyboardHook.h"
#include <vector>

#include <tlhelp32.h>

DWORD GetPIDByName(const std::wstring& name)
{
	DWORD pid = 0;

	// Create toolhelp snapshot.
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32 process;
	ZeroMemory(&process, sizeof(process));
	process.dwSize = sizeof(process);

	// Walkthrough all processes.
	if (Process32First(snapshot, &process))
	{
		do
		{
			if (std::wstring(process.szExeFile) == name)
			{
				pid = process.th32ProcessID;
				break;
			}
		} while (Process32Next(snapshot, &process));
	}

	CloseHandle(snapshot);

	return pid;
}

const DWORD getTID(DWORD& pid)
{
	HWND hCurWnd = nullptr;
	hCurWnd = FindWindowEx(nullptr, hCurWnd, nullptr, nullptr);
	
	return GetWindowThreadProcessId(hCurWnd, &pid);
}

int main(void)
{
	const std::wstring    dllName = L"Notepad++Hook.dll";
	const std::wstring    process  = L"notepad++.exe"; //L"new 1 - Notepad++";
	//const HWND			  handleToWindow = FindWindow(NULL, window.c_str());
	MSG				 	  msg;
	BOOL				  bRet = -1;
	DWORD				  thread_id = 0;

	try
	{
		DWORD pid = GetPIDByName(L"notepad++.exe");
		if (pid == 0)
		{
			throw std::runtime_error("Process id not found!");
		}
		else 
		{
			thread_id = getTID(pid);
			if (thread_id == 0)
			{
				throw std::runtime_error("Thread id not found!");
			}
			else
			{
				std::wcout << process << "ThreadProcessId found: " << thread_id << std::endl;
			}
		}
	}
	catch (const std::exception& exception)
	{
		std::cout << exception.what() << std::endl;
	}

	//try
	//{
	//	if (handleToWindow == NULL)
	//		throw std::runtime_error("Window not found!");
	//	else
	//	{
	//		thread_id = GetWindowThreadProcessId(handleToWindow, NULL);
	//		if (thread_id <= 0) 
	//		{
	//			throw std::runtime_error("ThreadProcessId not found!");
	//		}
	//		else {
	//			std::wcout << window << " ThreadProcessId found: " << thread_id << std::endl;
	//			std::wcout << "\tKeyboard Hook (for " << window << " )" << std::endl;
	//		}
	//	}
	//}
	//catch (const std::exception& exception)
	//{
	//	std::cout << exception.what() << std::endl;
	//}
	
	try
	{
		KeyboardHook hook(dllName, thread_id);

		while ((bRet = GetMessage(&msg, NULL, WM_INPUT, WM_INPUT)) != 0)
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
