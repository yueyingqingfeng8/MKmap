#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "mkfun.h"
#include "paintwidget.h"
#include <QPair>
#include <QMouseEvent>
#include <QKeyEvent>
#include "tray.h"

namespace Ui {
class Widget;
}
 
class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

    void initMkfun();
    void deleteMkfun();

    void showMouseInfo(QString str, int num);
    void showKeys(QString direction);

    void keyPressEvent(QKeyEvent *event);

private slots:
    void on_pb_install_clicked();

    void on_pb_unload_clicked();

    void on_pb_setOneWay_clicked();

    void on_pb_set_clicked();

    void on_pb_save_clicked();

    void on_pb_key1_clicked();

    void on_pb_key2_clicked();

    void on_pb_key3_clicked();

    void on_pb_keysSave_clicked();

    void on_sb_moveDistance_editingFinished();

    void on_sb_judgeDistance_editingFinished();

private:
    void runTrayCommand(Tray::TrayCommands num);
    void closeEvent(QCloseEvent *event);

private:
    Ui::Widget *ui;
     Tray *m_tray;

    Mkfun* m_mkfun;

    int m_keyState_1;
    int m_keyState_2;
    int m_keyState_3;

    QString m_currentDirection;
    QPair<int, QStringList>  m_currentKeysInfo;
};

enum PgTestIndex
{
    PG_TEST,
    PG_SET
};

enum PgSetIndex
{
    PG_WAYS,
    PG_ONEWAY
};


#endif // WIDGET_H
