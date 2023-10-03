#ifndef KEYBOARDORDER_H
#define KEYBOARDORDER_H

#include <QWidget>
#include "common.h"
#include "ordermap.h"
#include "WgKeysList/wg2keyorder.h"
#include "WgKeysList/wg3keyorder.h"
#include "WgKeysList/wgaddkeys.h"

#define ADD_KEY_WIDGET(item) \
    QListWidgetItem* listItem = new QListWidgetItem();\
    listItem->setSizeHint(item->size());\
    ui->lw_keysOrder->addItem(listItem);\
    ui->lw_keysOrder->setItemWidget(listItem, item)

#define INSERT_KEY_WIDGET(index, item) \
    QListWidgetItem* listItem = new QListWidgetItem();\
    listItem->setSizeHint(item->size());\
    ui->lw_keysOrder->insertItem(index, listItem);\
    ui->lw_keysOrder->setItemWidget(listItem, item)

namespace Ui {
class KeyboardOrder;
}

class KeyboardOrder : public QWidget
{
    Q_OBJECT

public:
    explicit KeyboardOrder(QWidget *parent = nullptr);
    ~KeyboardOrder();

    void showKeys();

private:
    void readKeysSet();
    void saveKeysSet();

    void showKeysWidgetList();
    void clearListWidget();
    void addWidget(int keysNum);
    void insertWidget(int& index);
    void addEndWidget();

signals:
    void signalShowOrderInfo(QString order);
    void signalClearOrderInfo();
    void signalSavedKeysSet();

    void signalLogInfo(mainUI::LogType logType, QString str);

private slots:
    void slotSetCurrentIndex(int index, int keysNum);
    void slotDeleteKeys(int index, int keysNum);
    void slotSaveKeys(int index, int oldKeysNum, int newKeysNum);
    void slotAddKeysWidget(int index);

private:
    Ui::KeyboardOrder *ui;

    int m_keyType;
    int m_keysNum;
    QSet<int> m_keysSet;
};

#endif // KEYBOARDORDER_H
