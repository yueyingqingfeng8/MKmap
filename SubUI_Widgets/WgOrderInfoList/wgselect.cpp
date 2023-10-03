#include "wgselect.h"
#include "ui_wgselect.h"

WgSelect::WgSelect(int index, QWidget *parent) :
    WgListItem(index, parent),
    ui(new Ui::WgSelect)
{
    ui->setupUi(this);
}

WgSelect::~WgSelect()
{
    delete ui;
}

void WgSelect::on_pb_shortcutKey_clicked()
{
    emit signalAddWidgetType(m_index, orderMap::SHORTCUT_KEY);
}


void WgSelect::on_pb_string_clicked()
{
    emit signalAddWidgetType(m_index, orderMap::STRING);
}

void WgSelect::setNormalState()
{
    ui->widget->setStyleSheet(WIDGET_COLOR_NORMAL);
}

void WgSelect::setSelectedState()
{
    ui->widget->setStyleSheet(WIDGET_COLOR_SELECTED);
}

void WgSelect::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        emit signalSetCurrentIndex(m_index);
    }
}

QPair<int, orderMap::orderInfo&> WgSelect::getOrderInfo()
{
    return {-1, m_orderInfo};
}

