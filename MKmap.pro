#-------------------------------------------------
#
# Project created by QtCreator 2023-07-24T20:44:50
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 5): QT += core5compat
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MKmap
TEMPLATE = app

RC_ICONS = MK.ico

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += $$PWD/include
include(SubMK_Hook/SubMK_Hook.pri)
include(SubMK_MkFun/SubMK_MkFun.pri)
include(SubMK_Utility/SubMK_Utility.pri)
include(SubUI_Widgets/SubUI_Widgets.pri)
include(SubUI_Kit/SubUI_Kit.pri)
include(SubUI_Modules/SubUI_Modules.pri)

SOURCES += \
        main.cpp \
        widget.cpp \


HEADERS += \
        widget.h


FORMS += \
        widget.ui
