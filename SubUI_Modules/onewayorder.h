#ifndef ONEWAYORDER_H
#define ONEWAYORDER_H

#include <QWidget>
#include "wgonewayorder.h"

namespace Ui {
class OneWayOrder;
}

class OneWayOrder : public QWidget
{
    Q_OBJECT

public:
    explicit OneWayOrder(QWidget *parent = nullptr);
    ~OneWayOrder();

    void showSetInfo();

private slots:
    void on_sb_moveDistance_editingFinished();

    void on_sb_judgeDistance_editingFinished();

signals:
    void signalShowOrderInfo(QString);
    void signalSavedDistanceSet();

    void signalLogInfo(mainUI::LogType logType, QString str);

private:
    Ui::OneWayOrder *ui;
};

#endif // ONEWAYORDER_H
