#include "drawingboard.h"

#include <QGuiApplication>
#include <QPainter>
#include <QScreen>
#include <QBitmap>
#include <QColor>
#include <cmath>

#define distance(x0,y0,x1,y1) sqrt(((x1)-(x0))*((x1)-(x0))+((y1)-(y0))*((y1)-(y0)))
#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))


DrawingBoard::DrawingBoard(QWidget * parent) :
    QWidget(parent),
    backgroundIdd(1),
    buffer0(nullptr)
{
    bgImage.load(":/images/backgrounds/paper8.jpg");

    pen0.setColor(QColor("#2f5fb6"));
    pen0.setCapStyle(Qt::PenCapStyle::RoundCap);
    pen0.setWidth(5);

    setBackgroundRole(QPalette::Base);
}

DrawingBoard::~DrawingBoard()
{
    if (buffer0 != nullptr)
        delete buffer0;
}

void DrawingBoard::clear()
{
    buffer0->fill(Qt::transparent);
    repaint();
}

void DrawingBoard::undo()
{

}

void DrawingBoard::redo()
{

}

void DrawingBoard::paintEvent(QPaintEvent *)
{
    if (buffer0 == nullptr)
    {
        buffer0 = new QPixmap(QSize(width(), height()));
        buffer0->fill(Qt::transparent);
    }

    else if (buffer0->rect() != rect())
    {
        delete buffer0;
        buffer0 = new QPixmap(QSize(width(), height()));
        buffer0->fill(Qt::transparent);
    }

    QPainter painter(this);

    if (backgroundIdd == 0)
    {
        painter.setPen(pen0);
        painter.drawRect(this->rect());
//        painter.fillRect(this->rect(), QColor(255, 255, 255, 10));
        painter.drawPixmap(this->rect(), *buffer0, buffer0->rect());
    }
    else
    {
        painter.drawPixmap(this->rect(), bgImage, bgImage.rect());
        painter.drawPixmap(this->rect(), *buffer0, buffer0->rect());
    }
}

void DrawingBoard::setBackground(int idd)
{
    this->backgroundIdd = idd;
    repaint();
}

int DrawingBoard::background()
{
    return backgroundIdd;
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
    showMouseEvent("press", event);

    if (event->buttons() & Qt::LeftButton)
    {
        last.setX(event->x());
        last.setY(event->y());
    }

    else if (event->buttons() & Qt::MiddleButton)
        clear();
}

void DrawingBoard::mouseMoveEvent(QMouseEvent *event)
{
    showMouseEvent("move", event);

    if (event->buttons() & Qt::LeftButton)
    {
        if (buffer0 == nullptr)
            return;

        QRect area;
        int const w = pen0.width();

        area.setTop(min(event->y()-w, last.y()-w));
        area.setBottom(max(event->y()+w, last.y()+w));

        area.setLeft(min(event->x()-w, last.x()-w));
        area.setRight(max(event->x()+w, last.x()+w));

        QPainter painter(buffer0);
        painter.setPen(pen0);
        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.setClipRect(area);
        painter.setBrush(Qt::NoBrush);
        painter.drawLine(last, event->pos());

        last = event->pos();

        update(area);
    }
}

void DrawingBoard::mouseReleaseEvent(QMouseEvent *event)
{
    showMouseEvent("release", event);
}

void DrawingBoard::mouseDoubleClickEvent(QMouseEvent *event)
{
    showMouseEvent("doubleClick", event);
}

void DrawingBoard::wheelEvent(QWheelEvent *event)
{
    QPoint numPixels = event->pixelDelta();
    QPoint numDegrees = event->angleDelta() / 8;
    qInfo("wheel: %d %d, %d %d", numPixels.x(), numPixels.y(), numDegrees.x(), numDegrees.y());
}
