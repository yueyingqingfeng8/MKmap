#ifndef MKFUN_H
#define MKFUN_H

#include "utility.h"
#include "common.h"
#include "hook.h"
#include "keyboard.h"
#include "mouseoneway.h"
#include "order.h"
#include "ordermap.h"


class Mkfun
{
public:
    explicit Mkfun(std::shared_ptr<orderMap::OrderMapBox> argOrderMap);
    ~Mkfun();

    int installHook();
    int uninstallHook();

    void installEffect(mkFun::MouseFunType&& mouseFunType, mkFun::KeyboardFunType&& keyboardFunType);

    void readOneWayDistanceSet(){
        m_mouseOneWay->readDistanceSet();
    }

    void readKeyboardSet(){
        m_keyboard->readKeysSet();
    }

private:
    mkFun::MouseFunType     m_mouseFunType;
    mkFun::KeyboardFunType  m_keyboardFunType;

    Keyboard*           m_keyboard;
    MouseOneWay*        m_mouseOneWay;
    Order*              m_order;
    mkFun::CommandInfo* m_commandInfo;
};

#endif // MKFUN_H
