#include "widget.h"
#include "ui_widget.h"
 
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    this->setWindowTitle("MKmap");

    m_tray = new Tray(this);
    connect(m_tray, &Tray::trayCommandSignal, this, &Widget::runTrayCommand);

    //鼠标指令和展示指令信息列表的连接
    connect(ui->wg_oneWayOrder, &OneWayOrder::signalShowOrderInfo, ui->wg_orderInfo, &TaskList::showOrderInfo);
    connect(ui->wg_oneWayOrder, &OneWayOrder::signalLogInfo, this, &Widget::showLogInfo);

    //键盘指令和展示指令信息列表的连接
    connect(ui->wg_keyboardOrder, &KeyboardOrder::signalShowOrderInfo, ui->wg_orderInfo, &TaskList::showOrderInfo);
    connect(ui->wg_keyboardOrder, &KeyboardOrder::signalClearOrderInfo, this, [this](){
        on_pb_clearOrderInfo_clicked();
    });
    connect(ui->wg_keyboardOrder, &KeyboardOrder::signalLogInfo, this, &Widget::showLogInfo);

    //指令任务列表
    connect(ui->wg_orderInfo, &TaskList::signalLogInfo, this, &Widget::showLogInfo);

    FileRWer::initFileRW(SETFILEPATH);
    initMkfun();

    ui->wg_keyboardOrder->showKeys();
    ui->wg_oneWayOrder->showSetInfo();
}

Widget::~Widget()
{
    deleteMkfun();

    delete ui;
}

void Widget::on_pb_install_clicked()
{
    if(m_mkfun->installHook())
        showLogInfo(mainUI::NORMAL, "安装钩子成功!");
    else
        showLogInfo(mainUI::ERR, "安装钩子失败!");
}

void Widget::on_pb_unload_clicked()
{
    if(m_mkfun->uninstallHook())
        showLogInfo(mainUI::NORMAL, "卸载钩子成功!");
    else
        showLogInfo(mainUI::ERR, "卸载钩子失败!");
}

void Widget::initMkfun()
{
    m_mkfun = new Mkfun(FileRWer::readSetFileMap());
    m_mkfun->installEffect(mkFun::MouseFunType::MOUSE_ONEWAY, mkFun::KeyboardFunType::KEYBOARD);

    //可以修改鼠标的触发距离和判断距离
    connect(ui->wg_oneWayOrder, &OneWayOrder::signalSavedDistanceSet, this, [this](){
        m_mkfun->readOneWayDistanceSet();
    });
    //可以修改键盘按键检测的集合
    connect(ui->wg_keyboardOrder, &KeyboardOrder::signalSavedKeysSet, this, [this](){
        m_mkfun->readKeyboardSet();
    });

    //增加qt按键键值到字符串的设置
    std::map<int, std::string> qtKeysMap;
    qtKeysMap.insert(std::make_pair(Qt::Key_Shift, "shift"));
    qtKeysMap.insert(std::make_pair(Qt::Key_Control, "ctrl"));
    qtKeysMap.insert(std::make_pair(Qt::Key_Alt, "alt"));
    KeysMap::AddKeys(qtKeysMap);
}

void Widget::deleteMkfun()
{
    delete m_mkfun;
}

void Widget::on_pb_test_clicked()
{
    if (PG_TEST == ui->sw_fun->currentIndex())
        ui->sw_fun->setCurrentIndex(PG_MAIN);
    else
        ui->sw_fun->setCurrentIndex(PG_TEST);
}

void Widget::runTrayCommand(Tray::TrayCommands num)
{
    switch(num)
    {
    case Tray::TrayCommands::RestoreMainUI:
        this->show();
        break;
    case Tray::TrayCommands::QuitProcess:
        qApp->quit();
        break;
    case Tray::TrayCommands::TrayIconActivated:
        this->show();
        break;
    default:
        break;
    }
}

void Widget::showLogInfo(mainUI::LogType type, QString str)
{
    switch (type)
    {
    case mainUI::NORMAL:
        ui->lb_logInfo->setStyleSheet("background-color: rgb(211, 255, 215);");
        break;
    case mainUI::WARN:
        ui->lb_logInfo->setStyleSheet("background-color: rgb(251, 208, 255);");
        break;
    case mainUI::ERR:
        ui->lb_logInfo->setStyleSheet("background-color: rgb(255, 171, 171);");
        break;
    default:
        break;
    }

    ui->lb_logInfo->setText(str);
}

void Widget::closeEvent(QCloseEvent *event)
{
//    event->ignore();
//    this->hide();
}

void Widget::on_pb_save_clicked()
{
    FileRWer::writeSetFileMap(OrderMap::getOrderMap());
}


void Widget::on_pb_addOrderInfo_clicked()
{
    ui->wg_orderInfo->addOrderInfo();
}


void Widget::on_pb_deleteOrderInfo_clicked()
{
    ui->wg_orderInfo->deleteOrderInfo();
}


void Widget::on_pb_saveOrderInfo_clicked()
{
    ui->wg_orderInfo->saveOrderInfo();
}


void Widget::on_pb_clearOrderInfo_clicked()
{
    ui->wg_orderInfo->clearOrderInfo();
}


