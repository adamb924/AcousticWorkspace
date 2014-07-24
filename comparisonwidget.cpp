#include "comparisonwidget.h"

#include "soundwidget.h"
#include "waveformdata.h"
#include "plotdisplayareawidget.h"
#include "plotviewwidget.h"
#include "intervalannotation.h"
#include "interval.h"
#include "intervaldisplaywidget.h"
#include "sound.h"

#include <QVBoxLayout>
#include <QInputDialog>
#include <QMenu>
#include <QContextMenuEvent>
#include <QDialog>
#include <QGridLayout>
#include <QtWidgets>

#include <QtDebug>

ComparisonWidget::ComparisonWidget(const Sound *primary, const QList<Sound*> *sounds, QWidget *parent) :
    QWidget(parent), mPrimary(primary), mSounds(sounds)
{
    colors << QColor(Qt::red) << QColor(Qt::green) << QColor(Qt::yellow) << QColor(Qt::cyan) << QColor(Qt::black);

    setMinimumSize(500,200);

    // skip the first because that's just the waveform
    for(int i = 1; i < mPrimary->waveformData()->count(); i++)
        mPrimaryCurves << new WaveformData( * (mPrimary->waveformData()->at(i)) );

    mDisplayWidget = new PlotDisplayAreaWidget;
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(mDisplayWidget);
    setLayout(layout);

    mPrimaryInterval = 0;
    if( mPrimary->intervals()->count() > 0 && QMessageBox::Yes == QMessageBox::question(this, tr("Acoustic Workspace"), tr("Will you be aligning the sounds with an interval annotation?"), QMessageBox::Yes | QMessageBox::No) )
    {
        // choose the intervals
        bool ok;
        QStringList items;
        for(int i=0; i< mPrimary->intervals()->count(); i++)
            items << mPrimary->intervals()->at(i)->mName;
        QString item = QInputDialog::getItem(this, tr("Acoustic Workspace"),tr("Choose the interval you want to use"), items, 0, false, &ok);
        if(!ok) { return; }
        mPrimaryInterval = mPrimary->intervals()->at( items.indexOf(item) );

        // choose the algorithm
        items.clear();
        items << "Linear" << "Accumulated Change";
        item = QInputDialog::getItem(this, tr("Acoustic Workspace"),tr("Choose the algorithm you want to use"), items, 0, false, &ok);
        if(!ok) { return; }
        mWarpAlgorithm = items.indexOf(item);

        if(mWarpAlgorithm==1)
        {
            items.clear();
            for(int i=0; i<mPrimary->waveformData()->count(); i++)
                items << mPrimary->waveformData()->at(i)->name();
            item = QInputDialog::getItem(this, tr("Acoustic Workspace"),tr("Choose the waveform that contains the spectral change information"), items, 0, false, &ok);
            if(!ok) { return; }
            mPrimaryChangeMetric = items.indexOf(item);
        }

        switch(mWarpAlgorithm)
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

    setWindowTitle(createWindowTitle());
}

ComparisonWidget::~ComparisonWidget()
{
    // quite a bit to do here
}

QString ComparisonWidget::createWindowTitle() const
{
    QString t;
    t = "Comparing (";
    t += mPrimary->name() + ", ";
    for(int i=0; i<mSounds->count(); i++)
        t += mSounds->at(i)->name() + ", ";
    t = t.trimmed() + ") with ";
    switch(mWarpAlgorithm)
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
    for(int i=0; i< mSounds->count(); i++)
        items << mSounds->at(i)->name();

    bool ok;
    QString item = QInputDialog::getItem(this, tr("Acoustic Workspace"),tr("Add a secondary sound for the comparison"), items, 0, false, &ok);
    if(!ok) { return; }
    int index = items.indexOf(item);

    if(index == -1) { return; }

    Sound *sec = mSounds->at(index);

    if(mPrimaryInterval != 0)
    {
        if(sec->intervals()->count() == 0)
        {
            QMessageBox::critical(this,tr("Error"),tr("You've chosen to align the sounds with an interval annotation, but there are no annotations for this sound."));
            return;
        }

        QStringList items;
        for(int i=0; i< sec->intervals()->count(); i++)
            items << sec->intervals()->at(i)->mName;

        bool ok;
        QString item = QInputDialog::getItem(this, tr("Acoustic Workspace"),tr("Choose the interval you want to use"), items, 0, false, &ok);

        if(!ok) { return; }
        IntervalAnnotation *interval = sec->intervals()->at( items.indexOf(item) );

        if( *(mPrimaryInterval) != *(interval) )
        {
            QMessageBox::critical(this,tr("Error"),tr("The labels of the interval you've chosen aren't exactly the same as those of the primary sound."));
            return;
        }

        mSecondaryIntervals << interval;
    }

    QList<QComboBox*> combos;
    QGridLayout *layout = new QGridLayout;
    layout->addWidget(new QLabel(tr("Select corresponding waveforms")),0,0,1,2);
    int i;
    for(i=0; i<mPrimary->waveformData()->count(); i++)
    {
        layout->addWidget(new QLabel(mPrimary->waveformData()->at(i)->name()),i+1,0);
        QStringList waveformNames;
        for(int j=0; j < sec->waveformData()->count(); j++)
        {
            waveformNames << sec->waveformData()->at(j)->name();
        }
        waveformNames << "<None>";
        QComboBox *combo = new QComboBox;
        combo->addItems(waveformNames);
        int comboIndex = waveformNames.indexOf(mPrimary->waveformData()->at(i)->name());
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
        if(ind == sec->waveformData()->count() )
            list << 0;
        else
            list << sec->waveformData()->at(ind);
    }

    //    qDebug() << list;

    if(mPrimaryInterval != 0 && mWarpAlgorithm == 1)
    {
        QStringList items;
        for(i=0; i<sec->waveformData()->count(); i++)
            items << sec->waveformData()->at(i)->name();
        item = QInputDialog::getItem(this, tr("Acoustic Workspace"),tr("Choose the waveform that contains the spectral change information"), items, 0, false, &ok);
        if(!ok) { return; }
        mSecondaryChangeMetrics << items.indexOf(item);
    }

    mSecondaryCurves << list;


    if(mPrimaryInterval != 0)
        switch(mWarpAlgorithm)
        {
        case 1:
            warpSecondaryCurvesAccumulated(mSecondaryCurves.count()-1);
            break;
        default:
            warpSecondaryCurvesLinear(mSecondaryCurves.count()-1);
            break;
        }

    drawCurves();
}

void ComparisonWidget::warpPrimaryCurvesLinear()
{
    for(int i=0; i<mPrimaryCurves.count(); i++) // for all curves
    {
        WaveformData *curve = mPrimaryCurves.at(i);

        double *newTimes = (double*)malloc(sizeof(double)*curve->size());

        for(int j=0; j<mPrimaryInterval->maIntervals.count(); j++) // for all intervals
        {
            int intervalLeftFrames = curve->getSampleFromTime( mPrimaryInterval->maIntervals.at(j)->mLeft );
            int intervalRightFrames = curve->getSampleFromTime( mPrimaryInterval->maIntervals.at(j)->mRight );
            double intervalLeftSeconds = mPrimaryInterval->maIntervals.at(j)->mLeft;

            double primaryLeftSeconds = curve->xData().at(intervalLeftFrames);
            double primaryLength = mPrimaryInterval->maIntervals.at(j)->mRight - mPrimaryInterval->maIntervals.at(j)->mLeft;
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
    for(int i=0; i<mSecondaryCurves.at(index).count(); i++)
    {
        WaveformData *curve = mSecondaryCurves.at(index).at(i);
        IntervalAnnotation *secondaryInterval = mSecondaryIntervals.at(index);
        if(curve==0) { continue;}

        double *newTimes = (double*)malloc(sizeof(double)*curve->size());
        // for all intervals
        for(int j=0; j<mPrimaryInterval->maIntervals.count(); j++)
        {
            double primaryIntervalLeftSeconds = mPrimaryInterval->maIntervals.at(j)->mLeft;

            size_t secondaryIntervalLeftFrames = curve->getSampleFromTime( secondaryInterval->maIntervals.at(j)->mLeft );
            size_t secondaryIntervalRightFrames = curve->getSampleFromTime( secondaryInterval->maIntervals.at(j)->mRight );

            double secondaryLeftSeconds = curve->xData().at(secondaryIntervalLeftFrames);
            double primaryIntervalLength = mPrimaryInterval->maIntervals.at(j)->mRight - mPrimaryInterval->maIntervals.at(j)->mLeft;
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
    for(int i=0; i<mPrimaryCurves.count(); i++)
    {
        WaveformData *curve = mPrimaryCurves.at(i);

        double *newTimes = (double*)malloc(sizeof(double)*curve->size());

        // for all intervals
        for(int j=0; j<mPrimaryInterval->maIntervals.count(); j++)
        {
            int intervalLeftFrames = curve->getSampleFromTime( mPrimaryInterval->maIntervals.at(j)->mLeft );
            int intervalRightFrames = curve->getSampleFromTime( mPrimaryInterval->maIntervals.at(j)->mRight );
            double intervalLeftSeconds = mPrimaryInterval->maIntervals.at(j)->mLeft;

            double *peChange = (double*)malloc(sizeof(double)*(intervalRightFrames-intervalLeftFrames+1));
            *(peChange+0) = mPrimary->waveformData()->at(mPrimaryChangeMetric)->yData().at(intervalLeftFrames);
            for(int k=intervalLeftFrames+1; k <= intervalRightFrames; k++)
                *(peChange+k-intervalLeftFrames) = *(peChange+k-intervalLeftFrames-1) + mPrimary->waveformData()->at(mPrimaryChangeMetric)->yData().at(k);

            for(int k=0; k < intervalRightFrames-intervalLeftFrames+1; k++)
                *(peChange+k) = *(peChange+k) / *(peChange+intervalRightFrames-intervalLeftFrames);


            double frameLengthSeconds = mPrimaryInterval->maIntervals.at(j)->mRight - mPrimaryInterval->maIntervals.at(j)->mLeft;

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
    for(int i=0; i<mSecondaryCurves.at(index).count(); i++)
    {
        WaveformData *curve = mSecondaryCurves.at(index).at(i);
        IntervalAnnotation *secondaryInterval = mSecondaryIntervals.at(index);
        if(curve==0) { continue;}

        double *newTimes = (double*)malloc(sizeof(double)*curve->size());

        // for all intervals
        for(int j=0; j<secondaryInterval->maIntervals.count(); j++)
        {
            int intervalLeftFrames = curve->getSampleFromTime( secondaryInterval->maIntervals.at(j)->mLeft );
            int intervalRightFrames = curve->getSampleFromTime( secondaryInterval->maIntervals.at(j)->mRight );
            double intervalLeftSeconds = mPrimaryInterval->maIntervals.at(j)->mLeft;

            double *peChange = (double*)malloc(sizeof(double)*(intervalRightFrames-intervalLeftFrames+1));
            *(peChange+0) = curve->yData().at(intervalLeftFrames);
            for(int k=intervalLeftFrames+1; k <= intervalRightFrames; k++)
                *(peChange+k-intervalLeftFrames) = *(peChange+k-intervalLeftFrames-1) + curve->yData().at(k);

            for(int k=0; k < intervalRightFrames-intervalLeftFrames+1; k++)
                *(peChange+k) = *(peChange+k) / *(peChange+intervalRightFrames-intervalLeftFrames);

            double frameLengthSeconds = mPrimaryInterval->maIntervals.at(j)->mRight - mPrimaryInterval->maIntervals.at(j)->mLeft;

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
    if(mDisplayWidget != 0) { delete mDisplayWidget; }
    mDisplayWidget = new PlotDisplayAreaWidget;

    for(int i=0; i< mPrimaryCurves.count(); i++) // for each of the curves in the primary
    {
        PlotViewWidget *tmp = new PlotViewWidget(mPrimaryCurves.at(i)->name());
        tmp->addCurveData(mPrimaryCurves.at(i),false,Qt::blue);

        for(int j=0; j < mSecondaryCurves.count(); j++) // for each secondary sound
            if( mSecondaryCurves.at(j).at(i) != 0 )
                tmp->addCurveData(mSecondaryCurves.at(j).at(i) ,false, colors.at(j % colors.length()));

        if(tmp->curves()->count() > 1)
            mDisplayWidget->addPlotView(tmp , "You were mistaken" );
        else
            delete tmp;
    }

    mDisplayWidget->setTimeMinMax( mPrimary->waveformData()->at(0)->tMin(), mPrimary->waveformData()->at(0)->tMax() );

    if(mPrimaryInterval != 0)
        mDisplayWidget->addAnnotation(new IntervalDisplayWidget(mPrimaryInterval,mDisplayWidget->plotViews()->first()));

    layout()->addWidget(mDisplayWidget);
}
