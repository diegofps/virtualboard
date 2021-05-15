#include "mainwindow.h"

#include <QApplication>
#include <iostream>
#include <thread>

void
move2(MainWindow * w)
{
    char cmd;

    while (true)
    {
        std::cin >> cmd;

        if (cmd == 'o')
        {
            QMetaObject::invokeMethod(w, "showOpaque", Qt::AutoConnection);
//            w->toggleMode(1);
        }

        else if (cmd == 't')
        {
            QMetaObject::invokeMethod(w, "showTransparent", Qt::AutoConnection);
//            w->toggleMode(0);
        }

        else if (cmd == 'h')
        {
            QMetaObject::invokeMethod(w, "hide", Qt::AutoConnection);
//            w->redoCanvas();
        }

        else if (cmd == 'c')
        {
            QMetaObject::invokeMethod(w, "clearCanvas", Qt::AutoConnection);
            //w->clearCanvas();
        }

        else if (cmd == 'u')
        {
            QMetaObject::invokeMethod(w, "undoCanvas", Qt::AutoConnection);
//            w->undoCanvas();
        }

        else if (cmd == 'r')
        {
            QMetaObject::invokeMethod(w, "redoCanvas", Qt::AutoConnection);
//            w->redoCanvas();
        }

    }
}

int main(int argc, char *argv[])
{
    qputenv("QT_XCB_TABLET_LEGACY_COORDINATES", "");

    QApplication a(argc, argv);
    MainWindow w;

//    w.show();

    std::thread t(move2, &w);

    return a.exec();
}
