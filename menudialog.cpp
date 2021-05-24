#include "menudialog.h"
#include "ui_menudialog.h"
#include "utils.h"


MenuDialog::MenuDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MenuDialog)
{
    ui->setupUi(this);

    setWindowFlags(
                Qt::Dialog
                | Qt::MSWindowsFixedSizeDialogHint
                | Qt::FramelessWindowHint
                | Qt::WindowDoesNotAcceptFocus
//                | Qt::WindowStaysOnTopHint
    );

    setAttribute(Qt::WA_NoSystemBackground, true);
    setAttribute(Qt::WA_TranslucentBackground, true);

    setBackgroundRole(QPalette::Background);
    setCursor(Qt::ArrowCursor);

    ui->colorPicker->setSettings(context);

    context.brushSize.addListener(this, [&](int const & value) {
        ui->brushLabel->setText(QString("Brush\n%1").arg(value));
        if (value != ui->brushSlider->value())
            ui->brushSlider->setValue(value);
    });

    context.eraserSize.addListener(this, [&](int const & value) {
        ui->eraserLabel->setText(QString("Eraser\n%1").arg(value));
        if (value != ui->eraserSlider->value())
            ui->eraserSlider->setValue(value);
    });

    context.numberOfPages.addListener(this, [&](int const & value) {
        auto txt = QString::asprintf("%d / %d", context.currentPage() + 1, value + 1);
        ui->pageCounterLabel->setText(txt);

        ui->firstButton->setEnabled(context.currentPage() != 0);
        ui->previousButton->setEnabled(context.currentPage() != 0);

        ui->nextButton->setEnabled(context.currentPage() != context.numberOfPages());
        ui->lastButton->setEnabled(context.currentPage() != context.numberOfPages());

        ui->erasePageButton->setEnabled(context.currentPage() != context.numberOfPages());
    });

    context.currentPage.addListener(this, [&](int const & value) {
        auto txt = QString::asprintf("%d / %d", value + 1, context.numberOfPages() + 1);
        ui->pageCounterLabel->setText(txt);

        ui->firstButton->setEnabled(context.currentPage() != 0);
        ui->previousButton->setEnabled(context.currentPage() != 0);

        ui->nextButton->setEnabled(context.currentPage() != context.numberOfPages());
        ui->lastButton->setEnabled(context.currentPage() != context.numberOfPages());

        ui->erasePageButton->setEnabled(context.currentPage() != context.numberOfPages());
    });

    context.notebookHasChanges.addListener(this, [&](bool const & value) {
        ui->saveButton->setEnabled(value);
    });

    context.menuPosition.addListener(this, [&](QPoint const & position) {
        move(position.x()-width()/2, position.y()-height()/2);
    });

    connect(ui->brushSlider, &QSlider::valueChanged, &context, &Context::setBrushSize);
    connect(ui->eraserSlider, &QSlider::valueChanged, &context, &Context::setEraserSize);

    connect(ui->firstButton, &QPushButton::clicked, &context, &Context::firstPage);
    connect(ui->nextButton, &QPushButton::clicked, &context, &Context::nextPage);
    connect(ui->erasePageButton, &QPushButton::clicked, &context, &Context::deletePage);
    connect(ui->previousButton, &QPushButton::clicked, &context, &Context::previousPage);
    connect(ui->lastButton, &QPushButton::clicked, &context, &Context::lastPage);

    connect(ui->newButton, &QPushButton::clicked, [&]() { context.newNotebookSafe(this); });
    connect(ui->openButton, &QPushButton::clicked, [&]() { context.openNotebookSafe(this); });
    connect(ui->saveButton, &QPushButton::clicked, [&]() { context.saveNotebookSafe(this); });
//    connect(ui->exportButton, &QPushButton::clicked, [this]() { context.exportNotebookSafe(this); });
    connect(ui->backgroundButton, &QPushButton::clicked, &context, &Context::toggleBackgroundType);

}

MenuDialog::~MenuDialog()
{
    delete ui;
}

void MenuDialog::hideEvent(QHideEvent *event)
{
//    qInfo() << "Inside hide event" << context.isMenuVisible();

    if (context.isMenuVisible())
        context.setMenuVisible(false);

    QWidget::hideEvent(event);
}

//void MenuDialog::hide()
//{
//    qInfo() << "Inside hide event" << context.isMenuVisible();

//    if (context.isMenuVisible())
//        context.setMenuVisible(false);

//    QDialog::hide();
//}
