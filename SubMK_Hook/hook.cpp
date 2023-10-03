#include "hook.h"
#include <QDebug>


Hook* Hook::s_hook = NULL;
static HHOOK g_mouseHook = NULL;
static HHOOK g_keyboardHook = NULL;

static void CALLBACK timerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);


//static int functionNum = 0;
//static int totalSteps = 3;
//static int currentSteps = 0;
//static QMap<QString, QList<QString>> keyMap;

Hook::Hook(int time)
    : m_timerNum(1), m_sendInfoTime(time),m_mouseEffectFun(NULL)
{
    m_sendInfoList = new std::list<HOOK::SENDINFO>();

    m_timer = SetTimer(NULL, m_timerNum, m_sendInfoTime, timerProc);

    KeysMap::InitKeysMap();     //必须初始化按键名称和数值的静态对象
}

Hook::~Hook()
{
    KillTimer(NULL, m_timer);
    delete m_sendInfoList;
}

static void CALLBACK timerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
    Hook::sendInfos();
}

LRESULT CALLBACK KeyboardHookCallback(int code, WPARAM wParam, LPARAM lParam)
{
    if (code == HC_ACTION)
    {
        if (Hook::runKeyboardEfffectFun(wParam, (KBDLLHOOKSTRUCT*)lParam))
            return 1;
    }

    return CallNextHookEx(g_keyboardHook, code, wParam, lParam);
}

static LRESULT CALLBACK MouseHookCallback(int code, WPARAM wParam, LPARAM lParam)
{
    if (code == HC_ACTION)
    {
        if (Hook::runMouseEffectFun(wParam, (MSLLHOOKSTRUCT*)lParam))
            return 1;
    }

    return CallNextHookEx(g_mouseHook, code, wParam, lParam);
}

void Hook::addSendInfo(HOOK::SENDINFO& info)
{
    m_sendInfoList->push_back(info);
}

void Hook::inSendInfos()
{
    if (!m_sendInfoList->empty())
    {
        HOOK::SENDINFO info = m_sendInfoList->front();
        m_sendInfoList->pop_front();

        addSentKeys(info);
        SendInput(info.data.size(), info.data.data(), sizeof(INPUT));
    }
}

void Hook::addSentKeys(HOOK::SENDINFO& info)
{
    for (unsigned int i = 0; i < info.data.size(); i++)
    {
        //键盘按键信息进行数字组装
        if (info.data[i].type == INPUT_KEYBOARD)
        {
            if (info.data[i].ki.dwFlags == KEYEVENTF_KEYUP)
                keySet.insert(info.data[i].ki.wVk * 1000 + WM_KEYUP);
            else
                keySet.insert(info.data[i].ki.wVk * 1000 + WM_KEYDOWN);
        }

        //鼠标按键信息进行数字组装
        if (info.data[i].type == INPUT_MOUSE)
        {
            switch (info.data[i].mi.dwFlags)
            {
            case MOUSEEVENTF_RIGHTDOWN:
                keySet.insert(SPLICE_NUM_3(info.data[i].mi.dx, info.data[i].mi.dy, WM_RBUTTONDOWN));
                break;
            case MOUSEEVENTF_RIGHTUP:
                keySet.insert(SPLICE_NUM_3(info.data[i].mi.dx, info.data[i].mi.dy, WM_RBUTTONUP));
                break;
            default:
                break;
            }
        }
    }
}

bool Hook::skipKeyDeal(WPARAM& wParam, int& key)
{
    //将多按键的数字编号和状态组合起来
    int num = 0;
    if (wParam == WM_KEYDOWN)
        num = key * 1000 + WM_KEYDOWN;
    else if (wParam == WM_KEYUP)
        num = key * 1000 + WM_KEYUP;
    else
        return true;

    //检查一下是自己发送的按键则,跳过后续处理
    auto it = keySet.find(num);
    if (it != keySet.end())
    {
        keySet.erase(it);
        return true;
    }

    return false;
}

