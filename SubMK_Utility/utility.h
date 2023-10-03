#ifndef UTILITY_H
#define UTILITY_H

#include "stdHeads.h"
#include "point.h"
#include "timer.h"
#include "keymap.h"

namespace mkFun {
    enum MouseFunType
    {
        BAN_MOUSE,
        MOUSE_ONEWAY

    };
    enum KeyboardFunType
    {
        BAN_KEYBOARD,
        KEYBOARD
    };

    typedef std::variant<std::string, long> CommandData;

    typedef struct commandInfo
    {
        std::list<std::tuple<int, CommandData>> commandList;
        std::mutex mutex;
        std::condition_variable condition;
    } CommandInfo;
}

typedef struct mouseOneWaySetInfo
{
    int moveDistance;
    int judgeDistance;
}MOUSE_OW_SETINFO;


#endif // UTILITY_H
