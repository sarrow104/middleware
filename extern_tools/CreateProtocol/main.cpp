#include "mainwindow.h"
#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    //FindDialog fd;
    //fd.show();
    return a.exec();
}

 /* vim: set expandtab ts=2 sw=2 sts=2 tw=100: */
