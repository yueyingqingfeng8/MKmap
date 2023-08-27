#ifndef COMMON_H
#define COMMON_H

#include <functional>
#include <windows.h>
#include <QTimer>
#include <QList>
#include <QFile>
#include <QJsonObject>
#include <QJsonParseError>
#include <QTextCodec>
#include <QTextStream>
#include <QJsonArray>
#include <QMap>
#include <QKeyEvent>
#include <tuple>


typedef std::function<void(QString, int)>   CALLBACKMOUSE_FUN;

enum Effect
{
    MK = 1,
    WAYS,
    ONEWAY
};

enum OneWayEnum
{
    U,
    D,
    L,
    R,
    UL,
    UR,
    DL,
    DR
};


#define  INTERCEPT_TIME          1000     //处理时间，超时恢复正常
#define  MOVE_DISTANCE           60       //滑动距离，由于鼠标每移动一个上下左右点就判断一次，所以要超出一个距离后再判断
#define  JUDGE_DISTANCE          30       //判断距离要小于根号2滑动距离才行，否则只有4个方向
#define  JUDGE_MOVE_RATIO        0.7      //判断距离要比移动距离小，最大比例相差 根号2

#define  MOVE_DISTANCE_MAX       200
#define  JUDGE_DISTANCE_MAX      140
#define  MOVE_DISTANCE_MIN       2
#define  JUDGE_DISTANCE_MIN      1

#define  SETFILEPATH             "../setFile.json"   //配置文件路径

#define  DDUU                    0        //同时按下
#define  DUDU                    1        //顺序输入

#endif // COMMON_H
