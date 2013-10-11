#include "comparisonwidget.h"

#include "soundwidget.h"
#include "waveformdata.h"
#include "plotdisplayareawidget.h"
#include "plotviewwidget.h"
#include "intervalannotation.h"
#include "intervaldisplaywidget.h"

#include <QVBoxLayout>
#include <QInputDialog>
#include <QMenu>
#include <QContextMenuEvent>
#include <QDialog>
#include <QGridLayout>
#include <QtWidgets>

#include <QtDebug>

ComparisonWidget::ComparisonWidget(const SoundWidget *primary, const QList<SoundWidget*> *sounds, QWidget *parent) :
	QWidget(parent)
{
    colors << QColor(Qt::red) << QColor(Qt::green) << QColor(Qt::yellow) << QColor(Qt::cyan) << QColor(Qt::black);

    this->setMinimumSize(500,200);

    this->primary = primary;
    this->sounds = sounds;

    // skip the first because that's just the waveform
    for(int i = 1; i < primary->aWaveformData.count(); i++)
        primaryCurves << new WaveformData( * (primary->aWaveformData.at(i)) );

    displayWidget = new PlotDisplayAreaWidget;
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(displayWidget);
    this->setLayout(layout);

    primaryInterval = 0;
    if( primary->aIntervalAnnotations.count() > 0 && QMessageBox::Yes == QMessageBox::question(this, tr("Acoustic Workspace"), tr("Will you be aligning the sounds with an interval annotation?"), QMessageBox::Yes | QMessageBox::No) )
    {
	// choose the intervals
	bool ok;
	QStringList items;
	for(int i=0; i< primary->aIntervalAnnotations.count(); i++)
	    items << primary->aIntervalAnnotations.at(i)->name;
	QString item = QInputDialog::getItem(this, tr("Acoustic Workspace"),tr("Choose the interval you want to use"), items, 0, false, &ok);
	if(!ok) { return; }
	primaryInterval = primary->aIntervalAnnotations.at( items.indexOf(item) );

	// choose the algorithm
	items.clear();
	items << "Linear" << "Accumulated Change";
	item = QInputDialog::getItem(this, tr("Acoustic Workspace"),tr("Choose the algorithm you want to use"), items, 0, false, &ok);
	if(!ok) { return; }
	warpAlgorithm = items.indexOf(item);

	if(warpAlgorithm==1)
	{
	    items.clear();
	    for(int i=0; i<primary->aWaveformData.count(); i++)
		items << primary->aWaveformData.at(i)->name();
	    item = QInputDialog::getItem(this, tr("Acoustic Workspace"),tr("Choose the waveform that contains the spectral change information"), items, 0, false, &ok);
	    if(!ok) { return; }
	    primaryChangeMetric = items.indexOf(item);
	}

	switch(warpAlgorithm)
	{
	case 1:
	    warpPrimaryCurvesAccumulated();
	    break;
	default:
	    warpPrimaryCurvesLinear();
	    break;
	}

	addSecondary();

	drawCurves();
    }

    this->setWindowTitle(createWindowTitle());
}

ComparisonWidget::~ComparisonWidget()
{
    // quite a bit to do here
}

QString ComparisonWidget::createWindowTitle() const
{
    QString t;
    t = "Comparing (";
    t += primary->windowTitle() + ", ";
    for(int i=0; i<sounds->count(); i++)
	t += sounds->at(i)->windowTitle() + ", ";
    t = t.trimmed() + ") with ";
    switch(warpAlgorithm)
    {
    case 1:
	t += "Accumulated change algorithm";
	break;
    default:
	t += "Linear warp";
	break;
    }
    return t;
}

void ComparisonWidget::contextMenuEvent(QContextMenuEvent *e)
{
    QAction *addSecondary = new QAction(tr("Add a secondary sound"),this);

    connect(addSecondary,SIGNAL(triggered()),this,SLOT(addSecondary()));

    QMenu menu;
    menu.addAction(addSecondary);
    menu.exec(e->globalPos());
}

