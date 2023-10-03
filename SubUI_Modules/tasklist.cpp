#include "tasklist.h"
#include "ui_tasklist.h"

TaskList::TaskList(QWidget *parent) :
    QWidget(parent), ui(new Ui::TaskList),
    m_currentIndex(-1), m_order("")
{
    ui->setupUi(this);

}

TaskList::~TaskList()
{
    delete ui;
}

void TaskList::showOrderInfo(QString order)
{
    m_order = order;
    initOrderInfo();
    emit signalLogInfo( mainUI::NORMAL, m_order);

    std::list<orderMap::OrderInfo> orderInfoList = OrderMap::searchOrder(order.toStdString());
    if (orderInfoList.size() == 0)
        return;

    for (auto& orderInfo : orderInfoList)
    {
        addWidget(orderInfo);
    }
}

void TaskList::addWidget(orderMap::OrderInfo &order)
{
    int index = ui->lw_order->count();

    //增加列表容器的单元数,插入进元素界面
    if (order.type == orderMap::SHORTCUT_KEY)
    {
        WgShortcutKey* item = new WgShortcutKey(index);
        item->setInfo(order);
        connect( item, &WgShortcutKey::signalSetCurrentIndex, this, &TaskList::slotSetCurrentIndex );
        m_currentIndex = index;

        ADD_TASK_WIDGET(item);

        m_orderNumMap.insert(index, order);
    }
    else if (order.type == orderMap::STRING)
    {
        WgString* item = new WgString(index);
        item->setInfo(order);
        connect( item, &WgString::signalSetCurrentIndex, this, &TaskList::slotSetCurrentIndex );
        m_currentIndex = index;

        ADD_TASK_WIDGET(item);

        m_orderNumMap.insert(index, order);
    }
}

void TaskList::addOrderInfo()
{
    m_currentIndex++;

    for(int i = m_currentIndex; i < ui->lw_order->count(); i++)
    {
        QWidget *widget = ui->lw_order->itemWidget(ui->lw_order->item(i));
        WgListItem *it = static_cast<WgListItem *>(widget);
        it->setIndex(i + 1);
    }

    WgSelect* item = new WgSelect(m_currentIndex);

    connect( item, &WgSelect::signalSetCurrentIndex, this, &TaskList::slotSetCurrentIndex );
    connect( item, &WgSelect::signalAddWidgetType, this, &TaskList::slotAddWidgetType );

    INSERT_TASK_WIDGET(m_currentIndex, item);

    addOrderNum();

    slotSetCurrentIndex(m_currentIndex);
}

void TaskList::clearListWidget()
{
    int count = ui->lw_order->count();

    for (int i = 0; i < count; i++)
    {
        delete ui->lw_order->itemWidget(ui->lw_order->item(i));
        delete ui->lw_order->takeItem(i);
    }

    ui->lw_order->clear();
}

void TaskList::deleteOrderInfo()
{
    if (-1 == m_currentIndex) return;

    if (m_orderNumMap.contains(m_currentIndex))
    {
        for(int i = m_currentIndex + 1; i < ui->lw_order->count(); i++)
        {
            QWidget *widget = ui->lw_order->itemWidget(ui->lw_order->item(i));
            WgListItem *it = static_cast<WgListItem *>(widget);
            it->setIndex(i - 1);
        }

        //删除对应widget和listWidgetItem
        delete ui->lw_order->itemWidget(ui->lw_order->item(m_currentIndex));
        delete ui->lw_order->takeItem(m_currentIndex);

        removeOrderNum();
    }

    if (!m_orderNumMap.contains(m_currentIndex))
    {
        m_currentIndex = ui->lw_order->count() - 1;
    }

    slotSetCurrentIndex(m_currentIndex);
}

void TaskList::saveOrderInfo()
{
    for(int i = 0; i < ui->lw_order->count(); i++)
    {
        QWidget *widget = ui->lw_order->itemWidget(ui->lw_order->item(i));
        WgListItem *it = static_cast<WgListItem *>(widget);
        it->setNormalState();

        auto[num, orderInfo] = it->getOrderInfo();
        if (num < 0)
            continue;
        else
            m_orderNumMap.insert(num, orderInfo);
    }

    saveOrder();
}

void TaskList::clearOrderInfo()
{
    initOrderInfo();
    m_orderNumMap.clear();
    saveOrder();
}

void TaskList::initOrderInfo()
{
    m_orderNumMap.clear();
    m_currentIndex = -1;

    clearListWidget();
}

void TaskList::slotSetCurrentIndex(int index)
{
    m_currentIndex = index;

    for (int i = 0; i < ui->lw_order->count(); ++i)
    {
        QWidget *widget = ui->lw_order->itemWidget(ui->lw_order->item(i));
        WgListItem *it = static_cast<WgListItem *>(widget);

        if (i == m_currentIndex)
            it->setSelectedState();
        else
            it->setNormalState();
    }
}

void TaskList::slotAddWidgetType(int index, orderMap::OrderType widgetType)
{
    if (m_orderNumMap.contains(index))
    {
        delete ui->lw_order->itemWidget(ui->lw_order->item(index));
        delete ui->lw_order->takeItem(index);
    }

    if (widgetType == orderMap::SHORTCUT_KEY)
    {
        WgShortcutKey* item = new WgShortcutKey(index);
        connect( item, &WgShortcutKey::signalSetCurrentIndex, this, &TaskList::slotSetCurrentIndex );

        INSERT_TASK_WIDGET(index, item);

        slotSetCurrentIndex(index);
    }
    else if (widgetType == orderMap::STRING)
    {
        WgString* item = new WgString(index);
        connect( item, &WgString::signalSetCurrentIndex, this, &TaskList::slotSetCurrentIndex );

        INSERT_TASK_WIDGET(index, item);

        slotSetCurrentIndex(index);
    }
}

void TaskList::saveOrder()
{
    std::list<orderMap::OrderInfo> list;
    for (auto& it : m_orderNumMap)
        list.push_back(it);

    OrderMap::modifyOrder(m_order.toStdString(), std::move(list));
}

void TaskList::removeOrderNum()
{
    for(int i = m_currentIndex + 1; i < m_orderNumMap.count(); i++)
    {
        orderMap::OrderInfo orderInfo =  m_orderNumMap.value(i);
        m_orderNumMap.insert(i - 1, orderInfo);
    }

    m_orderNumMap.remove(m_orderNumMap.count() - 1);

    saveOrder();
}

void TaskList::addOrderNum()
{
    for(int i = m_orderNumMap.count() - 1; i >= m_currentIndex; i--)
    {
        orderMap::OrderInfo orderInfo =  m_orderNumMap.value(i);
        m_orderNumMap.insert(i + 1, orderInfo);
    }

    orderMap::OrderInfo orderInfo1;
    m_orderNumMap.insert(m_currentIndex, orderInfo1);

}
