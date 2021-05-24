#ifndef MENUDIALOG_H
#define MENUDIALOG_H

#include <QDialog>

namespace Ui {
class MenuDialog;
}

class MenuDialog : public QDialog
{
    Q_OBJECT

public:

    explicit MenuDialog(QWidget *parent = nullptr);

    ~MenuDialog();

protected:

    void hideEvent(QHideEvent *event);

private:
    Ui::MenuDialog *ui;
};

#endif // MENUDIALOG_H