void  ComparisonWidget::addSecondary()
{
    QStringList items;
    for(int i=0; i< sounds->count(); i++)
	items << sounds->at(i)->windowTitle();

    bool ok;
    QString item = QInputDialog::getItem(this, tr("Acoustic Workspace"),tr("Add a secondary sound for the comparison"), items, 0, false, &ok);
    if(!ok) { return; }
    int index = items.indexOf(item);

    if(index == -1) { return; }

    SoundWidget *sec = sounds->at(index);

    if(primaryInterval != 0)
    {
	if(sec->aIntervalAnnotations.count() == 0)
	{
	    QMessageBox::critical(this,tr("Error"),tr("You've chosen to align the sounds with an interval annotation, but there are no annotations for this sound."));
	    return;
	}

	QStringList items;
	for(int i=0; i< sec->aIntervalAnnotations.count(); i++)
	    items << sec->aIntervalAnnotations.at(i)->name;

	bool ok;
	QString item = QInputDialog::getItem(this, tr("Acoustic Workspace"),tr("Choose the interval you want to use"), items, 0, false, &ok);

	if(!ok) { return; }
	IntervalAnnotation *interval = sec->aIntervalAnnotations.at( items.indexOf(item) );

	if( *(primaryInterval) != *(interval) )
	{
	    QMessageBox::critical(this,tr("Error"),tr("The labels of the interval you've chosen aren't exactly the same as those of the primary sound."));
	    return;
	}

	secondaryIntervals << interval;
    }

    QList<QComboBox*> combos;
    QGridLayout *layout = new QGridLayout;
    layout->addWidget(new QLabel(tr("Select corresponding waveforms")),0,0,1,2);
    int i;
    for(i=0; i<primary->aWaveformData.count(); i++)
    {
	layout->addWidget(new QLabel(primary->aWaveformData.at(i)->name()),i+1,0);
	QStringList waveformNames;
	for(int j=0; j < sec->aWaveformData.count(); j++)
	    waveformNames << sec->aWaveformData.at(j)->name();
	waveformNames << "<None>";
	QComboBox *combo = new QComboBox;
	combo->addItems(waveformNames);
	int comboIndex = waveformNames.indexOf(primary->aWaveformData.at(i)->name());
	if(comboIndex==-1)
	    combo->setCurrentIndex(waveformNames.count()-1);
	else
	    combo->setCurrentIndex(comboIndex);
	combos << combo;
	layout->addWidget(combo,i+1,1);
    }
    QDialog *dlg = new QDialog;

    QPushButton *bOk = new QPushButton(tr("OK"));
    QPushButton *cancel = new QPushButton(tr("Cancel"));
    layout->addWidget(bOk,i+1,0);
    layout->addWidget(cancel,i+1,1);
    connect(bOk,SIGNAL(clicked()),dlg,SLOT(accept()));
    connect(cancel,SIGNAL(clicked()),dlg,SLOT(reject()));
    dlg->setLayout(layout);
    dlg->exec();
    if(dlg->result() == QDialog::Rejected) { return; }

    QList<WaveformData*> list;
    // skip the first since it's the waveform
    for(i=1; i< combos.count(); i++)
    {
	int ind = combos.at(i)->currentIndex();
	if(ind == sec->aWaveformData.count() )
	    list << 0;
	else
	    list << sec->aWaveformData.at(ind);
    }

//    qDebug() << list;

    if(primaryInterval != 0 && warpAlgorithm == 1)
    {
	QStringList items;
	for(i=0; i<sec->aWaveformData.count(); i++)
	    items << sec->aWaveformData.at(i)->name();
	item = QInputDialog::getItem(this, tr("Acoustic Workspace"),tr("Choose the waveform that contains the spectral change information"), items, 0, false, &ok);
	if(!ok) { return; }
	secondaryChangeMetrics << items.indexOf(item);
    }

    secondaryCurves << list;


    if(primaryInterval != 0)
	switch(warpAlgorithm)
	{
	case 1:
	    warpSecondaryCurvesAccumulated(secondaryCurves.count()-1);
	    break;
	default:
	    warpSecondaryCurvesLinear(secondaryCurves.count()-1);
	    break;
	}

    drawCurves();
}

