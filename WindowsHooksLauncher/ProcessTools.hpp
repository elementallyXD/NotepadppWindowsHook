#pragma once
#include <string>
#include <windows.h>
#include <tlhelp32.h>
#include <stdexcept>

HWND g_HWND = NULL;

DWORD GetPIDByName(const std::wstring& name)
{
	DWORD pid = 0;

	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32 process;
	ZeroMemory(&process, sizeof(process));
	process.dwSize = sizeof(process);

	if (Process32First(snapshot, &process))
	{
		do
		{
			if (static_cast<std::wstring>(process.szExeFile) == name)
			{
				pid = process.th32ProcessID;
				break;
			}
		} while (Process32Next(snapshot, &process));
	}
	CloseHandle(snapshot);

	return pid;
}

BOOL CALLBACK EnumWindowsProcMy(HWND hwnd, LPARAM lParam)
{
	DWORD lpdwProcessId;
	GetWindowThreadProcessId(hwnd, &lpdwProcessId);
	if (lpdwProcessId == lParam)
	{
		g_HWND = hwnd;
		return FALSE;
	}
	return TRUE;
}

VOID ProcessSearch(const std::wstring& process, DWORD* threadID)
{
	DWORD pid = GetPIDByName(process);
	if (pid <= 0)
	{
		throw std::runtime_error("The process is not found!");
	}
	else
	{
		// geting HWND to process
		EnumWindows(EnumWindowsProcMy, pid);
		if (g_HWND)
		{
			*threadID = GetWindowThreadProcessId(g_HWND, &pid);
		}
		else
		{
			throw std::runtime_error("The handle to the window is not found!");
		}
	}
}