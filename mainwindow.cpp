#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QScreen>
#include <QWindow>
#include <QDebug>

#include <thread>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , mMenuDialog(new MenuDialog(this))
    , screen(nullptr)

    , newNotebookShortcut(QKeySequence(tr("Ctrl+N", "New notebook")), this)
    , openNotebookShortcut(QKeySequence(tr("Ctrl+O", "Open notebook")), this)
    , saveNotebookShortcut(QKeySequence(tr("Ctrl+S", "Save notebook")), this)
    , saveNotebookAsShortcut(QKeySequence(tr("Ctrl+Shift+S", "Save notebook as")), this)
    , exportNotebookAsShortcut(QKeySequence(tr("Ctrl+E", "Export notebook as PDF")), this)

    , hideMainWindowShortcut(QKeySequence(tr("Escape", "Hide notebook window")), this)
    , toggleMenuVisibilityShortcut(QKeySequence(tr("M", "Toggle menu visibility")), this)

    , clearPageShortcut(QKeySequence(tr("C", "Clear current page")), this)
    , deletePageShortcut(QKeySequence(tr("Delete", "Delete current page")), this)
    , nextPageShortcut(QKeySequence(tr("Right", "Go to next page")), this)
    , previousPageShortcut(QKeySequence(tr("Left", "Go to previous page")), this)
    , firstPageShortcut(QKeySequence(tr("Home", "Go to first page")), this)
    , lastPageShortcut(QKeySequence(tr("End", "Go to last page")), this)
{
    ui->setupUi(this);

    setWindowFlags(
                Qt::Dialog
                | Qt::FramelessWindowHint
    );

    setAttribute(Qt::WA_NoSystemBackground, true);
    setAttribute(Qt::WA_TranslucentBackground, true);
    setCursor(Qt::CrossCursor);

    context.isMenuVisible.addListener(this, [&](bool const & newValue) {
        if (newValue)
            mMenuDialog->show();
        else
            mMenuDialog->hide();
    });

//    context.isWindowVisible.addListener(this, [&](bool const & newValue) {
//        if (newValue)
//            show();
//        else
//            hide();
//    });

    connect(&newNotebookShortcut, &QShortcut::activated, [&]() { context.newNotebookSafe(this); });
    connect(&openNotebookShortcut, &QShortcut::activated, [&]() { context.openNotebookSafe(this); });
    connect(&saveNotebookShortcut, &QShortcut::activated, [&]() { context.saveNotebookSafe(this); });
//    connect(&saveNotebookAsShortcut, &QShortcut::activated, [&]() { context.saveNotebookAsSafe(this); });
//    connect(&exportNotebookAsShortcut, &QShortcut::activated, [&]() { context.exportNotebookAsSafe(this); });

    connect(&toggleMenuVisibilityShortcut, &QShortcut::activated, &context, &Context::toggleMenuVisibility);
    connect(&hideMainWindowShortcut, &QShortcut::activated, [&]() {
        context.setMenuVisible(false);
        hide();
    });

    connect(&clearPageShortcut, &QShortcut::activated, &context, &Context::clearPage);
    connect(&deletePageShortcut, &QShortcut::activated, &context, &Context::deletePage);

    connect(&firstPageShortcut, &QShortcut::activated, &context, &Context::firstPage);
    connect(&previousPageShortcut, &QShortcut::activated, &context, &Context::previousPage);
    connect(&nextPageShortcut, &QShortcut::activated, &context, &Context::nextPage);
    connect(&lastPageShortcut, &QShortcut::activated, &context, &Context::lastPage);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete mBoard;
    delete mMenuDialog;
}

void MainWindow::moveEvent(QMoveEvent *)
{
//    show();
}


QScreen * getCursorScreen()
{
    QList<QScreen*> screens = QGuiApplication::screens();

    if (screens.isEmpty())
        return nullptr;

    QPoint cursor = QCursor::pos();

    for (int i=0;i!=screens.size();++i)
        if (screens[i]->geometry().contains(cursor))
            return screens[i];

    return screens[0];
}

void MainWindow::showWindowWithBackgroundType(int background)
{
    QScreen * newScreen = getCursorScreen();

    if (isVisible())
    {
        bool sameScreen = newScreen == screen;
        bool sameBackground = background == context.backgroundType();

        if (sameScreen)
        {
            if (sameBackground)
            {
                screen = nullptr;
                hide();
            }
            else
            {
                context.setBackgroundType(background);
            }
        }
        else
        {
            screen = newScreen;
            context.setCanvasSize(screen->size());

            hide();
            std::this_thread::sleep_for(std::chrono::milliseconds(50));

            if (!sameBackground)
                context.setBackgroundType(background);

            windowHandle()->setScreen(screen);
            resize(100,100);
            show();

            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            showMaximized();
        }
    }

    else
    {
        screen = newScreen;
        context.setBackgroundType(background);
        context.setCanvasSize(screen->size());

        if (context.canvas()->width() == 0)
            context.newNotebook();

        showMaximized();
        windowHandle()->setScreen(screen);
    }
}

void MainWindow::show()
{
//    qInfo() << "Showing main window";

    QList<QScreen*> screens = QGuiApplication::screens();

    if (screens.isEmpty())
        return;

    QPoint cursor = QCursor::pos();
    int i;

    for (i=0;i!=screens.size();++i)
        if (screens[i]->geometry().contains(cursor))
            break;

    if (i == screens.size())
        i = 0;

    QScreen * screen = screens[i];
//    QRect geometry = screen->geometry();

    context.setCanvasSize(screen->size());

    if (context.canvas()->width() == 0)
        context.newNotebook();

//    QMainWindow::show();


//    showFullScreen();
//    this->move(geometry.topLeft());
    resize(100,100);
    showMaximized();

    windowHandle()->setScreen(screen);


//    std::this_thread::sleep_for(std::chrono::milliseconds(500));
//    setWindowState(Qt::WindowMaximized);


//    setWindowState(Qt::WindowFullScreen);

//    this->resize(geometry.width(), geometry.height());
//    this->move(QPoint(100,100));
//    this->move(geometry.topLeft());

//    QMainWindow::show();

}
