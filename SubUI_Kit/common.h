#ifndef COMMON_H
#define COMMON_H

#include "stdHeads.h"

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
#include <QVariantMap>
#include <QMenu>
#include <QtAlgorithms>
#include <QDebug>
#include <QPair>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QWidget>
#include <QPaintEvent>
#include <QRect>
#include <QPoint>
#include <QPainter>

#define WIDGET_COLOR_SELECTED   "QWidget#widget{background-color: #e0dcd1;}"
#define WIDGET_COLOR_NORMAL     "QWidget#widget{background-color: rgba(0,0,0,0);}"
#define KEYS_COLOR_SELECTED     "QPushButton{background-color: #a9b8a5;}"
#define KEYS_COLOR_NORMAL       "QPushButton{background-color: #f2f0e3;}"

namespace mainUI{
    enum LogType{
        NORMAL,
        WARN,
        ERR
    };
}

#endif // COMMON_H
