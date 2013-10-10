#include "plotdisplayareawidget.h"

#include <QtGui>

#include <qwt_slider.h>

#include "plotviewwidget.h"
#include "intervaldisplaywidget.h"

PlotDisplayAreaWidget::PlotDisplayAreaWidget(QWidget *parent) :
	QWidget(parent)
{
    tMin = 0;
    tMax = 1;
    leftPos = 0;
    rightPos = 1;

    verticalLayout = 0;
    prosodyHeight = 200;
    prosodySpacing = 10;
    border = 10;

    QVBoxLayout *mainLayout = new QVBoxLayout;

    scrollAreaWidget = new QWidget;

    QHBoxLayout *controlLayout = new QHBoxLayout;

    slider = new QwtSlider(0);
    connect(slider,SIGNAL(sliderMoved(double)),this,SLOT(updateTimeScaleFromSlider(double)));
    controlLayout->addWidget(slider,100);

    QPushButton *bIn, *bOut, *bAll, *bSel;
    bIn = new QPushButton(tr("In"));
    bOut = new QPushButton(tr("Out"));
    bAll = new QPushButton(tr("All"));
    bSel = new QPushButton(tr("Sel"));
    controlLayout->addWidget(bIn,0);
    controlLayout->addWidget(bOut,0);
    controlLayout->addWidget(bAll,0);
    controlLayout->addWidget(bSel,0);

    connect(bIn,SIGNAL(clicked()),this,SLOT(in()));
    connect(bOut,SIGNAL(clicked()),this,SLOT(out()));
    connect(bAll,SIGNAL(clicked()),this,SLOT(all()));
    connect(bSel,SIGNAL(clicked()),this,SLOT(sel()));

    //    this->setStyleSheet("QPushButton { margin: 10px;  };");
    verticalLayout = new QVBoxLayout(scrollAreaWidget);
    verticalLayout->setSizeConstraint(QLayout::SetMinimumSize);
    verticalLayout->setContentsMargins(0,0,0,0);

    QScrollArea *scrollArea = new QScrollArea;
    scrollArea->setBackgroundRole(QPalette::Dark);
    scrollArea->setWidget(scrollAreaWidget);
    scrollArea->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    mainLayout->addLayout(controlLayout,0);
    mainLayout->addWidget(scrollArea,1);

    this->setLayout(mainLayout);
    layout()->setContentsMargins(0,0,0,0);

    resize(600,400);

    setTimeAxes(0.0f,0.4f);
}

PlotDisplayAreaWidget::~PlotDisplayAreaWidget()
{
    qDeleteAll(aPlotViewWidgets.begin(), aPlotViewWidgets.end());

    aPlotViewWidgets.clear();
}

void PlotDisplayAreaWidget::all()
{
    setTimeAxes(tMin,tMax);
    setSliderFromWindow();
}

void PlotDisplayAreaWidget::addAnnotation(IntervalDisplayWidget *annotation)
{
    aAnnotations << annotation;
    connect( this, SIGNAL(timeAxisChanged(double,double)), annotation, SLOT(update()));
    drawVerticalLayout();
}

void PlotDisplayAreaWidget::addPlotView(PlotViewWidget *pr, QString name)
{
    aPlotViewWidgets << pr;
    aProsodyNames << name;
    aPlotViewWidgets.last()->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);

    connect(this,SIGNAL(timeAxisChanged(double,double)),aPlotViewWidgets.last(),SLOT(setHorizontalAxis(double,double)));

    emit timeAxisChanged(leftPos, rightPos);

    drawVerticalLayout();
}

void PlotDisplayAreaWidget::in()
{
    double mid = (rightPos - leftPos) / 2.0f;
    windowWidth /= 2.0f;
    setTimeAxes(mid-windowWidth/2.0f,mid+windowWidth/2.0f);
    setSliderFromWindow();
}

void PlotDisplayAreaWidget::out()
{
    double mid = (rightPos - leftPos) / 2.0f;
    windowWidth *= 2.0f;
    double left = mid-windowWidth/2.0f;
    double right = mid+windowWidth/2.0f;
    if( left < tMin ) { left = tMin; }
    if( right > tMax ) { right = tMax; }
    setTimeAxes(left,right);
    setSliderFromWindow();
}

void PlotDisplayAreaWidget::sel()
{
}

void PlotDisplayAreaWidget::updateTimeScaleFromSlider(double value)
{
    leftPos = (tMax-tMin-windowWidth)*(value/100.0f);
    rightPos = leftPos + windowWidth;
    setTimeAxes(leftPos,rightPos);
}

void PlotDisplayAreaWidget::setTimeMinMax(double min, double max)
{
    tMin = min;
    tMax = max;

    leftPos = min;
    rightPos = max;
    windowWidth = max - min;

    emit timeAxisChanged(leftPos, rightPos);
}

void PlotDisplayAreaWidget::setTimeAxes(double left, double right)
{
    leftPos = left;
    rightPos = right;
    windowWidth = right - left;

    emit timeAxisChanged(leftPos, rightPos);
}

void PlotDisplayAreaWidget::drawVerticalLayout()
{
    if(verticalLayout != 0) { delete verticalLayout; }

    verticalLayout = new QVBoxLayout(scrollAreaWidget);

    for(int i=0; i<aAnnotations.count(); i++)
	verticalLayout->addWidget(aAnnotations.at(i),0);

    for(int i=0; i<aPlotViewWidgets.count(); i++)
    {
        if( !aPlotViewWidgets.at(i)->name().isEmpty() )
            verticalLayout->addWidget(new QLabel(aPlotViewWidgets.at(i)->name()));
	verticalLayout->addWidget(aPlotViewWidgets.at(i),0);
    }

    verticalLayout->setSizeConstraint(QLayout::SetMinimumSize);
}

void PlotDisplayAreaWidget::setSliderFromWindow()
{
    slider->setValue( 100.0f * leftPos/(tMax-tMin-windowWidth)  );
}

void PlotDisplayAreaWidget::resizeEvent(QResizeEvent *event)
{
    scrollAreaWidget->resize(event->size().width()-border,(prosodyHeight+prosodySpacing)*aPlotViewWidgets.count());
}
