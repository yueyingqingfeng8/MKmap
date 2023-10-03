#ifndef WGADDKEYS_H
#define WGADDKEYS_H

#include "wgkeyslistitem.h"

namespace Ui {
class WgAddKeys;
}

class WgAddKeys : public WgKeysListItem
{
    Q_OBJECT

public:
    explicit WgAddKeys(int& index, QWidget *parent = nullptr);
    ~WgAddKeys();

    //不需要这些,只是接口需要实现一下
    void setKeysNum(int&&) override {}
    void setNormalState() override {}
    void setSelectedState() override {}

signals:
    void signalAddkeysWidget(int);

private slots:
    void on_pushButton_clicked();

private:
    Ui::WgAddKeys *ui;
};

#endif // WGADDKEYS_H
