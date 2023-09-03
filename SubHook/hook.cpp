#include "hook.h"

static HHOOK g_hHook = NULL;
static HOOK::EFFECT_FUN effectFun = NULL;
static HOOK::TIMER_FUN  timerFun = NULL;

static VOID CALLBACK timerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);

//static int addKeyboardInfoEvent(QString key, QString key2, QString key3);
//static WORD convertKeyValue(QString key);
//static void judgeDirection(MSLLHOOKSTRUCT* pMouseStruct);


//static int functionNum = 0;
//static int totalSteps = 3;
//static int currentSteps = 0;
//static QMap<QString, QList<QString>> keyMap;

Hook::Hook(int time)
{
    m_timerNum = 1;
    m_sendInfoTime = time;
    m_sendInfoList = new std::list<HOOK::SENDINFO>();

    timerFun = std::bind(&Hook::sendInfos, this);
    m_timer = SetTimer(NULL, m_timerNum, m_sendInfoTime, timerProc);

}

Hook::~Hook()
{
    KillTimer(NULL, m_timer);
    delete m_sendInfoList;
}

static VOID CALLBACK timerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
    if (timerFun)
        timerFun();
}

static LRESULT CALLBACK MouseHookCallback(int code, WPARAM wParam, LPARAM lParam)
{
    if (code == HC_ACTION)
    {
        if (effectFun != NULL)
            return effectFun(code, wParam, lParam);
    }

    return CallNextHookEx(g_hHook, code, wParam, lParam);
}

void Hook::addSendInfo(HOOK::SENDINFO& info)
{
    m_sendInfoList->push_back(info);
}

void Hook::sendInfos()
{
    if (!m_sendInfoList->empty())
    {
        HOOK::SENDINFO info = m_sendInfoList->front();
        m_sendInfoList->pop_front();

        SendInput(info.lenth, info.inputs, sizeof(INPUT));
    }
}

void Hook::setEffectFun(HOOK::EFFECT_FUN fun)
{
    effectFun = fun;
}

void Hook::setSendInfoTime(int time)
{
    KillTimer(NULL, m_timer);
    m_sendInfoTime = time;
    m_timer = SetTimer(NULL, m_timerNum, m_sendInfoTime, timerProc);
}

int Hook::installHook()
{
    g_hHook = SetWindowsHookEx(WH_MOUSE_LL, MouseHookCallback, NULL, 0);

    return g_hHook == NULL ? 0 : 1;
}

int Hook::uninstallHook()
{
    //成功，返回非0
    if(g_hHook != NULL)
    {
        HHOOK hook = g_hHook;
        g_hHook = NULL;
        return UnhookWindowsHookEx(hook);
    }

    return 1;
}

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





