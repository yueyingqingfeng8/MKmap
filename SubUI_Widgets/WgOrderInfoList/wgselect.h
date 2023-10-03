#ifndef WGSELECT_H
#define WGSELECT_H

#include <QWidget>
#include "wglistitem.h"
#include "ordermap.h"
#include "common.h"

namespace Ui {
class WgSelect;
}

class WgSelect : public WgListItem
{
    Q_OBJECT

public:
    explicit WgSelect(int index, QWidget *parent = nullptr);
    virtual ~WgSelect();

    void setIndex(int&& index) override{
        m_index = index;
    }

    void setNormalState() override;
    void setSelectedState() override;

    QPair<int, orderMap::orderInfo &> getOrderInfo() override;

private slots:
    void on_pb_shortcutKey_clicked();
    void on_pb_string_clicked();

private:
    void mousePressEvent(QMouseEvent *event) override;

signals:
    void signalAddWidgetType(int index, orderMap::OrderType widgetType);
    void signalSetCurrentIndex(int index);

private:
    Ui::WgSelect *ui;
};

#endif // WGSELECT_H
