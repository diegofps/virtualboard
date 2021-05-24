#include "mainwindow.h"
#include "context.h"

#include <QApplication>
#include <iostream>
#include <thread>

void
readInput()
{
    char cmd;

    while (true)
    {
        std::cin >> cmd;

        if (cmd == 'o')
        {
            context.setOpaqueBackground();
            context.toggleWindowVisibility();
        }

        else if (cmd == 't')
        {
            context.setTransparentBackground();
            context.toggleWindowVisibility();
        }

        else if (cmd == 'h')
        {
            context.setWindowVisible(false);
            context.setMenuVisible(false);
        }

        else if (cmd == 'c')
            context.clearPage();

        else if (cmd == 'u')
            context.undo();

        else if (cmd == 'r')
            context.redo();

        else if (cmd == 'n')
            context.nextPage();

        else if (cmd == 'p')
            context.previousPage();
    }
}

int main(int argc, char *argv[])
{
    qputenv("QT_XCB_TABLET_LEGACY_COORDINATES", "");

    QApplication a(argc, argv);
    context.init();

    MainWindow w;

//    w.show();

    std::thread t(readInput);

    return a.exec();
}
