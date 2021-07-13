#pragma once
#include <Windows.h>
#include <iostream>

class file_handle 
{
public:
	file_handle(std::wstring file) : file(file)
	{
		hFile = CreateFile(this->file.c_str(),
			GENERIC_WRITE,
			FILE_SHARE_READ,
			NULL,
			CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			NULL);

		if (hFile == INVALID_HANDLE_VALUE)
		{
			std::wcout << "Terminal failure: Unable to open file " << file << " for write.\n";
		}
	}

	void WriteToFile(const char* string)
	{
		DWORD dwBytesWritten = 0;
		DWORD dwBytesToWrite = (DWORD)sizeof(*string);

		BOOL bErrorFlag = FALSE;

		bErrorFlag = WriteFile(
			hFile,
			string,
			dwBytesToWrite,
			&dwBytesWritten,
			NULL);

		if (FALSE == bErrorFlag) 
			std::cout << "Terminal failure: Unable to write to file.\n";
		else
		{
			if (dwBytesWritten != dwBytesToWrite)
				std::cout << "Error: dwBytesWritten != dwBytesToWrite\n";
		}
	}

	file_handle(const file_handle& other) : hFile(other.hFile), file(other.file)
	{
	}

	~file_handle() noexcept
	{
		if (hFile)
			CloseHandle(hFile);
	}

private:
	HANDLE hFile;
	std::wstring file;
};