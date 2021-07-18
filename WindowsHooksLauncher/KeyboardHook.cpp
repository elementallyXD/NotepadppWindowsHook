#include "KeyboardHook.h"

KeyboardHook::KeyboardHook(const std::wstring& dllPath, const DWORD& NotepadProccessID) :
	m_dll(dllPath),
	m_id(NotepadProccessID)
{
	if (m_hHookDLL)
	{
		Unhook();
	}
	else 
	{
		if (!LoadDll()) 
		{
			throw std::runtime_error("Error: Unable to load library!");
		}

		if (!SetHook())
		{
			throw std::runtime_error("Unable to set the hook!");
		}
		else
		{
			std::wcout << "Successfully loaded library \"" << m_dll << "\" and set the hook for " << m_id << std::endl;
		}
	}
}

KeyboardHook::~KeyboardHook() noexcept
{
	Unhook();
}

BOOL KeyboardHook::LoadDll()
{
	m_hHookDLL = LoadLibrary(m_dll.c_str());

	if (m_hHookDLL)
	{
		reinterpret_cast<FARPROC&>(installHook) = GetProcAddress(m_hHookDLL, "SetKeyboardHook");
		reinterpret_cast<FARPROC&>(uninstallHook) = GetProcAddress(m_hHookDLL, "UnhookKeyboardHook");

		if (!installHook || !uninstallHook)
		{
			FreeLibrary(m_hHookDLL);
			return FALSE;
		}
		return TRUE;
	}

	return FALSE;
}

inline BOOL KeyboardHook::SetHook()
{
	return installHook ? installHook(m_id) : FALSE;
}

VOID KeyboardHook::Unhook()
{
	if (uninstallHook) 
	{
		uninstallHook();
	}

	installHook = NULL;
	uninstallHook = NULL;
	FreeLibrary(m_hHookDLL);
	m_hHookDLL = NULL;

	m_dll = L"\0";
	m_id = 0;
}
