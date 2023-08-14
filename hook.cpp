#include "hook.h"
#include <QPoint>
#include <QTimer>
#include <QMap>

static HHOOK g_hHook = NULL;
static EFFECT_FUN effectFun = NULL;

//static int addKeyboardInfoEvent(QString key, QString key2, QString key3);
//static WORD convertKeyValue(QString key);
//static void judgeDirection(MSLLHOOKSTRUCT* pMouseStruct);


//static int functionNum = 0;
//static int totalSteps = 3;
//static int currentSteps = 0;
//static QMap<QString, QList<QString>> keyMap;

Hook::Hook(QObject *parent) :QObject(parent)
{
    m_timer = new QTimer(this);
    sendInfoList = new QList<SENDINFO>;

    connect(m_timer, &QTimer::timeout, this, &Hook::sendInfos);

    m_timer->start(100);
}

Hook::~Hook()
{
    m_timer->stop();

    if(sendInfoList != NULL)
    {
        sendInfoList->clear();
    }

    delete sendInfoList;
    delete m_timer;
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

LRESULT CALLBACK MouseHookCallback(int code, WPARAM wParam, LPARAM lParam)
{
    if (code == HC_ACTION)
    {
        if(effectFun != NULL)
        {
            if(effectFun(code, wParam, lParam))
            {
                return 1;
            }
        }

//        if(gestureControl_unidirectional(wParam, lParam))
//        {

//        }

//        if(gestureControl_turn(wParam, lParam))
//        {
//            return 1;
//        }
    }

    return CallNextHookEx(g_hHook, code, wParam, lParam);
}

void Hook::addSendInfo(SENDINFO& info)
{
    sendInfoList->append(info);
}

void Hook::sendInfos()
{
    if(sendInfoList->count())
    {
        SENDINFO info = sendInfoList->takeFirst();

        SendInput(info.lenth, info.inputs, sizeof(INPUT));
    }
}


void Hook::setEffectFun(EFFECT_FUN fun)
{
    effectFun = fun;
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
        if(UnhookWindowsHookEx(g_hHook))
        {
            g_hHook = NULL;
            return 1;
        }
        else
        {
            g_hHook = NULL;
            return 0;
        }
    }

    return 1;
}








