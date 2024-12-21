#include <QApplication>
#include "mc.h"
#include "globalhook.hpp"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
    GlobalHook hook;
    
    MC w;
    w.showFullScreen();
    
    return a.exec();
}