#include "soundwidget.h"
#include "ui_soundwidget.h"

#include <qwt_plot.h>
#include <qwt_plot_marker.h>
#include <qwt_plot_curve.h>
#include <qwt_legend.h>
#include <qwt_series_data.h>
#include <qwt_text.h>
#include <qwt_math.h>
#include <qwt_color_map.h>
#include <qwt_scale_engine.h>
#include <qwt_text_label.h>
#include <qwt_slider.h>
#include <qwt_symbol.h>
#include <qwt_plot_curve.h>
#include <qwt_scale_engine.h>

#include <QtWidgets>
#include <QString>
#include <QStringList>
#include <QtScript>
#include <QObject>

#include "intervaldisplaywidget.h"
#include "waveformdata.h"
#include "regressiondialog.h"
#include "spectrogramdata.h"
#include "plotviewwidget.h"
#include "interfaces.h"
#include "mainwindow.h"
#include "plotmanagerdialog.h"
#include "datamanagerdialog.h"
#include "plotdisplayareawidget.h"
#include "intervalannotation.h"
#include "sound.h"
#include "soundview.h"

SoundWidget::SoundWidget(Sound * snd, QList<AbstractWaveform2WaveformMeasure*> *w2w, QList<AbstractWaveform2SpectrogramMeasure*> *w2s, QList<AbstractSpectrogram2WaveformMeasure*> *s2w, QList<AbstractSpectrogram2SpectrogramMeasure*> *s2s, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SoundWidget),
    mSound(snd),
    mScriptEngine(0),
    mW2wPlugins(w2w),
    mW2sPlugins(w2s),
    mS2wPlugins(s2w),
    mS2sPlugins(s2s)
{
    ui->setupUi(this);

    connect( ui->actionData_Manager, SIGNAL(triggered()), this, SLOT(launchDataManager()) );
    connect( ui->actionPlot_Manager, SIGNAL(triggered()), this, SLOT(launchPlotManager()) );

    /// @todo Look at how much of this functionality needs to be replaced
//    mMenuBar = new QMenuBar(this);
//    layout->setMenuBar(mMenuBar);
//    setupMenus();
//    setupActions();
//    setupScripting();

    setupLayout();

    setWindowTitle(mSound->name());
}

SoundWidget::~SoundWidget()
{
    if(mScriptEngine != 0)
        delete mScriptEngine;
}

Sound *SoundWidget::sound()
{
    return mSound;
}

void SoundWidget::setupLayout()
{
    const SoundView * v = mSound->soundView();
    ui->plotDisplayWidget->setTimeMinMax( v->tMin(), v->tMax() );
    ui->plotDisplayWidget->setTimeAxes( v->leftPos(), v->rightPos() );

    for(int i=0; i<v->plotParameters()->count(); i++)
    {
        PlotParameters *p = v->plotParameters()->at(i);
        PlotViewWidget *pvw = new PlotViewWidget( p->name() );
        pvw->setHasSecondaryAxis( p->hasSecondaryAxis() );
        pvw->setHeight( p->height() );

        // add the curves
        for(int j=0; j<p->curveParameters()->count(); j++)
        {
            CurveParameters *cp = p->curveParameters()->at(j);
            QwtPlotCurve * curve = pvw->addCurveData( cp->waveformData(), cp->isSecondary() );
            curve->setPen( cp->pen() );
            curve->setStyle( cp->curveStyle() );
            curve->setRenderHint(QwtPlotItem::RenderAntialiased, cp->antialiased() );

            QwtSymbol * sym = new QwtSymbol;
            sym->setBrush(cp->symbolBrush());
            sym->setPen(cp->symbolPen());
            sym->setSize(cp->symbolSize());
            sym->setStyle(cp->symbolStyle());
            curve->setSymbol(sym);
        }

        for(int j=0; j<p->spectrogramParameters()->count(); j++)
        {
            SpectrogramParameters *sp = p->spectrogramParameters()->at(j);
            QwtPlotSpectrogram * spectrogram = pvw->addSpectrogramData( sp->spectrogramData() );
            QwtLinearScaleEngine engine;
            spectrogram->plot()->setAxisScaleDiv(QwtPlot::yLeft, QwtScaleDiv( engine.divideScale( sp->lowerBound(), sp->upperBound(),10, 10) ));
        }

        ui->plotDisplayWidget->addPlotView( pvw , p->name() );
    }

    for(int i=0; i< mSound->intervals()->count(); i++)
    {
        ui->plotDisplayWidget->addAnnotation(new IntervalDisplayWidget(mSound->intervals()->at(i),ui->plotDisplayWidget->plotViews()->first(),this));
        addAnnotationMenu(mSound->intervals()->at(i));
    }
}

