#ifndef DRAWINGBOARD_H
#define DRAWINGBOARD_H

#include <QMouseEvent>
#include <QWidget>
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

    QPen penHistory;

    QPen penDrawing;

    bool transparent;

    QList<QVector<QPoint>*> strokes;

    QList<QVector<QPoint>*> undoedStrokes;

    QVector<QPoint> * currentStroke;

    QTimer timer;

public:

    DrawingBoard(QWidget * parent);

    ~DrawingBoard();

    void undo();

    void redo();

    void clear();

    void paintEvent(QPaintEvent *);

    void set_transparent(bool transparent);

    void mouseReleaseEvent(QMouseEvent *event);

    void mouseDoubleClickEvent(QMouseEvent *event);

    void wheelEvent(QWheelEvent *event);

protected:

    void mouseMoveEvent(QMouseEvent *event);

    void mousePressEvent(QMouseEvent *event);

};

#endif // DRAWINGBOARD_H
