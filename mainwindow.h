#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QShortcut>
#include <QStackedLayout>

#include "drawingboard.h"
#include "context.h"
#include "menudialog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

    void show();

    void moveEvent(QMoveEvent *event);

public slots:

    void showWindowWithBackgroundType(int background);

private:

    Ui::MainWindow *ui;

    DrawingBoard * mBoard;

    MenuDialog * mMenuDialog;

    QScreen * screen;

    QShortcut newNotebookShortcut;
    QShortcut openNotebookShortcut;
    QShortcut saveNotebookShortcut;
    QShortcut saveNotebookAsShortcut;
    QShortcut exportNotebookAsShortcut;

    QShortcut hideMainWindowShortcut;
    QShortcut toggleMenuVisibilityShortcut;

    QShortcut clearPageShortcut;
    QShortcut deletePageShortcut;
    QShortcut nextPageShortcut;
    QShortcut previousPageShortcut;
    QShortcut firstPageShortcut;
    QShortcut lastPageShortcut;

};

#endif // MAINWINDOW_H
