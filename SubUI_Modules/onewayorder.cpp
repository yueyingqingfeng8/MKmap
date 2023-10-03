#include "onewayorder.h"
#include "ui_onewayorder.h"

OneWayOrder::OneWayOrder(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OneWayOrder)
{
    ui->setupUi(this);

    connect(ui->wg_oneWayOrder, &WgOneWayOrder::signalShowOrderInfo,this, [this](QString order){
        emit signalShowOrderInfo(order);
    });

    //单向按键数据设置
    ui->sb_moveDistance->setMaximum(mouseOneWay::MoveDistanceMax);
    ui->sb_moveDistance->setMinimum(mouseOneWay::MoveDistanceMin);
    ui->sb_judgeDistance->setMaximum(mouseOneWay::JudgeDistanceMax);
    ui->sb_judgeDistance->setMinimum(mouseOneWay::JudgeDistanceMin);

    ui->sb_moveDistance->setValue(ui->wg_oneWayOrder->m_moveDistance);
    ui->sb_judgeDistance->setValue(ui->wg_oneWayOrder->m_judgeDistance);
}

OneWayOrder::~OneWayOrder()
{
    delete ui;
}

void OneWayOrder::on_sb_moveDistance_editingFinished()
{
    int moveDistance = ui->sb_moveDistance->value();
    int judgeDistance = ui->sb_judgeDistance->value();
    int judgeDistanceMax = moveDistance * mouseOneWay::JudgeMoveRatio;

    if (judgeDistance > judgeDistanceMax)
        judgeDistance = judgeDistanceMax;

    ui->sb_judgeDistance->setMaximum(judgeDistanceMax);
    ui->sb_judgeDistance->setMinimum(mouseOneWay::JudgeDistanceMin);

    ui->wg_oneWayOrder->setDistance(moveDistance, judgeDistance);

    std::list<int> distanceList;
    distanceList.push_back(moveDistance);
    distanceList.push_back(judgeDistance);

    OrderMap::modifySetting(MOW_DISTANCE, distanceList);
    emit signalSavedDistanceSet();
}

void OneWayOrder::on_sb_judgeDistance_editingFinished()
{
    int judgeDistance = ui->sb_judgeDistance->value();
    int moveDistance = ui->sb_moveDistance->value();

    int moveDistance_min = judgeDistance / mouseOneWay::JudgeMoveRatio;

    if (moveDistance < moveDistance_min)
        moveDistance = moveDistance_min;

    ui->sb_moveDistance->setMaximum(mouseOneWay::MoveDistanceMax);
    ui->sb_moveDistance->setMinimum(moveDistance_min);

    ui->wg_oneWayOrder->setDistance(moveDistance, judgeDistance);

    std::list<int> distanceList;
    distanceList.push_back(moveDistance);
    distanceList.push_back(judgeDistance);

    OrderMap::modifySetting(MOW_DISTANCE, distanceList);
    emit signalSavedDistanceSet();
}

void OneWayOrder::showSetInfo()
{
    //第一个是移动距离,第二个是判断距离
    auto list = OrderMap::searchSetting(MOW_DISTANCE);
    if (list.size() == 2)
    {
        int moveDistance = list.front();
        int judgeDistance = list.back();

        ui->sb_moveDistance->setValue(moveDistance);
        ui->sb_judgeDistance->setValue(judgeDistance);
        ui->wg_oneWayOrder->setDistance(moveDistance, judgeDistance);
    }
}
