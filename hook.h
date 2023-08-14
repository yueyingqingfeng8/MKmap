#ifndef HOOK_H
#define HOOK_H

#include <QObject>
#include <QTimer>
#include "common.h"

class Hook : public QObject
{
    Q_OBJECT

public:
    explicit Hook(QObject *parent = Q_NULLPTR);
    ~Hook();

    int installHook();
    int uninstallHook();

    void sendInfos();
    void addSendInfo(SENDINFO& info);

    void setEffectFun(EFFECT_FUN fun);

private:
    QTimer* m_timer;

    QList<SENDINFO>* sendInfoList;
};

#endif // HOOK_H
