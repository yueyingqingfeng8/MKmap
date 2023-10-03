#include "order.h"


Order::Order(mkFun::CommandInfo *commandInfo)
    : m_commandInfo(commandInfo), m_exit(false)
{
    //建立后分离并启动消费者线程
    m_thread = std::thread([this](){
        working();
    });

    m_thread.detach();
}

Order::~Order()
{
//    m_exit = true;
//    m_commandInfo->condition.notify_one();
}

void Order::working()
{
    std::unique_lock<std::mutex> lock(m_commandInfo->mutex);
    while (!m_exit)
    {
        //没有指令就等待
        if (m_commandInfo->commandList.empty())
            m_commandInfo->condition.wait(lock);

        //处理指令列表信息,解析指令后执行
        for (auto it = m_commandInfo->commandList.begin(); it != m_commandInfo->commandList.end(); it++)
        {
            runOrder(analyzeOrder(*it));

            it = m_commandInfo->commandList.erase(it);
        }
    }
    lock.unlock();
}

std::string Order::analyzeOrder(std::tuple<int, mkFun::CommandData> order)
{
    auto[funType, command] = order;

    //鼠标的指令(字符串表示),可以转换为字符串时处理
    if (std::holds_alternative<std::string>(command))
    {
        std::string str = std::get<std::string>(command);

        if (!str.empty())
        {
            //根据鼠标功能类型,确定指令名称
            switch (funType)
            {
            case mkFun::MouseFunType::MOUSE_ONEWAY:
                return  MOW(str);
            default:
                break;
            }
        }
    }
    //键盘按键的指令(多个按键数字组合表示),可以转换为数字时处理
    else if (std::holds_alternative<long>(command))
    {
        long num = std::get<long>(command);

        if (num != 0)
        {
            //根据键盘功能类型,确定指令名称
            switch (funType)
            {
            case mkFun::KeyboardFunType::KEYBOARD:
                 return KB(std::to_string(num));
            default:
                 break;
            }
        }
    }

    return "";
}

void Order::runOrder(std::string &&order)
{
    if (order.empty()) return;

    std::list<orderMap::OrderInfo> orderList = OrderMap::searchOrder(order);
    for (auto& oneOrder : orderList)
    {
        if (orderMap::SHORTCUT_KEY == oneOrder.type)
        {
            runShortcutKey(oneOrder);
        }
        if (orderMap::STRING == oneOrder.type)
        {
            runWriteString(oneOrder);
        }
    }
}

void Order::runShortcutKey(orderMap::OrderInfo& orderInfo)
{
    if (std::holds_alternative<std::list<int>>(orderInfo.data))
    {
        Hook::addKeyboardEventShortcutKey(std::get<std::list<int>>(orderInfo.data));
    }
}

void Order::runWriteString(orderMap::OrderInfo& orderInfo)
{
    if (std::holds_alternative<std::string>(orderInfo.data))
    {
        std::list<int> keysList;
        std::string str = std::get<std::string>(orderInfo.data);

        bool commandCharacter = false;
        for (auto key : str)
        {
            //遇到指令符号,输出指令按键,此次轮空
            if (key == '|')
            {
                commandCharacter = true;
                continue;
            }

            //触发指令按键执行指令按键
            std::string order;
            if (commandCharacter)
            {
                commandCharacter = false;
                order = std::move("|" + std::string(1, key));
            }
            else
            {
                order = std::move(std::string(1, key));
            }

            int keyInt = KeysMap::keyStr2Int(std::move(order));
            if (keyInt != 0)
                keysList.push_back(keyInt);
        }

        Hook::addKeyboardEventString(keysList);
    }
}



