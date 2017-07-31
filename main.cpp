#include <Windows.h>

#if !defined(EXECUTABLE) || !defined(HOMEVAR)
#error EXECUTABLE or HOMEVAR not defined...
#endif

static wchar_t szCommandLine[32767];

static void error(const wchar_t* msg)
{
    MessageBoxW(0, msg, L"Error", MB_SYSTEMMODAL);
    ExitProcess(1);
}

int CALLBACK WinMain(
    _In_ HINSTANCE hInstance,
    _In_ HINSTANCE hPrevInstance,
    _In_ LPSTR     lpCmdLine,
    _In_ int       nCmdShow
)
{
    GetEnvironmentVariableW(HOMEVAR, szCommandLine, _countof(szCommandLine));
    auto len = lstrlenW(szCommandLine);
    if(!len)
        error(L"Make sure to set the " HOMEVAR L" environment variable to your PYTHONHOME");
    if(szCommandLine[len - 1] != L'\\')
        lstrcatW(szCommandLine, L"\\");
    SetEnvironmentVariableW(L"PYTHONHOME", szCommandLine);
    lstrcatW(szCommandLine, EXECUTABLE);
    auto cmdLine = GetCommandLineW();
    const wchar_t* start = GetCommandLineW();
#define check(x) if(!*x) __debugbreak()
    if(*start == L'\"')
        do
        {
            start++;
            check(start);
        } while(*start != '\"');
    else
        for(; *start != ' '; start++)
            check(start);
    start++;
    check(start);
#undef check
    lstrcatW(szCommandLine, start);
    STARTUPINFOW si;
    RtlSecureZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);    
    PROCESS_INFORMATION pi;
    DWORD ExitCode = 1;
    if(CreateProcessW(nullptr, szCommandLine, nullptr, nullptr, FALSE, 0, nullptr, nullptr, &si, &pi))
    {
        CloseHandle(pi.hThread);
        WaitForSingleObject(pi.hProcess, INFINITE);
        GetExitCodeProcess(pi.hProcess, &ExitCode);
        CloseHandle(pi.hProcess);
    }
    else
        error(szCommandLine);
    return ExitCode;
}