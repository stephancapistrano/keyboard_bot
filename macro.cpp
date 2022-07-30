#include <Windows.h>
#include <stdio.h>

int running = false;
DWORD tid = 0;
HANDLE thread;


void mouse_click(void) {
    mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
    mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
}

void clickF1(void){
    keybd_event(VkKeyScan(VK_F1), 0, 0, 0);
    Sleep(300);
    keybd_event(VkKeyScan(VK_F1), 0, KEYEVENTF_KEYUP, 0);
}

void clickSpace(void){
    keybd_event(VkKeyScan(VK_SPACE), 0, 0, 0);
    Sleep(300);
    keybd_event(VkKeyScan(VK_SPACE), 0, KEYEVENTF_KEYUP, 0);
}

DWORD WINAPI MyThreadFunction(LPVOID lpParam) {
    int* r = (int*)lpParam;
    while (*r) {
        clickF1();
        Sleep(1000);
        clickSpace();
        Sleep(250000);
    }
    return 0;
}

LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    bool fEatKeyStroke = false;

    if (nCode == HC_ACTION) {
        switch (wParam) {
        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
        case WM_KEYUP:
        case WM_SYSKEYUP:
            PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT)lParam;
            if (fEatKeyStroke = (p->vkCode == VK_LCONTROL)) {
                if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) {

                    if (running == 0) {
                        printf("Autoclicker started. Press F6 to stop it.\n");
                        running = 1;
                        thread = CreateThread(NULL, 0, MyThreadFunction, &running, 0, &tid);
                    }
                    else {
                        running = 0;
                        CloseHandle(thread);
                        printf("Autoclicker stopped.\n");
                    }
                }
                else if (wParam == WM_KEYUP || wParam == WM_SYSKEYUP) {
                    // nothing
                }
                break;
            }
            break;
        }
    }

    return (fEatKeyStroke ? 1 : CallNextHookEx(NULL, nCode, wParam, lParam));
}


int main(void)
{
    HHOOK hhkLowLevelKybd = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, 0, 0);
    MSG msg;

    while (!GetMessage(&msg, NULL, NULL, NULL)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    UnhookWindowsHookEx(hhkLowLevelKybd);
    return 0;
}