#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include <functional>
 
#define KEYCOLOR_SELECT "rgb(194, 179, 255)"
#define KEYCOLOR_NORMAL "rgb(219, 232, 255)"
 
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    initMkfun();

    ui->sw_test->setCurrentIndex(PG_SET);
    ui->sw_set->setCurrentIndex(PG_ONEWAY);

    m_keyState_1 = 0;
    m_keyState_2 = 0;
    m_keyState_3 = 0;
}

Widget::~Widget()
{
    on_pb_save_clicked();
    deleteMkfun();

    delete ui;
}

void Widget::on_pb_install_clicked()
{
    if(m_mkfun->installHook())
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
    if(m_mkfun->uninstallHook())
    {
        ui->lb_return->setText("卸载钩子成功");
    }
    else
    {
        ui->lb_return->setText("卸载钩子失败");
    }
}

void Widget::showMouseInfo(QString str, int num)
{
    ui->lb_return->setText(str + QString::number(num));
//    ui->lb_return->setText(str);
}

void Widget::showKeys(QString direction)
{
    ui->pb_key1->setText("");
    ui->pb_key2->setText("");
    ui->pb_key3->setText("");

    if (direction.isEmpty())
        return;

    auto[type, strList] = m_mkfun->searchKeys(direction.toStdString());

    if (-1 == type)
        return;

    m_currentDirection = direction;

    m_currentKeysInfo.first = type;

    for (auto& info : strList)
    {
        m_currentKeysInfo.second.append(QString(info.c_str()));
    }

    if (!m_currentKeysInfo.second.isEmpty())
    {
        ui->pb_key1->setText(m_currentKeysInfo.second.takeFirst());
    }
    if (!m_currentKeysInfo.second.isEmpty())
    {
        ui->pb_key2->setText(m_currentKeysInfo.second.takeFirst());
    }
    if (!m_currentKeysInfo.second.isEmpty())
    {
        ui->pb_key3->setText(m_currentKeysInfo.second.takeFirst());
    }
}

void Widget::initMkfun()
{
    m_mkfun = new Mkfun();
    m_mkfun->installEffect(ONEWAY);

    CALLBACKMOUSE_FUN fun = std::bind(&Widget::showMouseInfo, this, std::placeholders::_1, std::placeholders::_2);
    m_mkfun->setPrintMouseInfoFun(fun);

    SHOWKEYS_FUN fun1 = std::bind(&Widget::showKeys, this, std::placeholders::_1);
    ui->widget->setShowKeysFun(fun1);

    ui->sb_moveDistance->setMaximum(MOVE_DISTANCE_MAX);
    ui->sb_moveDistance->setMinimum(MOVE_DISTANCE_MIN);
    ui->sb_judgeDistance->setMaximum(JUDGE_DISTANCE_MAX);
    ui->sb_judgeDistance->setMinimum(JUDGE_DISTANCE_MIN);

    ui->sb_moveDistance->setValue(ui->widget->m_moveDistance);
    ui->sb_judgeDistance->setValue(ui->widget->m_judgeDistance);

    showKeys(QString("UL"));
}

void Widget::deleteMkfun()
{
    delete m_mkfun;
}


void Widget::on_pb_set_clicked()
{
    if (PG_TEST == ui->sw_test->currentIndex())
    {
        ui->sw_test->setCurrentIndex(PG_SET);
    }
    else
    {
        ui->sw_test->setCurrentIndex(PG_TEST);
    }
}

void Widget::on_pb_setOneWay_clicked()
{
    ui->sw_set->setCurrentIndex(PG_ONEWAY);
}

void Widget::on_pb_save_clicked()
{
}

void Widget::on_pb_key1_clicked()
{
    if (ui->widget->m_currentPos.isEmpty()) return;

    if (m_keyState_1)
    {
        ui->pb_key1->setStyleSheet(QString("background-color: %1;").arg(KEYCOLOR_NORMAL));
        m_keyState_1 = 0;
    }
    else
    {
        ui->pb_key1->setStyleSheet(QString("background-color: %1;").arg(KEYCOLOR_SELECT));
        m_keyState_1 = 1;
    }
}