void ComparisonWidget::warpPrimaryCurvesLinear()
{
    for(int i=0; i<primaryCurves.count(); i++) // for all curves
    {
	WaveformData *curve = primaryCurves.at(i);

	double *newTimes = (double*)malloc(sizeof(double)*curve->size());

	for(int j=0; j<primaryInterval->aIntervals.count(); j++) // for all intervals
	{
	    int intervalLeftFrames = curve->getSampleFromTime( primaryInterval->aIntervals.at(j)->left );
	    int intervalRightFrames = curve->getSampleFromTime( primaryInterval->aIntervals.at(j)->right );
	    double intervalLeftSeconds = primaryInterval->aIntervals.at(j)->left;

        double primaryLeftSeconds = curve->xData().at(intervalLeftFrames);
	    double primaryLength = primaryInterval->aIntervals.at(j)->right - primaryInterval->aIntervals.at(j)->left;
        double frameLengthSeconds = curve->xData().at(intervalRightFrames) - curve->xData().at(intervalLeftFrames);

	    for(int k=intervalLeftFrames; k <= intervalRightFrames; k++)
        *(newTimes+k) = intervalLeftSeconds + (curve->xData().at(k)-primaryLeftSeconds)*(primaryLength/frameLengthSeconds);
	}
	curve->setXData(newTimes);
    }
}

void ComparisonWidget::warpSecondaryCurvesLinear(int index)
{
//    qDebug() << "ComparisonWidget::warpSecondaryCurvesLinear";

    // for all curves
    for(int i=0; i<secondaryCurves.at(index).count(); i++)
    {
	WaveformData *curve = secondaryCurves.at(index).at(i);
	IntervalAnnotation *secondaryInterval = secondaryIntervals.at(index);
	if(curve==0) { continue;}

	double *newTimes = (double*)malloc(sizeof(double)*curve->size());
	// for all intervals
	for(int j=0; j<primaryInterval->aIntervals.count(); j++)
	{
	    double primaryIntervalLeftSeconds = primaryInterval->aIntervals.at(j)->left;

	    size_t secondaryIntervalLeftFrames = curve->getSampleFromTime( secondaryInterval->aIntervals.at(j)->left );
	    size_t secondaryIntervalRightFrames = curve->getSampleFromTime( secondaryInterval->aIntervals.at(j)->right );

        double secondaryLeftSeconds = curve->xData().at(secondaryIntervalLeftFrames);
	    double primaryIntervalLength = primaryInterval->aIntervals.at(j)->right - primaryInterval->aIntervals.at(j)->left;
        double frameLengthSeconds = curve->xData().at(secondaryIntervalRightFrames) - curve->xData().at(secondaryIntervalLeftFrames);

	    for(size_t k=secondaryIntervalLeftFrames; k <= secondaryIntervalRightFrames; k++)
        *(newTimes + k) = primaryIntervalLeftSeconds + (curve->xData().at(k)-secondaryLeftSeconds)*(primaryIntervalLength/frameLengthSeconds);
	}

	curve->setXData(newTimes);
    }
}

void ComparisonWidget::warpPrimaryCurvesAccumulated()
{
    qDebug() << "ComparisonWidget::warpPrimaryCurvesAccumulated";

    // for all curves
    for(int i=0; i<primaryCurves.count(); i++)
    {
	WaveformData *curve = primaryCurves.at(i);

	double *newTimes = (double*)malloc(sizeof(double)*curve->size());

	// for all intervals
	for(int j=0; j<primaryInterval->aIntervals.count(); j++)
	{
	    int intervalLeftFrames = curve->getSampleFromTime( primaryInterval->aIntervals.at(j)->left );
	    int intervalRightFrames = curve->getSampleFromTime( primaryInterval->aIntervals.at(j)->right );
	    double intervalLeftSeconds = primaryInterval->aIntervals.at(j)->left;

	    double *peChange = (double*)malloc(sizeof(double)*(intervalRightFrames-intervalLeftFrames+1));
        *(peChange+0) = primary->aWaveformData.at(primaryChangeMetric)->yData().at(intervalLeftFrames);
	    for(int k=intervalLeftFrames+1; k <= intervalRightFrames; k++)
        *(peChange+k-intervalLeftFrames) = *(peChange+k-intervalLeftFrames-1) + primary->aWaveformData.at(primaryChangeMetric)->yData().at(k);

	    for(int k=0; k < intervalRightFrames-intervalLeftFrames+1; k++)
		*(peChange+k) = *(peChange+k) / *(peChange+intervalRightFrames-intervalLeftFrames);


	    double frameLengthSeconds = primaryInterval->aIntervals.at(j)->right - primaryInterval->aIntervals.at(j)->left;

	    for(int k=intervalLeftFrames; k <= intervalRightFrames; k++)
	    {
		*(newTimes+k) = intervalLeftSeconds + (*(peChange+k-intervalLeftFrames)*frameLengthSeconds);
//		qDebug() << k << intervalLeftSeconds + (*(peChange+k-intervalLeftFrames)*frameLengthSeconds);
//		curve->setXDataAt(k, intervalLeftSeconds + (*(peChange+k-intervalLeftFrames)*frameLengthSeconds) );
	    }
	    free(peChange);
	}
	curve->setXData(newTimes);

	for(size_t j=0; j<curve->size(); j++)
        qDebug() << j << curve->xData().at(j);
    }
}

