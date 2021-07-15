#pragma once
#include <windows.h>
#include <iostream>

class KeyboardHook
{
public:
	KeyboardHook(const std::wstring& dllPath, const DWORD& NotepadProccessID);
	~KeyboardHook() noexcept;

private:
	VOID Unhook();
	BOOL LoadDll();
	BOOL SetHook();

	HINSTANCE m_hHookDLL = NULL;

	typedef BOOL(CALLBACK* SetKeyboardHook)(DWORD thread);
	typedef VOID(CALLBACK* UnhookKeyboardHook)();

	SetKeyboardHook installHook = NULL;
	UnhookKeyboardHook uninstallHook = NULL;

	std::wstring m_dll = L"\0";
	DWORD m_id = 0;
};

