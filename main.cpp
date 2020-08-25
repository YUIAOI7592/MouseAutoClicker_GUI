#define UNICODE
#include <windows.h>

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
    switch (message)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, message, wparam, lparam);
}
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASS wndclass;
    ZeroMemory(&wndclass, sizeof(WNDCLASS));
    wndclass.style = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc = WndProc;
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
    wndclass.lpszClassName = L"window";
    RegisterClass(&wndclass);
    RECT rect = {0, 0, 200, 200};
    AdjustWindowRect(&rect, WS_CAPTION | WS_SYSMENU | WS_VISIBLE, 0);
    HWND window = CreateWindow(L"window", L"title",
                               WS_OVERLAPPED | WS_SYSMENU | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT,
                               rect.right - rect.left, rect.bottom - rect.top,
                               NULL, NULL, NULL, NULL);
    MSG msg;
    int ret;
    for (;;)
    {
        ret = GetMessage(&msg, NULL, 0, 0);
        if (ret <= 0)
            break;
        DispatchMessage(&msg);
    }
    return 0;
}