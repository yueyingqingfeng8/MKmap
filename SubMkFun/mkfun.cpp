#include "mkfun.h"
#include "keymap.h"
#include <QDebug>

Mkfun::Mkfun()
{
    initClass();

    initOnewayFunInfo();

    readControlsSet();
}

Mkfun::~Mkfun()
{
    exitClass();
}

int Mkfun::addMouseInfoEvent(int type, MSLLHOOKSTRUCT* mouseInfo)
{
    HOOK::SENDINFO info;
    memset(&info, 0x00, sizeof(HOOK::SENDINFO));

    info.lenth = 1;
    ZeroMemory(info.inputs, sizeof(info.inputs));

    info.inputs[0].type = INPUT_MOUSE;
    info.inputs[0].mi.dx = mouseInfo->pt.x;
    info.inputs[0].mi.dy = mouseInfo->pt.y;;
    info.inputs[0].mi.mouseData = 0;
    info.inputs[0].mi.dwFlags = type;
    info.inputs[0].mi.time = 0;
    info.inputs[0].mi.dwExtraInfo = 0;

    m_Hook->addSendInfo(info);

    return 1;
}

void Mkfun::installEffect(int funType)
{
    switch (funType)
    {
    case ONEWAY:
        m_Hook->setEffectFun(m_effectFun);
        break;
    default:
        break;
    }
}

void Mkfun::initOnewayFunInfo()
{
    m_pos->setX(-1);
    m_pos->setY(-1);
    m_timer->stop();
}

int Mkfun::effectFun_oneway(int& code, WPARAM& wParam, LPARAM& lParam)
{
    MSLLHOOKSTRUCT* pMouseStruct = (MSLLHOOKSTRUCT*)lParam;

    if(wParam == WM_RBUTTONDOWN)
    {
        if(m_rightKeyMkfun)
        {
            return 0;
        }
        if(!m_timer->isActive() && !m_keysInfo.isEmpty())
        {
            m_pos->setX(pMouseStruct->pt.x);
            m_pos->setY(pMouseStruct->pt.y);
            m_timer->start(m_intercept_time);
        }

        return 1;
    }

    if(wParam == WM_RBUTTONUP)
    {
        //右键被代替
        if(m_replaceRightKey)
        {
            m_replaceRightKey = false;
            return 1;
        }

        //右键功能
        if(m_rightKeyMkfun)
        {
            m_rightKeyMkfun = false;
            return 0;
        }

        if(m_timer->isActive())
        {
            initOnewayFunInfo();
        }

        m_rightKeyMkfun = true;
        addMouseInfoEvent(MOUSEEVENTF_RIGHTDOWN, pMouseStruct);
        addMouseInfoEvent(MOUSEEVENTF_RIGHTUP, pMouseStruct);

        return 1;
    }

    if(wParam == WM_MOUSEMOVE)
    {
        m_printMouseInfoFun("鼠标", pMouseStruct->pt.x * 10000 + pMouseStruct->pt.y);

        if(m_pos->x() == -1) return 0;

        if(m_timer->isActive())
        {
            judgeDirection(pMouseStruct);
        }
    }

    return 0;
}

