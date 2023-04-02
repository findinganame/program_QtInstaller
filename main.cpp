#include "qtinstaller.h"
#include <QApplication>
#include<QDebug>
#include <QFile>






int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QtInstaller w;
    w.show();
    return a.exec();
}
