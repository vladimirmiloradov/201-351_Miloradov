#include <iostream>
#include <windows.h>

int main()
{
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));
    wchar_t cmdLine[] = L"C:/Users/SIXTECH/Desktop/Development/build-lab1-Desktop_Qt_6_4_2_MSVC2019_64bit-Debug/debug/lab1.exe";

    if (CreateProcess(
        cmdLine, NULL,
        NULL, NULL,
        TRUE, NULL,
        NULL, NULL,
        &si, &pi)) {
        std::cout << "Create Process succes!" << std::endl;
        std::cout << "Create Process pid =" << std::dec << pi.dwProcessId << std::endl;
    }
    else {
        std::cout << "Create Process failed!" << std::endl;
    }

    DebugActiveProcess(pi.dwProcessId);

    DEBUG_EVENT DBEvent;

    while (true)
    {
        WaitForDebugEvent(&DBEvent, INFINITE);
        ContinueDebugEvent(pi.dwProcessId, DBEvent.dwThreadId, DBG_CONTINUE);
    }
}

// https://learn.microsoft.com/en-us/windows/win32/procthread/creating-processes
