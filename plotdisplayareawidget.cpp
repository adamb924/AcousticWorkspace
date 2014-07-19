#include "plotdisplayareawidget.h"

#include <QtWidgets>

#include <qwt_slider.h>

#include "plotviewwidget.h"
#include "intervaldisplaywidget.h"

PlotDisplayAreaWidget::PlotDisplayAreaWidget(QWidget *parent) :
	QWidget(parent)
{
    mTMin = 0;
    mTMax = 1;
    mLeftPos = 0;
    mRightPos = 1;

    mVerticalLayout = 0;
    mProsodyHeight = 200;
    mProsodySpacing = 10;
    mBorder = 10;

    QVBoxLayout *mainLayout = new QVBoxLayout;

    mScrollAreaWidget = new QWidget;

    QHBoxLayout *controlLayout = new QHBoxLayout;

    mSlider = new QwtSlider(Qt::Horizontal, 0);
    connect(mSlider,SIGNAL(sliderMoved(double)),this,SLOT(updateTimeScaleFromSlider(double)));
    controlLayout->addWidget(mSlider,100);

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

    //    setStyleSheet("QPushButton { margin: 10px;  };");
    mVerticalLayout = new QVBoxLayout(mScrollAreaWidget);
    mVerticalLayout->setSizeConstraint(QLayout::SetMinimumSize);
    mVerticalLayout->setContentsMargins(0,0,0,0);

    QScrollArea *scrollArea = new QScrollArea;
    scrollArea->setBackgroundRole(QPalette::Dark);
    scrollArea->setWidget(mScrollAreaWidget);
    scrollArea->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    mainLayout->addLayout(controlLayout,0);
    mainLayout->addWidget(scrollArea,1);

    setLayout(mainLayout);
    layout()->setContentsMargins(0,0,0,0);

    resize(600,400);

    setTimeAxes(0.0f,0.4f);
}

PlotDisplayAreaWidget::~PlotDisplayAreaWidget()
{
//    qDeleteAll(maPlotViewWidgets.begin(), maPlotViewWidgets.end());

//    maPlotViewWidgets.clear();
}

void PlotDisplayAreaWidget::all()
{
    setTimeAxes(mTMin,mTMax);
    setSliderFromWindow();
}

void PlotDisplayAreaWidget::addAnnotation(IntervalDisplayWidget *annotation)
{
    maAnnotations << annotation;
    connect( this, SIGNAL(timeAxisChanged(double,double)), annotation, SLOT(update()));
    drawVerticalLayout();
}

void PlotDisplayAreaWidget::addPlotView(PlotViewWidget *pr, QString name)
{
    maPlotViewWidgets << pr;
    maProsodyNames << name;
    maPlotViewWidgets.last()->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);

    connect(this,SIGNAL(timeAxisChanged(double,double)),maPlotViewWidgets.last(),SLOT(setHorizontalAxis(double,double)));

    emit timeAxisChanged(mLeftPos, mRightPos);

    drawVerticalLayout();
}

void PlotDisplayAreaWidget::in()
{
    double mid = (mRightPos - mLeftPos) / 2.0f;
    mWindowWidth /= 2.0f;
    setTimeAxes(mid-mWindowWidth/2.0f,mid+mWindowWidth/2.0f);
    setSliderFromWindow();
}

void PlotDisplayAreaWidget::out()
{
    double mid = (mRightPos - mLeftPos) / 2.0f;
    mWindowWidth *= 2.0f;
    double left = mid-mWindowWidth/2.0f;
    double right = mid+mWindowWidth/2.0f;
    if( left < mTMin ) { left = mTMin; }
    if( right > mTMax ) { right = mTMax; }
    setTimeAxes(left,right);
    setSliderFromWindow();
}

void PlotDisplayAreaWidget::sel()
{
}

void PlotDisplayAreaWidget::updateTimeScaleFromSlider(double value)
{
    mLeftPos = (mTMax-mTMin-mWindowWidth)*(value/100.0f);
    mRightPos = mLeftPos + mWindowWidth;
    setTimeAxes(mLeftPos,mRightPos);
}

void PlotDisplayAreaWidget::setTimeMinMax(double min, double max)
{
    mTMin = min;
    mTMax = max;

    mLeftPos = min;
    mRightPos = max;
    mWindowWidth = max - min;

    emit timeAxisChanged(mLeftPos, mRightPos);
}

void PlotDisplayAreaWidget::setTimeAxes(double left, double right)
{
    mLeftPos = left;
    mRightPos = right;
    mWindowWidth = right - left;

    emit timeAxisChanged(mLeftPos, mRightPos);
}

void PlotDisplayAreaWidget::drawVerticalLayout()
{
    if(mVerticalLayout != 0) { delete mVerticalLayout; }

    mVerticalLayout = new QVBoxLayout(mScrollAreaWidget);

    for(int i=0; i<maAnnotations.count(); i++)
	mVerticalLayout->addWidget(maAnnotations.at(i),0);

    for(int i=0; i<maPlotViewWidgets.count(); i++)
    {
        if( !maPlotViewWidgets.at(i)->name().isEmpty() )
            mVerticalLayout->addWidget(new QLabel(maPlotViewWidgets.at(i)->name()));
	mVerticalLayout->addWidget(maPlotViewWidgets.at(i),0);
    }

    mVerticalLayout->setSizeConstraint(QLayout::SetMinimumSize);
}

void PlotDisplayAreaWidget::setSliderFromWindow()
{
    mSlider->setValue( 100.0f * mLeftPos/(mTMax-mTMin-mWindowWidth)  );
}

void PlotDisplayAreaWidget::resizeEvent(QResizeEvent *event)
{
    mScrollAreaWidget->resize(event->size().width()-mBorder,(mProsodyHeight+mProsodySpacing)*maPlotViewWidgets.count());
}
