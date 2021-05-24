#include "menu.h"
#include "ui_menu.h"

#include <QMessageBox>
#include <QFileDialog>

Menu::Menu(Context & c, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Menu),
    c(c)
{
    ui->setupUi(this);
    setBackgroundRole(QPalette::Background);
    setCursor(Qt::ArrowCursor);

    ui->colorPicker->setSettings(c);

    c.brushSize.addListener(this, [&](int const & value) {
        ui->brushLabel->setText(QString("Brush\n%1").arg(value));
        if (value != ui->brushSlider->value())
            ui->brushSlider->setValue(value);
    });

    c.eraserSize.addListener(this, [&](int const & value) {
        ui->eraserLabel->setText(QString("Eraser\n%1").arg(value));
        if (value != ui->eraserSlider->value())
            ui->eraserSlider->setValue(value);
    });

    c.numberOfPages.addListener(this, [&](int const & value) {
        auto txt = QString::asprintf("%d / %d", c.currentPage() + 1, value + 1);
        ui->pageCounterLabel->setText(txt);

        ui->firstButton->setEnabled(c.currentPage() != 0);
        ui->previousButton->setEnabled(c.currentPage() != 0);

        ui->nextButton->setEnabled(c.currentPage() != c.numberOfPages());
        ui->lastButton->setEnabled(c.currentPage() != c.numberOfPages());

        ui->erasePageButton->setEnabled(c.currentPage() != c.numberOfPages());
    });

    c.currentPage.addListener(this, [&](int const & value) {
        auto txt = QString::asprintf("%d / %d", value + 1, c.numberOfPages() + 1);
        ui->pageCounterLabel->setText(txt);

        ui->firstButton->setEnabled(c.currentPage() != 0);
        ui->previousButton->setEnabled(c.currentPage() != 0);

        ui->nextButton->setEnabled(c.currentPage() != c.numberOfPages());
        ui->lastButton->setEnabled(c.currentPage() != c.numberOfPages());

        ui->erasePageButton->setEnabled(c.currentPage() != c.numberOfPages());
    });

    c.notebookHasChanges.addListener(this, [&](bool const & value) {
        ui->saveButton->setEnabled(value);
    });

    connect(ui->brushSlider, &QSlider::valueChanged, &c, &Context::setBrushSize);
    connect(ui->eraserSlider, &QSlider::valueChanged, &c, &Context::setEraserSize);

    connect(ui->firstButton, &QPushButton::clicked, &c, &Context::firstPage);
    connect(ui->nextButton, &QPushButton::clicked, &c, &Context::nextPage);
    connect(ui->erasePageButton, &QPushButton::clicked, &c, &Context::erasePage);
    connect(ui->previousButton, &QPushButton::clicked, &c, &Context::previousPage);
    connect(ui->lastButton, &QPushButton::clicked, &c, &Context::lastPage);

    connect(ui->newButton, &QPushButton::clicked, this, &Menu::newNotebook);
    connect(ui->openButton, &QPushButton::clicked, this, &Menu::openNotebook);
    connect(ui->saveButton, &QPushButton::clicked, this, &Menu::saveNotebook);
    connect(ui->backgroundButton, &QPushButton::clicked, &c, &Context::toggleBackground);
}

Menu::~Menu()
{
    delete ui;
}

void Menu::newNotebook()
{
    int ret = QMessageBox::No;

    if (c.notebookHasChanges())
    {
        QMessageBox msgBox;
        msgBox.setText("You have unsaved changes. Do you want to save them?");
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Yes);
        ret = msgBox.exec();
    }

    if (ret == QMessageBox::Cancel)
        return;

    if (ret == QMessageBox::Yes)
    {
        if (c.openFolder() == ".")
        {
            QString dir = QFileDialog::getSaveFileName(
                        this,
                        tr("Save notebook as"),
                        QDir::homePath());

            if (dir.isEmpty())
                return;

            c.saveNotebookAs(dir);
        }
        else
        {
            c.saveNotebook();
        }
    }

    c.newNotebook();
}

void Menu::openNotebook()
{
    int ret = QMessageBox::No;

    if (c.notebookHasChanges())
    {
        QMessageBox msgBox;
        msgBox.setText("You have unsaved changes. Do you want to save your current work?");
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Yes);
        ret = msgBox.exec();
    }

    if (ret == QMessageBox::Cancel)
        return;

    if (ret == QMessageBox::Yes)
    {
        if (c.openFolder() == ".")
        {
            QString dir = QFileDialog::getSaveFileName(
                        this,
                        tr("Save notebook as"),
                        QDir::homePath());

            if (dir.isEmpty())
                return;

            c.saveNotebookAs(dir);
        }
        else
        {
            c.saveNotebook();
        }
    }

    QString dir = QFileDialog::getExistingDirectory(
                this,
                tr("Open notebook"),
                QDir::homePath(),
                QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if (dir.isEmpty())
        return;

    c.openNotebook(dir);
}

void Menu::saveNotebook()
{
    if (c.openFolder() == ".")
    {
        QString dir = QFileDialog::getSaveFileName(
                    this,
                    tr("Save notebook as"),
                    QDir::homePath());

        if (dir.isEmpty())
            return;

        c.saveNotebookAs(dir);
    }
    else
    {
        c.saveNotebook();
    }
}
