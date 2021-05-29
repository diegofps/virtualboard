#include "mainwindow.h"
#include "context.h"

#include <QApplication>
#include <iostream>
#include <thread>
#include <string>

void
readInput(MainWindow * w)
{
    std::string cmd;

    while (true)
    {
        std::cin >> cmd;

        if (cmd == "showOpaque")
            QMetaObject::invokeMethod(w, "showWindowWithBackgroundType", Qt::AutoConnection, Q_ARG(int, 1));
//            context.showWindowWithBackgroundType(w, 1);
//        {
//            context.setOpaqueBackground();
//            context.toggleWindowVisibility();
//        }

        else if (cmd == "showTransparent")
            QMetaObject::invokeMethod(w, "showWindowWithBackgroundType", Qt::AutoConnection, Q_ARG(int, 0));
//            context.showWindowWithBackgroundType(w, 0);

        else if (cmd == "hide")
        {
            QMetaObject::invokeMethod(&context, "setWindowVisible", Qt::AutoConnection, Q_ARG(bool, false));
            QMetaObject::invokeMethod(&context, "setMenuVisible", Qt::AutoConnection, Q_ARG(bool, false));

//            context.setWindowVisible(false);
//            context.setMenuVisible(false);
        }

        else if (cmd == "undo")
            QMetaObject::invokeMethod(&context, "undo", Qt::AutoConnection);
//            context.undo();

        else if (cmd == "redo")
            QMetaObject::invokeMethod(&context, "redo", Qt::AutoConnection);
//            context.redo();

        else if (cmd == "clearPage")
            QMetaObject::invokeMethod(&context, "clearPage", Qt::AutoConnection);
//            context.clearPage();

        else if (cmd == "nextPage")
            QMetaObject::invokeMethod(&context, "nextPage", Qt::AutoConnection);
//            context.nextPage();

        else if (cmd == "previousPage")
            QMetaObject::invokeMethod(&context, "previousPage", Qt::AutoConnection);
//            context.previousPage();

        else if (cmd == "firstPage")
            QMetaObject::invokeMethod(&context, "firstPage", Qt::AutoConnection);
//            context.firstPage();

        else if (cmd == "lastPage")
            QMetaObject::invokeMethod(&context, "lastPage", Qt::AutoConnection);
//            context.lastPage();

        else if (cmd == "deletePage")
            QMetaObject::invokeMethod(&context, "deletePage", Qt::AutoConnection);
//            context.deletePage();

        else if (cmd == "newNotebook")
            QMetaObject::invokeMethod(&context, "newNotebookSafe", Qt::AutoConnection, Q_ARG(QWidget*, w));
//           context.newNotebookSafe(w);

        else if (cmd == "openNotebook")
            QMetaObject::invokeMethod(&context, "openNotebookSafe", Qt::AutoConnection, Q_ARG(QWidget*, w));
//            context.openNotebookSafe(w);

        else if (cmd == "saveNotebook")
            QMetaObject::invokeMethod(&context, "saveNotebookSafe", Qt::AutoConnection, Q_ARG(QWidget*, w));
//            context.saveNotebookSafe(w);

        else if (cmd == "saveNotebookAs")
            QMetaObject::invokeMethod(&context, "saveNotebookAsSafe", Qt::AutoConnection, Q_ARG(QWidget*, w));
//            context.saveNotebookAsSafe(w);

        else if (cmd == "exportNotebook")
            QMetaObject::invokeMethod(&context, "exportNotebook", Qt::AutoConnection, Q_ARG(QWidget*, w));
//            context.exportNotebook(w);

        else
            std::cout << "Unknown command:" << cmd << std::endl;
    }
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
