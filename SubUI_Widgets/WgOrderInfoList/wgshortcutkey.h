#ifndef WGSHORTCUTKEY_H
#define WGSHORTCUTKEY_H

#include "ordermap.h"
#include "utility.h"
#include "common.h"
#include "wglistitem.h"

namespace Ui {
class WgShortcutKey;
}

class WgShortcutKey : public WgListItem
{
    Q_OBJECT

public:
    explicit WgShortcutKey(int index, QWidget *parent = nullptr);
    virtual ~WgShortcutKey();

    void setInfo(orderMap::OrderInfo &orderInfo);

    void setIndex(int&& index) override {
        m_index = index;
    }

    void setNormalState() override;
    void setSelectedState() override;

    QPair<int, orderMap::orderInfo &> getOrderInfo() override;

public slots:
    bool eventFilter(QObject *,QEvent *);

private slots:
    void on_pb_key1_clicked();

    void on_pb_key2_clicked();

    void on_pb_key3_clicked();

private:
    void showShortcutKey();
    bool saveShortcutKey();
    void deleteShortcutKey();

    void initKeysState();

    void keyPressEvent(QKeyEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

signals:
    void signalSetCurrentIndex(int index);

private:
    Ui::WgShortcutKey *ui;

    int m_keyState_1;
    int m_keyState_2;
    int m_keyState_3;

    int m_changedFlag;
    QList<QString> m_keyStrList;
};

#endif // WGSHORTCUTKEY_H
