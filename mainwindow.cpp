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
                Qt::X11BypassWindowManagerHint);

    setAttribute(Qt::WA_NoSystemBackground, true);
    setAttribute(Qt::WA_TranslucentBackground, true);
    setAttribute(Qt::WA_TransparentForMouseEvents, true);
    setAttribute(Qt::WA_ShowWithoutActivating, true);

    setParent(nullptr);
    setCursor(Qt::CrossCursor);

//    this->setWindowState(Qt::WindowFullScreen);

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::show()
{
    QPoint cursor = QCursor::pos();

    QList<QScreen*> screens = QGuiApplication::screens();

    if (screens.isEmpty())
        return;

    for (int i=0;i!=screens.size();++i)
    {
        auto const & geometry = screens[i]->geometry();

        if (!geometry.contains(cursor))
            continue;

        QMainWindow::show();
        this->resize(geometry.width(), geometry.height());
        this->move(geometry.topLeft());
        return;
    }

    QMainWindow::show();
    auto const & geometry = screens.front()->geometry();
    this->resize(geometry.width(), geometry.height());
    this->move(geometry.topLeft());
}

void MainWindow::showTransparent()
{
    this->ui->board->setBackground(0);
    this->show();
}

void MainWindow::showOpaque()
{
    this->ui->board->setBackground(1);
    this->show();
}

void MainWindow::clearCanvas()
{
    this->ui->board->clear();
}

void MainWindow::undoCanvas()
{
    this->ui->board->undo();
}

void MainWindow::redoCanvas()
{
    this->ui->board->redo();
}
