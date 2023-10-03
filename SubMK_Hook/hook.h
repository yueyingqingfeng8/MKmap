#ifndef HOOK_H
#define HOOK_H

#include "utility.h"

#define SENDINFOTIME 30

#define SPLICE_NUM_3(num1, num2, num3)    (((num1) * 1000 + (num2)) * 1000 + (num3))

namespace HOOK {

    enum KeysType
    {
        SHORTCUT_KET,
        STRING
    };

    typedef std::function<int(WPARAM, Point)>  MOUSE_EFFECT_FUN;
    typedef std::function<int(WPARAM, int)>   KEYBOARD_EFFECT_FUN;

    typedef struct sendInfo {
        std::vector<INPUT> data;
    } SENDINFO;
}

class Hook
{
public:
    static Hook* initHook(int&& time = 10);

    //安装卸载钩子
    static int installHook(bool mouseHook, bool keyboardHook);
    static int uninstallHook(bool mouseHook, bool keyboardHook);

    //设置钩子的回调函数\发送按键信息时间间隔
    static void setSendInfoTime(int time);
    static void setEffectFun(HOOK::MOUSE_EFFECT_FUN mouseFun, HOOK::KEYBOARD_EFFECT_FUN keyboardFun);

    //运行钩子的回调函数
    static int runMouseEffectFun(WPARAM& wParam, MSLLHOOKSTRUCT* mouseStruct);
    static int runKeyboardEfffectFun(WPARAM& wParam, KBDLLHOOKSTRUCT *keyboardStruct);

    //增加鼠标和键盘事件
    static int addMouseInfoEvent(int type, Point &mousePos);
    static int addKeyboardEventShortcutKey(std::list<int>& keysList);
    static int addKeyboardEventString(std::list<int>& keysList);

    static void sendInfos();

private:
    Hook(int time = SENDINFOTIME);
    ~Hook();

    Hook(const Hook&, int) = delete;
    Hook& operator=(const Hook&) = delete;

    int inInstallMouseHook();
    int inInstallKeyboardHook();
    int inUninstallMouseHook();
    int inUninstallKeyboardHook();

    void inSetSendInfoTime(const int time);
    void inSetEffectFun(HOOK::MOUSE_EFFECT_FUN& mouseFun, HOOK::KEYBOARD_EFFECT_FUN& keyboardFun);

    int inRunMouseEffectFun(WPARAM& wParam, Point&& pos);
    int inRunKeyboardEfffectFun(WPARAM& wParam, int &key);

    int inAddMouseInfoEvent(int type, Point &mousePos);
    int inAddKeyboardEventShortcutKey(std::list<int>& keysList);
    int inAddKeyboardEventString(std::list<int>& keysList);

    void inSendInfos();

    void addSendInfo(HOOK::SENDINFO& info);

    bool skipKeyDeal(WPARAM& wParam, int &key);
    bool skipMouseDeal(WPARAM& type, Point &mousePos);
    void addSentKeys(HOOK::SENDINFO &info);

private:

    int m_timerNum;
    int m_sendInfoTime;
    UINT_PTR  m_timer;

    HOOK::MOUSE_EFFECT_FUN      m_mouseEffectFun;
    HOOK::KEYBOARD_EFFECT_FUN   m_keyboardEffectFun;

    std::list<HOOK::SENDINFO>* m_sendInfoList;
    std::multiset<int> keySet;

    static Hook* s_hook;

};

#endif // HOOK_H
