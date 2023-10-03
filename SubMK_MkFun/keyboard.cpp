#include "keyboard.h"

Keyboard::Keyboard(mkFun::CommandInfo *commandInfo)
    : m_commandInfo(commandInfo)
{
    m_effectFun = std::bind(&Keyboard::keyboardFun, this, std::placeholders::_1, std::placeholders::_2);

    readKeysSet();
}

int Keyboard::keyboardFun(WPARAM wParam, int argKeys)
{
    if (wParam != WM_KEYDOWN  && wParam != WM_KEYUP)
        return 0;

    //不包含检查的按键就返回
    if(m_keySet.find(argKeys) == m_keySet.end())
        return 0;

    //检查同时按下的按键
    return checkKey(wParam, argKeys);
}

int Keyboard::checkKey(WPARAM &keyState, int &key)
{
    //按键按下开始该按键的一个循环
    if (keyState == WM_KEYDOWN)
    {
        //已经被按下就返回,防止无限循环处理,未被按下就放入按下按键的集合
        if (m_keyPressSet.find(key) != m_keyPressSet.end())
            return 1;
        else
            m_keyPressSet.insert(key);

        //达到按键数目就启动特殊功能开关,并添加任务指令
        if (m_keyPressSet.size() == (std::set<int>::size_type)m_keysAmount)
        {
            m_runFunFlag = 1;

            //组合一下按键数字,方便传输,set自带排序
            int num = 0;
            for (auto& it : m_keyPressSet)
                num = num * 1000 + it;

            insertCommandList(num);
        }

        return 1;
    }

    if (keyState == WM_KEYUP)
    {
        //按键升起,表示该按键循环结束
        m_keyPressSet.erase(key);

        //如果运行特殊功能的标志打开,说明触发过特殊功能,此次按键松开不处理,否则执行原按键功能
        if (1 == m_runFunFlag)
        {
            //按键都松开了,任务周期完成,标志位清空
            if (m_keyPressSet.size() == 0)
                m_runFunFlag = 0;
        }
        else
        {
            originalKeyFun(key);
        }

        return 1;
    }

    return 0;
}

void Keyboard::insertCommandList(int& num)
{
    if(num != 0)
    {
        //上锁
        std::unique_lock<std::mutex> lock(m_commandInfo->mutex);
        //增加命令信息
        m_commandInfo->commandList.push_back(std::tuple<int, mkFun::CommandData>(mkFun::KeyboardFunType::KEYBOARD, num));
        // 释放互斥锁
        lock.unlock();
        //通知线程执行命令
        m_commandInfo->condition.notify_one();
    }
}

void Keyboard::originalKeyFun(int &key)
{
    std::list<int> keyList;
    keyList.push_back(key);

    Hook::addKeyboardEventString(keyList);
}

HOOK::KEYBOARD_EFFECT_FUN Keyboard::getEffectFun() const
{
    return m_effectFun;
}

void Keyboard::readKeysSet()
{
    //先初始化按键集合,默认是2键
    m_keysAmount = 2;
    m_keySet.clear();
    std::list<int> keysList = OrderMap::searchSetting(KEYS_SET_2);

    //读取按键类型和对应的按键列表
    auto typeList = OrderMap::searchSetting(KEYS_TYPE);
    if (!typeList.empty())
    {
        int type = typeList.front();
        if (type == orderMap::KEYS_TYPE_2)
        {
        }
        else if (type == orderMap::KEYS_TYPE_3)
        {
            m_keysAmount = 3;
            keysList = OrderMap::searchSetting(KEYS_SET_3);
        }
    }

    //将按键列表的检测按键加入检测集合
    for (auto& keys : keysList)
        addCheckKeys(keys);

    //防止只有一个按键的情况
    if (1 == m_keySet.size())
        m_keySet.clear();
}

void Keyboard::addCheckKeys(int &keysNum)
{
    int key1 = keysNum / 1000000;
    int key2 = keysNum % 1000000 / 1000;
    int key3 = keysNum % 1000;

    if (key1 > 0)
    {
        if (m_keySet.find(key1) == m_keySet.end())
            m_keySet.insert(key1);
    }

    if (key2 > 0)
    {
        if (m_keySet.find(key2) == m_keySet.end())
            m_keySet.insert(key2);
    }

    if (key3 > 0)
    {
        if (m_keySet.find(key3) == m_keySet.end())
            m_keySet.insert(key3);
    }
}

void Keyboard::removeCheckKeys(int &keysNum)
{
    int key1 = keysNum / 1000000;
    int key2 = keysNum % 1000000 / 1000;
    int key3 = keysNum % 1000;

    if (key1 > 0)
    {
        if (m_keySet.find(key1) != m_keySet.end())
            m_keySet.erase(key1);
    }

    if (key2 > 0)
    {
        if (m_keySet.find(key2) != m_keySet.end())
            m_keySet.erase(key2);
    }

    if (key3 > 0)
    {
        if (m_keySet.find(key3) != m_keySet.end())
            m_keySet.erase(key3);
    }
}
