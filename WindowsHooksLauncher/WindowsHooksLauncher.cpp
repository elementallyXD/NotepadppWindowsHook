#include "KeyboardHook.h"

int main(void)
{
	std::cout << "\tKeyboard Hook" << std::endl;

	const std::wstring	dllName = L"Notepad++Hook.dll";
	const HWND			handleToNotepad = FindWindow(NULL, L"new 1 - Notepad++");
	MSG					msg;
	BOOL				bRet = -1;
	DWORD				thread_id = 0;
	
	try
	{
		if (handleToNotepad == NULL)
			throw std::runtime_error("Window Notepad++ not found!");
		else
		{
			thread_id = GetWindowThreadProcessId(handleToNotepad, NULL);
			if (thread_id <= 0) 
			{
				throw std::runtime_error("Notepad++ ThreadProcessId not found!");
			}
			else std::cout << "Notepad++ ThreadProcessId found: " << thread_id << std::endl;
		}
	}
	catch (const std::exception& exception)
	{
		std::cout << exception.what() << std::endl;
	}
	
	try
	{
		KeyboardHook hook(dllName, thread_id);

		while ((bRet = GetMessage(&msg, handleToNotepad, WM_INPUT, WM_INPUT)) != 0)
		{
			if (bRet == -1)
			{
				hook.Unhook();
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
