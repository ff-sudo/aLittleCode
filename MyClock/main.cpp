#include "mainview.h"

#include <QApplication>
#include <QSystemTrayIcon>
#include <QMenu>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainView w;
    w.show();

    //! 系统托盘
    QSystemTrayIcon *trayIcon = new QSystemTrayIcon(QIcon("://icon.png"), &w);
    trayIcon->show();

    QMenu *trayMenu = new QMenu(&w);
    QAction *showAction = new QAction("Show", &w);
    QAction *quitAction = new QAction("Quit", &w);
    QAction *hideAction = new QAction("Hide", &w);
    trayMenu->addAction(showAction);
    trayMenu->addAction(quitAction);
    trayMenu->addAction(hideAction);
    trayIcon->setContextMenu(trayMenu);
    trayIcon->setToolTip("AHA");

    QObject::connect(showAction, &QAction::triggered, &w, &QWidget::show);
    QObject::connect(quitAction, &QAction::triggered, &a, &QApplication::quit);
    QObject::connect(hideAction, &QAction::triggered, &w, &QWidget::hide);

    a.setWindowIcon(QIcon("://icon.png"));
    return a.exec();
}
