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
    QDialog(parent), maProsodyViews(pv), maWaveformData(wfd), maSpectrogramData(spd)
{
    setSizeGripEnabled(true);

    QHBoxLayout *hlayout = new QHBoxLayout;

    mPvt = new PlotViewTreeWidget(pv,this);
    mPvt->setHeaderLabel(tr("Prosody Views"));
    mPvt->populate();

    QPushButton *addPlot = new QPushButton(tr("Add a plot"),this);

    QVBoxLayout *vlayout = new QVBoxLayout;
    QVBoxLayout *vlayout2 = new QVBoxLayout;
    mWaveformTree = new DataSourceTreeWidget("waveform");
    mSpectrogramTree = new DataSourceTreeWidget("spectrogram");

    mWaveformTree->setContextMenuPolicy(Qt::NoContextMenu);
    mSpectrogramTree->setContextMenuPolicy(Qt::NoContextMenu);

    mWaveformTree->setHeaderLabel(tr("Waveforms"));
    mWaveformTree->setDragEnabled(true);
    for(int i=0; i<maWaveformData->count(); i++)
    {
	mWaveformTree->addTopLevelItem(new QTreeWidgetItem(QStringList(maWaveformData->at(i)->name()),i));
    }

    mSpectrogramTree->setHeaderLabel(tr("Spectrograms"));
    mSpectrogramTree->setDragEnabled(true);
    for(int i=0; i<maSpectrogramData->count(); i++)
    {
	mSpectrogramTree->addTopLevelItem(new QTreeWidgetItem(QStringList(maSpectrogramData->at(i)->name()),i));
    }

    vlayout->addWidget(mWaveformTree);
    vlayout->addWidget(mSpectrogramTree);
    vlayout2->addWidget(mPvt);
    vlayout2->addWidget(addPlot);
    hlayout->addLayout(vlayout2);
    hlayout->addLayout(vlayout);
    setLayout(hlayout);

    connect(mPvt,SIGNAL(addSpectrogram(int,int)),this,SLOT(addSpectrogram(int,int)));
    connect(mPvt,SIGNAL(addWaveform(int,int)),this,SLOT(addWaveform(int,int)));

    connect(addPlot,SIGNAL(clicked()),this,SLOT(addPlot()));

    setWindowTitle("Acoustic Workspace Plot Manager");
}

void PlotManagerDialog::addWaveform(int plot, int waveform)
{
    WaveformData *tmp = maWaveformData->at(waveform);

    // if it doesn't have a secondary axis, just replot it
    if( !maProsodyViews->at(plot)->hasSecondaryAxis() )
    {
	maProsodyViews->at(plot)->addCurveData(tmp , false );
    maProsodyViews->at(plot)->replot();

	mPvt->populate();
	return;
    }

    QRectF limits = tmp->boundingRect();
    if( !limits.isValid() )
    {
	qDebug() << "Somehow the boundingRect of the data is not valid, which is entirely unexpected.";
	return;
    }

    QString info = "The range of the waveform is [" + QString::number(limits.bottom()) + ", " + QString::number(limits.top()) + "].\nOn which axis should the waveform be plotted?";
    QString primary = "Primary Axis - [" + QString::number(maProsodyViews->at(plot)->axisScaleDiv(QwtPlot::yLeft).lowerBound()) + ", " + QString::number(maProsodyViews->at(plot)->axisScaleDiv(QwtPlot::yLeft).upperBound()) + "].";

    QString secondary;
    if( maProsodyViews->at(plot)->axisScaleDiv(QwtPlot::yRight).lowerBound() == 0 && maProsodyViews->at(plot)->axisScaleDiv(QwtPlot::yRight).upperBound() == 1000 )
	secondary = "Secondary Axis";
    else
    secondary = "Secondary Axis - [" + QString::number(maProsodyViews->at(plot)->axisScaleDiv(QwtPlot::yRight).lowerBound()) + ", " + QString::number(maProsodyViews->at(plot)->axisScaleDiv(QwtPlot::yRight).upperBound()) + "].";

    QStringList items;
    items << primary << secondary;

    bool ok;
    QString item = QInputDialog::getItem(this, tr("Adding waveform"),
					 info, items, 0, false, &ok);
    if (ok && !item.isEmpty())
    {
	if(item == primary)
	    maProsodyViews->at(plot)->addCurveData(tmp , false );
	else
	    maProsodyViews->at(plot)->addCurveData(tmp , true );

    maProsodyViews->at(plot)->replot();
	mPvt->populate();
    }
}

void PlotManagerDialog::addSpectrogram(int plot, int spectrogram)
{
    maProsodyViews->at(plot)->addSpectrogramData( maSpectrogramData->at(spectrogram) );
    maProsodyViews->at(plot)->replot();
    mPvt->populate();
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
	mPvt->populate();
//	drawProsodyViewTree();
    }
}
