#include <iostream>

#include <QApplication>
#include <QPushButton>

#include "WinPcapFns.h"
#include "GUI/MainWindow.h"
#include "GUI/MasterWindow.h"

using namespace std;

int main(int argc, char *argv[])
{

    cout<<"App. Begin"<<endl;

    QApplication app(argc, argv);
    QApplication::setApplicationName("qtnetworkmonitor");
    QApplication::setOrganizationName("coolshou.idv");
    QApplication::setApplicationVersion(APP_VERSION);

    MasterWindow MasterWin;
    MasterWin.show();

    //MainWindow MainWin;
    //MainWin.show();

    int return_v = app.exec();

    cout<<"App. End"<<endl;

    return return_v;

}
