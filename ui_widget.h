/********************************************************************************
** Form generated from reading UI file 'widget.ui'
**
** Created by: Qt User Interface Compiler version 6.5.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>
#include <paintwidget.h>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QPushButton *pb_install;
    QPushButton *pb_unload;
    QStackedWidget *sw_test;
    QWidget *pg_test;
    QLabel *lb_return;
    QTextEdit *textEdit;
    QWidget *pg_set;
    QPushButton *pb_setOneWay;
    QStackedWidget *sw_set;
    QWidget *pg_oneWay;
    QWidget *pg_ways;
    PaintWidget *widget;
    QLabel *label;
    QSpinBox *sb_moveDistance;
    QLabel *label_2;
    QLabel *label_3;
    QSpinBox *sb_judgeDistance;
    QLabel *label_7;
    QLabel *label_8;
    QPushButton *pb_key1;
    QPushButton *pb_key2;
    QPushButton *pb_key3;
    QPushButton *pb_keysSave;
    QPushButton *pb_keysType;
    QPushButton *pb_save;
    QPushButton *pb_set;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName("Widget");
        Widget->resize(915, 561);
        pb_install = new QPushButton(Widget);
        pb_install->setObjectName("pb_install");
        pb_install->setGeometry(QRect(110, 30, 181, 41));
        pb_install->setFocusPolicy(Qt::NoFocus);
        pb_unload = new QPushButton(Widget);
        pb_unload->setObjectName("pb_unload");
        pb_unload->setGeometry(QRect(350, 30, 201, 41));
        pb_unload->setFocusPolicy(Qt::NoFocus);
        sw_test = new QStackedWidget(Widget);
        sw_test->setObjectName("sw_test");
        sw_test->setGeometry(QRect(20, 100, 871, 441));
        sw_test->setStyleSheet(QString::fromUtf8("background-color: rgb(231, 249, 255);"));
        pg_test = new QWidget();
        pg_test->setObjectName("pg_test");
        lb_return = new QLabel(pg_test);
        lb_return->setObjectName("lb_return");
        lb_return->setGeometry(QRect(180, 50, 471, 61));
        lb_return->setStyleSheet(QString::fromUtf8("font: 20pt \"\351\273\221\344\275\223\";\n"
"background-color: rgb(221, 255, 202);"));
        lb_return->setAlignment(Qt::AlignCenter);
        textEdit = new QTextEdit(pg_test);
        textEdit->setObjectName("textEdit");
        textEdit->setGeometry(QRect(140, 150, 551, 221));
        textEdit->setFocusPolicy(Qt::ClickFocus);
        textEdit->setStyleSheet(QString::fromUtf8(""));
        sw_test->addWidget(pg_test);
        pg_set = new QWidget();
        pg_set->setObjectName("pg_set");
        pb_setOneWay = new QPushButton(pg_set);
        pb_setOneWay->setObjectName("pb_setOneWay");
        pb_setOneWay->setGeometry(QRect(30, 70, 61, 41));
        pb_setOneWay->setFocusPolicy(Qt::NoFocus);
        sw_set = new QStackedWidget(pg_set);
        sw_set->setObjectName("sw_set");
        sw_set->setGeometry(QRect(120, 10, 741, 421));
        sw_set->setStyleSheet(QString::fromUtf8("background-color: rgb(219, 232, 255);\n"
"font: 14pt \"\351\273\221\344\275\223\";"));
        pg_oneWay = new QWidget();
        pg_oneWay->setObjectName("pg_oneWay");
        sw_set->addWidget(pg_oneWay);
        pg_ways = new QWidget();
        pg_ways->setObjectName("pg_ways");
        widget = new PaintWidget(pg_ways);
        widget->setObjectName("widget");
        widget->setGeometry(QRect(10, 10, 401, 401));
        widget->setStyleSheet(QString::fromUtf8("background-color: rgb(229, 255, 201);"));
        label = new QLabel(pg_ways);
        label->setObjectName("label");
        label->setGeometry(QRect(500, 30, 151, 41));
        label->setAlignment(Qt::AlignCenter);
        sb_moveDistance = new QSpinBox(pg_ways);
        sb_moveDistance->setObjectName("sb_moveDistance");
        sb_moveDistance->setGeometry(QRect(561, 250, 131, 31));
        sb_moveDistance->setFocusPolicy(Qt::ClickFocus);
        sb_moveDistance->setMaximum(200);
        label_2 = new QLabel(pg_ways);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(430, 250, 131, 31));
        label_2->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_3 = new QLabel(pg_ways);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(430, 300, 131, 31));
        label_3->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        sb_judgeDistance = new QSpinBox(pg_ways);
        sb_judgeDistance->setObjectName("sb_judgeDistance");
        sb_judgeDistance->setGeometry(QRect(561, 300, 131, 31));
        sb_judgeDistance->setFocusPolicy(Qt::ClickFocus);
        sb_judgeDistance->setMaximum(140);
        label_7 = new QLabel(pg_ways);
        label_7->setObjectName("label_7");
        label_7->setGeometry(QRect(510, 120, 31, 31));
        label_7->setStyleSheet(QString::fromUtf8("font: 16pt \"\351\273\221\344\275\223\";"));
        label_7->setAlignment(Qt::AlignCenter);
        label_8 = new QLabel(pg_ways);
        label_8->setObjectName("label_8");
        label_8->setGeometry(QRect(610, 120, 31, 31));
        label_8->setStyleSheet(QString::fromUtf8("font: 16pt \"\351\273\221\344\275\223\";"));
        label_8->setAlignment(Qt::AlignCenter);
        pb_key1 = new QPushButton(pg_ways);
        pb_key1->setObjectName("pb_key1");
        pb_key1->setGeometry(QRect(440, 120, 71, 31));
        pb_key1->setFocusPolicy(Qt::NoFocus);
        pb_key1->setStyleSheet(QString::fromUtf8(""));
        pb_key2 = new QPushButton(pg_ways);
        pb_key2->setObjectName("pb_key2");
        pb_key2->setGeometry(QRect(540, 120, 71, 31));
        pb_key2->setFocusPolicy(Qt::NoFocus);
        pb_key3 = new QPushButton(pg_ways);
        pb_key3->setObjectName("pb_key3");
        pb_key3->setGeometry(QRect(640, 120, 71, 31));
        pb_key3->setFocusPolicy(Qt::NoFocus);
        pb_keysSave = new QPushButton(pg_ways);
        pb_keysSave->setObjectName("pb_keysSave");
        pb_keysSave->setGeometry(QRect(590, 190, 71, 31));
        pb_keysSave->setFocusPolicy(Qt::NoFocus);
        pb_keysType = new QPushButton(pg_ways);
        pb_keysType->setObjectName("pb_keysType");
        pb_keysType->setGeometry(QRect(480, 190, 71, 31));
        pb_keysType->setFocusPolicy(Qt::NoFocus);
        sw_set->addWidget(pg_ways);
        pb_save = new QPushButton(pg_set);
        pb_save->setObjectName("pb_save");
        pb_save->setGeometry(QRect(30, 320, 61, 41));
        pb_save->setFocusPolicy(Qt::NoFocus);
        sw_test->addWidget(pg_set);
        pb_set = new QPushButton(Widget);
        pb_set->setObjectName("pb_set");
        pb_set->setGeometry(QRect(620, 30, 181, 41));
        pb_set->setFocusPolicy(Qt::NoFocus);

        retranslateUi(Widget);

        sw_test->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QCoreApplication::translate("Widget", "Widget", nullptr));
        pb_install->setText(QCoreApplication::translate("Widget", "\345\256\211\350\243\205\351\222\251\345\255\220", nullptr));
        pb_unload->setText(QCoreApplication::translate("Widget", "\345\215\270\350\275\275\351\222\251\345\255\220", nullptr));
        lb_return->setText(QString());
        textEdit->setHtml(QCoreApplication::translate("Widget", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'SimSun'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:14pt;\">123123123</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:14pt;\">5646456</span></p></body></html>", nullptr));
        pb_setOneWay->setText(QCoreApplication::translate("Widget", "\345\215\225\345\220\221", nullptr));
        label->setText(QCoreApplication::translate("Widget", "\345\275\223\345\211\215\345\277\253\346\215\267\351\224\256", nullptr));
        label_2->setText(QCoreApplication::translate("Widget", "\347\247\273\345\212\250\350\267\235\347\246\273\357\274\232", nullptr));
        label_3->setText(QCoreApplication::translate("Widget", "\345\210\244\346\226\255\350\267\235\347\246\273\357\274\232", nullptr));
        label_7->setText(QCoreApplication::translate("Widget", "+", nullptr));
        label_8->setText(QCoreApplication::translate("Widget", "+", nullptr));
        pb_key1->setText(QString());
        pb_key2->setText(QString());
        pb_key3->setText(QString());
        pb_keysSave->setText(QCoreApplication::translate("Widget", "\344\277\235\345\255\230", nullptr));
        pb_keysType->setText(QCoreApplication::translate("Widget", "\345\220\214\346\227\266", nullptr));
        pb_save->setText(QCoreApplication::translate("Widget", "\344\277\235\345\255\230", nullptr));
        pb_set->setText(QCoreApplication::translate("Widget", "\350\256\276\347\275\256", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
