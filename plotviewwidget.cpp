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
    qwtPlot->setAxisScale((int)QwtPlot::xBottom,left,right,0.0f);
    qwtPlot->replot();
}

PlotViewWidget::PlotViewWidget(QString name, QWidget *parent) : QWidget(parent)
{
    label = name;

    widgetHeight = 200;

    hlayout = new QHBoxLayout(this);

    QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    secondaryAxis = false;

    qwtPlot = new QwtPlot(this);
    qwtPlot->setSizePolicy(sizePolicy);
    qwtPlot->setMinimumHeight(widgetHeight);
    qwtPlot->setMaximumHeight(1500);

    qwtPlot->enableAxis(QwtPlot::yLeft);
    qwtPlot->enableAxis(QwtPlot::yRight);

    int scaleWidth = 60;
    qwtPlot->axisScaleDraw(QwtPlot::yLeft)->setMinimumExtent( scaleWidth );
    qwtPlot->axisScaleDraw(QwtPlot::yRight)->setMinimumExtent( scaleWidth );

    hlayout->addWidget(qwtPlot);
}

PlotViewWidget::~PlotViewWidget()
{
    qDeleteAll(aCurves.begin(), aCurves.end());
}

QSize PlotViewWidget::sizeHint() const
{
    return QSize(750,widgetHeight);
}

void PlotViewWidget::addCurveData(WaveformData *curveData, bool secondary, QColor col)
{
    aWaveformData << curveData;

    QwtPlotCurve *waveCurve = new QwtPlotCurve("dummy");
    waveCurve->setRenderHint(QwtPlotItem::RenderAntialiased);
    waveCurve->setPen(QPen(col));
    waveCurve->setData( *curveData );
    aCurves << waveCurve;
    waveCurve->attach(qwtPlot);

    if( !secondaryAxis || aCurves.length() == 0 ) // no secondary axis or just one plot
    {
	qwtPlot->replot();
	qwtPlot->setAxisScaleDiv(QwtPlot::yRight, *qwtPlot->axisScaleDiv(QwtPlot::yLeft));
	qwtPlot->replot();
    }
    else
    {
	if(secondary)
	{
	    qwtPlot->setAxisAutoScale(QwtPlot::yRight);
	    waveCurve->setYAxis(QwtPlot::yRight);
	}
	else
	{
	    waveCurve->setYAxis(QwtPlot::yLeft);
	}
    }

    qwtPlot->repaint();
}

void PlotViewWidget::addSpectrogramData(SpectrogramData *spectrogramData)
{
    aSpectrogramData << spectrogramData;

    QwtPlotSpectrogram *spectrogram = new QwtPlotSpectrogram();

    QwtLinearColorMap colorMap(Qt::white, Qt::black);
    //    QwtLinearColorMap colorMap(Qt::black, Qt::white);
    spectrogram->setColorMap(colorMap);
    spectrogram->setData(*spectrogramData);

    QwtDoubleRect r = spectrogramData->boundingRect();
    qwtPlot->setAxisScale( QwtPlot::yLeft , 0, 5000, 1000);
    qwtPlot->setAxisScale( QwtPlot::yRight , 0, 5000, 1000);
    qwtPlot->setAxisScale( QwtPlot::xBottom , r.left(), r.right(), 0.1);

    aSpectrograms << spectrogram;
    spectrogram->attach(qwtPlot);
    qwtPlot->repaint();
}


void PlotViewWidget::removeItemAt(int i)
{
    aCurves.at(i)->detach();
    aCurves.removeAt(i);
    aWaveformData.removeAt(i);

    if( !secondaryAxis || aCurves.length() == 1 ) // no secondary axis or just one plot
    {
	qwtPlot->replot();
	qwtPlot->setAxisScaleDiv(QwtPlot::yRight, *qwtPlot->axisScaleDiv(QwtPlot::yLeft));
	qwtPlot->replot();
    }
}

void PlotViewWidget::toggleCurveAxisAssociation(int index)
{
    if(index >= aCurves.length()) { return; }
    if(aCurves.at(index)->yAxis() == QwtPlot::yLeft)
    {
	qwtPlot->setAxisAutoScale(QwtPlot::yRight);
	aCurves.at(index)->setYAxis(QwtPlot::yRight);
    }
    else
    {
	aCurves.at(index)->setYAxis(QwtPlot::yLeft);
    }
    qwtPlot->replot();
}

