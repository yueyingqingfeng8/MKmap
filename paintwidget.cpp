#include "paintwidget.h"
#include "ui_paintwidget.h"

#include <QPainter>
#include <QDebug>

PaintWidget::PaintWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PaintWidget)
{
    ui->setupUi(this);

    m_showKeysFun = NULL;

    m_moveDistance = MOVE_DISTANCE;
    m_judgeDistance = JUDGE_DISTANCE;

    m_midpointX = size().width() / 2;
    m_midpointY = size().height() / 2;

    m_judgeRect = QRect(QPoint(0, 0), QPoint(m_midpointX - m_judgeDistance, m_midpointY - m_judgeDistance));

    m_currentPos.append("UL");
}

PaintWidget::~PaintWidget()
{
    delete ui;
}


void PaintWidget::setDistance(int& move, int& judge)
{
    m_moveDistance = move;
    m_judgeDistance = judge;

    update();
}

void PaintWidget::paintEvent(QPaintEvent *ev)
{
    QPainter p(this);

    //底色
    p.setBrush(QColor(229, 255, 201));
    p.drawRect(0, 0, size().width(), size().height());

    //选择区域
    p.setBrush(Qt::gray);
    p.drawRect(m_judgeRect);

    //恢复底色
    p.setBrush(QColor(229, 255, 201));

    //定义画笔
    QPen pen;
    pen.setWidth(3);    //设置线宽
    pen.setColor(Qt::blue);    //设置rgb颜色

    //安装画笔
    p.setPen(pen);

    //画圆
    p.drawEllipse(QPoint(m_midpointX, m_midpointY),m_moveDistance, m_moveDistance);

    //画矩形       点和长 宽
    p.drawRect(m_midpointX - m_judgeDistance, m_midpointY - m_judgeDistance, m_judgeDistance * 2, m_judgeDistance * 2);
}

void PaintWidget::judgeRect(QPoint pos)
{
    m_currentPos.clear();

    if(pow(pos.y() - m_midpointY, 2) + pow(pos.x() - m_midpointX, 2) < m_moveDistance * m_moveDistance)
    {
        m_judgeRect = QRect(0, 0, 0, 0);
        update();
        return;
    }

    if(pos.y() < m_midpointY - m_judgeDistance)
    {
        //上面
        if(pos.x() < m_midpointX - m_judgeDistance)       //左上
        {
            m_currentPos = "UL";
            m_judgeRect = QRect(QPoint(0, 0), QPoint(m_midpointX - m_judgeDistance, m_midpointY - m_judgeDistance));
        }
        else if(pos.x() > m_midpointX + m_judgeDistance)  //右上
        {
            m_currentPos = "UR";
            m_judgeRect = QRect(QPoint(m_midpointX + m_judgeDistance, 0), QPoint(size().width(), m_midpointY - m_judgeDistance));
        }
        else                                                    //上
        {
            m_currentPos = "U";
            m_judgeRect = QRect(QPoint(m_midpointX - m_judgeDistance, 0), QPoint(m_midpointX + m_judgeDistance, m_midpointY - m_judgeDistance));
        }
    }
    else if(pos.y() > m_midpointY + m_judgeDistance)
    {
        //下面
        if(pos.x() < m_midpointX - m_judgeDistance)       //左下
        {
            m_currentPos = "DL";
            m_judgeRect = QRect(QPoint(0, m_midpointY + m_judgeDistance), QPoint(m_midpointX - m_judgeDistance, size().height()));
        }
        else if(pos.x() > m_midpointX + m_judgeDistance)  //右下
        {
            m_currentPos = "DR";
            m_judgeRect = QRect(QPoint(m_midpointX + m_judgeDistance, m_midpointY + m_judgeDistance), QPoint(size().width(), size().height()));
        }
        else                                                    //下
        {
            m_currentPos = "D";
            m_judgeRect = QRect(QPoint(m_midpointX - m_judgeDistance, m_midpointY + m_judgeDistance), QPoint(m_midpointX + m_judgeDistance, size().height()));
        }
    }
    else if(pos.x() < m_midpointX - m_judgeDistance)
    {
        m_currentPos = "L";
        m_judgeRect = QRect(QPoint(0, m_midpointY - m_judgeDistance), QPoint(m_midpointX - m_judgeDistance, m_midpointY + m_judgeDistance));                      //左
    }
    else
    {
        m_currentPos = "R";
        m_judgeRect = QRect(QPoint(m_midpointX + m_judgeDistance, m_midpointY - m_judgeDistance), QPoint(size().width(), m_midpointY + m_judgeDistance));                        //右
    }

    update();
}

void PaintWidget::mousePressEvent(QMouseEvent *ev)
{
    judgeRect(ev->pos());
    m_showKeysFun(m_currentPos);
}

void PaintWidget::mouseMoveEvent(QMouseEvent *ev)
{
    judgeRect(ev->pos());
}

void PaintWidget::setShowKeysFun(SHOWKEYS_FUN fun)
{
    m_showKeysFun = fun;
}