void ComparisonWidget::warpSecondaryCurvesAccumulated(int index)
{
    qDebug() << "ComparisonWidget::warpSecondaryCurvesAccumulated";

    // for all curves
    for(int i=0; i<secondaryCurves.at(index).count(); i++)
    {
	WaveformData *curve = secondaryCurves.at(index).at(i);
	IntervalAnnotation *secondaryInterval = secondaryIntervals.at(index);
	if(curve==0) { continue;}

	double *newTimes = (double*)malloc(sizeof(double)*curve->size());

	// for all intervals
	for(int j=0; j<secondaryInterval->aIntervals.count(); j++)
	{
	    int intervalLeftFrames = curve->getSampleFromTime( secondaryInterval->aIntervals.at(j)->left );
	    int intervalRightFrames = curve->getSampleFromTime( secondaryInterval->aIntervals.at(j)->right );
	    double intervalLeftSeconds = primaryInterval->aIntervals.at(j)->left;

	    double *peChange = (double*)malloc(sizeof(double)*(intervalRightFrames-intervalLeftFrames+1));
        *(peChange+0) = curve->yData().at(intervalLeftFrames);
	    for(int k=intervalLeftFrames+1; k <= intervalRightFrames; k++)
        *(peChange+k-intervalLeftFrames) = *(peChange+k-intervalLeftFrames-1) + curve->yData().at(k);

	    for(int k=0; k < intervalRightFrames-intervalLeftFrames+1; k++)
		*(peChange+k) = *(peChange+k) / *(peChange+intervalRightFrames-intervalLeftFrames);

	    double frameLengthSeconds = primaryInterval->aIntervals.at(j)->right - primaryInterval->aIntervals.at(j)->left;

	    for(int k=intervalLeftFrames; k <= intervalRightFrames; k++)
	    {
		*(newTimes+k) = intervalLeftSeconds + (*(peChange+k-intervalLeftFrames)*frameLengthSeconds);
	    }
	    free(peChange);
	}
	curve->setXData(newTimes);

	for(size_t j=0; j<curve->size(); j++)
        qDebug() << j << curve->xData().at(j);
    }
}

void ComparisonWidget::drawCurves()
{
    if(displayWidget != 0) { delete displayWidget; }
    displayWidget = new PlotDisplayAreaWidget;

    for(int i=0; i< primaryCurves.count(); i++) // for each of the curves in the primary
    {
        PlotViewWidget *tmp = new PlotViewWidget(primaryCurves.at(i)->name());
	tmp->addCurveData(primaryCurves.at(i),false,Qt::blue);

	for(int j=0; j < secondaryCurves.count(); j++) // for each secondary sound
	    if( secondaryCurves.at(j).at(i) != 0 )
		tmp->addCurveData(secondaryCurves.at(j).at(i) ,false, colors.at(j % colors.length()));

	if(tmp->curves()->count() > 1)
            displayWidget->addPlotView(tmp , "You were mistaken" );
	else
	    delete tmp;
    }

    displayWidget->setTimeMinMax( primary->aWaveformData.at(0)->tMin(), primary->aWaveformData.at(0)->tMax() );

    if(primaryInterval != 0)
	displayWidget->addAnnotation(new IntervalDisplayWidget(primaryInterval,displayWidget->plotViews()->first()));

    this->layout()->addWidget(displayWidget);
}
