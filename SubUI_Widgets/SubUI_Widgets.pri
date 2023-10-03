INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS +=  \
    $$PWD/WgKeysList/wg2keyorder.h \
    $$PWD/WgKeysList/wg3keyorder.h \
    $$PWD/WgKeysList/wgaddkeys.h \
    $$PWD/WgKeysList/wgkeyslistitem.h \
    $$PWD/WgOrderInfoList/wglistitem.h \
    $$PWD/wgonewayorder.h \
    $$PWD/WgOrderInfoList/wgselect.h \
    $$PWD/WgOrderInfoList/wgshortcutkey.h \
    $$PWD/WgOrderInfoList/wgstring.h


SOURCES += \
    $$PWD/WgKeysList/wg2keyorder.cpp \
    $$PWD/WgKeysList/wg3keyorder.cpp \
    $$PWD/WgKeysList/wgaddkeys.cpp \
    $$PWD/wgonewayorder.cpp \
    $$PWD/WgOrderInfoList/wgselect.cpp \
    $$PWD/WgOrderInfoList/wgshortcutkey.cpp \
    $$PWD/WgOrderInfoList/wgstring.cpp


	
FORMS += \
    $$PWD/WgKeysList/wg2keyorder.ui \
    $$PWD/WgKeysList/wg3keyorder.ui \
    $$PWD/WgKeysList/wgaddkeys.ui \
    $$PWD/wgonewayorder.ui \
    $$PWD/WgOrderInfoList/wgselect.ui \
    $$PWD/WgOrderInfoList/wgshortcutkey.ui \
    $$PWD/WgOrderInfoList/wgstring.ui

