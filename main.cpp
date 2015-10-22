#include "mainwindow.h"
#include <QApplication>
int mandel(int,char**);

int main(int argc, char *argv[])
{
    if(argc == 1){
        QApplication a(argc, argv);
        MainWindow w;
        w.setFixedSize(800,600);
        w.show();

        return a.exec();
    }
    return mandel(argc,argv);
}
