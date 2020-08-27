#define UNICODE
#define IDB_START 3301
#define IDB_PAUSE 3302
#define IDB_END 3303
#define IDB_HOTKEYSTART1 3304
#define IDB_HOTKEYSTART2 3305
#define IDB_HOTKEYPAUSE1 3306
#define IDB_HOTKEYPAUSE2 3307
#define IDB_HOTKEYEND1 3308
#define IDB_HOTKEYEND2 3309
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
      int n = 0;
      CreateWindow(L"Button", L"開始", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                   45, 160, 50, 30, hwnd, (HMENU)IDB_START, NULL, NULL);
      CreateWindow(L"Button", L"暫停",
                   WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | WS_DISABLED, 125,
                   160, 50, 30, hwnd, (HMENU)IDB_PAUSE, NULL, NULL);
      CreateWindow(L"Button", L"結束", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                   205, 160, 50, 30, hwnd, (HMENU)IDB_END, NULL, NULL);
      HWND HOTKEYSTART1 = CreateWindow(
          L"COMBOBOX", L"IDB_HOTKEYSTART1",
          WS_VISIBLE | CBS_DROPDOWNLIST | WS_CHILD | WS_VSCROLL, 140, 10, 60,
          80, hwnd, (HMENU)IDB_HOTKEYSTART1, NULL, NULL);
      n = ComboBox_AddString(HOTKEYSTART1, TEXT("Ctrl"));
      ComboBox_SetItemData(HOTKEYSTART1, n, VK_CONTROL);
      n = ComboBox_AddString(HOTKEYSTART1, TEXT("Alt"));
      ComboBox_SetItemData(HOTKEYSTART1, n, VK_MENU);
      n = ComboBox_AddString(HOTKEYSTART1, TEXT("Shift"));
      ComboBox_SetItemData(HOTKEYSTART1, n, VK_SHIFT);
      ComboBox_SetCurSel(HOTKEYSTART1, 0);

      HWND HOTKEYSTART2 = CreateWindow(
          L"COMBOBOX", L"IDB_HOTKEYSTART2",
          WS_VISIBLE | CBS_DROPDOWNLIST | WS_CHILD | WS_VSCROLL, 230, 10, 60,
          260, hwnd, (HMENU)IDB_HOTKEYSTART2, NULL, NULL);
      n = ComboBox_AddString(HOTKEYSTART2, TEXT("F1"));
      ComboBox_SetItemData(HOTKEYSTART2, n, VK_F1);
      n = ComboBox_AddString(HOTKEYSTART2, TEXT("F2"));
      ComboBox_SetItemData(HOTKEYSTART2, n, VK_F2);
      n = ComboBox_AddString(HOTKEYSTART2, TEXT("F3"));
      ComboBox_SetItemData(HOTKEYSTART2, n, VK_F3);
      n = ComboBox_AddString(HOTKEYSTART2, TEXT("F4"));
      ComboBox_SetItemData(HOTKEYSTART2, n, VK_F4);
      n = ComboBox_AddString(HOTKEYSTART2, TEXT("F5"));
      ComboBox_SetItemData(HOTKEYSTART2, n, VK_F5);
      n = ComboBox_AddString(HOTKEYSTART2, TEXT("F6"));
      ComboBox_SetItemData(HOTKEYSTART2, n, VK_F6);
      n = ComboBox_AddString(HOTKEYSTART2, TEXT("F7"));
      ComboBox_SetItemData(HOTKEYSTART2, n, VK_F7);
      n = ComboBox_AddString(HOTKEYSTART2, TEXT("F8"));
      ComboBox_SetItemData(HOTKEYSTART2, n, VK_F8);
      n = ComboBox_AddString(HOTKEYSTART2, TEXT("F9"));
      ComboBox_SetItemData(HOTKEYSTART2, n, VK_F9);
      n = ComboBox_AddString(HOTKEYSTART2, TEXT("F10"));
      ComboBox_SetItemData(HOTKEYSTART2, n, VK_F10);
      n = ComboBox_AddString(HOTKEYSTART2, TEXT("F11"));
      ComboBox_SetItemData(HOTKEYSTART2, n, VK_F11);
      n = ComboBox_AddString(HOTKEYSTART2, TEXT("F12"));
      ComboBox_SetItemData(HOTKEYSTART2, n, VK_F12);
      ComboBox_SetCurSel(HOTKEYSTART2, 0);

      SetTimer(hwnd, GETKEY_TIMER, 100, NULL);
      return 0;
    }
    case WM_PAINT: {
      PAINTSTRUCT ps;
      HDC hdc = BeginPaint(hwnd, &ps);
      SetBkMode(hdc, TRANSPARENT);
      ExtTextOut(hdc, 10, 14, ETO_OPAQUE, NULL, L"開始快捷鍵設置：",
                 lstrlen(L"開始快捷鍵設置："), NULL);
      ExtTextOut(hdc, 207, 14, ETO_OPAQUE, NULL, L"＋", lstrlen(L"＋"), NULL);
      ExtTextOut(hdc, 10, 44, ETO_OPAQUE, NULL, L"暫停快捷鍵設置：",
                 lstrlen(L"暫停快捷鍵設置："), NULL);
      ExtTextOut(hdc, 207, 44, ETO_OPAQUE, NULL, L"＋", lstrlen(L"＋"), NULL);
      ExtTextOut(hdc, 10, 74, ETO_OPAQUE, NULL, L"結束快捷鍵設置：",
                 lstrlen(L"結束快捷鍵設置："), NULL);
      ExtTextOut(hdc, 207, 74, ETO_OPAQUE, NULL, L"＋", lstrlen(L"＋"), NULL);
      EndPaint(hwnd, &ps);
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
            case IDB_HOTKEYSTART1: {
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
  RECT rect = {0, 0, 300, 200};
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