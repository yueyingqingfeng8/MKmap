#include "wg3keyorder.h"
#include "ui_wg3keyorder.h"

Wg3KeyOrder::Wg3KeyOrder(int &index, QWidget *parent) :
    WgKeysListItem(index, parent), ui(new Ui::Wg3KeyOrder),
    m_keyState_1(0), m_keyState_2(0), m_keyState_3(0),m_saveFlag(0)
{
    ui->setupUi(this);

    ui->pb_key1->installEventFilter(this);
    ui->pb_key2->installEventFilter(this);
    ui->pb_key3->installEventFilter(this);
}

Wg3KeyOrder::~Wg3KeyOrder()
{
    delete ui;
}

void Wg3KeyOrder::showKeysInfo()
{
    ui->pb_key1->setText("");
    ui->pb_key2->setText("");
    ui->pb_key3->setText("");

    if (!m_keysNum) return;

    int key1 = m_keysNum / 1000000;
    int key2 = m_keysNum % 1000000 / 1000;
    int key3 = m_keysNum / 1000;

    //展示按键信息
    if (!key1)
        ui->pb_key1->setText(KeysMap::keyInt2Str(std::move(key1)).c_str());

    if (!key2)
        ui->pb_key2->setText(KeysMap::keyInt2Str(std::move(key2)).c_str());

    if (!key3)
        ui->pb_key2->setText(KeysMap::keyInt2Str(std::move(key3)).c_str());
}

void Wg3KeyOrder::setNormalState()
{
    initKeysState();
    ui->widget->setStyleSheet(WIDGET_COLOR_NORMAL);
}

void Wg3KeyOrder::setSelectedState()
{
    ui->widget->setStyleSheet(WIDGET_COLOR_SELECTED);
}

bool Wg3KeyOrder::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == ui->pb_key1 || watched == ui->pb_key2 || watched == ui->pb_key3)
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

void Wg3KeyOrder::on_pb_key1_clicked()
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

void Wg3KeyOrder::on_pb_key2_clicked()
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

void Wg3KeyOrder::on_pb_key3_clicked()
{
    if (m_keyState_3)
    {
        ui->pb_key1->setStyleSheet(KEYS_COLOR_NORMAL);
        m_keyState_3 = 0;
    }
    else
    {
        ui->pb_key1->setStyleSheet(KEYS_COLOR_SELECTED);
        m_keyState_3 = 1;
    }
}

void Wg3KeyOrder::keyPressEvent(QKeyEvent *event)
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
    if (1 == m_keyState_3)
    {
        ui->pb_key3->setText(QString(KeysMap::keyInt2Str(event->key()).c_str()));
        on_pb_key3_clicked();
        showSaveButton();

        return;
    }
}

void Wg3KeyOrder::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        emit signalSetCurrentIndex(m_index, m_keysNum);
    }
}

void Wg3KeyOrder::initKeysState()
{
    if (m_keyState_1)
        on_pb_key1_clicked();

    if (m_keyState_2)
        on_pb_key2_clicked();

    if (m_keyState_3)
        on_pb_key3_clicked();
}

void Wg3KeyOrder::on_pb_saveOrDel_clicked()
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

std::tuple<bool, int> Wg3KeyOrder::saveKeysInfo()
{
    int array[3] = {0, 0, 0};

    //获取一下按键对应的虚拟键值
    array[0] = KeysMap::keyStr2Int(ui->pb_key1->text().toStdString());
    array[1] = KeysMap::keyStr2Int(ui->pb_key2->text().toStdString());
    array[2] = KeysMap::keyStr2Int(ui->pb_key3->text().toStdString());

    if (array[0] + array[1] + array[2] == 0)
        return {false, 0};

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

    //组装成个整数
    int oldKeysNum = m_keysNum;
    m_keysNum = 0;
    for (auto& keyNum : list)
        m_keysNum = m_keysNum * 1000 + keyNum;

    showKeysInfo();

    return {true, oldKeysNum};
}

void Wg3KeyOrder::showSaveButton()
{
    if (!m_saveFlag)
    {
        ui->pb_saveOrDel->setText("√");
        m_saveFlag = 1;
    }
}

void Wg3KeyOrder::showDeleteButton()
{
    if (m_saveFlag)
    {
        ui->pb_saveOrDel->setText("×");
        m_saveFlag = 0;
    }
}

