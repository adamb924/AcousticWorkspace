#include "plotviewwidget.h"
#include "waveformdata.h"
#include <qwt_plot_curve.h>
#include <qwt_text_label.h>
#include <qwt_color_map.h>
#include <qwt_scale_div.h>
#include <qwt_scale_draw.h>
#include <qwt_scale_widget.h>

#include <QtDebug>
#include <QHBoxLayout>
#include <QMouseEvent>
#include <QContextMenuEvent>
#include <QMenu>
#include <QMouseEvent>

#include "textdisplaydialog.h"

#include "indexedaction.h"

#include "curvesettingsdialog.h"
#include "spectrogramsettingsdialog.h"
#include <qwt_plot.h>

void PlotViewWidget::setHorizontalAxis(double left, double right)
{
    mQwtPlot->setAxisScale((int)QwtPlot::xBottom,left,right,0.0f);
    mQwtPlot->replot();
}

PlotViewWidget::PlotViewWidget(QString name, QWidget *parent) : QWidget(parent), mLabel(name), mWidgetHeight(200), mSecondaryAxis(false)
{
    mHlayout = new QHBoxLayout(this);

    QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    mQwtPlot = new QwtPlot(this);
    mQwtPlot->setSizePolicy(sizePolicy);
    mQwtPlot->setMinimumHeight(mWidgetHeight);
    mQwtPlot->setMaximumHeight(1500);

    mQwtPlot->enableAxis(QwtPlot::yLeft);
    mQwtPlot->enableAxis(QwtPlot::yRight);

    int scaleWidth = 60;
    mQwtPlot->axisScaleDraw(QwtPlot::yLeft)->setMinimumExtent( scaleWidth );
    mQwtPlot->axisScaleDraw(QwtPlot::yRight)->setMinimumExtent( scaleWidth );

    mHlayout->addWidget(mQwtPlot);
}

PlotViewWidget::~PlotViewWidget()
{
//    qDeleteAll(maCurves.begin(), maCurves.end());
}

QSize PlotViewWidget::sizeHint() const
{
    return QSize(750,mWidgetHeight);
}

void PlotViewWidget::addCurveData(WaveformData *curveData, bool secondary, QColor col)
{
    maWaveformData << curveData;

    QwtPlotCurve *waveCurve = new QwtPlotCurve("dummy");
    waveCurve->setRenderHint(QwtPlotItem::RenderAntialiased);
    waveCurve->setPen(QPen(col));
    waveCurve->setSamples( curveData );
    maCurves << waveCurve;
    waveCurve->attach(mQwtPlot);

    if( !mSecondaryAxis || maCurves.length() == 0 ) // no secondary axis or just one plot
    {
	mQwtPlot->replot();
    mQwtPlot->setAxisScaleDiv(QwtPlot::yRight, mQwtPlot->axisScaleDiv(QwtPlot::yLeft));
	mQwtPlot->replot();
    }
    else
    {
	if(secondary)
	{
	    mQwtPlot->setAxisAutoScale(QwtPlot::yRight);
	    waveCurve->setYAxis(QwtPlot::yRight);
	}
	else
	{
	    waveCurve->setYAxis(QwtPlot::yLeft);
	}
    }

    mQwtPlot->repaint();
}

void PlotViewWidget::addSpectrogramData(SpectrogramData *spectrogramData)
{
    maSpectrogramData << spectrogramData;

    QwtPlotSpectrogram *spectrogram = new QwtPlotSpectrogram();

    QwtLinearColorMap * colorMap = new QwtLinearColorMap(Qt::white, Qt::black);
    //    QwtLinearColorMap colorMap(Qt::black, Qt::white);
    spectrogram->setColorMap(colorMap);
    spectrogram->setData(spectrogramData);

    QRectF r = spectrogramData->boundingRect();
    mQwtPlot->setAxisScale( QwtPlot::yLeft , 0, 5000, 1000);
    mQwtPlot->setAxisScale( QwtPlot::yRight , 0, 5000, 1000);
    mQwtPlot->setAxisScale( QwtPlot::xBottom , r.left(), r.right(), 0.1);

    maSpectrograms << spectrogram;
    spectrogram->attach(mQwtPlot);
    mQwtPlot->repaint();
}


