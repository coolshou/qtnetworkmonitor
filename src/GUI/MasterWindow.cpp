#include "MasterWindow.h"


MasterWindow::MasterWindow()
{
    setWindowTitle(tr("Qt Network Monitor v%1").arg(qApp->applicationVersion()));
    setWindowIcon(QIcon(":/GFX/systray/TrayIconNormal.png"));
    setting= new QSettings(this);
    loadSetting();
    //resize(520, 340);

    widgetMainWindow = new MainWindow();
    setCentralWidget(widgetMainWindow);

    buildMenu();
    buildTray();
}

void MasterWindow::buildTray()
{
    //Tray
    quitAction = new QAction(tr("&Quit"), this);
    connect(quitAction, SIGNAL(triggered()), this, SLOT(appClose()));

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon((QIcon(":/GFX/systray/TrayIconNormal.png")));

    trayIconMenu = new QMenu(this);
    trayIconMenu->setIcon((QIcon(":/GFX/systray/TrayIconNormal.png")));
    trayIconMenu->addAction(quitAction);

    trayIcon->setContextMenu(trayIconMenu);
    trayIcon->setVisible(true);

    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason) ),
             this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)) );
}

void MasterWindow::buildMenu()
{
    //Action
    ShowHideConsoleAct = new QAction(tr("&Show/Hide"), this);
    ShowHideConsoleAct->setStatusTip(tr("Show/Hide Console"));
    connect(ShowHideConsoleAct, SIGNAL(triggered()), widgetMainWindow, SLOT(toggleConsoleView()));
    ClearConsoleAct = new QAction(tr("&Clear"), this);
    ClearConsoleAct->setStatusTip(tr("Clear Console"));
    connect(ClearConsoleAct, SIGNAL(triggered()), widgetMainWindow, SLOT(clearConsoleView()));

    OptionAct = new QAction(tr("&Config"), this);
    OptionAct->setStatusTip(tr("Option config"));
    //TODO: optin act connect

    AboutAct = new QAction(tr("&About"), this);
    AboutAct->setStatusTip(tr("About"));
    connect(AboutAct, SIGNAL(triggered()), widgetMainWindow, SLOT(showAbout()));

    //menu
    showMenu = menuBar()->addMenu(tr("&Console"));
    showMenu->addAction(ShowHideConsoleAct);
    showMenu->addAction(ClearConsoleAct);

    optionMenu = menuBar()->addMenu(tr("&Option"));
    optionMenu->addAction(OptionAct);

    AboutMenu = menuBar()->addMenu(tr("&Help"));
    AboutMenu->addAction(AboutAct);
}


void MasterWindow::closeEvent(QCloseEvent *event)
{
    trayIcon->setVisible(true);

    if (trayIcon->isVisible())
    {
        hide();
        event->ignore();
    }
}

void MasterWindow::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason)
    {
    case QSystemTrayIcon::Trigger:
    case QSystemTrayIcon::DoubleClick:
        //trayIcon->setVisible(false);
        show();

    default:;
    }
}
void MasterWindow::appClose()
{
    this->saveSetting();
    trayIcon->setVisible(false);
    qApp->exit(0);
    //qApp->quit();
}

void  MasterWindow::loadSetting()
{
    //TODO: loadSetting
    widgetMainWindow->loadSetting();
    setting->beginGroup("MainWindow");
    this->setGeometry(setting->value("geometry", QRect(0,0, 520, 500)).toRect());
    setting->endGroup();
}
void  MasterWindow::saveSetting()
{
    widgetMainWindow->saveSetting();
    //TODO: saveSetting
    setting->beginGroup("MainWindow");
    setting->setValue("geometry", this->window()->geometry());
    setting->endGroup();
}
