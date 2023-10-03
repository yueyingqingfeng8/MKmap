#include "wgstring.h"
#include "ui_wgstring.h"

WgString::WgString(int index, QWidget *parent) :
    WgListItem(index, parent), ui(new Ui::WgString),
    m_changedFlag(0)
{
    ui->setupUi(this);

    ui->te_str->installEventFilter(this);
}

WgString::~WgString()
{
    delete ui;
}

void WgString::setInfo(orderMap::OrderInfo &orderInfo)
{
    m_orderInfo = orderInfo;

    showOrderInfo();
}

void WgString::showOrderInfo()
{
    if (m_orderInfo.type != orderMap::STRING) return;

    if (std::holds_alternative<std::string>(m_orderInfo.data))
    {
        std::string str = std::get<std::string>(m_orderInfo.data);

        ui->te_str->setText(QString(str.c_str()));
    }
}

QPair<int, orderMap::orderInfo&> WgString::getOrderInfo()
{
    //内容被修改过并且数据有效则返回正常信息
    if (m_changedFlag)
    {
        if (saveOrderInfo())
            return {m_index, m_orderInfo};
    }

    m_changedFlag = 0;
    return {-1, m_orderInfo};
}

bool WgString::saveOrderInfo()
{
    QString str = ui->te_str->toPlainText();

    if (str == "")
        return false;

    m_orderInfo.type = orderMap::STRING;
    m_orderInfo.delayTime = 0;
    m_orderInfo.data = str.toStdString();

    showOrderInfo();

    return true;
}

void WgString::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        emit signalSetCurrentIndex(m_index);
    }
}

bool WgString::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == ui->te_str)
    {
        if (event->type()==QEvent::FocusIn)
        {
            emit signalSetCurrentIndex(m_index);
        }
    }

    return QWidget::eventFilter(watched,event);
}

void WgString::setNormalState()
{
    ui->widget->setStyleSheet(WIDGET_COLOR_NORMAL);
}

void WgString::setSelectedState()
{
    ui->widget->setStyleSheet(WIDGET_COLOR_SELECTED);
}

void WgString::on_te_str_textChanged()
{
    m_changedFlag = 1;
}

