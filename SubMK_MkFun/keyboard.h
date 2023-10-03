#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "stdHeads.h"
#include "hook.h"
#include "ordermap.h"

class Keyboard
{
public:
    Keyboard(mkFun::CommandInfo* commandInfo);

    HOOK::KEYBOARD_EFFECT_FUN getEffectFun() const;

    void addCheckKeys(int& keysNum);
    void removeCheckKeys(int& keysNum);

    void readKeysSet();

private:
    int keyboardFun(WPARAM wParam, int argKeys);
    int checkKey(WPARAM &keyState,  int& key);

    void insertCommandList(int &num);
    void originalKeyFun(int &key);

private:
    std::set<int> m_keySet;             //需要检测的按键集合
    std::set<int> m_keyPressSet;        //用来判断哪些按键被同时按下

    int m_keysAmount = 2;
    int m_runFunFlag = 0;

    mkFun::CommandInfo* m_commandInfo;

    HOOK::KEYBOARD_EFFECT_FUN   m_effectFun;
};

#endif // KEYBOARD_H