void Widget::on_pb_key2_clicked()
{
    if (ui->widget->m_currentPos.isEmpty()) return;

    if (m_keyState_2)
    {
        ui->pb_key2->setStyleSheet(QString("background-color: %1;").arg(KEYCOLOR_NORMAL));
        m_keyState_2 = 0;
    }
    else
    {
        ui->pb_key2->setStyleSheet(QString("background-color: %1;").arg(KEYCOLOR_SELECT));
        m_keyState_2 = 1;
    }
}

void Widget::on_pb_key3_clicked()
{
    if (ui->widget->m_currentPos.isEmpty()) return;

    if (m_keyState_3)
    {
        ui->pb_key3->setStyleSheet(QString("background-color: %1;").arg(KEYCOLOR_NORMAL));
        m_keyState_3 = 0;
    }
    else
    {
        ui->pb_key3->setStyleSheet(QString("background-color: %1;").arg(KEYCOLOR_SELECT));
        m_keyState_3 = 1;
    }
}

void Widget::keyPressEvent(QKeyEvent *event)
{
    if (1 == m_keyState_1)
    {
        ui->pb_key1->setText(m_mkfun->searchInt2Keys(event->key()));
        on_pb_key1_clicked();
        return;
    }
    if (1 == m_keyState_2)
    {
        ui->pb_key2->setText(m_mkfun->searchInt2Keys(event->key()));
        on_pb_key2_clicked();
        return;
    }
    if (1 == m_keyState_3)
    {
        ui->pb_key3->setText(m_mkfun->searchInt2Keys(event->key()));
        on_pb_key3_clicked();
        return;
    }
}

void Widget::on_pb_keysSave_clicked()
{
    QStringList keysList;
    QString str1 = ui->pb_key1->text();

    if (!str1.isEmpty() && !keysList.contains(str1))
    {
        keysList.append(str1);
    }

    QString str2 = ui->pb_key2->text();
    if (!str2.isEmpty() && !keysList.contains(str2))
    {
        keysList.append(str2);
    }

    QString str3 = ui->pb_key3->text();
    if (!str3.isEmpty() && !keysList.contains(str3))
    {
        keysList.append(str3);
    }

    if (!keysList.isEmpty())
    {
        m_mkfun->modifyControlSet(m_currentDirection, m_currentKeysInfo.first, keysList);
        showKeys(m_currentDirection);
        m_mkfun->saveControlsSet();
    }
}

void Widget::on_sb_moveDistance_editingFinished()
{
    int moveDistance = ui->sb_moveDistance->value();
    int judgeDistance = ui->sb_judgeDistance->value();
    int judgeDistanceMax = moveDistance * JUDGE_MOVE_RATIO;

    if (judgeDistance > judgeDistanceMax)
        judgeDistance = judgeDistanceMax;

    ui->sb_judgeDistance->setMaximum(judgeDistanceMax);
    ui->sb_judgeDistance->setMinimum(JUDGE_DISTANCE_MIN);

    m_mkfun->setDistance(moveDistance, judgeDistance);
    ui->widget->setDistance(moveDistance, judgeDistance);
}

void Widget::on_sb_judgeDistance_editingFinished()
{
    int judgeDistance = ui->sb_judgeDistance->value();
    int moveDistance = ui->sb_moveDistance->value();

    int moveDistance_min = judgeDistance / JUDGE_MOVE_RATIO;

    if (moveDistance < moveDistance_min)
        moveDistance = moveDistance_min;

    ui->sb_moveDistance->setMaximum(MOVE_DISTANCE_MAX);
    ui->sb_moveDistance->setMinimum(moveDistance_min);

    m_mkfun->setDistance(moveDistance, judgeDistance);
    ui->widget->setDistance(moveDistance, judgeDistance);
}
