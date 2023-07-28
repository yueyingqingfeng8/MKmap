#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTimer>
#include <windows.h>
#include "hook.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

    void showMouse(QString str, int num);

private slots:
    void on_pb_install_clicked();

    void on_pb_unload_clicked();

    void on_pb_turn_clicked();

    void makeMouseInfo();


private:
    Ui::Widget *ui;

    Hook* m_Hook;

    QTimer* m_timer;
};




#endif // WIDGET_H
