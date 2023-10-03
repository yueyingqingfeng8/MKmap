#include "wgonewayorder.h"
#include "ui_wgonewayorder.h"

WgOneWayOrder::WgOneWayOrder(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WgOneWayOrder)
{
    ui->setupUi(this);

    m_moveDistance = mouseOneWay::MoveDistance;
    m_judgeDistance = mouseOneWay::JudgeDistance;

    m_midpointX = size().width() / 2;
    m_midpointY = size().height() / 2;

    m_judgeRect = QRect(QPoint(0, 0), QPoint(0, 0));

    m_currentPos = "";
}

WgOneWayOrder::~WgOneWayOrder()
{
    delete ui;
}

void WgOneWayOrder::setDistance(int& move, int& judge)
{
    m_moveDistance = move;
    m_judgeDistance = judge;

    update();
}

void WgOneWayOrder::paintEvent(QPaintEvent *ev)
{
    QPainter p(this);

    p.setRenderHint(QPainter::Antialiasing);

    //底色
    p.setBrush(QColor(211, 255, 215));
    p.drawRect(0, 0, size().width(), size().height());

    //选择区域
    p.setBrush(QColor(0xb9, 0xc2, 0xbf));
    p.drawRect(m_judgeRect);

    //恢复底色
    p.setBrush(QColor(211, 255, 215));

    //定义画笔
    QPen pen;
    pen.setWidth(3);            //设置线宽
    pen.setColor(Qt::blue);     //设置rgb颜色

    //安装画笔
    p.setPen(pen);

    //画圆
    p.drawEllipse(QPoint(m_midpointX, m_midpointY),m_moveDistance, m_moveDistance);

    //画矩形       点和长 宽
    p.drawRect(m_midpointX - m_judgeDistance, m_midpointY - m_judgeDistance, m_judgeDistance * 2, m_judgeDistance * 2);
}

void WgOneWayOrder::judgeRect(QPoint pos)
{
    std::string str;

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
            str = "UL";
            m_judgeRect = QRect(QPoint(0, 0), QPoint(m_midpointX - m_judgeDistance, m_midpointY - m_judgeDistance));
        }
        else if(pos.x() > m_midpointX + m_judgeDistance)  //右上
        {
            str = "UR";
            m_judgeRect = QRect(QPoint(m_midpointX + m_judgeDistance, 0), QPoint(size().width(), m_midpointY - m_judgeDistance));
        }
        else                                                    //上
        {
            str = "U";
            m_judgeRect = QRect(QPoint(m_midpointX - m_judgeDistance, 0), QPoint(m_midpointX + m_judgeDistance, m_midpointY - m_judgeDistance));
        }
    }
    else if(pos.y() > m_midpointY + m_judgeDistance)
    {
        //下面
        if(pos.x() < m_midpointX - m_judgeDistance)       //左下
        {
            str = "DL";
            m_judgeRect = QRect(QPoint(0, m_midpointY + m_judgeDistance), QPoint(m_midpointX - m_judgeDistance, size().height()));
        }
        else if(pos.x() > m_midpointX + m_judgeDistance)  //右下
        {
            str = "DR";
            m_judgeRect = QRect(QPoint(m_midpointX + m_judgeDistance, m_midpointY + m_judgeDistance), QPoint(size().width(), size().height()));
        }
        else                                                    //下
        {
            str = "D";
            m_judgeRect = QRect(QPoint(m_midpointX - m_judgeDistance, m_midpointY + m_judgeDistance), QPoint(m_midpointX + m_judgeDistance, size().height()));
        }
    }
    else if(pos.x() < m_midpointX - m_judgeDistance)
    {
        str = "L";
        m_judgeRect = QRect(QPoint(0, m_midpointY - m_judgeDistance), QPoint(m_midpointX - m_judgeDistance, m_midpointY + m_judgeDistance));                      //左
    }
    else
    {
        str = "R";
        m_judgeRect = QRect(QPoint(m_midpointX + m_judgeDistance, m_midpointY - m_judgeDistance), QPoint(size().width(), m_midpointY + m_judgeDistance));                        //右
    }

    m_currentPos = QString((MOW(str)).c_str());

    update();
}

void WgOneWayOrder::mousePressEvent(QMouseEvent *ev)
{
    judgeRect(ev->pos());

    if (m_currentPos != "")
        emit signalShowOrderInfo(m_currentPos);
}

void WgOneWayOrder::mouseMoveEvent(QMouseEvent *ev)
{
    judgeRect(ev->pos());
}
