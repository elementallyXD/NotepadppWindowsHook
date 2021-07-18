#include "pch.h"
#include "Guard.h"

Guard::Guard(const std::wstring& file) : m_filename(file)
{
	m_hFile = CreateFileW(
		m_filename.c_str(),
		GENERIC_WRITE,
		FILE_SHARE_READ,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);
}

BOOL Guard::IsHandle()
{
	if (m_hFile == INVALID_HANDLE_VALUE || m_hFile == NULL)
	{
		return FALSE;
	}
	
	return TRUE;
}

const HANDLE Guard::GetHandle() noexcept
{
	if (IsHandle()) {
		return m_hFile;
	}

	return NULL;
}

Guard::~Guard() noexcept
{
	if (m_hFile) {
		CloseHandle(m_hFile);
		m_hFile = NULL;
	}
}