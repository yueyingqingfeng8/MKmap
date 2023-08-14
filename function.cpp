#include "mkfun.h"

Mkfun::Mkfun(QObject *parent) :QObject(parent)
{

    m_Hook = new Hook(this);
    m_timer = new QTimer(this);

//    CALLBACKMOUSEFUN fun = std::bind(&Widget::showMouse, this, std::placeholders::_1, std::placeholders::_2);
//    m_Hook->setMouseCallbackFun(fun);

    initPosInfo();

    callbackFun_1 = std::bind(&Mkfun::effectFun_ONEWAY, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

}

Mkfun::~Mkfun()
{
    m_Hook->uninstallHook();

    delete m_Hook;
    delete m_timer;
}

int Mkfun::addMouseInfoEvent(int type, MSLLHOOKSTRUCT* mouseInfo)
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

    m_Hook->addSendInfo(info);

    return 1;
}

void Mkfun::installEffect(int fun)
{
    switch (fun)
    {
    case ONEWAY:
        m_Hook->setEffectFun(callbackFun_1);
        break;
    default:
        break;
    }
}

void Mkfun::initPosInfo()
{
    pos.setX(-1);
    pos.setY(-1);
    m_timer->stop();
    direction.clear();
}

int Mkfun::effectFun_ONEWAY(int& code, WPARAM& wParam, LPARAM& lParam)
{
    MSLLHOOKSTRUCT* pMouseStruct = (MSLLHOOKSTRUCT*)lParam;

    if(wParam == WM_RBUTTONDOWN)
    {
        if(rightKeyMkfun)
        {
            return 0;
        }
        if(!m_timer->isActive())
        {
            pos.setX(pMouseStruct->pt.x);
            pos.setY(pMouseStruct->pt.y);
            m_timer->start(INTERCEPT_TIME);
        }

        return 1;
    }

    if(wParam == WM_RBUTTONUP)
    {
        //右键被代替
        if(replaceRightKey)
        {
            replaceRightKey = false;
            return 1;
        }

        //右键功能
        if(rightKeyMkfun)
        {
            rightKeyMkfun = false;
            return 0;
        }

        if(m_timer->isActive())
        {
            initPosInfo();
        }

        rightKeyMkfun = true;
        addMouseInfoEvent(MOUSEEVENTF_RIGHTDOWN, pMouseStruct);
        addMouseInfoEvent(MOUSEEVENTF_RIGHTUP, pMouseStruct);

        return 1;
    }

    if(wParam == WM_MOUSEMOVE)
    {
//        dealMouseInfo("鼠标", pMouseStruct->pt.x * 10000 + pMouseStruct->pt.y);

        if(pos.x() == -1) return 0;

        if(m_timer->isActive())
        {
            judgeDirection(pMouseStruct);
        }
    }

    return 0;
}

void Mkfun::judgeDirection(MSLLHOOKSTRUCT *pMouseStruct)
{
    if(pow(pMouseStruct->pt.y - pos.y(), 2) + pow(pMouseStruct->pt.x - pos.x(), 2) < MOVE_DISTANCE * MOVE_DISTANCE)
    {
        return;
    }

    if(pMouseStruct->pt.y < pos.y() - JUDGE_DISTANCE)
    {
        //上面
        if(pMouseStruct->pt.x < pos.x() - JUDGE_DISTANCE)       //左上
        {
            addKeyboardInfoEvent("q", "", "");
        }
        else if(pMouseStruct->pt.x > pos.x() + JUDGE_DISTANCE)  //右上
        {
            addKeyboardInfoEvent("e", "", "");
        }
        else                                                    //上
        {
            addKeyboardInfoEvent("w", "", "");
        }
    }
    else if(pMouseStruct->pt.y > pos.y() + JUDGE_DISTANCE)
    {
        //下面
        if(pMouseStruct->pt.x < pos.x() - JUDGE_DISTANCE)       //左下
        {
            addKeyboardInfoEvent("z", "", "");
        }
        else if(pMouseStruct->pt.x > pos.x() + JUDGE_DISTANCE)  //右下
        {
            addKeyboardInfoEvent("c", "", "");
        }
        else                                                    //下
        {
            addKeyboardInfoEvent("ctrl", "alt", "c");
        }
    }
    else if(pMouseStruct->pt.x < pos.x() - JUDGE_DISTANCE)
    {
        addKeyboardInfoEvent("a", "", "");                      //左
    }
    else
    {
        addKeyboardInfoEvent("d", "", "");                      //右
    }

    replaceRightKey = true;
    initPosInfo();
}

int Mkfun::addKeyboardInfoEvent(QString key1, QString key2, QString key3)
{
    int key1_t = 0;
    int key2_t = 0;
    int key3_t = 0;

    SENDINFO info;
    memset(&info, 0x00, sizeof(SENDINFO));

    info.lenth = 0;
    ZeroMemory(info.inputs, sizeof(info.inputs));

    key1_t = convertKeyValue(key1);
    if(key1_t)
    {
        info.inputs[info.lenth].type = INPUT_KEYBOARD;
        info.inputs[info.lenth].ki.wVk = key1_t;
        info.lenth++;
    }

    key2_t = convertKeyValue(key2);
    if(key2_t)
    {
        info.inputs[info.lenth].type = INPUT_KEYBOARD;
        info.inputs[info.lenth].ki.wVk = key2_t;
        info.lenth++;
    }

    key3_t = convertKeyValue(key3);
    if(key3_t)
    {
        info.inputs[info.lenth].type = INPUT_KEYBOARD;
        info.inputs[info.lenth].ki.wVk = key3_t;
        info.lenth++;
    }

    key1_t = convertKeyValue(key1);
    if(key1_t)
    {
        info.inputs[info.lenth].type = INPUT_KEYBOARD;
        info.inputs[info.lenth].ki.wVk = key1_t;
        info.inputs[info.lenth].ki.dwFlags = KEYEVENTF_KEYUP;
        info.lenth++;
    }

    key2_t = convertKeyValue(key2);
    if(key2_t)
    {
        info.inputs[info.lenth].type = INPUT_KEYBOARD;
        info.inputs[info.lenth].ki.wVk = key2_t;
        info.inputs[info.lenth].ki.dwFlags = KEYEVENTF_KEYUP;
        info.lenth++;
    }

    key3_t = convertKeyValue(key3);
    if(key3_t)
    {
        info.inputs[info.lenth].type = INPUT_KEYBOARD;
        info.inputs[info.lenth].ki.wVk = key3_t;
        info.inputs[info.lenth].ki.dwFlags = KEYEVENTF_KEYUP;
        info.lenth++;
    }

    m_Hook->addSendInfo(info);

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

WORD Mkfun::convertKeyValue(QString key)
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

void Mkfun::showMouse(QString str, int num)
{

}

int Mkfun::installHook()
{
    return m_Hook->installHook();
}

int Mkfun::uninstallHook()
{
    return m_Hook->uninstallHook();
}
