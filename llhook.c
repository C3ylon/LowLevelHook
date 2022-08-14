
#include <Windows.h>
#include <stdio.h>

HHOOK hHook = NULL;

int Event(DWORD vkCode) {
    if(vkCode == 0x51) {
        keybd_event(0x53, 0, 0, 0);
        keybd_event(0x53, 0, KEYEVENTF_KEYUP, 0);
        keybd_event(0x44, 0, 0, 0);
        keybd_event(0x44, 0, KEYEVENTF_KEYUP, 0);
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