void PlotViewWidget::removeItemAt(int i)
{
    maCurves.at(i)->detach();
    maCurves.removeAt(i);
    maWaveformData.removeAt(i);

    if( !mSecondaryAxis || maCurves.length() == 1 ) // no secondary axis or just one plot
    {
	mQwtPlot->replot();
    mQwtPlot->setAxisScaleDiv(QwtPlot::yRight, mQwtPlot->axisScaleDiv(QwtPlot::yLeft));
	mQwtPlot->replot();
    }
}

void PlotViewWidget::toggleCurveAxisAssociation(int index)
{
    if(index >= maCurves.length()) { return; }
    if(maCurves.at(index)->yAxis() == QwtPlot::yLeft)
    {
	mQwtPlot->setAxisAutoScale(QwtPlot::yRight);
	maCurves.at(index)->setYAxis(QwtPlot::yRight);
    }
    else
    {
	maCurves.at(index)->setYAxis(QwtPlot::yLeft);
    }
    mQwtPlot->replot();
}

void PlotViewWidget::mouseMoveEvent ( QMouseEvent * event )
{
    if( event->pos().y() >= (rect().bottom()-2) || event->buttons() == Qt::LeftButton  )
    {
/*
	setCursor(QCursor(Qt::SizeVerCursor));
	if( event->buttons() == Qt::LeftButton )
	{
	    widgetHeight = event->pos().y();
	    resize( QSize(width(), widgetHeight ));
	    qwtPlot->replot();
	}
*/
    }
    else
    {
	setCursor(QCursor(Qt::CrossCursor));
    }
}

void PlotViewWidget::setHeight(int height)
{
    resize( QSize(width(), height ));
}

inline QString countString(int i)
{
    return QString::number(i+1) + ". ";
}

void PlotViewWidget::contextMenuEvent ( QContextMenuEvent * event )
{
    QMenu menu(this);

    if( maCurves.count() > 0 )
    {
	QMenu *curveSettingsMenu = menu.addMenu("Curve settings");
	for(int i=0; i<maCurves.count(); i++)
	{
	    IndexedAction *tmp = new IndexedAction(countString(i)+maWaveformData.at(i)->name(),i,curveSettingsMenu);
	    curveSettingsMenu->addAction( tmp );
	    connect(tmp, SIGNAL(indexClicked(int)), this, SLOT(launchCurveSettings(int)) );
	}
    }

    if( maSpectrograms.count() > 0 )
    {
	QMenu *spectrogramSettingsMenu = menu.addMenu("Spectogram settings");
	for(int i=0; i<maCurves.count(); i++)
	{
	    IndexedAction *tmp = new IndexedAction(countString(i)+maSpectrogramData.at(i)->name(),i,spectrogramSettingsMenu);
	    spectrogramSettingsMenu->addAction( tmp );
	    connect(tmp, SIGNAL(indexClicked(int)), this, SLOT(launchSpectrogramSettings(int)) );
	}
    }

    if( maCurves.count() > 0 )
    {
	menu.addSeparator();

	QMenu *waveformTimesMenu = menu.addMenu("Curve times");
	for(int i=0; i<maCurves.count(); i++)
	{
	    IndexedAction *tmp = new IndexedAction(countString(i)+maWaveformData.at(i)->name(),i,waveformTimesMenu);
	    waveformTimesMenu->addAction( tmp );
	    connect(tmp, SIGNAL(indexClicked(int)), this, SLOT(showCurveTimes(int)) );
	}

	QMenu *allDataMenu = menu.addMenu("Curve data");
	for(int i=0; i<maCurves.count(); i++)
	{
	    IndexedAction *tmp = new IndexedAction(countString(i)+maWaveformData.at(i)->name(),i,allDataMenu);
	    allDataMenu->addAction( tmp );
	    connect(tmp, SIGNAL(indexClicked(int)), this, SLOT(showCurveTimesData(int)) );
	}
    }

    if( maSpectrograms.count() > 0 )
    {
	menu.addSeparator();

	QMenu *spectrogramTimeMenu = menu.addMenu("Spectrogram times");
	for(int i=0; i<maSpectrograms.count(); i++)
	{
	    IndexedAction *tmp = new IndexedAction(countString(i)+maSpectrogramData.at(i)->name(),i,spectrogramTimeMenu);
	    spectrogramTimeMenu->addAction( tmp );
	    connect(tmp, SIGNAL(indexClicked(int)), this, SLOT(showSpectrogramTimes(int)) );
	}

	QMenu *spectrogramFrequencyMenu = menu.addMenu("Spectrogram frequencies");
	for(int i=0; i<maSpectrograms.count(); i++)
	{
	    IndexedAction *tmp = new IndexedAction(countString(i)+maSpectrogramData.at(i)->name(),i,spectrogramFrequencyMenu);
	    spectrogramFrequencyMenu->addAction( tmp );
	    connect(tmp, SIGNAL(indexClicked(int)), this, SLOT(showSpectrogramFrequencies(int)) );
	}
    }

    menu.exec(event->globalPos());
}

