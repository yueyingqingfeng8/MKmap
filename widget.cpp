#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include <functional>
 
 
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    m_timer = new QTimer(this);
    m_Hook = new Hook(this);

    CALLBACKMOUSEFUN fun = std::bind(&Widget::showMouse, this, std::placeholders::_1, std::placeholders::_2);
    m_Hook->setMouseCallbackFun(fun);

    connect(m_timer, &QTimer::timeout, m_Hook, &Hook::sendInfos);

    m_timer->start(100);
}

Widget::~Widget()
{
    m_timer->stop();
    m_Hook->uninstallHook();

    delete m_Hook;
    delete m_timer;
    delete ui;
}

void Widget::on_pb_install_clicked()
{
    if(m_Hook->installHook())
    {
        ui->lb_return->setText("安装钩子成功");
    }
    else
    {
        ui->lb_return->setText("安装钩子失败");
    }
}

void Widget::on_pb_unload_clicked()
{
    if(m_Hook->uninstallHook())
    {
        ui->lb_return->setText("卸载钩子成功");
    }
    else
    {
        ui->lb_return->setText("卸载钩子失败");
    }
}

void Widget::showMouse(QString str, int num)
{
    ui->lb_return->setText(str + QString::number(num));
//    ui->lb_return->setText(str);
}

void Widget::makeMouseInfo()
{
//    if(mouseFlag)
//    {
//        SendInput(sendMouse.lenth, sendMouse.inputs, sizeof(INPUT));
//        mouseFlag = 0;
//    }

}