void Mkfun::judgeDirection(MSLLHOOKSTRUCT *pMouseStruct)
{
    if(pow(pMouseStruct->pt.y - m_pos->y(), 2) + pow(pMouseStruct->pt.x - m_pos->x(), 2) < m_moveDistance * m_moveDistance)
    {
        return;
    }

    if(pMouseStruct->pt.y < m_pos->y() - m_judgeDistance)
    {
        //上面
        if(pMouseStruct->pt.x < m_pos->x() - m_judgeDistance)       //左上
        {
            addKeyboardInfoEvent("UL");
        }
        else if(pMouseStruct->pt.x > m_pos->x() + m_judgeDistance)  //右上
        {
            addKeyboardInfoEvent("UR");
        }
        else                                                    //上
        {
            addKeyboardInfoEvent("U");
        }
    }
    else if(pMouseStruct->pt.y > m_pos->y() + m_judgeDistance)
    {
        //下面
        if(pMouseStruct->pt.x < m_pos->x() - m_judgeDistance)       //左下
        {
            addKeyboardInfoEvent("DL");
        }
        else if(pMouseStruct->pt.x > m_pos->x() + m_judgeDistance)  //右下
        {
            addKeyboardInfoEvent("DR");
        }
        else                                                    //下
        {
            addKeyboardInfoEvent("D");
        }
    }
    else if(pMouseStruct->pt.x < m_pos->x() - m_judgeDistance)
    {
        addKeyboardInfoEvent("L");                     //左
    }
    else
    {
        addKeyboardInfoEvent("R");                      //右
    }

    m_replaceRightKey = true;
    initOnewayFunInfo();
}

int Mkfun::addKeyboardInfoEvent(QString keys)
{
    QJsonObject keysInfo = m_keysInfo.value(keys).toObject();

    if(keysInfo.isEmpty()) return 0;

    int type = keysInfo.value("type").toInt();
    QJsonArray array = keysInfo.value("keys").toArray();

    if (!array.isEmpty())
    {
        HOOK::SENDINFO info;
        memset(&info, 0x00, sizeof(HOOK::SENDINFO));

        info.lenth = 0;
        ZeroMemory(info.inputs, sizeof(info.inputs));

        if (DDUU == type)
        {
            for (int i = 0; i < array.count(); i++)
            {
                info.inputs[info.lenth].type = INPUT_KEYBOARD;
                info.inputs[info.lenth].ki.wVk = array[i].toInt();
                info.lenth++;
            }

            for (int i = 0; i < array.count(); i++)
            {
                info.inputs[info.lenth].type = INPUT_KEYBOARD;
                info.inputs[info.lenth].ki.wVk = array[i].toInt();
                info.inputs[info.lenth].ki.dwFlags = KEYEVENTF_KEYUP;
                info.lenth++;
            }
        }
        else if (DUDU == type)
        {
            for (int i = 0; i < array.count(); i++)
            {
                info.inputs[info.lenth].type = INPUT_KEYBOARD;
                info.inputs[info.lenth].ki.wVk = array[i].toInt();
                info.lenth++;

                info.inputs[info.lenth].type = INPUT_KEYBOARD;
                info.inputs[info.lenth].ki.wVk = array[i].toInt();
                info.inputs[info.lenth].ki.dwFlags = KEYEVENTF_KEYUP;
                info.lenth++;
            }
        }


        m_Hook->addSendInfo(info);

    }

    return 1;
}

