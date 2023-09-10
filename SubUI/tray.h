#ifndef TRAY_H
#define TRAY_H

#include <QObject>
#include <QSystemTrayIcon>
#include <QCloseEvent>
#include <QAction>
#include <QMenu>
#include <QWidget>

class Tray : public QWidget
{
    Q_OBJECT
public:
    explicit Tray(QWidget *parent = nullptr);
    ~Tray();

    enum TrayCommands
    {
        RestoreMainUI,
        QuitProcess,
        TrayIconActivated
    };

private:
    QSystemTrayIcon *createQSystemTrayIcon();

    template<typename Func>
    void creatAction(const QString& name, Func slotFunc);

    void restoreMainUI();
    void quitProcess();
    void trayIconActivated(QSystemTrayIcon::ActivationReason reason);

signals:
    void trayCommandSignal(Tray::TrayCommands num);

private:
    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;
};

#endif // TRAY_H
