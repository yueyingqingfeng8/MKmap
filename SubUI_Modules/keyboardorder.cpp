#include "keyboardorder.h"
#include "ui_keyboardorder.h"

KeyboardOrder::KeyboardOrder(QWidget *parent) :
    QWidget(parent), ui(new Ui::KeyboardOrder),
    m_keyType(orderMap::KEYS_TYPE_2), m_keysNum(0)
{
    ui->setupUi(this);
}

KeyboardOrder::~KeyboardOrder()
{
    delete ui;
}

void KeyboardOrder::showKeys()
{
    readKeysSet();
    showKeysWidgetList();
}

void KeyboardOrder::readKeysSet()
{
    //初始化按键集合,默认2键
    m_keysSet.clear();
    m_keyType = orderMap::KEYS_TYPE_2;
    std::list<int> keysList = OrderMap::searchSetting(KEYS_SET_2);

    //取出按键列表信息后加入按键集合
    auto typeList = OrderMap::searchSetting(KEYS_TYPE);
    if (!typeList.empty())
    {
        m_keyType = typeList.front();
        if (orderMap::KEYS_TYPE_2 == m_keyType)
        {
        }
        else if (orderMap::KEYS_TYPE_3 == m_keyType)
        {
            keysList = OrderMap::searchSetting(KEYS_SET_3);
        }
    }

    for (auto& keysNum : keysList)
        m_keysSet.insert(keysNum);
}

void KeyboardOrder::showKeysWidgetList()
{
    clearListWidget();

    QList<int> keysList = m_keysSet.values();

    for (auto& keysNum : keysList)
    {
        if (keysNum > 0)
            addWidget(keysNum);
    }

    addEndWidget();
}

void KeyboardOrder::addWidget(int keysNum)
{
    int index = ui->lw_keysOrder->count();

    if (m_keyType == orderMap::KEYS_TYPE_2)
    {
        Wg2KeyOrder* item = new Wg2KeyOrder(index);
        item->setKeysNum(std::move(keysNum));
        connect( item, &Wg2KeyOrder::signalSetCurrentIndex, this, &KeyboardOrder::slotSetCurrentIndex );
        connect( item, &Wg2KeyOrder::signalDeleteKeys, this, &KeyboardOrder::slotDeleteKeys );
        connect( item, &Wg2KeyOrder::signalSaveKeys, this, &KeyboardOrder::slotSaveKeys );

        ADD_KEY_WIDGET(item);
    }

    if (m_keyType == orderMap::KEYS_TYPE_3)
    {
        Wg3KeyOrder* item = new Wg3KeyOrder(index);
        item->setKeysNum(std::move(keysNum));
        connect( item, &Wg3KeyOrder::signalSetCurrentIndex, this, &KeyboardOrder::slotSetCurrentIndex );
        connect( item, &Wg3KeyOrder::signalDeleteKeys, this, &KeyboardOrder::slotDeleteKeys );
        connect( item, &Wg3KeyOrder::signalSaveKeys, this, &KeyboardOrder::slotSaveKeys );

        ADD_KEY_WIDGET(item);
    }
}

void KeyboardOrder::insertWidget(int& index)
{
    for(int i = index; i < ui->lw_keysOrder->count(); i++)
    {
        QWidget *widget = ui->lw_keysOrder->itemWidget(ui->lw_keysOrder->item(i));
        WgKeysListItem *it = static_cast<WgKeysListItem *>(widget);
        it->setIndex(i + 1);
    }

    if (m_keyType == orderMap::KEYS_TYPE_2)
    {
        Wg2KeyOrder* item = new Wg2KeyOrder(index);
        item->setKeysNum(0);
        connect( item, &Wg2KeyOrder::signalSetCurrentIndex, this, &KeyboardOrder::slotSetCurrentIndex );
        connect( item, &Wg2KeyOrder::signalDeleteKeys, this, &KeyboardOrder::slotDeleteKeys );
        connect( item, &Wg2KeyOrder::signalSaveKeys, this, &KeyboardOrder::slotSaveKeys );

        INSERT_KEY_WIDGET(index, item);
    }

    if (m_keyType == orderMap::KEYS_TYPE_3)
    {
        Wg3KeyOrder* item = new Wg3KeyOrder(index);
        item->setKeysNum(0);
        connect( item, &Wg3KeyOrder::signalSetCurrentIndex, this, &KeyboardOrder::slotSetCurrentIndex );
        connect( item, &Wg3KeyOrder::signalDeleteKeys, this, &KeyboardOrder::slotDeleteKeys );
        connect( item, &Wg3KeyOrder::signalSaveKeys, this, &KeyboardOrder::slotSaveKeys );

        INSERT_KEY_WIDGET(index, item);
    }
}