// Private slots

void SoundWidget::addAnnotationMenu(IntervalAnnotation *annotation)
{
    QMenu *tmp = ui->menuAnnotations->addMenu(annotation->mName);
    QAction *visible = new QAction(tr("Visible"),tmp);
    visible->setCheckable(true);
    visible->setChecked(true);
    visible->setData(0);

    QAction *deleteAction = new QAction(tr("Delete"),tmp);
    deleteAction->setData(1000);

    tmp->addAction(visible);
    tmp->addAction(deleteAction);
    maAnnotationMenus << tmp;

    connect(tmp,SIGNAL(triggered(QAction*)),this,SLOT(annotationMenuAction(QAction*)));
}

void SoundWidget::launchDataManager()
{
    DataManagerDialog *dm = new DataManagerDialog(mW2wPlugins, mW2sPlugins, mS2wPlugins, mS2sPlugins, mSound->waveformData(),mSound->spectrogramData(),this);
    connect(dm, SIGNAL(removeWaveform(int)),this,SLOT(removeWaveform(int)));
    connect(dm, SIGNAL(removeSpectrogram(int)),this,SLOT(removeSpectrogram(int)));
    dm->exec();
}

void SoundWidget::launchPlotManager()
{
    PlotManagerDialog pm(ui->plotDisplayWidget->plotViews(),mSound->waveformData(),mSound->spectrogramData());
    connect(&pm,SIGNAL(addProsody(PlotViewWidget*,QString)),ui->plotDisplayWidget,SLOT(addPlotView(PlotViewWidget*,QString)));
    pm.exec();
    disconnect(&pm,SIGNAL(addProsody(PlotViewWidget*,QString)),ui->plotDisplayWidget,SLOT(addPlotView(PlotViewWidget*,QString)));

//    qDebug() << "end of SoundWidget::launchPlotManager";
}

void SoundWidget::importTextGrid()
{
    /// @todo replace this functionality
//    if( ui->plotDisplayWidget->plotViews()->count() <1 )
//    {
//	QMessageBox::critical(this,tr("Error"),tr("You need at least one plot window to import an annotation."));
//	return;
//    }

//    QString fileName;
//    fileName= QFileDialog::getOpenFileName(this, tr("Open TextGrid"), "", tr("TextGrid files (*.*)"));
//    if(!fileName.isNull())
//    {
//	QFileInfo info(fileName);
//	if(!info.exists())
//	{
//	    QMessageBox::critical(0,"Error","The file "+fileName+" does not exist.");
//	    return;
//	}
//	readTextGridFromFile(fileName);
//    }
}

void SoundWidget::newRegression()
{
    RegressionDialog *rd = new RegressionDialog(mSound->waveformData(),mSound->spectrogramData(),this);
    connect(rd,SIGNAL(regressionObject(RegressionModel*)),mSound,SLOT(addRegression(RegressionModel*)));
    rd->exec();
}

// private methods