bool Hook::skipMouseDeal(WPARAM& type, Point &mousePos)
{
    int num = 0;

    //需要处理鼠标移动,不能跳过,键盘只处理按下和升起
    switch (type)
    {
    case WM_RBUTTONDOWN:
        num = SPLICE_NUM_3(mousePos.x, mousePos.y, WM_RBUTTONDOWN);
        break;
    case WM_RBUTTONUP:
        num = SPLICE_NUM_3(mousePos.x, mousePos.y, WM_RBUTTONUP);
        break;
    default:
        return false;
    }

    //能找到说明是自己发送的信息,跳过后续处理
    auto it = keySet.find(num);
    if (it != keySet.end())
    {
        keySet.erase(it);
        return true;
    }

    return false;
}

void Hook::inSetSendInfoTime(const int time)
{
    KillTimer(NULL, m_timer);
    m_sendInfoTime = time;
    m_timer = SetTimer(NULL, m_timerNum, m_sendInfoTime, timerProc);
}

void Hook::inSetEffectFun(HOOK::MOUSE_EFFECT_FUN& mouseFun, HOOK::KEYBOARD_EFFECT_FUN& keyboardFun)
{
    mouseFun != NULL ? m_mouseEffectFun = mouseFun : NULL;
    keyboardFun != NULL ? m_keyboardEffectFun = keyboardFun : NULL;
}

int Hook::inRunMouseEffectFun(WPARAM& wParam, Point&& pos)
{
    if (NULL == m_mouseEffectFun) return 0;

    if (skipMouseDeal(wParam, pos)) return 0;

    return m_mouseEffectFun(wParam, pos);
}

int Hook::inRunKeyboardEfffectFun(WPARAM& wParam, int& key)
{
    if (NULL == m_mouseEffectFun) return 0;

    if (skipKeyDeal(wParam, key)) return 0;

    return m_keyboardEffectFun(wParam, key);
}

int Hook::inInstallMouseHook()
{
    if (g_mouseHook != NULL) return 1;

    g_mouseHook = SetWindowsHookEx(WH_MOUSE_LL, MouseHookCallback, NULL, 0);

    return g_mouseHook != NULL ? 1 : 0;
}

int Hook::inInstallKeyboardHook()
{
    if (g_keyboardHook != NULL) return 1;

    g_keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardHookCallback, NULL, 0);

    return g_keyboardHook != NULL ? 1 : 0;
}

int Hook::inUninstallMouseHook()
{
    //成功，返回非0
    if(g_mouseHook != NULL)
    {
        HHOOK hook = g_mouseHook;
        g_mouseHook = NULL;

        return UnhookWindowsHookEx(hook);
    }

    return 1;
}

int Hook::inUninstallKeyboardHook()
{
    //成功，返回非0
    if(g_keyboardHook != NULL)
    {
        HHOOK hook = g_keyboardHook;
        g_keyboardHook = NULL;

        return UnhookWindowsHookEx(hook);
    }

    return 1;
}

//需修改为可以输入不同鼠标事件的函数
int Hook::inAddMouseInfoEvent(int type, Point& mousePos)
{
    HOOK::SENDINFO info;
    info.data.reserve(1);

    INPUT input;
    ZeroMemory(&input, sizeof(INPUT));

    input.type = INPUT_MOUSE;
    input.mi.dx = mousePos.x;
    input.mi.dy = mousePos.y;;
    input.mi.mouseData = 0;
    input.mi.dwFlags = type;
    input.mi.time = 0;
    input.mi.dwExtraInfo = 0;

    info.data.push_back(input);

    addSendInfo(info);

    return 1;
}

int Hook::inAddKeyboardEventShortcutKey(std::list<int>& keysList)
{
    if (!keysList.empty())
    {
        HOOK::SENDINFO info;

        info.data.reserve(keysList.size() * 2);

        for (auto& key : keysList)
        {
            INPUT input1;
            ZeroMemory(&input1, sizeof(INPUT));

            input1.type = INPUT_KEYBOARD;
            input1.ki.wVk = key;
            info.data.push_back(input1);
        }

        for (auto& key : keysList)
        {
            INPUT input2;
            ZeroMemory(&input2, sizeof(INPUT));

            input2.type = INPUT_KEYBOARD;
            input2.ki.wVk = key;
            input2.ki.dwFlags = KEYEVENTF_KEYUP;
            info.data.push_back(input2);
        }

        addSendInfo(info);
    }

    return 1;
}

