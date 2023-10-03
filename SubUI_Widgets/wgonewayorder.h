#ifndef WGONEWAYORDER_H
#define WGONEWAYORDER_H

#include "common.h"
#include "mouseoneway.h"

namespace Ui {
class WgOneWayOrder;
}

class WgOneWayOrder : public QWidget
{
    Q_OBJECT

public:
    explicit WgOneWayOrder(QWidget *parent = nullptr);
    ~WgOneWayOrder();

    void setDistance(int& move, int& judge);

signals:
    void signalShowOrderInfo(QString);

private:
    void paintEvent(QPaintEvent *ev);
    void mousePressEvent(QMouseEvent *ev);
    void mouseMoveEvent(QMouseEvent *ev);

    void judgeRect(QPoint pos);

public:
    int m_moveDistance;
    int m_judgeDistance;

    QString m_currentPos;

private:
    Ui::WgOneWayOrder *ui;

    QRect m_judgeRect;

    int m_midpointX;
    int m_midpointY;
};

#endif // WGONEWAYORDER_H