void KeyboardOrder::addEndWidget()
{
    int index = ui->lw_keysOrder->count();

    WgAddKeys* item = new WgAddKeys(index);
    connect(item, &WgAddKeys::signalAddkeysWidget, this, &KeyboardOrder::slotAddKeysWidget);

    ADD_KEY_WIDGET(item);
}

void KeyboardOrder::clearListWidget()
{
    int count = ui->lw_keysOrder->count();

    for (int i = 0; i < count; i++)
    {
        delete ui->lw_keysOrder->itemWidget(ui->lw_keysOrder->item(i));
        delete ui->lw_keysOrder->takeItem(i);
    }

    ui->lw_keysOrder->clear();
}

void KeyboardOrder::slotSetCurrentIndex(int index, int keysNum)
{
    m_keysNum = keysNum;

    //设置高亮
    for (int i = 0; i < ui->lw_keysOrder->count(); ++i)
    {
        QWidget *widget = ui->lw_keysOrder->itemWidget(ui->lw_keysOrder->item(i));
        WgKeysListItem *it = static_cast<WgKeysListItem *>(widget);

        if (i == index)
            it->setSelectedState();
        else
            it->setNormalState();
    }

    //展示按键指令信息
    if (m_keysNum)
    {
        std::string order = KB(std::to_string(m_keysNum));
        emit signalShowOrderInfo(QString(order.c_str()));
    }
}

void KeyboardOrder::slotDeleteKeys(int index, int keysNum)
{
    for(int i = index + 1; i < ui->lw_keysOrder->count(); i++)
    {
        QWidget *widget = ui->lw_keysOrder->itemWidget(ui->lw_keysOrder->item(i));
        WgKeysListItem *it = static_cast<WgKeysListItem *>(widget);
        it->setIndex(i - 1);
    }

    //删除对应widget和listWidgetItem
    delete ui->lw_keysOrder->itemWidget(ui->lw_keysOrder->item(index));
    delete ui->lw_keysOrder->takeItem(index);

    m_keysNum = 0;
    m_keysSet.remove(keysNum);

    emit signalClearOrderInfo();
    saveKeysSet();
}

void KeyboardOrder::slotSaveKeys(int index, int oldKeysNum, int newKeysNum)
{
    //已存在则报错展示原先按键
    if (m_keysSet.contains(newKeysNum))
    {
        QWidget *widget = ui->lw_keysOrder->itemWidget(ui->lw_keysOrder->item(index));
        WgKeysListItem *it = static_cast<WgKeysListItem *>(widget);
        it->setKeysNum(std::move(oldKeysNum));

        emit signalLogInfo(mainUI::WARN, "该按键已存在!");
        return;
    }

    //替换旧按键
    m_keysNum = newKeysNum;
    m_keysSet.remove(oldKeysNum);
    m_keysSet.insert(newKeysNum);
    OrderMap::replaceOrder(KB(std::to_string(newKeysNum)), KB(std::to_string(oldKeysNum)));

    saveKeysSet();

    std::string order = KB(std::to_string(m_keysNum));
    emit signalShowOrderInfo(QString(order.c_str()));
}

void KeyboardOrder::slotAddKeysWidget(int index)
{
    insertWidget(index);
}

void KeyboardOrder::saveKeysSet()
{
    QList<int> list = m_keysSet.values();
    std::list<int> keysList;

    for (auto& keysNum : list)
        keysList.push_back(keysNum);

    if (m_keyType == orderMap::KEYS_TYPE_2)
        OrderMap::modifySetting(KEYS_SET_2, keysList);
    else if (m_keyType == orderMap::KEYS_TYPE_3)
        OrderMap::modifySetting(KEYS_SET_3, keysList);

    emit signalSavedKeysSet();
}


