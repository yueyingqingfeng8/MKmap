#ifndef WGLISTITEM_H
#define WGLISTITEM_H

#include <QWidget>
#include "ordermap.h"

class WgListItem : public QWidget
{
public:
    explicit WgListItem(int index, QWidget *parent = nullptr)
        : QWidget{parent}, m_index(index){
    }

    virtual ~WgListItem(){}

    virtual void setIndex(int&& index) = 0;
    virtual void setNormalState() = 0;
    virtual void setSelectedState() = 0;

    virtual QPair<int, orderMap::orderInfo&> getOrderInfo() = 0;

protected:
    int m_index;                   //用于表示自己位于外面widgetList中的位置
    orderMap::OrderInfo m_orderInfo;
};

#endif // WGLISTITEM_H
