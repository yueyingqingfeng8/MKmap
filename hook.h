#ifndef HOOK_H
#define HOOK_H

#include <windows.h>
#include <functional>
#include <list>

#define SENDINFOTIME 100

namespace HOOK {

    typedef std::function<int(int&, WPARAM&, LPARAM&)>  EFFECT_FUN;
    typedef std::function<VOID()>                       TIMER_FUN;

    typedef struct sendInfo {
        int lenth;
        INPUT inputs[10];
    } SENDINFO;
}

class Hook
{
public:
    explicit Hook(int time = SENDINFOTIME);
    ~Hook();

    int installHook();
    int uninstallHook();

    void setSendInfoTime(int time);
    void setEffectFun(HOOK::EFFECT_FUN fun);
    void addSendInfo(HOOK::SENDINFO& info);

private:
    void sendInfos();

private:

    int m_timerNum;
    int m_sendInfoTime;
    UINT_PTR  m_timer;

    std::list<HOOK::SENDINFO>* m_sendInfoList;
};

#endif // HOOK_H
