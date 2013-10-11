#include "intervaldisplaywidget.h"

#include <QtWidgets>
//#include <QPainter>
//#include <QPaintEvent>
#include <QtDebug>

#include <qwt/qwt_plot.h>
#include <qwt/qwt_plot_canvas.h>
#include <qwt/qwt_scale_widget.h>

#include "plotviewwidget.h"
#include "intervalannotation.h"

IntervalDisplayWidget::IntervalDisplayWidget(IntervalAnnotation *annotation, PlotViewWidget *prosody, QWidget *parent) : QWidget(parent)
{
    this->annotation = annotation;
    this->prosody = prosody;

    QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    this->setSizePolicy(sizePolicy);
}

QSize IntervalDisplayWidget::sizeHint() const
{
    return QSize(1000,20);
}

void IntervalDisplayWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    double left, right;
    int pLeft, pRight;
    int pHeight = this->height();
    left = prosody->plot()->axisScaleDiv(QwtPlot::xBottom).lowerBound();
    right = prosody->plot()->axisScaleDiv(QwtPlot::xBottom).upperBound();
    pLeft = prosody->plot()->pos().x() + prosody->plot()->canvas()->pos().x() + prosody->plot()->canvasMap(QwtPlot::xBottom).transform(left);
    pRight = prosody->plot()->pos().x() + prosody->plot()->canvas()->pos().x() + prosody->plot()->canvasMap(QwtPlot::xBottom).transform(right);


    // multiply times by scale to get numbers
    double scale = (pRight-pLeft)/(right-left);

    QBrush noBrush(Qt::NoBrush);
    QPen simpleBlack(QBrush(Qt::black),1);
    painter.setBrush(noBrush);
    painter.setPen(simpleBlack);

    for(int i=0; i<annotation->aIntervals.count(); i++)
    {
	if(annotation->aIntervals.at(i)->inRange(left, right))
	{
	    Interval *clipped = annotation->aIntervals.at(i)->clip(left, right);
	    int L, R;
	    L = pLeft + (clipped->left-left) * scale;
	    R = pLeft + (clipped->right-left) * scale;

	    QRect drawingRect(L, 0, R-L, pHeight-1);
	    painter.drawRect(drawingRect);
	    painter.drawText(drawingRect,Qt::AlignHCenter|Qt::AlignVCenter,clipped->label);

	    delete clipped;
	}
    }
}