int Mkfun::addKeyboardInfoEvent(QString key1, QString key2, QString key3)
{
    int key1_t = 0;
    int key2_t = 0;
    int key3_t = 0;

    HOOK::SENDINFO info;
    memset(&info, 0x00, sizeof(HOOK::SENDINFO));

    info.lenth = 0;
    ZeroMemory(info.inputs, sizeof(info.inputs));

    key1_t = convertKeyValue(key1);
    if(key1_t)
    {
        info.inputs[info.lenth].type = INPUT_KEYBOARD;
        info.inputs[info.lenth].ki.wVk = key1_t;
        info.lenth++;
    }

    key2_t = convertKeyValue(key2);
    if(key2_t)
    {
        info.inputs[info.lenth].type = INPUT_KEYBOARD;
        info.inputs[info.lenth].ki.wVk = key2_t;
        info.lenth++;
    }

    key3_t = convertKeyValue(key3);
    if(key3_t)
    {
        info.inputs[info.lenth].type = INPUT_KEYBOARD;
        info.inputs[info.lenth].ki.wVk = key3_t;
        info.lenth++;
    }

    key1_t = convertKeyValue(key1);
    if(key1_t)
    {
        info.inputs[info.lenth].type = INPUT_KEYBOARD;
        info.inputs[info.lenth].ki.wVk = key1_t;
        info.inputs[info.lenth].ki.dwFlags = KEYEVENTF_KEYUP;
        info.lenth++;
    }

    key2_t = convertKeyValue(key2);
    if(key2_t)
    {
        info.inputs[info.lenth].type = INPUT_KEYBOARD;
        info.inputs[info.lenth].ki.wVk = key2_t;
        info.inputs[info.lenth].ki.dwFlags = KEYEVENTF_KEYUP;
        info.lenth++;
    }

    key3_t = convertKeyValue(key3);
    if(key3_t)
    {
        info.inputs[info.lenth].type = INPUT_KEYBOARD;
        info.inputs[info.lenth].ki.wVk = key3_t;
        info.inputs[info.lenth].ki.dwFlags = KEYEVENTF_KEYUP;
        info.lenth++;
    }

    m_Hook->addSendInfo(info);

    return 0;

#if 0  //和如下效果类似
    INPUT inputs[6] = {};
    ZeroMemory(inputs, sizeof(inputs));

    inputs[0].type = INPUT_KEYBOARD;
    inputs[0].ki.wVk = VK_CONTROL;

    inputs[1].type = INPUT_KEYBOARD;
    inputs[1].ki.wVk = VK_SHIFT;

    inputs[2].type = INPUT_KEYBOARD;
    inputs[2].ki.wVk = 'O';


    inputs[3].type = INPUT_KEYBOARD;
    inputs[3].ki.wVk = VK_CONTROL;
    inputs[3].ki.dwFlags = KEYEVENTF_KEYUP;

    inputs[4].type = INPUT_KEYBOARD;
    inputs[4].ki.wVk = VK_SHIFT;
    inputs[4].ki.dwFlags = KEYEVENTF_KEYUP;

    inputs[5].type = INPUT_KEYBOARD;
    inputs[5].ki.wVk =  'o';
    inputs[5].ki.dwFlags = KEYEVENTF_KEYUP;
#endif
}

void Mkfun::setDistance(int& move, int& judge)
{
    m_moveDistance = move;
    m_judgeDistance = judge;

    m_setInfo.insert("OneWayMoveDistance", m_moveDistance);
    m_setInfo.insert("OneWayJudgeDistance", m_judgeDistance);
}

WORD Mkfun::convertKeyValue(QString key)
{
    if(key == "shift") return VK_SHIFT;

    if(key == "ctrl") return VK_CONTROL;

    if(key == "alt") return VK_MENU;

    if(key == " ") return VK_SPACE;

    if(key == "tab") return VK_TAB;

    if(key == "a") return 65;
    if(key == "b") return 66;
    if(key == "c") return 67;
    if(key == "d") return 68;
    if(key == "e") return 69;
    if(key == "f") return 70;
    if(key == "g") return 71;
    if(key == "h") return 72;
    if(key == "i") return 73;
    if(key == "j") return 74;
    if(key == "k") return 75;
    if(key == "l") return 76;
    if(key == "m") return 77;
    if(key == "n") return 78;
    if(key == "o") return 79;
    if(key == "p") return 80;
    if(key == "q") return 81;
    if(key == "r") return 82;
    if(key == "s") return 83;
    if(key == "t") return 84;
    if(key == "u") return 85;
    if(key == "v") return 86;
    if(key == "w") return 87;
    if(key == "x") return 88;
    if(key == "y") return 89;
    if(key == "z") return 90;

    return 0;
}

void Mkfun::setPrintMouseInfoFun(CALLBACKMOUSE_FUN fun)
{
    m_printMouseInfoFun = fun;
}

int Mkfun::installHook()
{
    return m_Hook->installHook();
}

int Mkfun::uninstallHook()
{
    return m_Hook->uninstallHook();
}

