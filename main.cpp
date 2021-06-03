#include "mainwindow.h"
#include "context.h"

#include <QApplication>
#include <iostream>
#include <fstream>
#include <thread>
#include <string>

#include <sys/stat.h>
#include <unistd.h>

const char * named_pipe = "/tmp/virtualboard";

void
readInput(MainWindow * w)
{
    mkfifo(named_pipe, 0666);

    std::string cmd;

    while (true)
    {
        std::ifstream ifs(named_pipe);
        ifs >> cmd;

        if (cmd == "showOpaque")
            QMetaObject::invokeMethod(w, "showWindowWithBackgroundType", Qt::AutoConnection, Q_ARG(int, 1));

        else if (cmd == "showTransparent")
            QMetaObject::invokeMethod(w, "showWindowWithBackgroundType", Qt::AutoConnection, Q_ARG(int, 0));

        else if (cmd == "undo")
            QMetaObject::invokeMethod(&context, "undo", Qt::AutoConnection);

        else if (cmd == "redo")
            QMetaObject::invokeMethod(&context, "redo", Qt::AutoConnection);

        else if (cmd == "clearPage")
            QMetaObject::invokeMethod(&context, "clearPage", Qt::AutoConnection);

        else if (cmd == "nextPage")
            QMetaObject::invokeMethod(&context, "nextPage", Qt::AutoConnection);

        else if (cmd == "previousPage")
            QMetaObject::invokeMethod(&context, "previousPage", Qt::AutoConnection);

        else if (cmd == "firstPage")
            QMetaObject::invokeMethod(&context, "firstPage", Qt::AutoConnection);

        else if (cmd == "lastPage")
            QMetaObject::invokeMethod(&context, "lastPage", Qt::AutoConnection);

        else if (cmd == "deletePage")
            QMetaObject::invokeMethod(&context, "deletePage", Qt::AutoConnection);

        else if (cmd == "newNotebook")
            QMetaObject::invokeMethod(&context, "newNotebookSafe", Qt::AutoConnection, Q_ARG(QWidget*, w));

        else if (cmd == "openNotebook")
            QMetaObject::invokeMethod(&context, "openNotebookSafe", Qt::AutoConnection, Q_ARG(QWidget*, w));

        else if (cmd == "saveNotebook")
            QMetaObject::invokeMethod(&context, "saveNotebookSafe", Qt::AutoConnection, Q_ARG(QWidget*, w));

        else if (cmd == "saveNotebookAs")
            QMetaObject::invokeMethod(&context, "saveNotebookAsSafe", Qt::AutoConnection, Q_ARG(QWidget*, w));

        else if (cmd == "exportNotebook")
            QMetaObject::invokeMethod(&context, "exportNotebook", Qt::AutoConnection, Q_ARG(QWidget*, w));

        else if (cmd == "hide")
        {
            QMetaObject::invokeMethod(&context, "setWindowVisible", Qt::AutoConnection, Q_ARG(bool, false));
            QMetaObject::invokeMethod(&context, "setMenuVisible", Qt::AutoConnection, Q_ARG(bool, false));
        }

        else
            std::cout << "Unknown command:" << cmd << std::endl;
    }

    unlink(named_pipe);
}

int main(int argc, char *argv[])
{
    qputenv("QT_XCB_TABLET_LEGACY_COORDINATES", "");

    QApplication a(argc, argv);
    context.init();

    MainWindow w;

//    w.show();

    std::thread t(readInput, &w);

    return a.exec();
}
