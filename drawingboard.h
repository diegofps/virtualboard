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

#include "menu.h"
#include "context.h"

class DrawingBoard : public QWidget
{
private:

    QPixmap bgImage;

    QPen penBorder;

    QPen penBrush;

    QPen penEraser;

    int backgroundIdd;

    QVector<QBrush*> pendingBackgrounds;

    QVector<QPoint> pendingLines;

    QPoint last;

    QPixmap * buffer0;

    bool mHasChanges;

    int64_t mTimestamp;

public:

    DrawingBoard(QWidget * parent = nullptr);

    ~DrawingBoard();

    void paintEvent(QPaintEvent *);

    void mouseReleaseEvent(QMouseEvent *event);

protected:

    void mouseMoveEvent(QMouseEvent *event);

    void mousePressEvent(QMouseEvent *event);

};

#endif // DRAWINGBOARD_H
