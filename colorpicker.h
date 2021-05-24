#ifndef COLORPICKER_H
#define COLORPICKER_H

#include "context.h"

#include <QWidget>
#include <QPen>
#include <cmath>

QString const colors[] = { "#ffed00", "#a2c037", "#6aa842", "#56a099", "#009de0", "#4470b5", "#5a287f", "#80207e", "#c20d19", "#cb4f24", "#dc911b", "#e7b041" };
const int ySize = 2;

class ColorRegion
{
public:

    QPolygon * mPolygon;

    int mX;

    int mY;

    QBrush mBrush;

    bool mHovering;

    bool mSelected;

public:

    ColorRegion(int const x, int const y);

    ~ColorRegion();

    void resizeEvent(QResizeEvent *event);

    void mouseMoveEvent(QPoint & pos);

    bool mousePressEvent(QPoint & pos);

    void paintEvent(QPainter & painter, QPen & selectedPen, QPen & hoveringPen);

};

class ColorPicker : public QWidget
{
private:

    QVector<ColorRegion*> mColors;

    QPen mSelectedPen;

    QPen mHoverPen;

    Context * c;

public:

    ColorPicker(QWidget * parent);

    ~ColorPicker();

    void mouseMoveEvent(QMouseEvent *event);

    void mousePressEvent(QMouseEvent *event);

    void paintEvent(QPaintEvent *event);

    void resizeEvent(QResizeEvent *event);

    void setSettings(Context &s);
};

#endif // COLORPICKER_H
