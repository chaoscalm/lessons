#include <windows.h>

using namespace std;

BOOL CALLBACK enumFunc(HWND hwnd, LPARAM lParam)
{
    DWORD thisId;
    GetWindowThreadProcessId(hwnd, &thisId);
    if (thisId == GetCurrentProcessId()) {
        *(HWND*)lParam = hwnd;
        return FALSE; // stop enumeration
    }

    // Continue enumerating
    return TRUE;
}

HWND findTopWindow()
{
    HWND hwnd = 0;
    EnumWindows(enumFunc, (LPARAM)&hwnd);
    return hwnd;
}

DWORD WINAPI annoyThread(LPVOID lpParameter)
{
    HWND mainWnd = findTopWindow();
    while(true) {
        MessageBoxA(mainWnd, "Hello", "Caption", MB_OK);
        Sleep(2000);
    }
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CreateThread(NULL, 0, annoyThread, 0, 0, NULL);
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