void SoundWidget::setupActions()
{
    mOpenProjectAction = new QAction(tr("Open Sound"),this);
    mOpenProjectAction->setShortcut(QKeySequence("Ctrl+O"));

    mDataManagerAction = new QAction(tr("Data Manager"),this);
    mDataManagerAction->setShortcut(QKeySequence("Ctrl+D"));
    connect(mDataManagerAction,SIGNAL(triggered()),this,SLOT(launchDataManager()));

    mPlotManagerAction = new QAction(tr("Plot Manager"),this);
    mPlotManagerAction->setShortcut(QKeySequence("Ctrl+M"));
    connect(mPlotManagerAction,SIGNAL(triggered()),this,SLOT(launchPlotManager()));

    mNewSoundAction = new QAction(tr("New Sound (new window)"),this);
    mNewSoundAction->setShortcut(QKeySequence("Ctrl+N"));
    //    MdiArea* mdi = qobject_cast<MdiArea*>(mainWnd->centralWidget());
    //    qDebug() << mainWnd->mdi();
    //    connect(newSoundAction,SIGNAL(triggered()),mainWnd->mdi(),SLOT(newSoundWindow()));

    mNewRegressionAction = new QAction(tr("New regression"),this);
    connect(mNewRegressionAction,SIGNAL(triggered()),this,SLOT(newRegression()));
    mNewRegressionAction->setShortcut(QKeySequence("Ctrl+R"));

    mImportTextGridAction = new QAction(tr("Import TextGrid"),this);
    connect(mImportTextGridAction,SIGNAL(triggered()),this,SLOT(importTextGrid()));
    mImportTextGridAction->setShortcut(QKeySequence("Ctrl+T"));

    mFileMenu->addAction(mOpenProjectAction);
    mFileMenu->addAction(mSaveProjectAction);
    mFileMenu->addAction(mSaveProjectAsAction);
    mFileMenu->addAction(mCloseAction);
    mFileMenu->addSeparator();
    mFileMenu->addAction(mImportSoundAction);
    mFileMenu->addAction(mImportTextGridAction);
    //    fileMenu->addSeparator();
    //    fileMenu->addAction(newSoundAction);

    mOptionsMenu->addAction(mDataManagerAction);
    mOptionsMenu->addAction(mPlotManagerAction);
    mOptionsMenu->addSeparator();

    mVisibilityMenu->addSeparator();

    mRegressionMenu->addAction(mNewRegressionAction);
    mRegressionMenu->addSeparator();

    QAction* runScriptAction = new QAction(tr("Run script..."),this);
    mScriptingMenu->addAction(runScriptAction);
    connect(runScriptAction,SIGNAL(triggered()),this,SLOT(runScript()));
}

void SoundWidget::setupMenus()
{
    mFileMenu = mMenuBar->addMenu(tr("File"));
    mOptionsMenu = mMenuBar->addMenu(tr("Options"));
    mVisibilityMenu = mMenuBar->addMenu(tr("Display"));
    mVisibilityMenu->setEnabled(false);
    mRegressionMenu = mMenuBar->addMenu(tr("Regressions"));
    mScriptingMenu = mMenuBar->addMenu(tr("Scripting"));
}

void SoundWidget::setupScripting()
{
    mScriptEngine = new QScriptEngine;

    // set up the connections to receive spectrograms & waveforms that plugins emit
    // this is, technically, broader than just the scripting environment, but I want to make sure that this connection is made before the others
    for(int i=0; i < mW2sPlugins->count(); i++ )
    connect( mW2sPlugins->at(i), SIGNAL(spectrogramCreated(SpectrogramData*)), this, SLOT(addSpectrogram(SpectrogramData*)) );
    for(int i=0; i < mS2sPlugins->count(); i++ )
    connect( mS2sPlugins->at(i), SIGNAL(spectrogramCreated(SpectrogramData*)), this, SLOT(addSpectrogram(SpectrogramData*)) );
    for(int i=0; i < mS2wPlugins->count(); i++ )
    connect( mS2wPlugins->at(i), SIGNAL(waveformCreated(WaveformData*)), mSound, SLOT(addWaveform(WaveformData*)) );
    for(int i=0; i < mW2wPlugins->count(); i++ )
    connect( mW2wPlugins->at(i), SIGNAL(waveformCreated(WaveformData*)), mSound, SLOT(addWaveform(WaveformData*)) );

    // update the script environment when the script requests it
    connect(this,SIGNAL(requestScriptDataRefresh()),this,SLOT(setupScriptEnvironment()));

    connect(this,SIGNAL(scriptDataChanged()),this,SLOT(setupScriptEnvironment()));
}

void SoundWidget::regressionMenuAction(QAction *action)
{
    Q_UNUSED(action);
    /// @todo rethink this implementation
//    int actiontype = action->data().toInt();
//    int index = maRegressionMenus.indexOf(  qobject_cast<QMenu*>(action->parentWidget()) );


//    if(actiontype < 1000) // edit
//    {
//    RegressionDialog *rd = new RegressionDialog(mSound->waveformData(),mSound->spectrogramData(),this);
//	rd->setFromRegression(maRegressions.at(index));
//    connect(rd,SIGNAL(regressionObject(RegressionModel*)),mSound,SLOT(addRegression(RegressionModel*)));
//	rd->exec();
//    }
//    else if(actiontype < 2000) // delete
//    {
//	delete maRegressions.at(index);
//	maRegressions.removeAt(index);
//	mRegressionMenu->removeAction(maRegressionMenus.at(index)->menuAction());
//	maRegressionMenus.removeAt(index);
//    }
}

