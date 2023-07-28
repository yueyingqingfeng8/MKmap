#include "hook.h"

static HHOOK g_hHook = NULL;

CALLBACKMOUSEFUN dealMouseInfo = NULL;
static QList<SENDINFO>* sendInfoList = new QList<SENDINFO>;

static int addMouseInfoEvent(int type, MSLLHOOKSTRUCT* mouseInfo);
static int addKeyboardInfoEvent(QString key, QString key2, QString key3);
static WORD convertKeyValue(QString key);

Hook::Hook(QObject *parent) :QObject(parent)
{

}

Hook::~Hook()
{
    if(sendInfoList != NULL)
    {
        delete sendInfoList;
    }
}

static void addSendInfo(SENDINFO info)
{
    sendInfoList->append(info);
}

LRESULT CALLBACK MouseHookCallback(int code, WPARAM wParam, LPARAM lParam)
{
    // 鼠标按下左键
    if (WM_LBUTTONDOWN == wParam)
    {
        dealMouseInfo("左键", 1);
    }

    if (WM_RBUTTONDOWN == wParam)
    {
        dealMouseInfo("右键", 2);
    }

    //如果允许进行处理，则返回非零值表示拦截该事件，不让它继续传递
    if (code == HC_ACTION)
    {
        MSLLHOOKSTRUCT* pMouseStruct = (MSLLHOOKSTRUCT*)lParam;

        if (wParam == WM_RBUTTONDOWN)
        {
//            addMouseInfoEvent(MOUSEEVENTF_LEFTDOWN, pMouseStruct);

            return 1;
        }


        if (wParam == WM_RBUTTONUP)
        {
//            addMouseInfoEvent(MOUSEEVENTF_LEFTUP, pMouseStruct);

            addKeyboardInfoEvent("ctrl", "alt", "c");

            return 1;
        }

        //双击
        if (wParam == WM_RBUTTONDBLCLK)
        {
            return 1;
        }
    }

    return CallNextHookEx(g_hHook, code, wParam, lParam);
}

void Hook::sendInfos()
{
    if(sendInfoList->count())
    {
        SENDINFO info = sendInfoList->takeFirst();

        SendInput(info.lenth, info.inputs, sizeof(INPUT));
    }
}

void Hook::setMouseCallbackFun(CALLBACKMOUSEFUN fun)
{
    dealMouseInfo = fun;
}

