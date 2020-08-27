#define UNICODE
#define IDB_START 3301
#define IDB_PAUSE 3302
#define IDB_END 3303
#define IDB_HOTKEY1 3304
#define IDB_HOTKEY2 3305
#define GETKEY_TIMER 1
#include <windows.h>
#include <windowsx.h>

#include <chrono>
#include <cstdlib>
#include <iostream>
#include <string>
#include <thread>
using namespace std;
void Clicker();
void TimerProc(HWND hwnd, UINT nmsg, UINT ntimerid, DWORD dwtime);
bool Click = false;
bool ClickFlag = false;

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wparam,
                         LPARAM lparam) {
  switch (message) {
    case WM_CREATE: {
      CreateWindow(L"Button", L"開始", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                   45, 260, 50, 30, hwnd, (HMENU)IDB_START, NULL, NULL);
      CreateWindow(L"Button", L"暫停",
                   WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | WS_DISABLED, 125,
                   260, 50, 30, hwnd, (HMENU)IDB_PAUSE, NULL, NULL);
      CreateWindow(L"Button", L"結束", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                   205, 260, 50, 30, hwnd, (HMENU)IDB_END, NULL, NULL);
      HWND HotKey1 =
          CreateWindow(L"COMBOBOX", L"IDB_HOTKEY1",
                       WS_VISIBLE | CBS_DROPDOWNLIST | WS_CHILD | WS_VSCROLL,
                       10, 10, 100, 200, hwnd, (HMENU)IDB_HOTKEY1, NULL, NULL);
      int n = 0;
      n = ComboBox_AddString(HotKey1, TEXT("Ctrl"));
      ComboBox_SetItemData(HotKey1, n, VK_CONTROL);
      n = ComboBox_AddString(HotKey1, TEXT("Alt"));
      ComboBox_SetItemData(HotKey1, n, VK_MENU);
      n = ComboBox_AddString(HotKey1, TEXT("Shift"));
      ComboBox_SetItemData(HotKey1, n, VK_SHIFT);
      ComboBox_SetCurSel(HotKey1, 0);
      HWND HotKey2 =
          CreateWindow(L"COMBOBOX", L"IDB_HOTKEY2",
                       WS_VISIBLE | CBS_DROPDOWNLIST | WS_CHILD | WS_VSCROLL,
                       120, 10, 100, 200, hwnd, (HMENU)IDB_HOTKEY2, NULL, NULL);

      SetTimer(hwnd, GETKEY_TIMER, 100, NULL);
      return 0;
    }
    case WM_CLOSE: {
      DestroyWindow(hwnd);
      return 0;
    }
    case WM_DESTROY: {
      KillTimer(hwnd, GETKEY_TIMER);
      PostQuitMessage(0);
      return 0;
    }
    case WM_COMMAND: {
      switch (HIWORD(wparam)) {
        case CBN_SELCHANGE: {
          SetFocus(hwnd);
          switch (LOWORD(wparam)) {
            case IDB_HOTKEY1: {
              switch (ComboBox_GetItemData((HWND)lparam,
                                           ComboBox_GetCurSel((HWND)lparam))) {
                case VK_CONTROL: {
                  MessageBox(hwnd, L"VK_CONTROL", L"CBN_SELCHANGE", MB_OK);
                  break;
                }
                case VK_MENU: {
                  MessageBox(hwnd, L"VK_MENU", L"CBN_SELCHANGE", MB_OK);
                  break;
                }
                case VK_SHIFT: {
                  MessageBox(hwnd, L"VK_SHIFT", L"CBN_SELCHANGE", MB_OK);
                  break;
                }
                default:
                  break;
              }
              break;
            }
            default:
              break;
          }
          return 0;
        }
        case BN_CLICKED: {
          SetFocus(hwnd);
          switch (LOWORD(wparam)) {
            case IDB_START: {
              Button_Enable(GetDlgItem(hwnd, IDB_START), false);
              Button_Enable(GetDlgItem(hwnd, IDB_PAUSE), true);
              Click = true;
              if (!ClickFlag) {
                thread ClickerThread(Clicker);
                ClickerThread.detach();
              }
              ClickFlag = true;
              break;
            }
            case IDB_PAUSE: {
              Button_Enable(GetDlgItem(hwnd, IDB_PAUSE), false);
              Button_Enable(GetDlgItem(hwnd, IDB_START), true);
              Click = false;
              ClickFlag = false;
              break;
            }
            case IDB_END: {
              DestroyWindow(hwnd);
              break;
            }
            default:
              break;
          }
          break;
        }
        default:
          break;
      }
      break;
    }
    case WM_TIMER: {
      if (GetAsyncKeyState(VK_CONTROL) & 0x8000 &&
          GetAsyncKeyState(VK_F1) & 0x8000) {
        SendMessage(hwnd, WM_COMMAND, IDB_START, 0);
      }
      if (GetAsyncKeyState(VK_CONTROL) & 0x8000 &&
          GetAsyncKeyState(VK_F2) & 0x8000) {
        SendMessage(hwnd, WM_COMMAND, IDB_PAUSE, 0);
      }
      if (GetAsyncKeyState(VK_CONTROL) & 0x8000 &&
          GetAsyncKeyState(VK_F3) & 0x8000) {
        SendMessage(hwnd, WM_COMMAND, IDB_END, 0);
      }
      break;
    }
    default:
      return DefWindowProc(hwnd, message, wparam, lparam);
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
    this_thread::sleep_for(chrono::milliseconds(1));
  }
  Click = false;
  ClickFlag = false;
}