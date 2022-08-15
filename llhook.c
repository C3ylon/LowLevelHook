#include <Windows.h>
#include <stdio.h>

#define SIMKEY(N) {\
            keybd_event(N, 0, 0, 0);\
            keybd_event(N, 0, KEYEVENTF_KEYUP, 0);\
        }

HHOOK hHook = NULL;

// #define SENDINPUT(VKCODE) {\
//     INPUT inputs[2] = { 0 };\
//     inputs[0].type = INPUT_KEYBOARD;\
//     inputs[0].ki.wVk = VKCODE;\
//     inputs[1].type = INPUT_KEYBOARD;\
//     inputs[1].ki.wVk = VKCODE;\
//     inputs[1].ki.dwFlags = KEYEVENTF_KEYUP;\
//     UINT uSent = SendInput(2, inputs, sizeof(INPUT));\
// }

#define SENDINPUT(VKCODE) {\
    INPUT input = { 0 };\
    input.type = INPUT_KEYBOARD;\
    input.ki.dwFlags = KEYEVENTF_SCANCODE;\
    input.ki.wVk = VKCODE;\
    input.ki.wScan = MapVirtualKey(VKCODE, 0);\
    SendInput(1, &input, sizeof(INPUT));\
    Sleep(10);\
    input.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;\
    SendInput(1, &input, sizeof(INPUT));\
    Sleep(10);\
}

int Event(DWORD vkCode) {
    if(vkCode == 0x41) {
        SENDINPUT(0x44);
        return 1;
    }
    if(vkCode == 0x44) {
        SENDINPUT(0x41);
        return 1;
    }
    if(vkCode == 0x43) {
        SENDINPUT(0x53);
        SENDINPUT(0x41);
        SENDINPUT(0x4A);
        return 1;
    }
    return 0;
}

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if( nCode >= 0 ) {
        KBDLLHOOKSTRUCT *keyinfo = (KBDLLHOOKSTRUCT *)lParam;
        if ( wParam == WM_KEYDOWN && Event(keyinfo->vkCode)) {
            return 1;
        }
    }
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

int SetHook(void) {
    hHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, 0, 0);
    if (hHook == NULL) return 1;
    printf("[+]hHOOK: %p\n", hHook);
    return 0;
}

void HookStop(void) {
    if( hHook ) {
        UnhookWindowsHookEx(hHook);
        hHook = NULL;
    }
}


int main(void) {	
    int wrongcode = SetHook();
    if(wrongcode) {
            printf("[!]set hook fail\n");
	        return -1;
    }
    MSG msg;
    while(GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    HookStop();
	return 0;
}