int Hook::installHook()
{
    g_hHook = SetWindowsHookEx(WH_MOUSE_LL, MouseHookCallback, NULL, 0);
    if(g_hHook == NULL)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

int Hook::uninstallHook()
{
    //成功，返回非0
    if(g_hHook != NULL)
    {
        return UnhookWindowsHookEx(g_hHook);
    }

    return 1;
}

int addMouseInfoEvent(int type, MSLLHOOKSTRUCT* mouseInfo)
{
    SENDINFO info;
    memset(&info, 0x00, sizeof(SENDINFO));

    info.lenth = 1;
    ZeroMemory(info.inputs, sizeof(info.inputs));

    info.inputs[0].type = INPUT_MOUSE;
    info.inputs[0].mi.dx = mouseInfo->pt.x;
    info.inputs[0].mi.dy = mouseInfo->pt.y;;
    info.inputs[0].mi.mouseData = 0;
    info.inputs[0].mi.dwFlags = type;
    info.inputs[0].mi.time = 0;
    info.inputs[0].mi.dwExtraInfo = 0;

    addSendInfo(info);

    return 1;
}

int addKeyboardInfoEvent(QString key1, QString key2, QString key3)
{
    int key1_t = 0;
    int key2_t = 0;
    int key3_t = 0;

    SENDINFO info;
    memset(&info, 0x00, sizeof(SENDINFO));

    info.lenth = 0;
    ZeroMemory(info.inputs, sizeof(info.inputs));

    if(key1_t = convertKeyValue(key1))
    {
        info.inputs[info.lenth].type = INPUT_KEYBOARD;
        info.inputs[info.lenth].ki.wVk = key1_t;
        info.lenth++;
    }
    if(key2_t = convertKeyValue(key2))
    {
        info.inputs[info.lenth].type = INPUT_KEYBOARD;
        info.inputs[info.lenth].ki.wVk = key2_t;
        info.lenth++;
    }
    if(key3_t = convertKeyValue(key3))
    {
        info.inputs[info.lenth].type = INPUT_KEYBOARD;
        info.inputs[info.lenth].ki.wVk = key3_t;
        info.lenth++;
    }

    if(key1_t = convertKeyValue(key1))
    {
        info.inputs[info.lenth].type = INPUT_KEYBOARD;
        info.inputs[info.lenth].ki.wVk = key1_t;
        info.inputs[info.lenth].ki.dwFlags = KEYEVENTF_KEYUP;
        info.lenth++;
    }
    if(key2_t = convertKeyValue(key2))
    {
        info.inputs[info.lenth].type = INPUT_KEYBOARD;
        info.inputs[info.lenth].ki.wVk = key2_t;
        info.inputs[info.lenth].ki.dwFlags = KEYEVENTF_KEYUP;
        info.lenth++;
    }
    if(key3_t = convertKeyValue(key3))
    {
        info.inputs[info.lenth].type = INPUT_KEYBOARD;
        info.inputs[info.lenth].ki.wVk = key3_t;
        info.inputs[info.lenth].ki.dwFlags = KEYEVENTF_KEYUP;
        info.lenth++;
    }

    addSendInfo(info);

    return 0;

#if 0  //和如下效果类似
    INPUT inputs[6] = {};
    ZeroMemory(inputs, sizeof(inputs));

    inputs[0].type = INPUT_KEYBOARD;
    inputs[0].ki.wVk = VK_CONTROL;

    inputs[1].type = INPUT_KEYBOARD;
    inputs[1].ki.wVk = VK_SHIFT;

    inputs[2].type = INPUT_KEYBOARD;
    inputs[2].ki.wVk = 'O';


    inputs[3].type = INPUT_KEYBOARD;
    inputs[3].ki.wVk = VK_CONTROL;
    inputs[3].ki.dwFlags = KEYEVENTF_KEYUP;

    inputs[4].type = INPUT_KEYBOARD;
    inputs[4].ki.wVk = VK_SHIFT;
    inputs[4].ki.dwFlags = KEYEVENTF_KEYUP;

    inputs[5].type = INPUT_KEYBOARD;
    inputs[5].ki.wVk =  'o';
    inputs[5].ki.dwFlags = KEYEVENTF_KEYUP;
#endif
}

WORD convertKeyValue(QString key)
{
    if(key == "shift") return VK_SHIFT;

    if(key == "ctrl") return VK_CONTROL;

    if(key == "alt") return VK_MENU;

    if(key == " ") return VK_SPACE;

    if(key == "tab") return VK_TAB;

    if(key == "a") return 65;
    if(key == "b") return 66;
    if(key == "c") return 67;
    if(key == "d") return 68;
    if(key == "e") return 69;
    if(key == "f") return 70;
    if(key == "g") return 71;
    if(key == "h") return 72;
    if(key == "i") return 73;
    if(key == "j") return 74;
    if(key == "k") return 75;
    if(key == "l") return 76;
    if(key == "m") return 77;
    if(key == "n") return 78;
    if(key == "o") return 79;
    if(key == "p") return 80;
    if(key == "q") return 81;
    if(key == "r") return 82;
    if(key == "s") return 83;
    if(key == "t") return 84;
    if(key == "u") return 85;
    if(key == "v") return 86;
    if(key == "w") return 87;
    if(key == "x") return 88;
    if(key == "y") return 89;
    if(key == "z") return 90;

    return 0;
}





