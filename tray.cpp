#include <windows.h>
#include <shellapi.h>

#define WM_TRAYICON (WM_APP + 1)
#define ID_TRAY 1001

#define ID_OPEN 2001
#define ID_EXIT 2002

NOTIFYICONDATA nid = {};
HWND g_mainWindow = NULL;
UINT WM_TASKBARCREATED;

LRESULT CALLBACK TrayWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

// меню
HMENU CreateTrayMenu()
{
    HMENU hMenu = CreatePopupMenu();
    AppendMenu(hMenu, MF_STRING, ID_OPEN, L"Открыть");
    AppendMenu(hMenu, MF_STRING, ID_EXIT, L"Выход");
    return hMenu;
}

// инициализация трея
void InitTray(HINSTANCE hInstance, HWND mainHwnd)
{
    g_mainWindow = mainHwnd;

    WM_TASKBARCREATED = RegisterWindowMessage(L"TaskbarCreated");

    const wchar_t CLASS_NAME[] = L"TrayWindow";

    WNDCLASS wc = {};
    wc.lpfnWndProc = TrayWndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        L"",
        0,
        0, 0, 0, 0,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    nid.cbSize = sizeof(nid);
    nid.hWnd = hwnd;
    nid.uID = ID_TRAY;
    nid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
    nid.uCallbackMessage = WM_TRAYICON;
    nid.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcscpy_s(nid.szTip, L"Моё приложение");

    Shell_NotifyIcon(NIM_ADD, &nid);
}

// обработчик трея
LRESULT CALLBACK TrayWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_TRAYICON:
    {
        if (lParam == WM_LBUTTONDOWN)
        {
            ShowWindow(g_mainWindow, SW_RESTORE);
            SetForegroundWindow(g_mainWindow);
        }
        else if (lParam == WM_RBUTTONDOWN)
        {
            HMENU hMenu = CreateTrayMenu();

            POINT pt;
            GetCursorPos(&pt);

            SetForegroundWindow(hwnd);

            TrackPopupMenu(
                hMenu,
                TPM_RIGHTBUTTON,
                pt.x,
                pt.y,
                0,
                hwnd,
                NULL
            );

            DestroyMenu(hMenu);
        }
    }
    break;

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case ID_OPEN:
            ShowWindow(g_mainWindow, SW_RESTORE);
            SetForegroundWindow(g_mainWindow);
            break;

        case ID_EXIT:
            DestroyWindow(hwnd);
            break;
        }
        break;

    case WM_DESTROY:
        Shell_NotifyIcon(NIM_DELETE, &nid);
        PostQuitMessage(0);
        break;

    default:
        if (msg == WM_TASKBARCREATED)
        {
            Shell_NotifyIcon(NIM_ADD, &nid);
        }
        break;
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}