int Hook::inAddKeyboardEventString(std::list<int>& keysList)
{
    if (!keysList.empty())
    {
        HOOK::SENDINFO info;

        info.data.reserve(keysList.size() * 2);

        for (auto& key : keysList)
        {
            //shift下的按键
            if(key >= 1000)
            {
                int key1 = key / 1000;
                int key2 = key % 1000;

                //按键按下
                INPUT input1;
                ZeroMemory(&input1, sizeof(INPUT));

                input1.type = INPUT_KEYBOARD;
                input1.ki.wVk = key1;
                info.data.push_back(input1);

                input1.type = INPUT_KEYBOARD;
                input1.ki.wVk = key2;
                info.data.push_back(input1);

                //按键升起
                INPUT input2;
                ZeroMemory(&input2, sizeof(INPUT));

                input2.type = INPUT_KEYBOARD;
                input2.ki.wVk = key1;
                input2.ki.dwFlags = KEYEVENTF_KEYUP;
                info.data.push_back(input2);

                input2.type = INPUT_KEYBOARD;
                input2.ki.wVk = key2;
                input2.ki.dwFlags = KEYEVENTF_KEYUP;
                info.data.push_back(input2);

            }
            else
            {
                //按键按下
                INPUT input1;
                ZeroMemory(&input1, sizeof(INPUT));

                input1.type = INPUT_KEYBOARD;
                input1.ki.wVk = key;
                info.data.push_back(input1);

                //按键升起
                INPUT input2;
                ZeroMemory(&input2, sizeof(INPUT));

                input2.type = INPUT_KEYBOARD;
                input2.ki.wVk = key;
                input2.ki.dwFlags = KEYEVENTF_KEYUP;
                info.data.push_back(input2);
            }
        }

        addSendInfo(info);
    }

    return 1;
}

Hook* Hook::initHook(int&& time)
{
    if (s_hook == NULL){
        s_hook = new Hook(time);
    }
    return s_hook;
}
int Hook::installHook(bool mouseHook, bool keyboardHook)
{
    if (mouseHook && keyboardHook)
        return s_hook->inInstallMouseHook() && s_hook->inInstallKeyboardHook();
    if (mouseHook && !keyboardHook)
        return s_hook->inInstallMouseHook();
    if (!mouseHook && keyboardHook)
        return s_hook->inInstallKeyboardHook();

    return 0;
}

int Hook::uninstallHook(bool mouseHook, bool keyboardHook)
{
    if (mouseHook && keyboardHook)
        return s_hook->inUninstallMouseHook() && s_hook->inUninstallKeyboardHook();
    if (mouseHook && !keyboardHook)
        return s_hook->inUninstallMouseHook();
    if (!mouseHook && keyboardHook)
        return s_hook->inUninstallKeyboardHook();

    return 0;
}

void Hook::setSendInfoTime(int time)
{
    return s_hook->inSetSendInfoTime(time);
}

void Hook::setEffectFun(HOOK::MOUSE_EFFECT_FUN mouseFun, HOOK::KEYBOARD_EFFECT_FUN keyboardFun)
{
    s_hook->inSetEffectFun(mouseFun, keyboardFun);
}

int Hook::runMouseEffectFun(WPARAM& wParam, MSLLHOOKSTRUCT *mouseStruct)
{
    return s_hook->inRunMouseEffectFun(wParam, Point(mouseStruct->pt.x, mouseStruct->pt.y));
}

int Hook::runKeyboardEfffectFun(WPARAM& wParam, KBDLLHOOKSTRUCT* keyboardStruct)
{
    return s_hook->inRunKeyboardEfffectFun(wParam, (int&)keyboardStruct->vkCode);
}

void Hook::sendInfos()
{
    s_hook->inSendInfos();
}

int Hook::addMouseInfoEvent(int type, Point &mousePos)
{
    return s_hook->inAddMouseInfoEvent(type, mousePos);
}

int Hook::addKeyboardEventShortcutKey(std::list<int>& keysList)
{
    return s_hook->inAddKeyboardEventShortcutKey(keysList);
}

int Hook::addKeyboardEventString(std::list<int>& keysList)
{
    return s_hook->inAddKeyboardEventString(keysList);
}

