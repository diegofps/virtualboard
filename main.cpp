#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
//    qputenv("QT_ENABLE_HIGHDPI_SCALING", "0");
    qputenv("QT_XCB_TABLET_LEGACY_COORDINATES", "");
    //QApplication::setAttribute(Qt::AA_DisableHighDpiScaling, true);
    QApplication a(argc, argv);
    MainWindow w;

    w.show(true);

    return a.exec();
}
