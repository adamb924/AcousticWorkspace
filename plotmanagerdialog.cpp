#include "plotmanagerdialog.h"

#include "plotviewwidget.h"
#include "waveformdata.h"
#include "spectrogramdata.h"

#include <qwt_scale_engine.h>
#include <qwt_plot.h>

#include <QtDebug>
#include <QtWidgets>
#include <QTreeWidgetItem>
#include <QStringList>
#include <QPushButton>
#include <QInputDialog>
#include <QStringList>

#include <QLayout>

PlotManagerDialog::PlotManagerDialog(QList<PlotViewWidget*> *pv, const QList<WaveformData*> *wfd, const QList<SpectrogramData*> *spd, QWidget *parent) :
    QDialog(parent)
{
    setSizeGripEnabled(true);

    aProsodyViews = pv;
    aWaveformData = wfd;
    aSpectrogramData = spd;

    QHBoxLayout *hlayout = new QHBoxLayout;

    pvt = new PlotViewTreeWidget(pv,this);
    pvt->setHeaderLabel(tr("Prosody Views"));
    pvt->populate();

    QPushButton *addPlot = new QPushButton(tr("Add a plot"),this);

    QVBoxLayout *vlayout = new QVBoxLayout;
    QVBoxLayout *vlayout2 = new QVBoxLayout;
    waveformTree = new DataSourceTreeWidget("waveform");
    spectrogramTree = new DataSourceTreeWidget("spectrogram");

    waveformTree->setContextMenuPolicy(Qt::NoContextMenu);
    spectrogramTree->setContextMenuPolicy(Qt::NoContextMenu);

    waveformTree->setHeaderLabel(tr("Waveforms"));
    waveformTree->setDragEnabled(true);
    for(int i=0; i<aWaveformData->count(); i++)
    {
	waveformTree->addTopLevelItem(new QTreeWidgetItem(QStringList(aWaveformData->at(i)->name()),i));
    }

    spectrogramTree->setHeaderLabel(tr("Spectrograms"));
    spectrogramTree->setDragEnabled(true);
    for(int i=0; i<aSpectrogramData->count(); i++)
    {
	spectrogramTree->addTopLevelItem(new QTreeWidgetItem(QStringList(aSpectrogramData->at(i)->name()),i));
    }

    vlayout->addWidget(waveformTree);
    vlayout->addWidget(spectrogramTree);
    vlayout2->addWidget(pvt);
    vlayout2->addWidget(addPlot);
    hlayout->addLayout(vlayout2);
    hlayout->addLayout(vlayout);
    this->setLayout(hlayout);

    connect(pvt,SIGNAL(addSpectrogram(int,int)),this,SLOT(addSpectrogram(int,int)));
    connect(pvt,SIGNAL(addWaveform(int,int)),this,SLOT(addWaveform(int,int)));

    connect(addPlot,SIGNAL(clicked()),this,SLOT(addPlot()));

    this->setWindowTitle("Acoustic Workspace Plot Manager");
}

void PlotManagerDialog::addWaveform(int plot, int waveform)
{
    WaveformData *tmp = aWaveformData->at(waveform);

    // if it doesn't have a secondary axis, just replot it
    if( !aProsodyViews->at(plot)->hasSecondaryAxis() )
    {
	aProsodyViews->at(plot)->addCurveData(tmp , false );
	aProsodyViews->at(plot)->plot()->replot();

	pvt->populate();
	return;
    }

    QRectF limits = tmp->boundingRect();
    if( !limits.isValid() )
    {
	qDebug() << "Somehow the boundingRect of the data is not valid, which is entirely unexpected.";
	return;
    }

    QString info = "The range of the waveform is [" + QString::number(limits.bottom()) + ", " + QString::number(limits.top()) + "].\nOn which axis should the waveform be plotted?";
    QString primary = "Primary Axis - [" + QString::number(aProsodyViews->at(plot)->plot()->axisScaleDiv(QwtPlot::yLeft).lowerBound()) + ", " + QString::number(aProsodyViews->at(plot)->plot()->axisScaleDiv(QwtPlot::yLeft).upperBound()) + "].";

    QString secondary;
    if( aProsodyViews->at(plot)->plot()->axisScaleDiv(QwtPlot::yRight).lowerBound() == 0 && aProsodyViews->at(plot)->plot()->axisScaleDiv(QwtPlot::yRight).upperBound() == 1000 )
	secondary = "Secondary Axis";
    else
    secondary = "Secondary Axis - [" + QString::number(aProsodyViews->at(plot)->plot()->axisScaleDiv(QwtPlot::yRight).lowerBound()) + ", " + QString::number(aProsodyViews->at(plot)->plot()->axisScaleDiv(QwtPlot::yRight).upperBound()) + "].";

    QStringList items;
    items << primary << secondary;

    bool ok;
    QString item = QInputDialog::getItem(this, tr("Adding waveform"),
					 info, items, 0, false, &ok);
    if (ok && !item.isEmpty())
    {
	if(item == primary)
	    aProsodyViews->at(plot)->addCurveData(tmp , false );
	else
	    aProsodyViews->at(plot)->addCurveData(tmp , true );

	aProsodyViews->at(plot)->plot()->replot();
	pvt->populate();
    }
}

void PlotManagerDialog::addSpectrogram(int plot, int spectrogram)
{
    aProsodyViews->at(plot)->addSpectrogramData( aSpectrogramData->at(spectrogram) );
    aProsodyViews->at(plot)->plot()->replot();
    pvt->populate();
}

void PlotManagerDialog::addPlot()
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("Enter a name for the new plot"),
					 tr("Enter a name for the new plot"), QLineEdit::Normal,
					 "", &ok);
    if (ok && !text.isEmpty())
    {
	emit addProsody(new PlotViewWidget(text),text);
	pvt->populate();
//	drawProsodyViewTree();
    }
}
