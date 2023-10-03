#ifndef ORDER_H
#define ORDER_H

#include "utility.h"
#include "hook.h"
#include "ordermap.h"

class Order
{
public:
    Order(mkFun::CommandInfo* commandInfo);
    ~Order();

private:
    //线程任务
    void working();

    //分析指令
    std::string analyzeOrder(std::tuple<int, mkFun::CommandData> order);

    //运行指令
    void runOrder(std::string&& order);
    void runShortcutKey(orderMap::OrderInfo& orderInfo);
    void runWriteString(orderMap::OrderInfo &orderInfo);

private:
    std::thread m_thread;
    mkFun::CommandInfo* m_commandInfo;

    bool m_exit;
};

#endif // ORDER_H
