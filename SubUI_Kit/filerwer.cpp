#include "filerwer.h"

std::shared_ptr<orderMap::OrderMapBox> FileRWer::insideReadSetFileMap()
{
    QFile file(m_path);
    QByteArray buffer;

    if(!file.exists())
        return std::shared_ptr<orderMap::OrderMapBox>();

    if(file.open(QFile::ReadOnly))
    {
        buffer = file.readAll();
        file.close();
    }

    QJsonParseError jsonTemp;
    QJsonDocument doucment = QJsonDocument::fromJson(buffer, &jsonTemp);

    // 解析未发生错误
    if (!doucment.isNull() &&
        (jsonTemp.error == QJsonParseError::NoError))
    {
        if (doucment.isObject())
        {
            return readFileToMap(doucment.object());
        }
    }

    return std::shared_ptr<orderMap::OrderMapBox>();
}

void FileRWer::insideWriteSetFileMap(std::shared_ptr<orderMap::OrderMapBox> orderMap)
{
    QJsonDocument jsonDoc(*(readMapToFile(orderMap)));
    QByteArray text = jsonDoc.toJson();     //将文档转换为带缩进的json格式字符数组(相当于一种带换行和制表符的字符串)

    //由于json转换成字符数组时默认是 GBK 的格式,所以可以转化为 UTF-8 的格式,方法是先转换为 unicode 格式字符串,再转为UTF-8
    QTextCodec *gbk = QTextCodec::codecForName("GBK");      //设置一种文本格式为GBK
    QString str_unicode = gbk->toUnicode(text);             //将GBK格式转到unicode格式,返回其格式字符串
    QTextCodec *utf8 = QTextCodec::codecForName("UTF-8");   //设置一种格式是UTF-8
    QString str_uft8 = utf8->fromUnicode(str_unicode);      //将unicode格式转到UTF-8,返回其格式字符串

    QFile file(m_path);               //用文本流打开文件后写入UTF-8格式的字符串
    if( file.open(QIODevice::WriteOnly) )
    {
        QTextStream stream(&file);
        stream << str_uft8 ;

        file.close();
    }
}

std::shared_ptr<orderMap::OrderMapBox> FileRWer::readFileToMap(QJsonObject&& jsFile)
{
    orderMap::OrderMapBox orderMapTemp;
    std::shared_ptr<orderMap::OrderMapBox> orderMap = std::make_shared<orderMap::OrderMapBox>(orderMapTemp);

    QStringList keys = jsFile.keys();
    for (auto& key : keys)
    {
        std::list<orderMap::OrderInfo> list;
        QJsonValue value = jsFile.value(key);
        if (value.isArray())
        {
            QJsonArray array = value.toArray();

            for (auto item : array)
            {
                if (item.isObject())
                {
                    QJsonObject info = item.toObject();

                    std::shared_ptr<orderMap::OrderInfo> orderInfo = dealOrderInfo(info);
                    list.push_back(*orderInfo);
                }
            }
        }

        orderMap->insert(std::make_pair(key.toStdString(), list));
    }

    return orderMap;
}

std::shared_ptr<orderMap::OrderInfo> FileRWer::dealOrderInfo(QJsonObject& jsObject)
{
    orderMap::OrderInfo orderInfo{};

    orderInfo.type = jsObject.value("type").toInt();
    orderInfo.delayTime = jsObject.value("delayTime").toInt();

    //data数据可能是字符串和数组
    QJsonValue jsValue = jsObject.value("data");
    if (jsValue.isArray())
    {
        QJsonArray array = jsValue.toArray();

        std::list<int> dataList;
        for (auto it = array.begin(); it != array.end(); ++it)
            dataList.push_back((*it).toInt());

        orderInfo.data = dataList;
    }
    else if (jsValue.isString())
    {
        orderInfo.data = jsValue.toString().toStdString();
    }

    std::shared_ptr<orderMap::OrderInfo> info = std::make_shared<orderMap::OrderInfo>(orderInfo);

    return info;
}

std::shared_ptr<QJsonObject> FileRWer::readMapToFile(std::shared_ptr<orderMap::OrderMapBox> orderMap)
{
    QJsonObject root;

    if (orderMap->empty())
        return std::make_shared<QJsonObject>(root);

    for (auto& pair : *orderMap)
    {
        std::string strame = pair.first;
        std::list<orderMap::OrderInfo> infoList = pair.second;

        if (infoList.empty())
            continue;

        QJsonArray array;
        for (auto& info : infoList)
        {
            QJsonObject orderObject;

            int type = info.type;
            int delayTime = info.delayTime;

            orderObject.insert("type", type);
            orderObject.insert("delayTime", delayTime);

            if (std::holds_alternative<std::list<int>>(info.data))
            {
                QJsonArray jsDataArray;
                std::list<int> list = std::get<std::list<int>>(info.data);

                for (auto item : list)
                    jsDataArray.append(item);

                orderObject.insert("data", jsDataArray);
            }

            if (std::holds_alternative<std::string>(info.data))
            {
                std::string str = std::get<std::string>(info.data);
                orderObject.insert("data", QString(str.c_str()));
            }

            array.append(orderObject);
        }

        root.insert(QString(strame.c_str()), array);
    }

    return std::make_shared<QJsonObject>(root);
}
