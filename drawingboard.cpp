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
    buffer(nullptr)
{
    bgTransp.setColor(QColor(255, 255, 255, 10));
    bgTransp.setStyle(Qt::BrushStyle::SolidPattern);

    bgFill.setColor(QColor(255, 255, 255, 255));
    bgFill.setStyle(Qt::BrushStyle::SolidPattern);

    penDrawing.setColor(QColor(200, 0, 0, 255));
    penDrawing.setCapStyle(Qt::PenCapStyle::RoundCap);
    penDrawing.setWidth(5);

//    setAutoBufferSwap(false);
    setAutoFillBackground(false);
    setBackgroundRole(QPalette::Base);
    setAttribute(Qt::WA_TranslucentBackground);

//    connect(&timer, SIGNAL(timeout()), this, SLOT(repaint()));
//    timer.start(50);
}

DrawingBoard::~DrawingBoard()
{

}

void DrawingBoard::clear()
{
    paintBackground();
    repaint();
}

void DrawingBoard::paintEvent(QPaintEvent *)
{
    if (buffer == nullptr)
    {
        buffer = new QPixmap(QSize(width(), height()));
        paintBackground();
    }

    QPainter painter2(this);
    painter2.drawImage(QPoint(0,0), buffer->toImage());
}

void DrawingBoard::paintBackground()
{
    if (transparent)
        buffer->fill(bgTransp.color());
    else
        buffer->fill(bgFill.color());
}

void DrawingBoard::setTransparent(bool transparent)
{
    this->transparent = transparent;
//    clear();
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
        if (buffer == nullptr)
            return;

        QRect area;
        int const w = penDrawing.width();

        area.setTop(min(event->y()-w, last.y()-w));
        area.setBottom(max(event->y()+w, last.y()+w));

        area.setLeft(min(event->x()-w, last.x()-w));
        area.setRight(max(event->x()+w, last.x()+w));

        QPainter painter(buffer);
        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.setClipRect(area);
        painter.setBrush(Qt::NoBrush);
        painter.setPen(penDrawing);
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
