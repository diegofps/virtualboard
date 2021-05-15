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
                Qt::WindowStaysOnTopHint);

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
    this->ui->board->set_transparent(transparent);

    QPoint cursor = QCursor::pos();

    QList<QScreen*> screens = QGuiApplication::screens();

    if (screens.isEmpty())
        return;

    for (int i=0;i!=screens.size();++i)
    {
        auto const & geometry = screens[i]->geometry();

        if (!geometry.contains(cursor))
            continue;

        this->resize(geometry.width(), geometry.height());
        this->move(geometry.topLeft());
        QMainWindow::show();
        return;
    }

    auto const & geometry = screens.front()->geometry();
    this->resize(geometry.width(), geometry.height());
    this->move(geometry.topLeft());
    QMainWindow::show();
}
