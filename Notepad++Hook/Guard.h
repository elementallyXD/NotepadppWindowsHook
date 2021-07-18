#pragma once
#include <Windows.h>
#include <iostream>

class Guard 
{
public:
	explicit Guard(const std::wstring& file);
	const HANDLE GetHandle() noexcept;
	~Guard() noexcept;

private:
	BOOL IsHandle();
	
	HANDLE m_hFile = NULL;
	std::wstring m_filename;
};