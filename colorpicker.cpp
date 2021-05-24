#include "colorpicker.h"

#include <QPainter>
#include <QPolygon>
#include <QResizeEvent>


ColorPicker::ColorPicker(QWidget * parent) :
    QWidget(parent),
    c(nullptr)
{
    mSelectedPen.setCapStyle(Qt::PenCapStyle::RoundCap);
    mSelectedPen.setColor(QColor(50,50,50));
    mSelectedPen.setWidth(2);

    mHoverPen.setCapStyle(Qt::PenCapStyle::RoundCap);
    mHoverPen.setColor(QColor(50,50,255));
    mHoverPen.setWidth(2);

    for (int x=0;x!=12;++x)
        for (int y=0;y!=ySize;++y)
            mColors.push_back(new ColorRegion(x, y));

    mColors.push_back(new ColorRegion(-1, -1));
    setMouseTracking(true);
}

ColorPicker::~ColorPicker()
{
    for (int i=0;i!=mColors.size();++i)
        delete mColors[i];
}

void ColorPicker::mouseMoveEvent(QMouseEvent *event)
{
    QPoint pos(event->x() - width() / 2, event->y() - height() / 2);

    for (int i=0;i!=mColors.size();++i)
        mColors[i]->mouseMoveEvent(pos);

    update();
}

void ColorPicker::mousePressEvent(QMouseEvent *event)
{
    QPoint pos(event->x() - width() / 2, event->y() - height() / 2);

    for (int i=0;i!=mColors.size();++i)
        if (mColors[i]->mousePressEvent(pos) && c != nullptr)
            c->setColor(mColors[i]->mBrush.color());

    update();
}

void ColorPicker::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.translate(width() / 2, height() / 2);

    for (int i=0;i!=mColors.size();++i)
        mColors[i]->paintEvent(painter, mSelectedPen, mHoverPen);
}

void ColorPicker::resizeEvent(QResizeEvent *event)
{
    for (int i=0;i!=mColors.size();++i)
        mColors[i]->resizeEvent(event);
}

void ColorPicker::setSettings(Context & s)
{
    this->c = &s;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// COLOR REGION
//////////////////////////////////////////////////////////////////////////////////////////////////

ColorRegion::ColorRegion(const int x, const int y) :
    mPolygon(nullptr),
    mX(x),
    mY(y),
    mHovering(false),
    mSelected(false)
{
    if (y == -1)
    {
        mBrush.setColor(QColor(255, 255, 255));
    }
    else
    {
        double const a1 = (mY + 1) / double(ySize);
        double const a2 = 1.0 - a1;

        QColor color(colors[x]);

        int const r = 255 * a2 + color.red() * a1;
        int const g = 255 * a2 + color.green() * a1;
        int const b = 255 * a2 + color.blue() * a1;
        int const a = 255;

        mBrush.setColor(QColor(r, g, b, a));
    }

    mBrush.setStyle(Qt::BrushStyle::SolidPattern);
}

ColorRegion::~ColorRegion()
{
    if (mPolygon != nullptr)
        delete mPolygon;
}

void ColorRegion::resizeEvent(QResizeEvent *event)
{
    if (mPolygon != nullptr)
        delete mPolygon;

    int const w = event->size().width();
    int const h = event->size().height();

    QVector<QPoint> points;

    double len = (w < h ? w : h) / 2.0;

    if (mY == -1)
    {
        for (int x=0;x!=12;++x)
        {
            double angle1 = (-90 -15 + x * 30) / 360.0 * 2.0 * M_PI;
            double y1 = 1 / double(ySize+1)  * len;

            double s1 = sin(angle1);
            double c1 = cos(angle1);
            points.push_back(QPoint(c1 * y1, s1 * y1));
        }
    }
    else
    {
        double angle1 = (-90 -15 + mX * 30) / 360.0 * 2.0 * M_PI;
        double angle2 = (-90-15 + (mX + 1) * 30) / 360.0 * 2.0 * M_PI;

        double y1 = (mY+1) / double(ySize+1)  * len;
        double y2 = (mY+2) / double(ySize+1)  * len;

        double s1 = sin(angle1);
        double c1 = cos(angle1);
        points.push_back(QPoint(c1 * y1, s1 * y1));
        points.push_back(QPoint(c1 * y2, s1 * y2));

        double s2 = sin(angle2);
        double c2 = cos(angle2);
        points.push_back(QPoint(c2 * y2, s2 * y2));
        points.push_back(QPoint(c2 * y1, s2 * y1));
    }

    mPolygon = new QPolygon(points);
}

void ColorRegion::mouseMoveEvent(QPoint & pos)
{
    mHovering = mPolygon->containsPoint(pos, Qt::FillRule::OddEvenFill);
}

bool ColorRegion::mousePressEvent(QPoint & pos)
{
    mSelected = mPolygon->containsPoint(pos, Qt::FillRule::OddEvenFill);
    return mSelected;
}

void ColorRegion::paintEvent(QPainter & painter, QPen & selectedPen, QPen & hoveringPen)
{
    painter.setPen(Qt::NoPen);
    painter.setBrush(mBrush);
    painter.drawPolygon(*mPolygon);

    if (mSelected)
    {
        painter.setPen(selectedPen);
        painter.setBrush(Qt::NoBrush);
        painter.drawPolygon(*mPolygon);
    }

    else if (mHovering)
    {
        painter.setPen(hoveringPen);
        painter.setBrush(Qt::NoBrush);
        painter.drawPolygon(*mPolygon);
    }
}