//鼠标多方向指令判断
#if 0
int gestureControl_turn(WPARAM wParam, LPARAM lParam)
{
    MSLLHOOKSTRUCT* pMouseStruct = (MSLLHOOKSTRUCT*)lParam;

    if(wParam == WM_RBUTTONDOWN)
    {
        if(rightKeyFunction)
        {
            return 0;
        }
        if(!timer.isActive())
        {
            pos.setX(pMouseStruct->pt.x);
            pos.setY(pMouseStruct->pt.y);
            timer.start(INTERCEPT_TIME);
            currentSteps = 0;
            direction.clear();
        }
        return 1;
    }

    if(wParam == WM_RBUTTONUP)
    {
        if(currentSteps != 0 && !direction.isEmpty())
        {
            replaceRightKey = true;
            findAndSendKeys();
            direction.clear();
            currentSteps = 0;

            if(timer.isActive())
            {
                initPosInfo();
                currentSteps = 0;
            }
        }

        //右键被代替
        if(replaceRightKey)
        {
            replaceRightKey = false;
            return 1;
        }

        //右键功能
        if(rightKeyFunction)
        {
            rightKeyFunction = false;
            return 0;
        }

        if(timer.isActive())
        {
            initPosInfo();
            currentSteps = 0;
        }

        rightKeyFunction = true;
        addMouseInfoEvent(MOUSEEVENTF_RIGHTDOWN, pMouseStruct);
        addMouseInfoEvent(MOUSEEVENTF_RIGHTUP, pMouseStruct);

        return 1;
    }

    if(wParam == WM_MOUSEMOVE)
    {
        if(pos.x() != -1 && pos.y() != -1)
        {
            if(timer.isActive())
            {
                if(currentSteps < totalSteps)
                {
                    if(pMouseStruct->pt.y < pos.y() - 100)
                    {
                        pos.setX(pMouseStruct->pt.x);
                        pos.setY(pMouseStruct->pt.y);
                        direction.append("U");
                        currentSteps++;
                    }
                    else if(pMouseStruct->pt.y > pos.y() + 100)
                    {
                        pos.setX(pMouseStruct->pt.x);
                        pos.setY(pMouseStruct->pt.y);
                        direction.append("D");
                        currentSteps++;
                    }
                    else if(pMouseStruct->pt.x < pos.x() - 100)
                    {
                        pos.setX(pMouseStruct->pt.x);
                        pos.setY(pMouseStruct->pt.y);
                        direction.append("L");
                        currentSteps++;
                    }
                    else if(pMouseStruct->pt.x > pos.x() + 100)
                    {
                        pos.setX(pMouseStruct->pt.x);
                        pos.setY(pMouseStruct->pt.y);
                        direction.append("R");
                        currentSteps++;
                    }
                }
            }

            if(currentSteps >= totalSteps || !timer.isActive())
            {
                timer.stop();
                if(!direction.isEmpty())
                {
                    replaceRightKey = true;
                    findAndSendKeys();
                    direction.clear();
                    currentSteps = 0;
                }
            }
        }

    }

    return 0;
}

void findAndSendKeys()
{
    dealMouseInfo(direction + "    ", 1 * 10000 + 2);
    if(direction == "U")
    {
        addKeyboardInfoEvent("u", "", "");
    }
    else if(direction == "R")
    {
        addKeyboardInfoEvent("r", "", "");
    }
    else if(direction == "L")
    {
        addKeyboardInfoEvent("l", "", "");
    }
    else if(direction == "D")
    {
        addKeyboardInfoEvent("d", "", "");
    }
    else if(direction == "DR")
    {
        addKeyboardInfoEvent("d", "r", "");
    }
    else if(direction == "DL")
    {
        addKeyboardInfoEvent("d", "l", "");
    }
    else if(direction == "DU")
    {
        addKeyboardInfoEvent("d", "u", "");
    }
    else if(direction == "DD")
    {
        addKeyboardInfoEvent("d", "d", "");
    }
    else
    {
        addKeyboardInfoEvent("s", "", "");
    }
}

int gestureControl_click(WPARAM wParam, LPARAM lParam)
{
    //如果允许进行处理，则返回非零值表示拦截该事件，不让它继续传递
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
#endif

