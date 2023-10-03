#ifndef WGSTRING_H
#define WGSTRING_H

#include "wglistitem.h"
#include "ordermap.h"
#include "common.h"

namespace Ui {
class WgString;
}

class WgString : public WgListItem
{
    Q_OBJECT

public:
    explicit WgString(int index, QWidget *parent = nullptr);
    virtual ~WgString();

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
    void on_te_str_textChanged();

private:
    void showOrderInfo();
    bool saveOrderInfo();
    void deleteOrderInfo();

    void showSaveButton();
    void showDeleteButton();

    void mousePressEvent(QMouseEvent *event) override;

signals:
    void signalSaveOrder(int num, orderMap::OrderInfo orderInfo);
    void signalDeleteOrder(int index);
    void signalSetCurrentIndex(int index);


private:
    Ui::WgString *ui;

    int m_changedFlag;
};

#endif // WGSTRING_H
