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

    QPixmap bgImage;

    QPen pen0;

    int backgroundIdd;

    QVector<QBrush*> pendingBackgrounds;

    QVector<QPoint> pendingLines;

    QPoint last;

    QPixmap * buffer0;

public:

    DrawingBoard(QWidget * parent);

    ~DrawingBoard();

    void clear();

    void paintEvent(QPaintEvent *);

    void mouseReleaseEvent(QMouseEvent *event);

    void mouseDoubleClickEvent(QMouseEvent *event);

    void wheelEvent(QWheelEvent *event);

    void undo();

    void redo();

    void setBackground(int idd);

    int background();

protected:

    void mouseMoveEvent(QMouseEvent *event);

    void mousePressEvent(QMouseEvent *event);

};

#endif // DRAWINGBOARD_H
