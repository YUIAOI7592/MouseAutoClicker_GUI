#define UNICODE
#define IDB_START 3301
#define IDB_PAUSE 3302
#define IDB_END 3303
#include <windows.h>
#include <windowsx.h>

#include <cstdlib>
#include <iostream>
#include <thread>
using namespace std;
void Clicker();
bool Click = false;
bool ClickFlag = false;
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wparam,
                         LPARAM lparam) {
  switch (message) {
    case WM_CREATE: {
      HWND Shwnd = CreateWindow(L"Button", L"開始",
                                WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 45, 260,
                                50, 30, hwnd, (HMENU)IDB_START, NULL, NULL);
      HWND Phwnd = CreateWindow(L"Button", L"暫停",
                                WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 125, 260,
                                50, 30, hwnd, (HMENU)IDB_PAUSE, NULL, NULL);
      HWND Ehwnd = CreateWindow(L"Button", L"結束",
                                WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 205, 260,
                                50, 30, hwnd, (HMENU)IDB_END, NULL, NULL);
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
        case IDB_START: {
          Click = true;
          if (!ClickFlag) {
            thread ClickerThread(Clicker);
            ClickerThread.detach();
          }
          ClickFlag = true;
          return 0;
        }
        case IDB_PAUSE: {
          Click = false;
          return 0;
        }
        case IDB_END: {
          PostQuitMessage(0);
          return 0;
        }
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
  }
  return msg.wParam;
}
void Clicker() {
  enum sendInputs { MOUSE_CLICK, NUM_INPUTS };
  INPUT inputs[NUM_INPUTS];
  inputs[MOUSE_CLICK].type = INPUT_MOUSE;
  inputs[MOUSE_CLICK].mi.dwFlags = MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP;
  while (Click && ClickFlag) {
    SendInput(NUM_INPUTS, inputs, sizeof(inputs));
    Sleep(1);
  }
  Click = false;
  ClickFlag = false;
}