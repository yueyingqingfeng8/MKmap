#ifndef MKFUN_H
#define MKFUN_H

#include <QObject>
#include <QKeyEvent>
#include "hook.h"


class Mkfun : public QObject
{
    Q_OBJECT
public:
    explicit Mkfun(QObject *parent = Q_NULLPTR);
    ~Mkfun();

    void initClass();
    void exitClass();

    int installHook();
    int uninstallHook();

    int addMouseInfoEvent(int type, MSLLHOOKSTRUCT* mouseInfo);
    int addKeyboardInfoEvent(QString key1, QString key2, QString key3);
    int addKeyboardInfoEvent(QString keys);

    void installEffect(int fun);
    void setPrintMouseInfoFun(CALLBACKMOUSE_FUN fun);

    void readControlsSet();
    void saveControlsSet();
    void makeControlsSet();
    void initKeysMap();
    void modifyControlSet(QString direction, int type, QStringList strList);

    QList<QString> searchKeys(QString direction, int &type);
    QString searchInt2Keys(int num);

    int mkFunType();
    QString mkFunTypeStr(int &type);

    //右键单向的功能
    void initOnewayFunInfo();
    int effectFun_oneway(int& code, WPARAM& wParam, LPARAM& lParam);
    void judgeDirection(MSLLHOOKSTRUCT *pMouseStruct);
    WORD convertKeyValue(QString key);
    void setDistance(int& move, int& judge);

private:
    Hook* m_Hook;

    int m_mkFunType;

    QPoint* m_pos;
    QTimer* m_timer;

    int m_intercept_time;

    EFFECT_FUN         m_effectFun;
    CALLBACKMOUSE_FUN  m_printMouseInfoFun;

    QJsonObject m_setInfo;
    QJsonObject m_keysInfo;

    QMap<QString, int>* m_keys2int;
    QMap<int, QString>* m_int2keys;

    //右键功能
    bool m_rightKeyMkfun;
    bool m_replaceRightKey;
    int m_moveDistance;
    int m_judgeDistance;
};

#endif // MKFUN_H
