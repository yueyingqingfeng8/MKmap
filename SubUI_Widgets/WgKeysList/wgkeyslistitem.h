#ifndef WGKEYSLISTITEM_H
#define WGKEYSLISTITEM_H

#include <QWidget>

class WgKeysListItem : public QWidget
{
public:
    explicit WgKeysListItem(int& index, QWidget *parent = nullptr)
        : QWidget{parent}, m_index(index){
    }

    virtual ~WgKeysListItem(){}

    void setIndex(int&& index){
        m_index = index;
    }

    virtual void setKeysNum(int&&) = 0;
    virtual void setNormalState() = 0;
    virtual void setSelectedState() = 0;

protected:
    int m_index;
    int m_keysNum;
};

#endif // WGKEYSLISTITEM_H
