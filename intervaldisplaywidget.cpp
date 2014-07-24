#include "intervaldisplaywidget.h"

#include "interval.h"

#include <QtWidgets>
#include <QtDebug>

#include <qwt_plot.h>
#include <qwt_plot_canvas.h>
#include <qwt_scale_widget.h>

#include "plotviewwidget.h"
#include "intervalannotation.h"

IntervalDisplayWidget::IntervalDisplayWidget(IntervalAnnotation *annotation, PlotViewWidget *prosody, QWidget *parent)
    : QWidget(parent), mAnnotation(annotation), mProsody(prosody)
{
    QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    setSizePolicy(sizePolicy);
}

QSize IntervalDisplayWidget::sizeHint() const
{
    return QSize(1000,20);
}

void IntervalDisplayWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);

    double left, right;
    int pLeft, pRight;
    int pHeight = height();
    left = mProsody->axisScaleDiv(QwtPlot::xBottom).lowerBound();
    right = mProsody->axisScaleDiv(QwtPlot::xBottom).upperBound();
    pLeft = mProsody->pos().x() + mProsody->canvas()->pos().x() + mProsody->canvasMap(QwtPlot::xBottom).transform(left);
    pRight = mProsody->pos().x() + mProsody->canvas()->pos().x() + mProsody->canvasMap(QwtPlot::xBottom).transform(right);


    // multiply times by scale to get numbers
    double scale = (pRight-pLeft)/(right-left);

    QBrush noBrush(Qt::NoBrush);
    QPen simpleBlack(QBrush(Qt::black),1);
    painter.setBrush(noBrush);
    painter.setPen(simpleBlack);

    for(int i=0; i<mAnnotation->maIntervals.count(); i++)
    {
	if(mAnnotation->maIntervals.at(i)->inRange(left, right))
	{
	    Interval *clipped = mAnnotation->maIntervals.at(i)->clip(left, right);
	    int L, R;
	    L = pLeft + (clipped->mLeft-left) * scale;
	    R = pLeft + (clipped->mRight-left) * scale;

	    QRect drawingRect(L, 0, R-L, pHeight-1);
	    painter.drawRect(drawingRect);
	    painter.drawText(drawingRect,Qt::AlignHCenter|Qt::AlignVCenter,clipped->mLabel);

	    delete clipped;
	}
    }
}