void SoundWidget::annotationMenuAction(QAction *action)
{
    Q_UNUSED(action);
    /// @todo rethink this implementation
//    int actiontype = action->data().toInt();
//    int index = maAnnotationMenus.indexOf( qobject_cast<QMenu*>(action->parentWidget()) );

//    if(actiontype < 1000) // visible
//    {
//        if(action->isChecked())
//            ui->plotDisplayWidget->annotations()->at(index)->show();
//        else
//            ui->plotDisplayWidget->annotations()->at(index)->hide();
//    }
//    else if(actiontype < 2000) // delete
//    {
//        delete maIntervalAnnotations.at(index);
//        maIntervalAnnotations.removeAt(index);
//        mAnnotationMenu->removeAction(maAnnotationMenus.at(index)->menuAction());
//        maAnnotationMenus.removeAt(index);

//        delete ui->plotDisplayWidget->annotations()->at(index);
//        ui->plotDisplayWidget->annotations()->removeAt(index);

//        ui->plotDisplayWidget->drawVerticalLayout();
//    }
}


QScriptValue waveformArrayToScriptValue(QScriptEngine *engine, const QList<WaveformData*> &aWaveformData)
{
    // assemble the waveforms into an array
    QScriptValue waveformArray = engine->newArray( aWaveformData.count() );
    for(int i=0; i< aWaveformData.count(); i++)
    {
	QScriptValue tmp = engine->newQObject( aWaveformData.at(i) );
	waveformArray.setProperty(i, tmp);
    }
    return waveformArray;
}

void waveformArrayFromScriptValue(const QScriptValue &obj, QList<WaveformData*> &aWaveformData)
{
    Q_UNUSED(obj);
    Q_UNUSED(aWaveformData);
    /* At this point I don't see a point to implementing this function. */
}

QScriptValue spectrogramArrayToScriptValue(QScriptEngine *engine, const QList<SpectrogramData*> &aSpectrogramData)
{
    // assemble the waveforms into an array
    QScriptValue spectrogramArray = engine->newArray( aSpectrogramData.count() );
    for(int i=0; i< aSpectrogramData.count(); i++)
    {
	QScriptValue tmp = engine->newQObject( aSpectrogramData.at(i) );
	spectrogramArray.setProperty(i, tmp);
    }
    return spectrogramArray;
}

void spectrogramArrayFromScriptValue(const QScriptValue &obj, QList<SpectrogramData*> &aSpectrogramData)
{
    Q_UNUSED(obj);
    Q_UNUSED(aSpectrogramData);
    /* At this point I don't see a point to implementing this function. */
}

void SoundWidget::runScript()
{
    QString fileName;
    fileName= QFileDialog::getOpenFileName(this, tr("Open AW Script"), "", tr("AW script files (*.awscript)"));
    if(!fileName.isNull())
    {
	QFile data(fileName);
	if (! data.open(QFile::ReadOnly)) { return; }

	QTextStream in(&data);
	QString code = in.readAll();

	executeCode(code);
    }
}

void SoundWidget::executeCode(QString code)
{
    setupScriptEnvironment();

    QScriptSyntaxCheckResult check = mScriptEngine->checkSyntax(code);
    if( check.state() != QScriptSyntaxCheckResult::Valid )
    {
        QMessageBox::critical(this,tr("Script error"),tr("Line %1, Column %2: %3").arg(check.errorLineNumber()).arg(check.errorColumnNumber()).arg(check.errorMessage()));
        return;
    }

    QScriptValue result = mScriptEngine->evaluate(code,"error.log");
    if (mScriptEngine->hasUncaughtException()) {
        int line = mScriptEngine->uncaughtExceptionLineNumber();
        qDebug() << "uncaught exception at line" << line << ":" << result.toString();
    }
}

