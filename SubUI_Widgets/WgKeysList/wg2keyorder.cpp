#include "wg2keyorder.h"
#include "ui_wg2keyorder.h"

Wg2KeyOrder::Wg2KeyOrder(int &index, QWidget *parent) :
    WgKeysListItem(index, parent), ui(new Ui::Wg2KeyOrder),
    m_keyState_1(0), m_keyState_2(0),m_saveFlag(0)
{
    ui->setupUi(this);

    ui->pb_key1->installEventFilter(this);
    ui->pb_key2->installEventFilter(this);
}

Wg2KeyOrder::~Wg2KeyOrder()
{
    delete ui;
}

void Wg2KeyOrder::showKeysInfo()
{
    ui->pb_key1->setText("");
    ui->pb_key2->setText("");

    if (!m_keysNum) return;

    int key1 = m_keysNum / 1000;
    int key2 = m_keysNum % 1000;

    //展示按键信息
    if (key1)
        ui->pb_key1->setText(KeysMap::keyInt2Str(std::move(key1)).c_str());

    if (key2)
        ui->pb_key2->setText(KeysMap::keyInt2Str(std::move(key2)).c_str());
}

void Wg2KeyOrder::setNormalState()
{
    initKeysState();
    ui->widget->setStyleSheet(WIDGET_COLOR_NORMAL);
}

void Wg2KeyOrder::setSelectedState()
{
    ui->widget->setStyleSheet(WIDGET_COLOR_SELECTED);
}

bool Wg2KeyOrder::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == ui->pb_key1 || watched == ui->pb_key2)
    {
        //如果是焦点进入,就设置部件列表的当前索引
        if (event->type()==QEvent::FocusIn)
        {
            emit signalSetCurrentIndex(m_index, m_keysNum);
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

void Wg2KeyOrder::on_pb_key1_clicked()
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

void Wg2KeyOrder::on_pb_key2_clicked()
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

void Wg2KeyOrder::keyPressEvent(QKeyEvent *event)
{
    if (1 == m_keyState_1)
    {
        ui->pb_key1->setText(QString(KeysMap::keyInt2Str(event->key()).c_str()));
        on_pb_key1_clicked();
        showSaveButton();

        return;
    }
    if (1 == m_keyState_2)
    {
        ui->pb_key2->setText(QString(KeysMap::keyInt2Str(event->key()).c_str()));
        on_pb_key2_clicked();
        showSaveButton();

        return;
    }
}

void Wg2KeyOrder::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        emit signalSetCurrentIndex(m_index, m_keysNum);
    }
}

void Wg2KeyOrder::initKeysState()
{
    if (m_keyState_1)
        on_pb_key1_clicked();

    if (m_keyState_2)
        on_pb_key2_clicked();
}

void Wg2KeyOrder::on_pb_saveOrDel_clicked()
{
    if (m_saveFlag)
    {
        auto[saved, oldKeysNum] = saveKeysInfo();
        if (saved)
            emit signalSaveKeys(m_index, oldKeysNum, m_keysNum);

        showDeleteButton();
    }
    else
    {
        emit signalDeleteKeys(m_index, m_keysNum);
    }
}

std::tuple<bool, int> Wg2KeyOrder::saveKeysInfo()
{
    //获取一下按键对应的虚拟键值
    int key1 = KeysMap::keyStr2Int(ui->pb_key1->text().toStdString());
    int key2 = KeysMap::keyStr2Int(ui->pb_key2->text().toStdString());

    if (key1 + key2 == 0)
        return {false, 0};

    if (key1 == key2)
        return {false, 0};

    //排序
    if (key1 > key2)
    {
        int key = key1;
        key1 = key2;
        key2 = key;
    }

    int oldKeysNum = m_keysNum;
    m_keysNum = key1 * 1000 + key2;

    showKeysInfo();

    return {true, oldKeysNum};
}

void Wg2KeyOrder::showSaveButton()
{
    if (!m_saveFlag)
    {
        ui->pb_saveOrDel->setText("√");
        m_saveFlag = 1;
    }
}

void Wg2KeyOrder::showDeleteButton()
{
    if (m_saveFlag)
    {
        ui->pb_saveOrDel->setText("×");
        m_saveFlag = 0;
    }
}

