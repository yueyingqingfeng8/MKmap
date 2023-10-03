#ifndef TASKLIST_H
#define TASKLIST_H

#include <QWidget>
#include "ordermap.h"
#include "WgOrderInfoList/wgshortcutkey.h"
#include "WgOrderInfoList/wgstring.h"
#include "WgOrderInfoList/wgselect.h"

#define ADD_TASK_WIDGET(item) \
    QListWidgetItem* listItem = new QListWidgetItem();\
    listItem->setSizeHint(item->size());\
    ui->lw_order->addItem(listItem);\
    ui->lw_order->setItemWidget(listItem, item)

#define INSERT_TASK_WIDGET(index, item) \
    QListWidgetItem* listItem = new QListWidgetItem();\
    listItem->setSizeHint(item->size());\
    ui->lw_order->insertItem(index, listItem);\
    ui->lw_order->setItemWidget(listItem, item)

namespace Ui {
class TaskList;
}

class TaskList : public QWidget
{
    Q_OBJECT

public:
    explicit TaskList(QWidget *parent = nullptr);
    ~TaskList();

    void showOrderInfo(QString order);

    void addWidget(orderMap::OrderInfo& order);

    void addOrderInfo();
    void deleteOrderInfo();
    void saveOrderInfo();
    void clearOrderInfo();

private slots:
    void slotSetCurrentIndex(int index);
    void slotAddWidgetType(int index, orderMap::OrderType widgetType);

private:
    void initOrderInfo();
    void clearListWidget();
    void saveOrder();
    void removeOrderNum();
    void addOrderNum();

signals:
    void signalLogInfo(mainUI::LogType, QString);

private:
    Ui::TaskList *ui;

    int m_currentIndex;

    QString m_order;
    QMap<int, orderMap::OrderInfo> m_orderNumMap;
};

#endif // TASKLIST_H
