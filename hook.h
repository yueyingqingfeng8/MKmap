#ifndef HOOK_H
#define HOOK_H

#include <QObject>
#include <windows.h>
#include <QList>

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
