#ifndef KEYSINT_H
#define KEYSINT_H

#include <windows.h>

#define SHIFT(num) (VK_SHIFT * 1000 + num)

namespace keysInt{
enum KeyMap {
    _shift = VK_SHIFT,
    _ctrl  = VK_CONTROL,
    _alt   = VK_MENU,
    _space = VK_SPACE,
    _tab   = VK_TAB,
    _enter = VK_RETURN,
    _back  = VK_BACK,
    _esc   = VK_ESCAPE,
    _del   = VK_DELETE,
    _home  = VK_HOME,
    _end   = VK_END,
    _up    = VK_UP,
    _down  = VK_DOWN,
    _left  = VK_LEFT,
    _right = VK_RIGHT,

    _backtick               = VK_OEM_3,             //  `
    _tilde                  = SHIFT(VK_OEM_3),      //  ~
    _sub                    = VK_OEM_MINUS,         //  -
    _underscore             = SHIFT(VK_OEM_MINUS),  //  _
    _equal                  = VK_OEM_PLUS,          //  =
    _add                    = SHIFT(VK_OEM_PLUS),   //  +
    _lSquareBrackets        = VK_OEM_4,             //  [
    _lBrace                 = SHIFT(VK_OEM_4),      //  {
    _rSquareBrackets        = VK_OEM_6,             //  ]
    _rBrace                 = SHIFT(VK_OEM_6),      //  }
    _backslash              = VK_OEM_5,             // 反斜杠
    _pipe                   = SHIFT(VK_OEM_5),      //  |
    _semicolon              = VK_OEM_1,             //  ;
    _colon                  = SHIFT(VK_OEM_1),      //  :
    _singleQuotationMarks   = VK_OEM_7,             //  '
    _doubleQuotationMarks   = SHIFT(VK_OEM_7),      //  "
    _comma                  = VK_OEM_COMMA,         //  ,
    _lessThan               = SHIFT(VK_OEM_COMMA),  //  <
    _fullStop               = VK_OEM_PERIOD,        //  .
    _greaterThan            = SHIFT(VK_OEM_PERIOD), //  >
    _forwardSlash           = VK_OEM_2,             //  /
    _questionMark           = SHIFT(VK_OEM_2),      //  ?
    _shiftLeft              = SHIFT(VK_LEFT),       //  shift + 左
    _shiftRight             = SHIFT(VK_RIGHT),      //  shift + 右
    _shiftUp                = SHIFT(VK_UP),         //  shitf + 上
    _shiftDown              = SHIFT(VK_DOWN),       //  shift + 下
    _shiftHome              = SHIFT(VK_HOME),       //  shift + home
    _shiftEnd               = SHIFT(VK_END),        //  shift + end
    _shiftTab               = SHIFT(VK_TAB),        //  shift + tab

    _0 = 48, _1,  _2, _3, _4, _5, _6, _7, _8, _9,

    _rParenthesis = SHIFT(48), _exclamationMark, _at, _hashtag, _money, _percentSign, _caret, _ampersand, _star, _lParenthesis,

    _a = 65,        _b, _c, _d, _e, _f, _g, _h, _i, _j, _k, _l, _m, _n, _o, _p, _q, _r, _s, _t, _u, _v, _w, _x, _y, _z,
    _A = SHIFT(65), _B, _C, _D, _E, _F, _G, _H, _I, _J, _K, _L, _M, _N, _O, _P, _Q, _R, _S, _T, _U, _V, _W, _X, _Y, _Z,

    _f1 = VK_F1, _f2, _f3, _f4, _f5, _f6, _f7, _f8, _f9, _f10, _f11, _f12

};
}


#endif // KEYSINT_H
