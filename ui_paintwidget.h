/********************************************************************************
** Form generated from reading UI file 'paintwidget.ui'
**
** Created by: Qt User Interface Compiler version 6.5.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PAINTWIDGET_H
#define UI_PAINTWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PaintWidget
{
public:

    void setupUi(QWidget *PaintWidget)
    {
        if (PaintWidget->objectName().isEmpty())
            PaintWidget->setObjectName("PaintWidget");
        PaintWidget->resize(401, 401);
        PaintWidget->setStyleSheet(QString::fromUtf8("background-color: rgb(229, 255, 201);"));

        retranslateUi(PaintWidget);

        QMetaObject::connectSlotsByName(PaintWidget);
    } // setupUi

    void retranslateUi(QWidget *PaintWidget)
    {
        PaintWidget->setWindowTitle(QCoreApplication::translate("PaintWidget", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class PaintWidget: public Ui_PaintWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PAINTWIDGET_H
