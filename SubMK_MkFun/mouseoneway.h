#ifndef MOUSEONEWAY_H
#define MOUSEONEWAY_H

#include "utility.h"
#include "interface.h"
#include "hook.h"
#include "common.h"
#include "ordermap.h"

namespace mouseOneWay {
    const int   MoveDistanceMax   = 180;
    const int   JudgeDistanceMax  = 124;
    const int   MoveDistanceMin   = 2;
    const int   JudgeDistanceMin  = 1;
    const int   InterceptTime     = 1000;   //处理时间，超时恢复正常
    const int   MoveDistance      = 60;     //滑动距离，由于鼠标每移动一个上下左右点就判断一次，所以要超出一个距离后再判断
    const int   JudgeDistance     = 30;     //判断距离要小于根号2滑动距离才行，否则只有4个方向
    const float JudgeMoveRatio    = 0.7;    //判断距离要比移动距离小，最大比例相差 根号2
}


class MouseOneWay :public Interface
{
public:
    enum Direction
    {
        U,D,L,R,UL,UR,DL,DR
    };

public:
    MouseOneWay(mkFun::CommandInfo* commandInfo);
    ~MouseOneWay();

    HOOK::MOUSE_EFFECT_FUN getEffectFun() const;

    void setDistance(int move, int judge);
    void readDistanceSet();

private:
    void initOnewayFunInfo();
    int mouseOneWayFun(WPARAM mouseType, Point mousePos);

    std::string judgeDirection(Point &mousePos);
    void insertCommandList(std::string&& command);

private:
    Point         m_pos;
    Timer*        m_timer;

    mkFun::CommandInfo* m_commandInfo;      //存放指令的列表

    bool m_replaceRightKey;     //右键功能被替换标志

    int m_moveDistance;         //移动距离
    int m_judgeDistance;        //判断距离
    int m_intercept_time;       //开启的时间段长度

    HOOK::MOUSE_EFFECT_FUN  m_effectFun;
};

#endif // MOUSEONEWAY_H
