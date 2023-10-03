#ifndef KEYMAP_H
#define KEYMAP_H

#include <map>
#include "keysInt.h"

class KeysMap
{
public:

    static KeysMap& InitKeysMap(){
        static KeysMap m_keysMap;
        return m_keysMap;
    }

    //不包含时,默认为0或空字符串
    static std::string keyInt2Str(int&& keyInt){
        return InitKeysMap().insideKeyInt2Str(keyInt);
    }

    static int keyStr2Int(std::string&& str){
        return InitKeysMap().insideKeyStr2Int(str);
    }

    static void AddKeys(std::map<int, std::string>& keysMap){
        InitKeysMap().insideAddKeys(keysMap);
    }

private:
    KeysMap(const KeysMap& other) = delete;
    KeysMap& operator=(const KeysMap& other) = delete;

    KeysMap(){
        using namespace keysInt;
        keyStr2IntMap.insert(std::make_pair("shift", _shift));
        keyStr2IntMap.insert(std::make_pair("ctrl", _ctrl));
        keyStr2IntMap.insert(std::make_pair("alt", _alt));
        keyStr2IntMap.insert(std::make_pair(" ", _space));
        keyStr2IntMap.insert(std::make_pair("\t", _tab));
        keyStr2IntMap.insert(std::make_pair("\n", _enter));
        keyStr2IntMap.insert(std::make_pair("\\", _backslash));
        keyStr2IntMap.insert(std::make_pair("'", _singleQuotationMarks));
        keyStr2IntMap.insert(std::make_pair("\"", _doubleQuotationMarks));
        keyStr2IntMap.insert(std::make_pair("`", _backtick));
        keyStr2IntMap.insert(std::make_pair("~", _tilde));
        keyStr2IntMap.insert(std::make_pair("-", _sub));
        keyStr2IntMap.insert(std::make_pair("_", _underscore));
        keyStr2IntMap.insert(std::make_pair("=", _equal));
        keyStr2IntMap.insert(std::make_pair("+", _add));
        keyStr2IntMap.insert(std::make_pair("[", _lSquareBrackets));
        keyStr2IntMap.insert(std::make_pair("{", _lBrace));
        keyStr2IntMap.insert(std::make_pair("]", _rSquareBrackets));
        keyStr2IntMap.insert(std::make_pair("}", _rBrace));
        keyStr2IntMap.insert(std::make_pair("||", _pipe));      // |符号被用来指定一些按键操作,使用||才会输出|符号
        keyStr2IntMap.insert(std::make_pair(";", _semicolon));
        keyStr2IntMap.insert(std::make_pair(":", _colon));
        keyStr2IntMap.insert(std::make_pair(",", _comma));
        keyStr2IntMap.insert(std::make_pair("<", _lessThan));
        keyStr2IntMap.insert(std::make_pair(".", _fullStop));
        keyStr2IntMap.insert(std::make_pair(">", _greaterThan));
        keyStr2IntMap.insert(std::make_pair("/", _forwardSlash));
        keyStr2IntMap.insert(std::make_pair("?", _questionMark));
        keyStr2IntMap.insert(std::make_pair("!", _exclamationMark));
        keyStr2IntMap.insert(std::make_pair("@", _at));
        keyStr2IntMap.insert(std::make_pair("#", _hashtag));
        keyStr2IntMap.insert(std::make_pair("$", _money));
        keyStr2IntMap.insert(std::make_pair("%", _percentSign));
        keyStr2IntMap.insert(std::make_pair("^", _caret));
        keyStr2IntMap.insert(std::make_pair("&", _ampersand));
        keyStr2IntMap.insert(std::make_pair("*", _star));
        keyStr2IntMap.insert(std::make_pair("(", _lParenthesis));
        keyStr2IntMap.insert(std::make_pair(")", _rParenthesis));

        //特殊的可以在字符串中操作的按键
        keyStr2IntMap.insert(std::make_pair("|0", _esc));
        keyStr2IntMap.insert(std::make_pair("|1", _back));
        keyStr2IntMap.insert(std::make_pair("|2", _del));

        keyStr2IntMap.insert(std::make_pair("|a", _up));
        keyStr2IntMap.insert(std::make_pair("|b", _down));
        keyStr2IntMap.insert(std::make_pair("|c", _left));
        keyStr2IntMap.insert(std::make_pair("|d", _right));
        keyStr2IntMap.insert(std::make_pair("|e", _home));
        keyStr2IntMap.insert(std::make_pair("|f", _end));
        keyStr2IntMap.insert(std::make_pair("|A", _shiftUp));
        keyStr2IntMap.insert(std::make_pair("|B", _shiftDown));
        keyStr2IntMap.insert(std::make_pair("|C", _shiftLeft));
        keyStr2IntMap.insert(std::make_pair("|D", _shiftRight));
        keyStr2IntMap.insert(std::make_pair("|E", _shiftHome));
        keyStr2IntMap.insert(std::make_pair("|F", _shiftEnd));

        for (auto& ch : std::string("abcdefghijklmnopqrstuvwxyz"))
            keyStr2IntMap.insert(std::make_pair(std::string(1, ch), int(ch) - int('a') + _a));

        for (auto& ch : std::string("ABCDEFGHIJKLMNOPQRSTUVWXYZ"))
            keyStr2IntMap.insert(std::make_pair(std::string(1, ch), int(ch) - int('A') + _A));

        for (auto& ch : std::string("1234567890"))
            keyStr2IntMap.insert(std::make_pair(std::string(1, ch), int(ch) - int('1') + _1));

        //将字符到数字转化为数字到字符的map
        for (auto& keyValue : keyStr2IntMap)
            keyInt2StrMap.insert(std::make_pair(keyValue.second, keyValue.first));
    }

    std::string insideKeyInt2Str(int& keyInt){
        if (keyInt2StrMap.find(keyInt) == keyInt2StrMap.end())
            return "";
        return keyInt2StrMap[keyInt];
    }

    int insideKeyStr2Int(std::string& str){
        if (keyStr2IntMap.find(str) == keyStr2IntMap.end())
            return 0;
        return keyStr2IntMap[str];
    }

    void insideAddKeys(std::map<int, std::string>& keysMap){
        for (auto& key : keysMap)
            keyInt2StrMap.insert(key);
    }

private:
    std::map<int, std::string> keyInt2StrMap;
    std::map<std::string, int> keyStr2IntMap;


    static KeysMap m_keysMap;
};


#endif // KEYMAP_H
