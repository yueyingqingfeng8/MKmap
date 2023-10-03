#ifndef WG2KEYORDER_H
#define WG2KEYORDER_H

#include "wgkeyslistitem.h"
#include "common.h"
#include "keymap.h"

namespace Ui {
class Wg2KeyOrder;
}

class Wg2KeyOrder : public WgKeysListItem
{
    Q_OBJECT

public:
    explicit Wg2KeyOrder(int &index, QWidget *parent = nullptr);
    ~Wg2KeyOrder();

    virtual void setKeysNum(int&& keysNum) override{
        m_keysNum = keysNum;
        showKeysInfo();
    }

    void setNormalState() override;
    void setSelectedState() override;

public slots:
    bool eventFilter(QObject *,QEvent *);

private slots:
    void on_pb_key1_clicked();

    void on_pb_key2_clicked();

    void on_pb_saveOrDel_clicked();

private:
    std::tuple<bool, int> saveKeysInfo();
    void showKeysInfo();

    void initKeysState();
    void showSaveButton();
    void showDeleteButton();

    void keyPressEvent(QKeyEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

signals:
    void signalSetCurrentIndex(int index, int keysNum);
    void signalDeleteKeys(int index, int keysNum);
    void signalSaveKeys(int index, int oldKeysNum, int newKeysNum);

private:
    Ui::Wg2KeyOrder *ui;

    int m_keyState_1;
    int m_keyState_2;
    int m_saveFlag;
};

#endif // WG2KEYORDER_H