void SoundWidget::setupScriptEnvironment()
{
    /// @todo Replace this functionality
//    qScriptRegisterMetaType(mScriptEngine, waveformArrayToScriptValue, waveformArrayFromScriptValue);
//    qScriptRegisterMetaType(mScriptEngine, spectrogramArrayToScriptValue, spectrogramArrayFromScriptValue);

//    QStringList waveformNames;
//    for(int i=0; i< mSound->waveformData()->count(); i++)
//    waveformNames << mSound->waveformData()->at(i)->name();
//    mScriptEngine->globalObject().setProperty("waveformNames", mScriptEngine->toScriptValue(waveformNames));

//    QStringList spectrogramNames;
//    for(int i=0; i< mSound->spectrogramData()->count(); i++)
//    spectrogramNames << mSound->spectrogramData()->at(i)->name();
//    mScriptEngine->globalObject().setProperty("spectrogramNames", mScriptEngine->toScriptValue(spectrogramNames));

//    mScriptEngine->globalObject().setProperty("waveforms", mScriptEngine->toScriptValue(mSound->waveformData()));
//    mScriptEngine->globalObject().setProperty("spectrograms", mScriptEngine->toScriptValue(mSound->waveformData()));

//    mScriptEngine->globalObject().setProperty("widget", mScriptEngine->newQObject(this));

//    // add the w2s plugins as objects
//    for(int i=0; i<mW2sPlugins->count(); i++)
//    {
//    QObject *someObject = mW2sPlugins->at(i);
//	QScriptValue objectValue = mScriptEngine->newQObject(someObject);
//    mScriptEngine->globalObject().setProperty(mW2sPlugins->at(i)->scriptName(), objectValue);
//    }

//    // add the w2w plugins as objects
//    for(int i=0; i<mW2wPlugins->count(); i++)
//    {
//    QObject *someObject = mW2wPlugins->at(i);
//	QScriptValue objectValue = mScriptEngine->newQObject(someObject);
//    mScriptEngine->globalObject().setProperty(mW2wPlugins->at(i)->scriptName(), objectValue);
//    }

//    // add the s2s plugins as objects
//    for(int i=0; i<mS2sPlugins->count(); i++)
//    {
//    QObject *someObject = mS2sPlugins->at(i);
//	QScriptValue objectValue = mScriptEngine->newQObject(someObject);
//    mScriptEngine->globalObject().setProperty(mS2sPlugins->at(i)->scriptName(), objectValue);
//    }

//    // add the s2w plugins as objects
//    for(int i=0; i<mS2wPlugins->count(); i++)
//    {
//    QObject *someObject = mS2wPlugins->at(i);
//	QScriptValue objectValue = mScriptEngine->newQObject(someObject);
//    mScriptEngine->globalObject().setProperty(mS2wPlugins->at(i)->scriptName(), objectValue);

////        qDebug() << "added" << mainWnd->s2w()->at(i)->scriptName() << engine->globalObject().property(mainWnd->s2w()->at(i)->scriptName()).isQObject();
//    }
}

void SoundWidget::removeWaveform(int index)
{
    if( !(index < mSound->waveformData()->count()) ) { return; }
    // first delete it from all the plots
    for(int i=0; i<ui->plotDisplayWidget->plotViews()->count(); i++) // every plot
    {
    for(int j=0; j<ui->plotDisplayWidget->plotViews()->at(i)->curves()->count(); j++) // every plot item
	{
        if( ui->plotDisplayWidget->plotViews()->at(i)->curveData(j) == mSound->waveformData()->at(index) ) // see if we're going to delete this curve's data
	    {
        ui->plotDisplayWidget->plotViews()->at(i)->removeItemAt(j); // if so, remove the curve
		j--; // back up one since the j-th item is one we haven't looked at yet
	    }
	}
    }
    // then delete the data itself
    delete mSound->waveformData()->takeAt(index);
}

void SoundWidget::removeSpectrogram(int index)
{
    if( !(index < mSound->spectrogramData()->count()) ) { return; }
    // first delete it from all the plots
    for(int i=0; i<ui->plotDisplayWidget->plotViews()->count(); i++) // every plot item
    {
    for(int j=0; j<ui->plotDisplayWidget->plotViews()->at(i)->spectrograms()->count(); j++) // every spectrogram
	{
        if( ui->plotDisplayWidget->plotViews()->at(i)->spectrogramData(j) == mSound->spectrogramData()->at(index) ) // see if we're going to delete this spectrograms's data
	    {
        ui->plotDisplayWidget->plotViews()->at(i)->removeItemAt(j); // if so, remove the spectrogram
		j--; // back up one since the j-th item is one we haven't looked at yet
	    }
	}
    }
    // then delete the data itself
    delete mSound->spectrogramData()->takeAt(index);
}
