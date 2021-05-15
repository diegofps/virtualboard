#ifndef DRAWINGBOARD_H
#define DRAWINGBOARD_H

#include <QMouseEvent>
#include <QWidget>
#include <QBitmap>
#include <QPoint>
#include <QList>
#include <QLine>
#include <QPen>
#include <QTimer>

class DrawingBoard : public QWidget
{
private:

    QBrush bgFill;

    QBrush bgTransp;

    QPen penDrawing;

    bool transparent;

    QVector<QBrush*> pendingBackgrounds;

    QVector<QPoint> pendingLines;

    QPoint last;

    QPixmap * buffer;

public:

    DrawingBoard(QWidget * parent);

    ~DrawingBoard();

    void clear();

    void paintEvent(QPaintEvent *);

    void setTransparent(bool transparent);

    void mouseReleaseEvent(QMouseEvent *event);

    void mouseDoubleClickEvent(QMouseEvent *event);

    void wheelEvent(QWheelEvent *event);

protected:

    void paintBackground();

    void mouseMoveEvent(QMouseEvent *event);

    void mousePressEvent(QMouseEvent *event);

};

#endif // DRAWINGBOARD_H
