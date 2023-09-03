#ifndef PAINTWIDGET_H
#define PAINTWIDGET_H

#include <QWidget>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QRect>
#include <QPoint>
#include "common.h"

typedef std::function<void(QString)> SHOWKEYS_FUN;

namespace Ui {
class PaintWidget;
}

class PaintWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PaintWidget(QWidget *parent = 0);
    ~PaintWidget();

    void paintEvent(QPaintEvent *ev);

    void mousePressEvent(QMouseEvent *ev);
    void mouseMoveEvent(QMouseEvent *ev);

    void setDistance(int& move, int& judge);

    void judgeRect(QPoint pos);

    void setShowKeysFun(SHOWKEYS_FUN fun);

public:
    int m_moveDistance;
    int m_judgeDistance;

    QString m_currentPos;

private:
    Ui::PaintWidget *ui;

    QRect m_judgeRect;

    int m_midpointX;
    int m_midpointY;

    SHOWKEYS_FUN m_showKeysFun;
};

#endif // PAINTWIDGET_H
