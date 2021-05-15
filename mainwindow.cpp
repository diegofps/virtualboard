#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QScreen>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Configure borders, size and so on
    setWindowFlags(
                Qt::Dialog |
                Qt::MSWindowsFixedSizeDialogHint |
                Qt::FramelessWindowHint |
                Qt::WindowStaysOnTopHint |
                Qt::X11BypassWindowManagerHint);

    setAttribute(Qt::WA_NoSystemBackground, true);
    setAttribute(Qt::WA_TranslucentBackground, true);
    setAttribute(Qt::WA_TransparentForMouseEvents, true);
    setAttribute(Qt::WA_ShowWithoutActivating, true);

    setParent(nullptr);

//    this->setWindowState(Qt::WindowFullScreen);

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::show(bool transparent)
{
    this->ui->board->setTransparent(transparent);

    QPoint cursor = QCursor::pos();

    QList<QScreen*> screens = QGuiApplication::screens();

    if (screens.isEmpty())
        return;

    qInfo("mouse: %d %d", cursor.x(), cursor.y());
    for (int i=0;i!=screens.size();++i)
    {
        auto const & geometry = screens[i]->geometry();
        qInfo("screen %d at %d, %d with %d, %d", i, geometry.topLeft().x(), geometry.topLeft().y(), geometry.width(), geometry.height());
    }

    for (int i=0;i!=screens.size();++i)
    {
        auto const & geometry = screens[i]->geometry();

        if (!geometry.contains(cursor))
        {
            qInfo("Mouse not inside screen %d", i);
            continue;
        }
        else
            qInfo("Inside screen %d", i);

        QMainWindow::show();
        this->resize(geometry.width(), geometry.height());
        this->move(geometry.topLeft());
        qInfo("Moving to %d, %d", geometry.topLeft().x(), geometry.topLeft().y());
        return;
    }

    QMainWindow::show();
    auto const & geometry = screens.front()->geometry();
    this->resize(geometry.width(), geometry.height());
    this->move(geometry.topLeft());
}
