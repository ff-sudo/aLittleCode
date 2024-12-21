// GlobalHook.h
#ifndef GLOBALHOOK_H
#define GLOBALHOOK_H

#include <windows.h>
#include <QDebug>

#include "mc.h"

class GlobalHook
{
public:
    GlobalHook(){
        m_hHook = SetWindowsHookEx(WH_MOUSE_LL, LowLevelMouseProc, NULL, 0);
        if (m_hHook == NULL) {
            qDebug() << "Failed to install hook!";
        }
    }
    ~GlobalHook(){
        UnhookWindowsHookEx(m_hHook);

    }

private:
    HHOOK m_hHook;

    static LRESULT CALLBACK LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam){
            if (nCode >= 0) {
                MSLLHOOKSTRUCT *pMouseStruct = (MSLLHOOKSTRUCT*)lParam;
                if (wParam == WM_LBUTTONDOWN) {
                    qDebug() << "Left mouse button down at:" << pMouseStruct->pt.x << pMouseStruct->pt.y;
                    MC::SetLpPt(pMouseStruct->pt.x, pMouseStruct->pt.y);
                } else if (wParam == WM_MOUSEMOVE) {
                    qDebug() << "Mouse moved to:" << pMouseStruct->pt.x << pMouseStruct->pt.y;
                    MC::SetMovePt(pMouseStruct->pt.x, pMouseStruct->pt.y);
                }
        }
        return CallNextHookEx(NULL, nCode, wParam, lParam);
    }
};

#endif // GLOBALHOOK_H