#include <iostream>
#include <windows.h>
#include <tchar.h>
#include <conio.h>   

#pragma comment( lib, "user32.lib" )

typedef BOOL(CALLBACK* SetKeyboardHook)(DWORD thread);
typedef VOID(CALLBACK* UnhookKeyboardHook)();

SetKeyboardHook installHook = NULL;  
UnhookKeyboardHook uninstallHook = NULL;

HINSTANCE hHookDLL = NULL;

VOID Unhook() 
{
	if (uninstallHook)
		uninstallHook();

	// Free Library
	installHook = NULL;
	uninstallHook = NULL;
	FreeLibrary(hHookDLL);
	hHookDLL = NULL;
}

BOOL SetHook(DWORD thread_id)
{
	if (installHook) 
	{
		return installHook(thread_id);
	}
	return FALSE;
}

BOOL LoadDll()
{
	const std::wstring dllName = L"Notepad++Hook.dll";
	hHookDLL = LoadLibrary(dllName.c_str());

	if (hHookDLL)
	{
		(FARPROC&)installHook = GetProcAddress(hHookDLL, "SetKeyboardHook");
		(FARPROC&)uninstallHook = GetProcAddress(hHookDLL, "UnhookKeyboardHook");

		if (!installHook || !uninstallHook)
		{
			std::cout << "Error: Unable to GetProcAddress!" << std::endl;
			FreeLibrary(hHookDLL);
			return FALSE;
		}
		std::wcout << "Successfully loaded library: " << dllName << std::endl;

		return TRUE;
	}

	return FALSE;
}

int main(void)
{
	std::cout << "\tKeyboard Hook" << std::endl;

	if (hHookDLL)
	{
		Unhook();
	}
	else
	{
		if (!LoadDll())
		{
			std::cout << "Error: Unable to load library!" << std::endl;
			return 1;
		}
		
		//GetModuleHandle(0)
		DWORD thread_id = 0;
		HWND handleToNotepad = FindWindow(NULL, L"new 1 - Notepad++");
		if (handleToNotepad == NULL) {
			std::cout << "Window Notepad not found" << std::endl;
		}
		else {
			thread_id = GetWindowThreadProcessId(handleToNotepad, NULL);
			if (thread_id == 0) {
				std::cout << "ID not found: " << std::endl;
			}
			else std::cout << "Notepad++ ID found: " << thread_id  << std::endl;
		}

		if (SetHook(thread_id))
		{
			std::cout << "Successfully set hook!" << std::endl;
			
			MSG msg;
			BOOL bRet;

			while ((bRet = GetMessage(&msg, (HWND)NULL, 0, 0)) != 0)
			{
				if (bRet == -1)
				{
					Unhook();
					return 1;
				}
				else
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			}
		}
		else
		{
			std::cout << "Unable to set the hook!" << std::endl;
			Unhook();
			return 1;
		}
	}

	Unhook();
	return 0;
}
