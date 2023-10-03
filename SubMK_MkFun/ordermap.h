#ifndef ORDERMAP_H
#define ORDERMAP_H

#include "stdHeads.h"

#define MOW(str)   (std::string("MOW_") + std::string(str))
#define KB(str)    (std::string("KB_") + std::string(str))
#define SET(str)   (std::string("SET_") + std::string(str))

#define KEYS_SET_2          "SET_keysSet_2"         //用来读取按键集合的关键字,2键类型
#define KEYS_SET_3          "SET_keysSet_3"         //用来读取按键集合的关键字,3键类型
#define KEYS_TYPE           "SET_keysType"          //按键类型2键或3键
#define MOW_DISTANCE        "SET_mowDistance"       //鼠标单向移动距离和判断距离

namespace orderMap {
    enum OrderType{
        SHORTCUT_KEY,       //list<int>
        STRING,             //string
        SET_INFO            //list<int>
    };

    typedef struct orderInfo
    {
        int type;
        int delayTime;
        std::variant<std::string, std::list<int>> data;
    } OrderInfo;

    typedef std::map<std::string, std::list<orderMap::OrderInfo>> OrderMapBox;

    enum KeysType{
        KEYS_TYPE_2,
        KEYS_TYPE_3
    };
}

class OrderMap
{
public:
    static OrderMap& initOrderMap(std::shared_ptr<orderMap::OrderMapBox> argOrderMap){
        static OrderMap orderMapSelf(argOrderMap);
        return orderMapSelf;
    }

    static std::list<orderMap::OrderInfo> searchOrder(const std::string& order){
        return initOrderMap(NULL).insideSearchOrder(order);
    }

    static void modifyOrder(std::string&& orderName, std::list<orderMap::OrderInfo>&& orderList){
        initOrderMap(NULL).insideModifyOrder(std::move(orderName), std::move(orderList));
    }

    static void replaceOrder(std::string&& newOrder, std::string&& oldOrder){
        initOrderMap(NULL).insedeReplaceOrder(newOrder, oldOrder);
    }

    static std::list<int> searchSetting(const std::string& order){
        return initOrderMap(NULL).insideSearchSetting(order);
    }

    static void modifySetting(std::string&& orderName, std::list<int>& data){
        return initOrderMap(NULL).insideModifySetting(orderName, data);
    }

    static std::shared_ptr<orderMap::OrderMapBox> getOrderMap() {
        return initOrderMap(NULL).m_orderMap;
    }

private:
    OrderMap(std::shared_ptr<orderMap::OrderMapBox> argOrderMap)
        : m_orderMap(argOrderMap)  { }

    OrderMap(const OrderMap&) = delete;
    OrderMap& operator=(const OrderMap&) = delete;

    std::list<orderMap::OrderInfo> insideSearchOrder(const std::string& order){
        if (NULL == m_orderMap)
            return std::list<orderMap::OrderInfo>();

        if (m_orderMap->find(order) != m_orderMap->end())
            return m_orderMap->at(order);
        else
            return std::list<orderMap::OrderInfo>();
    }

    void insideModifyOrder(std::string&& orderName, std::list<orderMap::OrderInfo>&& orderList){
        if (NULL == m_orderMap) return;

        //能找到就用[]方式更新,insert无法更新,由于智能指针的存在,只能用operator[]函数
        if (m_orderMap->find(orderName) != m_orderMap->end())
            m_orderMap->operator[](orderName) = orderList;
        else
            m_orderMap->insert(std::make_pair(orderName, orderList));
    }

    void insedeReplaceOrder(std::string& newOrder, std::string& oldOrder){
        if (NULL == m_orderMap) return;

        if (m_orderMap->find(oldOrder) != m_orderMap->end())
        {
            auto orderList = m_orderMap->at(oldOrder);
            m_orderMap->erase(oldOrder);

            if (m_orderMap->find(newOrder) != m_orderMap->end())
                m_orderMap->operator[](newOrder) = orderList;
            else
                m_orderMap->insert(std::make_pair(newOrder, orderList));
        }
    }

    std::list<int> insideSearchSetting(const std::string& order){
        if (NULL == m_orderMap)
            return std::list<int>();

        if (m_orderMap->find(order) != m_orderMap->end())
        {
            //取出第一个order中的数据
            std::list<orderMap::OrderInfo> orderList = m_orderMap->operator[](order);
            if (!orderList.empty())
            {
                orderMap::OrderInfo order = orderList.front();
                if (order.type == orderMap::SET_INFO && std::holds_alternative<std::list<int>>(order.data))
                    return std::get<std::list<int>>(order.data);
            }

        }

        return std::list<int>();
    }

    void insideModifySetting(std::string& orderName, std::list<int>& data){
        if (NULL == m_orderMap) return;

        orderMap::orderInfo orderInfo{orderMap::SET_INFO, 0, data};
        std::list<orderMap::OrderInfo> orderInfoList;
        orderInfoList.push_back(orderInfo);

        if (m_orderMap->find(orderName) != m_orderMap->end())
            m_orderMap->operator[](orderName) = orderInfoList;
        else
            m_orderMap->insert(std::make_pair(orderName, orderInfoList));
    }

private:
    std::shared_ptr<orderMap::OrderMapBox> m_orderMap;

    static OrderMap orderMapSelf;
};

#endif // ORDERMAP_H
