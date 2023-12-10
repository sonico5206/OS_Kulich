#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <tchar.h>

int main(int argc, char* argv) {
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    if (!CreateProcess(
        L"\\wsl.localhost\Ubuntu\home\sofiko\OS\lab1\lab_lin_signal.cpp",
        NULL,
        NULL,
        NULL,
        FALSE,
        0,   
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