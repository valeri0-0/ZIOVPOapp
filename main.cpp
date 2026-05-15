#include <windows.h>

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

void InitTray(HINSTANCE hInstance, HWND mainHwnd);

#define ID_MENU_EXIT 3001

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
    // MUTEX
    HANDLE hMutex = CreateMutex(NULL, TRUE, L"MyUniqueAppMutex");

    if (GetLastError() == ERROR_ALREADY_EXISTS)
    {
        return 0;
    }

    const wchar_t CLASS_NAME[] = L"MyWindowClass";

    WNDCLASS wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        L"Моё приложение",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 500, 400,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    if (hwnd == NULL)
        return 0;

    // СОЗДАЁМ МЕНЮ
    HMENU hMenuBar = CreateMenu();
    HMENU hFileMenu = CreatePopupMenu();

    AppendMenu(hFileMenu, MF_STRING, ID_MENU_EXIT, L"Выход");
    AppendMenu(hMenuBar, MF_POPUP, (UINT_PTR)hFileMenu, L"Файл");

    SetMenu(hwnd, hMenuBar);

    // ТРЕЙ
    InitTray(hInstance, hwnd);

    if (nCmdShow != SW_HIDE)
    {
        ShowWindow(hwnd, nCmdShow);
    }

    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

// ОБРАБОТКА
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_CLOSE:
        ShowWindow(hwnd, SW_HIDE);
        return 0;

    case WM_COMMAND:
        if (LOWORD(wParam) == ID_MENU_EXIT)
        {
            DestroyWindow(hwnd);
        }
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}