void Mkfun::initClass()
{
    m_Hook  = new Hook();
    m_timer = new Timer();
    m_pos   = new QPoint(-1, -1);
    m_keys2int = new QMap<QString, int>();
    m_int2keys = new QMap<int, QString>();

    m_intercept_time = INTERCEPT_TIME;
    m_effectFun = std::bind(&Mkfun::effectFun_oneway, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

    m_rightKeyMkfun   = false;
    m_replaceRightKey = false;
    m_mkFunType       = ONEWAY;

    m_moveDistance = MOVE_DISTANCE;
    m_judgeDistance = JUDGE_DISTANCE;

    initKeysMap();
}

void Mkfun::exitClass()
{
    m_Hook->uninstallHook();
    m_keys2int->clear();
    m_int2keys->clear();

    delete m_keys2int;
    delete m_int2keys;
    delete m_Hook;
    delete m_pos;
    delete m_timer;
}

std::tuple<int, std::list<std::string>> Mkfun::searchKeys(std::string direction)
{
    std::list<std::string> strList;

    QJsonObject temp = m_keysInfo.value(QString(direction.c_str())).toObject();
    if (temp.isEmpty())
        return { -1, strList };

    QJsonArray array = temp.value("keys").toArray();
    if (array.isEmpty())
        return { -1, strList };

    for (int i = 0; i < array.count(); i++)
    {
        QString temp = m_int2keys->value(array[i].toInt());
        strList.push_back(temp.toStdString());
    }

    int type = temp.value("type").toInt();
    return { type, strList };
}

QString Mkfun::searchInt2Keys(int num)
{
    if (m_int2keys->contains(num))
    {
        return m_int2keys->value(num);
    }

    return QString("");
}

void Mkfun::readControlsSet()
{
    QFile file(SETFILEPATH);
    QByteArray buffer;

    if(!file.exists()) //文件不存在则生成一下基础配置
    {
        makeControlsSet();
        return;
    }

    if(file.open(QFile::ReadOnly))
    {
        buffer = file.readAll();

        file.close();
    }

    QJsonParseError jsonTemp;
    QJsonDocument doucment = QJsonDocument::fromJson(buffer, &jsonTemp); // 转化为 JSON 文档

    // 解析未发生错误
    if (!doucment.isNull() &&
       (jsonTemp.error == QJsonParseError::NoError))
    {
        if (doucment.isObject())
        {
            m_setInfo = doucment.object();

            if (m_setInfo.contains("OneWay"))
                m_keysInfo = m_setInfo.value("OneWay").toObject();
            if (m_setInfo.contains("OneWayMoveDistance"))
                m_moveDistance = m_setInfo.value("OneWayMoveDistance").toInt();
            if (m_setInfo.contains("OneWayJudgeDistance"))
                m_judgeDistance = m_setInfo.value("OneWayJudgeDistance").toInt();
        }
    }
}

void Mkfun::saveControlsSet()
{
    QJsonDocument jsonDoc(m_setInfo);      //创建json文档类对象,主要用于处理json文件的转换
    QByteArray text = jsonDoc.toJson();     //将文档转换为带缩进的json格式字符数组(相当于一种带换行和制表符的字符串)

    //由于json转换成字符数组时默认是 GBK 的格式,所以可以转化为 UTF-8 的格式,方法是先转换为 unicode 格式字符串,再转为UTF-8
    QTextCodec *gbk = QTextCodec::codecForName("GBK");      //设置一种文本格式为GBK
    QString str_unicode = gbk->toUnicode(text);             //将GBK格式转到unicode格式,返回其格式字符串
    QTextCodec *utf8 = QTextCodec::codecForName("UTF-8");   //设置一种格式是UTF-8
    QString str_uft8 = utf8->fromUnicode(str_unicode);      //将unicode格式转到UTF-8,返回其格式字符串

    QFile file(SETFILEPATH);               //用文本流打开文件后写入UTF-8格式的字符串
    if( file.open(QIODevice::WriteOnly) )
    {
        QTextStream stream(&file);
        stream << str_uft8 ;

        file.close();
    }
}

void Mkfun::makeControlsSet()
{
    QJsonObject controlsMap;

    QJsonArray keys1;
    keys1.append(_shift);
    keys1.append(_w);

    QJsonObject temp1;
    temp1.insert("type", DUDU);
    temp1.insert("keys", keys1);
    controlsMap.insert("U", temp1);

    QJsonArray keys2;
    keys2.append(_shift);
    keys2.append(_s);

    QJsonObject temp2;
    temp2.insert("type", DDUU);
    temp2.insert("keys", keys2);
    controlsMap.insert("D", temp2);

    QJsonArray keys3;
    keys3.append(_a);

    QJsonObject temp3;
    temp3.insert("type", DDUU);
    temp3.insert("keys", keys3);
    controlsMap.insert("L", temp3);

    QJsonArray keys4;
    keys4.append(_d);

    QJsonObject temp4;
    temp4.insert("type", DDUU);
    temp4.insert("keys", keys4);
    controlsMap.insert("R", temp4);

    QJsonArray keys5;
    keys5.append(_q);

    QJsonObject temp5;
    temp5.insert("type", DDUU);
    temp5.insert("keys", keys5);
    controlsMap.insert("UL", temp5);

    QJsonArray keys6;
    keys6.append(_e);

    QJsonObject temp6;
    temp6.insert("type", DDUU);
    temp6.insert("keys", keys6);
    controlsMap.insert("UR", temp6);

    QJsonArray keys7;
    keys7.append(_z);

    QJsonObject temp7;
    temp7.insert("type", DDUU);
    temp7.insert("keys", keys7);
    controlsMap.insert("DL", temp7);

    QJsonArray keys8;
    keys8.append(_c);

    QJsonObject temp8;
    temp8.insert("type", DDUU);
    temp8.insert("keys", keys8);
    controlsMap.insert("DR", temp8);

    m_setInfo.insert("OneWay", controlsMap);

    m_keysInfo = m_setInfo.value("OneWay").toObject();

    saveControlsSet();
}

void Mkfun::initKeysMap()
{
    m_keys2int->insert("shift", _shift);
    m_keys2int->insert("ctrl", _ctrl);
    m_keys2int->insert("alt", _alt);
    m_keys2int->insert("space", _space);
    m_keys2int->insert("tab", _tab);

    QString str = "abcdefghijklmnopqrstuvwxyz";
    for (int i = 0; i < 26; i++)
    {
        QString temp;
        temp.append(str.at(i));
        m_keys2int->insert(temp, i + _a);
    }

    QList<QString> keyList = m_keys2int->keys();
    QList<int> valueList = m_keys2int->values();

    while (!keyList.isEmpty())
    {
        m_int2keys->insert(valueList.takeFirst(), keyList.takeFirst());
    }

    //增加Qt的非字母键信息
    m_int2keys->insert(Qt::Key_Shift, QString("shift"));
    m_int2keys->insert(Qt::Key_Control, QString("ctrl"));
    m_int2keys->insert(Qt::Key_Tab, QString("tab"));
    m_int2keys->insert(Qt::Key_Alt, QString("alt"));
}

void Mkfun::modifyControlSet(QString direction, int type, QStringList strList)
{
    if (direction.isEmpty() || type == -1 || strList.count() == 0) return;

    QJsonArray array;

    while (!strList.isEmpty())
    {
        QJsonValue value(m_keys2int->value(strList.takeFirst()));
        array.append(value);
    }

    QJsonObject object;
    object.insert("type", type);
    object.insert("keys", array);

    m_keysInfo.insert(direction, object);

    if (!mkFunTypeStr(m_mkFunType).isEmpty())
    {
        m_setInfo.insert(mkFunTypeStr(m_mkFunType), m_keysInfo);
    }
}

int Mkfun::mkFunType()
{
    return m_mkFunType;
}

QString Mkfun::mkFunTypeStr(int& type)
{
    if (type == ONEWAY) return QString("OneWay");
    if (type == WAYS)   return QString("Ways");
    if (type == MK)     return QString("MK");

    return QString("");
}




