#ifndef FILERWER_H
#define FILERWER_H

#include <QObject>
#include "common.h"
#include "ordermap.h"


class FileRWer
{
public:

    static FileRWer& initFileRW(const char* str){
        static FileRWer fileRWerSelf(str);
        return fileRWerSelf;
    }

    static std::shared_ptr<orderMap::OrderMapBox> readSetFileMap(){
        return initFileRW(NULL).insideReadSetFileMap();
    }
    static void writeSetFileMap(std::shared_ptr<orderMap::OrderMapBox> orderMap){
        return initFileRW(NULL).insideWriteSetFileMap(orderMap);
    }

private:
    FileRWer(const char* str):m_path(str){}
    FileRWer(const FileRWer& other) = delete;
    FileRWer& operator=(const FileRWer& other) = delete;

    std::shared_ptr<orderMap::OrderMapBox> insideReadSetFileMap();
    void insideWriteSetFileMap(std::shared_ptr<orderMap::OrderMapBox> orderMap);

    std::shared_ptr<orderMap::OrderMapBox> readFileToMap(QJsonObject &&jsFile);
    std::shared_ptr<orderMap::OrderInfo> dealOrderInfo(QJsonObject& jsObject);

    std::shared_ptr<QJsonObject> readMapToFile(std::shared_ptr<orderMap::OrderMapBox> orderMap);

private:
    const char* m_path;

    static FileRWer fileRWerSelf;
};

#endif // FILERWER_H
