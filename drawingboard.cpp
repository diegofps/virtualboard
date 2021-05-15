#include "drawingboard.h"

#include <QGuiApplication>
#include <QPainter>
#include <QScreen>
#include <QColor>
#include <cmath>

#define distance(x0,y0,x1,y1) sqrt(((x1)-(x0))*((x1)-(x0))+((y1)-(y0))*((y1)-(y0)))

DrawingBoard::DrawingBoard(QWidget * parent) :
    QWidget(parent),
    currentStroke(new QVector<QPoint>())
{
    bgTransp.setColor(QColor(255, 255, 255, 10));
    bgTransp.setStyle(Qt::BrushStyle::SolidPattern);

    bgFill.setColor(QColor(255, 255, 255, 255));
    bgFill.setStyle(Qt::BrushStyle::SolidPattern);

    penHistory.setColor(QColor(200, 0, 0, 255));
    penHistory.setWidth(5);

    penDrawing.setColor(QColor(200, 0, 0, 255));
    penDrawing.setWidth(5);

//    setBackgroundRole(QPalette::Base);

//    connect(&timer, SIGNAL(timeout()), this, SLOT(repaint()));
//    timer.start(50);
}

DrawingBoard::~DrawingBoard()
{
    for (int i=0;i!=strokes.size();++i)
        delete strokes[i];

    for (int i=0;i!=undoedStrokes.size();++i)
        delete undoedStrokes[i];
}

void DrawingBoard::undo()
{
    if (strokes.isEmpty())
        return;

    auto * const stroke = strokes.back();
    strokes.pop_back();

    undoedStrokes.append(stroke);
    repaint();
}

void DrawingBoard::redo()
{
    if (undoedStrokes.isEmpty())
        return;

    auto * const stroke = undoedStrokes.back();
    undoedStrokes.pop_back();

    strokes.append(stroke);
    repaint();
}

void DrawingBoard::clear()
{
    for (auto & s : strokes)
        delete s;

    for (auto & s : undoedStrokes)
        delete s;

    strokes.clear();
    undoedStrokes.clear();
    currentStroke->clear();
    repaint();
}

void DrawingBoard::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    // Draw background
    if (transparent)
    {
//        qInfo("Drawing transparent bg");
        painter.setBrush(bgTransp);
        painter.setPen(Qt::NoPen);
        painter.drawRect(0, 0, width(), height());
    }
    else
    {
//        qInfo("Drawing filled bg");
        painter.setBrush(bgFill);
        painter.setPen(Qt::NoPen);
        painter.drawRect(0, 0, width(), height());
    }

    // Draw lines
//    qInfo("Drawing lines");
    painter.setBrush(Qt::NoBrush);
    painter.setPen(penHistory);

    for (int i=0;i!=strokes.size();++i)
    {
//        qInfo("Drawing stroke %d with %d points", i, strokes[i]->size());
        painter.drawPolyline(strokes[i]->data(), strokes[i]->size());
    }

//    qInfo("Drawing currentStroke with %d points", currentStroke->size());
    painter.setBrush(Qt::NoBrush);
    painter.setPen(penDrawing);
    painter.drawPolyline(currentStroke->data(), currentStroke->size());
}

void DrawingBoard::set_transparent(bool transparent)
{
    this->transparent = transparent;
    repaint();
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
        currentStroke->append(QPoint(event->x(), event->y()));

    else if (event->buttons() & Qt::BackButton)
        undo();

    else if (event->buttons() & Qt::ForwardButton)
        redo();

    else if (event->buttons() & Qt::MiddleButton)
        clear();
}

void DrawingBoard::mouseMoveEvent(QMouseEvent *event)
{
    showMouseEvent("move", event);

    if (event->buttons() & Qt::LeftButton)
    {
        currentStroke->append(QPoint(event->x(), event->y()));
        qInfo("Current size: %d", currentStroke->size());
        repaint();
    }
}

void DrawingBoard::mouseReleaseEvent(QMouseEvent *event)
{
    showMouseEvent("release", event);

    if (!currentStroke->isEmpty())
    {
        strokes.append(currentStroke);
        currentStroke = new QVector<QPoint>();

        for (int i=0;i!=undoedStrokes.size();++i)
            delete undoedStrokes[i];

        undoedStrokes.clear();
        repaint();
    }
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
