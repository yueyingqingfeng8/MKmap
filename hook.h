#ifndef HOOK_H
#define HOOK_H

#include <QObject>
#include <windows.h>
#include <QList>

#define INTERCEPT_TIME      1000     //处理时间，超时恢复正常
#define MOVE_DISTANCE       60      //滑动距离，由于鼠标每移动一个上下左右点就判断一次，所以要超出一个距离后再判断
#define JUDGE_DISTANCE      40       //判断距离要小于根号2滑动距离才行，否则只有4个方向

typedef std::function<void(QString, int)> CALLBACKMOUSEFUN;

typedef struct sendInfo {
    int lenth;
    INPUT inputs[10];
} SENDINFO;

class Hook : public QObject
{
    Q_OBJECT

public:
    explicit Hook(QObject *parent = Q_NULLPTR);
    ~Hook();

    int installHook();
    int uninstallHook();

    void sendInfos();
    void setMouseCallbackFun(CALLBACKMOUSEFUN fun);
};

#endif // HOOK_H
