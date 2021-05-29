#include "drawingboard.h"

#include <QGuiApplication>
#include <QPainter>
#include <QScreen>
#include <QBitmap>
#include <QColor>
#include <QDebug>
#include <cmath>

#include "context.h"

#define distance(x0,y0,x1,y1) sqrt(((x1)-(x0))*((x1)-(x0))+((y1)-(y0))*((y1)-(y0)))
#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))


DrawingBoard::DrawingBoard(QWidget * parent) :
    QWidget(parent),
    backgroundIdd(1),
    mHasChanges(false),
    mTimestamp(time(NULL))
{
    bgImage.load(":/images/backgrounds/paper8.jpg");

    penBorder.setColor(QColor("#f3f3f3"));
    penBorder.setWidth(3);

    penBrush.setCapStyle(Qt::PenCapStyle::RoundCap);

    penEraser.setColor(QColor(0, 0, 0, 0));
    penEraser.setCapStyle(Qt::PenCapStyle::RoundCap);

    setBackgroundRole(QPalette::Base);

    context.color1.addListener(this, [&](QColor const & color) {
        penBrush.setColor(color);
    });

    context.brushSize.addListener(this, [&](int const & size) {
        penBrush.setWidth(size);
    });

    context.eraserSize.addListener(this, [&](int const & size) {
        penEraser.setWidth(size);
    });

    context.backgroundType.addListener(this, [&](int const & type) {
        backgroundIdd = type;
        repaint();
    });

    context.canvas.addListener(this, [&](QPixmap* const & page) {
        qInfo() << "Board has received a new canvas, size=" << page->width() << "," << page->height();
        buffer0 = page;
        update();
    });
}

DrawingBoard::~DrawingBoard()
{

}

void DrawingBoard::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    if (backgroundIdd == 0)
    {
        painter.setPen(penBorder);
        painter.drawRect(this->rect());
        painter.drawPixmap(this->rect(), *buffer0, buffer0->rect());
    }
    else
    {
        painter.drawPixmap(this->rect(), bgImage, bgImage.rect());
        painter.drawPixmap(this->rect(), *buffer0, buffer0->rect());
    }
}

void showMouseEvent(std::string label, QMouseEvent * event)
{
    int x = event->x();
    int y = event->y();

    int btLeft = (event->buttons() & Qt::LeftButton) != 0;
    int btRight = (event->buttons() & Qt::RightButton) != 0;
    int btMiddle = (event->buttons() & Qt::MiddleButton) != 0;
    int btBack = (event->buttons() & Qt::BackButton) != 0;
    int btForward = (event->buttons() & Qt::ForwardButton) != 0;

    qInfo("%s: %d %d, %d %d %d, %d %d", label.c_str(), x, y, btLeft, btMiddle, btRight, btBack, btForward);
}

void DrawingBoard::mousePressEvent(QMouseEvent *event)
{
//    showMouseEvent("press", event);

    last = event->pos();

    if (event->buttons() & Qt::MiddleButton)
        setCursor(Qt::PointingHandCursor);

    else if (event->buttons() & Qt::RightButton)
    {
        if (!context.isMenuVisible())
            context.setMenuPosition(mapToGlobal(event->pos()));
        context.setMenuVisible(!context.isMenuVisible());
    }
}

void DrawingBoard::mouseMoveEvent(QMouseEvent *event)
{
//    showMouseEvent("move", event);

    if (!(event->buttons() & Qt::LeftButton || cursor() == Qt::PointingHandCursor))
        return;

//    if (distance(last.x(), last.y(), event->x(), event->y()) < 10)
//        return;

    QRect area;
    QPen & pen = cursor() == Qt::CrossCursor ? penBrush : penEraser;
    int const w = pen.width();

    area.setTop(min(event->y()-w, last.y()-w));
    area.setBottom(max(event->y()+w, last.y()+w));

    area.setLeft(min(event->x()-w, last.x()-w));
    area.setRight(max(event->x()+w, last.x()+w));

    QPainter painter(buffer0);
    painter.setRenderHint(QPainter::Antialiasing, true);

    if (cursor() == Qt::PointingHandCursor)
        painter.setCompositionMode(QPainter::CompositionMode_Clear);

    double const ww = buffer0->width() / double(width());
    double const hh = buffer0->height() / double(height());

    QPoint p1(round(last.x() * ww), round(last.y() * hh));
    QPoint p2(round(event->x() * ww), round(event->y() * hh));

    painter.setBrush(Qt::NoBrush);
    painter.setPen(pen);
//    painter.setClipRect(area);
    painter.drawLine(p1, p2);

    update(area);

    last = event->pos();

    context.setHasChanges();
}

void DrawingBoard::mouseReleaseEvent(QMouseEvent *)
{
    setCursor(Qt::CrossCursor);
}
