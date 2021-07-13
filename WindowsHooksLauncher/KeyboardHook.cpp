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
			throw std::runtime_error("Error: Unable to load library!");

		if (!SetHook())
		{
			throw std::runtime_error("Unable to set the hook!");
		}
		else
		{
			std::wcout << "Successfully loaded library \"" << m_dll << "\" and set hook for " << m_id << std::endl;
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
		(FARPROC&)installHook = GetProcAddress(m_hHookDLL, "SetKeyboardHook");
		(FARPROC&)uninstallHook = GetProcAddress(m_hHookDLL, "UnhookKeyboardHook");

		if (!installHook || !uninstallHook)
		{
			FreeLibrary(m_hHookDLL);
			return FALSE;
		}
		return TRUE;
	}

	return FALSE;
}

BOOL KeyboardHook::SetHook()
{
	if (installHook)
	{
		return installHook(m_id);
	}
	return FALSE;
}

VOID KeyboardHook::Unhook()
{
	if (uninstallHook)
		uninstallHook();

	installHook = NULL;
	uninstallHook = NULL;
	FreeLibrary(m_hHookDLL);
	m_hHookDLL = NULL;
}
