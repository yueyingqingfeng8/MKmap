#ifndef WIDGET_H
#define WIDGET_H

#include "common.h"
#include "mkfun.h"
#include "mouseoneway.h"
#include "tray.h"
#include "filerwer.h"

#define  SETFILEPATH     "../setFile.json"   //配置文件路径

namespace Ui {
class Widget;
}
 
class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

    enum PgTestIndex
    {
        PG_TEST,
        PG_MAIN
    };

private slots:
    void on_pb_install_clicked();

    void on_pb_unload_clicked();

    void on_pb_test_clicked();

    void on_pb_save_clicked();

    void on_pb_addOrderInfo_clicked();

    void on_pb_deleteOrderInfo_clicked();

    void on_pb_saveOrderInfo_clicked();

    void on_pb_clearOrderInfo_clicked();

    void showLogInfo(mainUI::LogType type, QString str);


private:
    void initMkfun();
    void deleteMkfun();

    void closeEvent(QCloseEvent *event) override;

    //托盘程序
    void runTrayCommand(Tray::TrayCommands num);

private:
    Ui::Widget *ui;
     Tray *m_tray;

    Mkfun* m_mkfun;

    QString m_currentDirection;
    QPair<int, QStringList>  m_currentKeysInfo;

};






#endif // WIDGET_H
