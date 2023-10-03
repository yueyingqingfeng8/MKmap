#include "mkfun.h"
#include "stdHeads.h"

Mkfun::Mkfun(std::shared_ptr<orderMap::OrderMapBox> argOrderMap)
    : m_mouseFunType(mkFun::MouseFunType::BAN_MOUSE), m_keyboardFunType(mkFun::KeyboardFunType::BAN_KEYBOARD)
{
    Hook::initHook();
    OrderMap::initOrderMap(argOrderMap);

    m_commandInfo = new mkFun::CommandInfo();
    m_keyboard = new Keyboard(m_commandInfo);
    m_mouseOneWay = new MouseOneWay(m_commandInfo);
    m_order = new Order(m_commandInfo);

    Hook::installHook(true, true);
}

Mkfun::~Mkfun()
{
    delete m_order;
    delete m_keyboard;
    delete m_mouseOneWay;
    delete m_commandInfo;
}

void Mkfun::installEffect(mkFun::MouseFunType &&mouseFunType, mkFun::KeyboardFunType &&keyboardFunType)
{
    switch (mouseFunType)
    {
    case mkFun::MouseFunType::MOUSE_ONEWAY:
        Hook::setEffectFun(m_mouseOneWay->getEffectFun(), NULL);
        break;
    default:
        break;
    }

    switch (keyboardFunType)
    {
    case mkFun::KeyboardFunType::KEYBOARD:
        Hook::setEffectFun(NULL, m_keyboard->getEffectFun());
        break;
    default:
        break;
    }
}

int Mkfun::installHook()
{
    return Hook::installHook(true, true);
}

int Mkfun::uninstallHook()
{
    return Hook::uninstallHook(true, true);
}