void PlotViewWidget::mouseMoveEvent ( QMouseEvent * event )
{
    if( event->pos().y() >= (this->rect().bottom()-2) || event->buttons() == Qt::LeftButton  )
    {
/*
	setCursor(QCursor(Qt::SizeVerCursor));
	if( event->buttons() == Qt::LeftButton )
	{
	    widgetHeight = event->pos().y();
	    resize( QSize(this->width(), widgetHeight ));
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
    resize( QSize(this->width(), height ));
}

inline QString countString(int i)
{
    return QString::number(i+1) + ". ";
}

void PlotViewWidget::contextMenuEvent ( QContextMenuEvent * event )
{
    QMenu menu(this);

    if( aCurves.count() > 0 )
    {
	QMenu *curveSettingsMenu = menu.addMenu("Curve settings");
	for(int i=0; i<aCurves.count(); i++)
	{
	    IndexedAction *tmp = new IndexedAction(countString(i)+aWaveformData.at(i)->name(),i,curveSettingsMenu);
	    curveSettingsMenu->addAction( tmp );
	    connect(tmp, SIGNAL(indexClicked(int)), this, SLOT(launchCurveSettings(int)) );
	}
    }

    if( aSpectrograms.count() > 0 )
    {
	QMenu *spectrogramSettingsMenu = menu.addMenu("Spectogram settings");
	for(int i=0; i<aCurves.count(); i++)
	{
	    IndexedAction *tmp = new IndexedAction(countString(i)+aSpectrogramData.at(i)->name(),i,spectrogramSettingsMenu);
	    spectrogramSettingsMenu->addAction( tmp );
	    connect(tmp, SIGNAL(indexClicked(int)), this, SLOT(launchSpectrogramSettings(int)) );
	}
    }

    if( aCurves.count() > 0 )
    {
	menu.addSeparator();

	QMenu *waveformTimesMenu = menu.addMenu("Curve times");
	for(int i=0; i<aCurves.count(); i++)
	{
	    IndexedAction *tmp = new IndexedAction(countString(i)+aWaveformData.at(i)->name(),i,waveformTimesMenu);
	    waveformTimesMenu->addAction( tmp );
	    connect(tmp, SIGNAL(indexClicked(int)), this, SLOT(showCurveTimes(int)) );
	}

	QMenu *allDataMenu = menu.addMenu("Curve data");
	for(int i=0; i<aCurves.count(); i++)
	{
	    IndexedAction *tmp = new IndexedAction(countString(i)+aWaveformData.at(i)->name(),i,allDataMenu);
	    allDataMenu->addAction( tmp );
	    connect(tmp, SIGNAL(indexClicked(int)), this, SLOT(showCurveTimesData(int)) );
	}
    }

    if( aSpectrograms.count() > 0 )
    {
	menu.addSeparator();

	QMenu *spectrogramTimeMenu = menu.addMenu("Spectrogram times");
	for(int i=0; i<aSpectrograms.count(); i++)
	{
	    IndexedAction *tmp = new IndexedAction(countString(i)+aSpectrogramData.at(i)->name(),i,spectrogramTimeMenu);
	    spectrogramTimeMenu->addAction( tmp );
	    connect(tmp, SIGNAL(indexClicked(int)), this, SLOT(showSpectrogramTimes(int)) );
	}

	QMenu *spectrogramFrequencyMenu = menu.addMenu("Spectrogram frequencies");
	for(int i=0; i<aSpectrograms.count(); i++)
	{
	    IndexedAction *tmp = new IndexedAction(countString(i)+aSpectrogramData.at(i)->name(),i,spectrogramFrequencyMenu);
	    spectrogramFrequencyMenu->addAction( tmp );
	    connect(tmp, SIGNAL(indexClicked(int)), this, SLOT(showSpectrogramFrequencies(int)) );
	}
    }

    menu.exec(event->globalPos());
}

void PlotViewWidget::showCurveTimes(int index)
{
    QString str;
    for(quint32 i=0; i<aWaveformData.at(index)->getNSamples(); i++)
    {
	str.append(QString::number(aWaveformData.at(index)->getTimeAtSample(i))+"\n");
    }
    TextDisplayDialog tdd(str, this);
    tdd.exec();
}

void PlotViewWidget::showCurveTimesData(int index)
{
    QString str;
    for(quint32 i=0; i<aWaveformData.at(index)->getNSamples(); i++)
    {
	str.append(QString::number(aWaveformData.at(index)->getTimeAtSample(i))+"\t"+QString::number(aWaveformData.at(index)->getYAtSample(i))+"\n");
    }
    TextDisplayDialog tdd(str, this);
    tdd.exec();
}

void PlotViewWidget::showSpectrogramTimes(int index)
{
    QString str;
    for(quint32 i=0; i<aSpectrogramData.at(index)->getNTimeSteps(); i++)
    {
	str.append(QString::number(aSpectrogramData.at(index)->getTimeFromIndex(i) )+"\n");
    }
    TextDisplayDialog tdd(str, this);
    tdd.exec();
}

void PlotViewWidget::showSpectrogramFrequencies(int index)
{
    QString str;
    for(quint32 i=0; i<aSpectrogramData.at(index)->getNFrequencyBins(); i++)
    {
	str.append(QString::number(aSpectrogramData.at(index)->getFrequencyFromIndex(i))+"\n");
    }
    TextDisplayDialog tdd(str, this);
    tdd.exec();
}

void PlotViewWidget::mouseDoubleClickEvent ( QMouseEvent *event )
{
    if(aCurves.length() > 0)
	launchCurveSettings(0);
    else if(aSpectrograms.length() > 0)
	launchSpectrogramSettings(0);
}

void PlotViewWidget::launchCurveSettings(int index)
{
    if( index >= aCurves.count() ) { return; }
    CurveSettingsDialog cs(aCurves.at(index), qwtPlot,this);
    cs.exec();
}

void PlotViewWidget::launchSpectrogramSettings(int index)
{
    if( index >= aSpectrograms.count() ) { return; }
    SpectrogramSettingsDialog ss(aSpectrograms.at(index), qwtPlot, this);
    ss.exec();
}

QString PlotViewWidget::name() const
{
    return label;
}

void PlotViewWidget::setName(QString n)
{
    label = n;
}

QList<QwtPlotCurve*>* PlotViewWidget::curves()
{
    return &aCurves;
}

QList<QwtPlotSpectrogram*>* PlotViewWidget::spectrograms()
{
    return &aSpectrograms;
}

WaveformData* PlotViewWidget::curveData(int i)
{
    return aWaveformData.at(i);
}

SpectrogramData* PlotViewWidget::spectrogramData(int i)
{
    return aSpectrogramData.at(i);
}

QwtPlot* PlotViewWidget::plot()
{
    return qwtPlot;
}

bool PlotViewWidget::hasSecondaryAxis()
{
    return secondaryAxis;
}

void PlotViewWidget::setHasSecondaryAxis(bool hasSecondaryAxis)
{
    secondaryAxis = hasSecondaryAxis;
}
