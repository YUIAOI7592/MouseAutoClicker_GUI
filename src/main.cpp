#define UNICODE
#define IDB_START 3301
#define IDB_PAUSE 3302
#define IDB_END 3303
#include <windows.h>

#include <cstdlib>
#include <iostream>
using namespace std;
void CheckStart(bool* ClickStart);
void CheckEnd(bool* ClickEnd);
void Clicker();
bool Click = false;
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wparam,
                         LPARAM lparam) {
  switch (message) {
    case WM_CREATE: {
      CreateWindow(L"Button", L"開始", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                   45, 260, 50, 30, hwnd, (HMENU)IDB_START, NULL, NULL);
      CreateWindow(L"Button", L"暫停", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                   125, 260, 50, 30, hwnd, (HMENU)IDB_PAUSE, NULL, NULL);
      CreateWindow(L"Button", L"結束", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                   205, 260, 50, 30, hwnd, (HMENU)IDB_END, NULL, NULL);
      return 0;
    }
    case WM_CLOSE: {
      DestroyWindow(hwnd);
      return 0;
    }
    case WM_DESTROY: {
      PostQuitMessage(0);
      return 0;
    }
    case WM_COMMAND: {
      switch (LOWORD(wparam)) {
        case IDB_START:
          Click = true;
          return 0;
        case IDB_PAUSE:
          Click = false;
          return 0;
        case IDB_END:
          PostQuitMessage(0);
          return 0;
        default:
          return 0;
      }
    }
      return 0;
    default: {
      return DefWindowProc(hwnd, message, wparam, lparam);
    }
  }
  return 0;
}
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine, int nCmdShow) {
  WNDCLASS wndclass;
  ZeroMemory(&wndclass, sizeof(WNDCLASS));
  wndclass.style = CS_HREDRAW | CS_VREDRAW;
  wndclass.lpfnWndProc = WndProc;
  wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
  wndclass.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
  wndclass.lpszClassName = L"window";
  RegisterClass(&wndclass);
  RECT rect = {0, 0, 300, 300};
  AdjustWindowRect(&rect, WS_CAPTION | WS_SYSMENU | WS_VISIBLE, 0);
  HWND window =
      CreateWindow(L"window", L"滑鼠連點器",
                   WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE,
                   CW_USEDEFAULT, CW_USEDEFAULT, rect.right - rect.left,
                   rect.bottom - rect.top, NULL, NULL, NULL, NULL);
  MSG msg;
  while (GetMessage(&msg, NULL, 0, 0)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
    if (Click) Clicker();
  }
  return msg.wParam;
}
void CheckStart(bool* ClickStart) {
  if ((GetKeyState(VK_CONTROL) & 0x8000) && (GetAsyncKeyState(VK_F2) & 0x8000))
    *ClickStart = true;
}
void CheckEnd(bool* ClickEnd) {
  if ((GetKeyState(VK_CONTROL) & 0x8000) && (GetAsyncKeyState(VK_F3) & 0x8000))
    *ClickEnd = false;
}
void Clicker() {
  INPUT input[3];
  input[1].type = INPUT_MOUSE;
  input[1].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
  input[2].type = INPUT_MOUSE;
  input[2].mi.dwFlags = MOUSEEVENTF_LEFTUP;
  SendInput(_countof(input), input, sizeof(INPUT));
  // mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
}
