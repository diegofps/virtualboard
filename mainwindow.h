#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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

public slots:

    void clearCanvas();

    void undoCanvas();

    void redoCanvas();

    void showTransparent();

    void showOpaque();

private:

    Ui::MainWindow *ui;

};
#endif // MAINWINDOW_H
