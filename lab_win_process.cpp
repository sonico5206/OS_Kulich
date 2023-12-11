#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <tchar.h>

int main(int argc, char* argv) {
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    if (!CreateProcess(
        L"D:\\SONIK\\Unik2\\OS\\lab_wind_signal\\lab_wind_signal\\x64\Debug\\lab_wind_signal.exe.recipe",
        NULL,
        NULL,
        NULL,
        FALSE,
        CREATE_NEW_CONSOLE,    
        NULL,
        NULL,
        &si,
        &pi)
        )
    {
        printf("CreateProcess failed (%d).\n", GetLastError());
    }

    WaitForSingleObject(pi.hProcess, INFINITE);

    DWORD exitCode = 1;
    GetExitCodeProcess(pi.hProcess, &exitCode);
    std::cout << "Process ended with code: " << exitCode;

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}