void PlotViewWidget::showCurveTimes(int index)
{
    QString str;
    for(quint32 i=0; i<maWaveformData.at(index)->getNSamples(); i++)
    {
    str.append(QString::number(maWaveformData.at(index)->xData().at(i) )+"\n");
    }
    TextDisplayDialog tdd(str, this);
    tdd.exec();
}

void PlotViewWidget::showCurveTimesData(int index)
{
    QString str;
    for(quint32 i=0; i<maWaveformData.at(index)->getNSamples(); i++)
    {
    str.append(QString::number(maWaveformData.at(index)->xData().at(i))+"\t"+QString::number(maWaveformData.at(index)->yData().at(i))+"\n");
    }
    TextDisplayDialog tdd(str, this);
    tdd.exec();
}

void PlotViewWidget::showSpectrogramTimes(int index)
{
    QString str;
    for(quint32 i=0; i<maSpectrogramData.at(index)->getNTimeSteps(); i++)
    {
	str.append(QString::number(maSpectrogramData.at(index)->getTimeFromIndex(i) )+"\n");
    }
    TextDisplayDialog tdd(str, this);
    tdd.exec();
}

void PlotViewWidget::showSpectrogramFrequencies(int index)
{
    QString str;
    for(quint32 i=0; i<maSpectrogramData.at(index)->getNFrequencyBins(); i++)
    {
	str.append(QString::number(maSpectrogramData.at(index)->getFrequencyFromIndex(i))+"\n");
    }
    TextDisplayDialog tdd(str, this);
    tdd.exec();
}

void PlotViewWidget::mouseDoubleClickEvent ( QMouseEvent *event )
{
    Q_UNUSED(event);
    if(maCurves.length() > 0)
	launchCurveSettings(0);
    else if(maSpectrograms.length() > 0)
	launchSpectrogramSettings(0);
}

void PlotViewWidget::launchCurveSettings(int index)
{
    if( index >= maCurves.count() ) { return; }
    CurveSettingsDialog cs(maCurves.at(index), mQwtPlot,this);
    cs.exec();
}

void PlotViewWidget::launchSpectrogramSettings(int index)
{
    if( index >= maSpectrograms.count() ) { return; }
    SpectrogramSettingsDialog ss(maSpectrograms.at(index), mQwtPlot, this);
    ss.exec();
}

QString PlotViewWidget::name() const
{
    return mLabel;
}

void PlotViewWidget::setName(QString n)
{
    mLabel = n;
}

QList<QwtPlotCurve*>* PlotViewWidget::curves()
{
    return &maCurves;
}

QList<QwtPlotSpectrogram*>* PlotViewWidget::spectrograms()
{
    return &maSpectrograms;
}

WaveformData* PlotViewWidget::curveData(int i)
{
    return maWaveformData.at(i);
}

SpectrogramData* PlotViewWidget::spectrogramData(int i)
{
    return maSpectrogramData.at(i);
}

QwtPlot* PlotViewWidget::plot()
{
    return mQwtPlot;
}

bool PlotViewWidget::hasSecondaryAxis()
{
    return mSecondaryAxis;
}

void PlotViewWidget::setHasSecondaryAxis(bool hasSecondaryAxis)
{
    mSecondaryAxis = hasSecondaryAxis;
}
