#include <Windows.h>

DWORD WINAPI EntryThread(LPVOID lpParam)  // ע�������� LPVOID������ HMODULE
{
    MessageBoxA(0, "DLL Injected Successfully!", "Test", MB_OK);
    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID)
{
    if (reason == DLL_PROCESS_ATTACH)
        CreateThread(nullptr, 0, EntryThread, hModule, 0, nullptr);
    return TRUE;
}
