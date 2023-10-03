#include "wgshortcutkey.h"
#include "ui_wgshortcutkey.h"

WgShortcutKey::WgShortcutKey(int index, QWidget *parent) :
    WgListItem(index, parent), ui(new Ui::WgShortcutKey),
    m_keyState_1(0), m_keyState_2(0), m_keyState_3(0), m_changedFlag(0)
{
    ui->setupUi(this);

    ui->pb_key1->installEventFilter(this);
    ui->pb_key2->installEventFilter(this);
    ui->pb_key3->installEventFilter(this);
}

WgShortcutKey::~WgShortcutKey()
{
    delete ui;
}

void WgShortcutKey::setInfo(orderMap::OrderInfo& orderInfo)
{
    m_orderInfo = orderInfo;

    showShortcutKey();
}

void WgShortcutKey::showShortcutKey()
{
    ui->pb_key1->setText("");
    ui->pb_key2->setText("");
    ui->pb_key3->setText("");

    //取出按键信息
    if (m_orderInfo.type != orderMap::SHORTCUT_KEY) return;

    if (std::holds_alternative<std::list<int>>(m_orderInfo.data))
    {
        std::list<int> list = std::get<std::list<int>>(m_orderInfo.data);

        m_keyStrList.clear();

        for (auto& key : list)
        {
            std::string str = KeysMap::keyInt2Str(int(key));
            m_keyStrList.append(QString(str.c_str()));
        }
    }

    //展示按键信息
    if (!m_keyStrList.isEmpty())
    {
        ui->pb_key1->setText(m_keyStrList.takeFirst());
    }
    if (!m_keyStrList.isEmpty())
    {
        ui->pb_key2->setText(m_keyStrList.takeFirst());
    }
    if (!m_keyStrList.isEmpty())
    {
        ui->pb_key3->setText(m_keyStrList.takeFirst());
    }
}

void WgShortcutKey::keyPressEvent(QKeyEvent *event)
{
    if (1 == m_keyState_1)
    {
        ui->pb_key1->setText(QString(KeysMap::keyInt2Str(event->key()).c_str()));
        on_pb_key1_clicked();
        m_changedFlag = 1;

        return;
    }
    if (1 == m_keyState_2)
    {
        ui->pb_key2->setText(QString(KeysMap::keyInt2Str(event->key()).c_str()));
        on_pb_key2_clicked();
        m_changedFlag = 1;

        return;
    }
    if (1 == m_keyState_3)
    {
        ui->pb_key3->setText(QString(KeysMap::keyInt2Str(event->key()).c_str()));
        on_pb_key3_clicked();
        m_changedFlag = 1;

        return;
    }
}

void WgShortcutKey::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        emit signalSetCurrentIndex(m_index);
    }
}

bool WgShortcutKey::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == ui->pb_key1 || watched == ui->pb_key2 || watched == ui->pb_key3)
    {
        //焦点进入设置部件列表的当前索引
        if (event->type()==QEvent::FocusIn)
        {
            emit signalSetCurrentIndex(m_index);
        }

        //防止空格触发按键点击效果
        if (event->type() == QEvent::KeyPress)
        {
            QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
            if (keyEvent->key() == Qt::Key_Space)
            {
                keyPressEvent(keyEvent);
                return true;
            }
        }
    }

    return QWidget::eventFilter(watched,event);
}

void WgShortcutKey::setNormalState()
{
    initKeysState();
    ui->widget->setStyleSheet(WIDGET_COLOR_NORMAL);
}

void WgShortcutKey::setSelectedState()
{
    ui->widget->setStyleSheet(WIDGET_COLOR_SELECTED);
}

QPair<int, orderMap::orderInfo&> WgShortcutKey::getOrderInfo()
{
    if (m_changedFlag)
    {
        //被修改过并且保存内容有效,返回有效信息
        if (saveShortcutKey())
            return {m_index, m_orderInfo};
    }

    m_changedFlag = 0;
    return {-1, m_orderInfo};
}

bool WgShortcutKey::saveShortcutKey()
{
    int array[3] = {0, 0, 0};

    //获取一下按键对应的虚拟键值
    array[0] = KeysMap::keyStr2Int(ui->pb_key1->text().toStdString());
    array[1] = KeysMap::keyStr2Int(ui->pb_key2->text().toStdString());
    array[2] = KeysMap::keyStr2Int(ui->pb_key3->text().toStdString());

    if (array[0] + array[1] + array[2] == 0)
        return false;

    //排序
    std::sort(array, array + sizeof(array) / sizeof(array[0]));

    //将list数据加入order,数据不能重复
    std::list<int> list;
    if (array[0])
        list.push_back(array[0]);

    if (array[1] && array[1] != array[0])
        list.push_back(array[1]);

    if (array[2] && array[2] != array[1] && array[2] != array[0])
        list.push_back(array[2]);

    m_orderInfo.type = orderMap::SHORTCUT_KEY;
    m_orderInfo.delayTime = 0;
    m_orderInfo.data = list;

    showShortcutKey();

    return true;
}

void WgShortcutKey::on_pb_key1_clicked()
{
    if (m_keyState_1)
    {
        ui->pb_key1->setStyleSheet(KEYS_COLOR_NORMAL);
        m_keyState_1 = 0;
    }
    else
    {
        ui->pb_key1->setStyleSheet(KEYS_COLOR_SELECTED);
        m_keyState_1 = 1;
    }
}


void WgShortcutKey::on_pb_key2_clicked()
{
    if (m_keyState_2)
    {
        ui->pb_key2->setStyleSheet(KEYS_COLOR_NORMAL);
        m_keyState_2 = 0;
    }
    else
    {
        ui->pb_key2->setStyleSheet(KEYS_COLOR_SELECTED);
        m_keyState_2 = 1;
    }
}


void WgShortcutKey::on_pb_key3_clicked()
{
    if (m_keyState_3)
    {
        ui->pb_key3->setStyleSheet(KEYS_COLOR_NORMAL);
        m_keyState_3 = 0;
    }
    else
    {
        ui->pb_key3->setStyleSheet(KEYS_COLOR_SELECTED);
        m_keyState_3 = 1;
    }
}

void WgShortcutKey::initKeysState()
{
    if (m_keyState_1)
        on_pb_key1_clicked();

    if (m_keyState_2)
        on_pb_key2_clicked();

    if (m_keyState_3)
        on_pb_key3_clicked();
}

