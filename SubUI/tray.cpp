#include "tray.h"
#include <functional>

Tray::Tray(QWidget *parent)
    : QWidget{parent}
{
    trayIconMenu = new QMenu(this);
    trayIcon = createQSystemTrayIcon();

    creatAction("Restore", &Tray::restoreMainUI);
    creatAction("Quit", &Tray::quitProcess);

    trayIcon->show();
}

Tray::~Tray()
{
    delete trayIconMenu;
}

QSystemTrayIcon *Tray::createQSystemTrayIcon()
{
    QSystemTrayIcon *trayIcon = new QSystemTrayIcon(this);

    trayIcon->setIcon(QIcon("../mk.png"));
    trayIcon->setContextMenu(trayIconMenu);

    connect(trayIcon, &QSystemTrayIcon::activated, this, &Tray::trayIconActivated);

    return trayIcon;
}

template<typename Func>
void Tray::creatAction(const QString& name, Func slotFunc)
{
    QAction* action = new QAction(name);

    connect(action, &QAction::triggered, this, slotFunc);

    trayIconMenu->addAction(action);
}

void Tray::restoreMainUI()
{
    emit trayCommandSignal(TrayCommands::RestoreMainUI);
}

void Tray::quitProcess()
{
    emit trayCommandSignal(TrayCommands::QuitProcess);
}

void Tray::trayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    if (reason == QSystemTrayIcon::Trigger)
    {
        emit trayCommandSignal(TrayCommands::TrayIconActivated);
    }
}


