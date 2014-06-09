#include "soundwidget.h"


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
#include "sndfile.h"
#include "plotviewwidget.h"
#include "interfaces.h"
#include "mainwindow.h"
#include "plotmanagerdialog.h"
#include "datamanagerdialog.h"
#include "plotdisplayareawidget.h"
#include "intervalannotation.h"
#include "mdiarea.h"

SoundWidget::SoundWidget(QWidget *parent, MainWindow *wnd) :
    QWidget(parent), mMainWnd(wnd)
{
    QVBoxLayout *layout = new QVBoxLayout;
    mPlotDisplay = new PlotDisplayAreaWidget;
    layout->addWidget(mPlotDisplay);

    resize(600,400);

    mPlotDisplay->setTimeAxes(0.0f,0.4f);

    this->setLayout(layout);

    mMenuBar = new QMenuBar(this);
    layout->setMenuBar(mMenuBar);
    setupMenus();
    setupActions();
    setupScripting();

    this->setWindowTitle(tr("Acoustic Workspace"));
}

SoundWidget::~SoundWidget()
{
    qDeleteAll(maWaveformData.begin(), maWaveformData.end());
    qDeleteAll(maSpectrogramData.begin(), maSpectrogramData.end());
    qDeleteAll(maRegressions.begin(), maRegressions.end());

    maWaveformData.clear();
    maSpectrogramData.clear();

    if(mScriptEngine != 0)
	delete mScriptEngine;
}

// Private slots

void SoundWidget::addAnnotationMenu(IntervalAnnotation *annotation)
{
    QMenu *tmp = mAnnotationMenu->addMenu(annotation->mName);
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

void SoundWidget::addRegression(RegressionModel *regression)
{
    maRegressions << regression;
    int index = maRegressions.count() - 1;

    QMenu *tmp = mRegressionMenu->addMenu(regression->name());
    QAction *editRegression = new QAction(tr("Edit"),tmp);
    editRegression->setData(index);
    QAction *deleteRegression = new QAction(tr("Delete"),tmp);
    deleteRegression->setData(1000+index);
    tmp->addAction(editRegression);
    tmp->addAction(deleteRegression);
    maRegressionMenus << tmp;

    connect(tmp,SIGNAL(triggered(QAction*)),this,SLOT(regressionMenuAction(QAction*)));

    emit scriptDataChanged();
}

void SoundWidget::addSpectrogram(SpectrogramData *data)
{
    // the need for this test arises because the plugins live in the MainWindow, and emitting a signal from a plugin, while advantageous for other reasons, sends the signal to all of the child windows
    QMdiSubWindow* tmp = qobject_cast<QMdiSubWindow*>(this->parentWidget());
    if(tmp == 0) { return; }

    if( this->parentWidget() == tmp->mdiArea()->currentSubWindow() )
	maSpectrogramData << data;

    emit scriptDataChanged();
}

void SoundWidget::addWaveform(WaveformData *data)
{
//    qDebug() << "SoundWidget::addWaveform data" << data;

    // the need for this test arises because the plugins live in the MainWindow, and emitting a signal from a plugin, while advantageous for other reasons, sends the signal to all of the child windows
    QMdiSubWindow* tmp = qobject_cast<QMdiSubWindow*>(this->parentWidget());
    if(tmp == 0) { return; }

//    qDebug() << "SoundWidget::addWaveform here1";

    if( this->parentWidget() == tmp->mdiArea()->currentSubWindow() )
	maWaveformData << data;

//    qDebug() << "SoundWidget::addWaveform here2";

    emit scriptDataChanged();

//    qDebug() << "SoundWidget::addWaveform here3";
}

void SoundWidget::close()
{
    if(mPlotDisplay != 0) { delete mPlotDisplay; }
    mPlotDisplay = new PlotDisplayAreaWidget;
    this->layout()->addWidget(mPlotDisplay);

    qDeleteAll(maWaveformData.begin(), maWaveformData.end());
    maWaveformData.clear();

    qDeleteAll(maSpectrogramData.begin(), maSpectrogramData.end());
    maSpectrogramData.clear();

    qDeleteAll(maRegressionMenus.begin(), maRegressionMenus.end());
    maRegressionMenus.clear();

    this->setWindowTitle(tr("Acoustic Workspace"));
    mCloseAction->setEnabled(false);
}

void SoundWidget::launchDataManager()
{
    DataManagerDialog *dm = new DataManagerDialog(mMainWnd->w2w(), mMainWnd->w2s(), mMainWnd->s2w(), mMainWnd->s2s(), &maWaveformData,&maSpectrogramData,this);
    connect(dm, SIGNAL(removeWaveform(int)),this,SLOT(removeWaveform(int)));
    connect(dm, SIGNAL(removeSpectrogram(int)),this,SLOT(removeSpectrogram(int)));

//    dm->setModal(false);
//    dm->show();

    dm->exec();

//    qDebug() << "end of SoundWidget::launchDataManager";
}

void SoundWidget::launchPlotManager()
{
    PlotManagerDialog pm(mPlotDisplay->plotViews(),&maWaveformData,&maSpectrogramData);
    connect(&pm,SIGNAL(addProsody(PlotViewWidget*,QString)),this->mPlotDisplay,SLOT(addPlotView(PlotViewWidget*,QString)));
    pm.exec();
    disconnect(&pm,SIGNAL(addProsody(PlotViewWidget*,QString)),this->mPlotDisplay,SLOT(addPlotView(PlotViewWidget*,QString)));

//    qDebug() << "end of SoundWidget::launchPlotManager";
}

void SoundWidget::importSound()
{
    QString fileName;
    fileName= QFileDialog::getOpenFileName(this, tr("Open Sound"), "", tr("Sound files (*.*)"));
    if(!fileName.isNull())
    {
	QFileInfo info(fileName);
	if(!info.exists())
	{
	    QMessageBox::critical(0,"Error","The file "+fileName+" does not exist.");
	    return;
	}
	loadSound(fileName);
    }
}

void SoundWidget::loadSound(QString fileName)
{
    SF_INFO sndInfo;
    SNDFILE *sndFile = sf_open(fileName.toUtf8(), SFM_READ, &sndInfo);
    if(sndFile==NULL)
    {
	QMessageBox::critical(0,"Error","The file "+fileName+" could not be opened.");
	return;
    }

    if(sndInfo.channels > 1)
    {
	QMessageBox::warning(0,"Warning","The file has more than one channel, but only the first channel is going to be read.");
    }

    double *data = (double*)malloc(sizeof(double)*sndInfo.frames);
    Q_CHECK_PTR(data);
    if( sf_read_double(sndFile,data,sndInfo.frames) != sndInfo.frames )
    {
	QMessageBox::critical(0,"Error","There was some kind of error in reading the file (not enough data).");
	sf_close(sndFile);
	return;
    }
    sf_close(sndFile);

    double *times = (double*)malloc(sizeof(double)*sndInfo.frames);
    Q_CHECK_PTR(times);

    for(quint32 i=0; i < sndInfo.frames; i++)
    {
	*(times+i) = ((double)i)/sndInfo.samplerate;;
    }

    QFileInfo info(fileName);
    WaveformData *sound = new WaveformData(info.fileName(),times,data,sndInfo.frames,sndInfo.samplerate);
    maWaveformData << sound;

    PlotViewWidget *tmp = new PlotViewWidget("Waveform");
    mPlotDisplay->addPlotView( tmp ,"Waveform");

    tmp->addCurveData(sound, false, Qt::blue);

    tmp->curves()->last()->setStyle(QwtPlotCurve::Dots);
    tmp->curves()->last()->setRenderHint(QwtPlotItem::RenderAntialiased,false);

    mPlotDisplay->setTimeMinMax(sound->tMin(),sound->tMax());

    mCloseAction->setEnabled(true);

    this->setWindowTitle(sound->name());
}


void SoundWidget::importTextGrid()
{
    if( mPlotDisplay->plotViews()->count() <1 )
    {
	QMessageBox::critical(this,tr("Error"),tr("You need at least one plot window to import an annotation."));
	return;
    }

    QString fileName;
    fileName= QFileDialog::getOpenFileName(this, tr("Open TextGrid"), "", tr("TextGrid files (*.*)"));
    if(!fileName.isNull())
    {
	QFileInfo info(fileName);
	if(!info.exists())
	{
	    QMessageBox::critical(0,"Error","The file "+fileName+" does not exist.");
	    return;
	}
	readTextGridFromFile(fileName);
    }
}

void SoundWidget::readTextGridFromFile(QString fileName)
{
    int count = maIntervalAnnotations.count();
    QFile data(fileName);
    if (data.open(QFile::ReadOnly)) {
	QTextStream in(&data);

	while( !in.atEnd() )
	{
        bool inInterval = false;
        QString line = in.readLine();
	    //		qDebug() << line;
	    if( line.contains("class = \"IntervalTier\""))
	    {
		maIntervalAnnotations << new IntervalAnnotation;
		inInterval = true;
	    }
	    else if( line.contains("class = \"TextTier\""))
	    {
		inInterval = false;
	    }
	    else if( line.contains("name = \""))
	    {
		QRegExp rx("\"(.*)\"");
		rx.indexIn(line);
		if(rx.captureCount() < 1) { continue; }
		if(inInterval)
		    maIntervalAnnotations.last()->mName = rx.capturedTexts().at(1);
	    }
	    else if( line.contains(QRegExp("intervals \\[\\d*\\]:")))
	    {
		//		    qDebug() << "Interval" << line;
		if(inInterval)
		    maIntervalAnnotations.last()->maIntervals << new Interval;
	    }
	    else if( line.contains("xmin = "))
	    {
		if(inInterval && maIntervalAnnotations.count() > 0 && maIntervalAnnotations.last()->maIntervals.count() > 0)
		{
		    QRegExp rx("xmin = (\\d*\\.\\d*)"); // will this get the decimal?
		    rx.indexIn(line);
		    //			qDebug() << rx.capturedTexts();
		    if(rx.captureCount() < 1) { continue; }
		    QString tmp = rx.capturedTexts().at(1);
		    //			qDebug() << tmp.toDouble();
		    maIntervalAnnotations.last()->maIntervals.last()->mLeft = tmp.toDouble();
		}
	    }
	    else if( line.contains("xmax = "))
	    {
		if(inInterval && maIntervalAnnotations.count() > 0 && maIntervalAnnotations.last()->maIntervals.count() > 0)
		{
		    QRegExp rx("xmax = (\\d*\\.\\d*)"); // will this get the decimal?
		    rx.indexIn(line);
		    //			qDebug() << rx.capturedTexts() << rx.captureCount();
		    if(rx.captureCount() < 1) { continue; }
		    QString tmp = rx.capturedTexts().at(1);
		    //			qDebug() << tmp.toDouble();
		    maIntervalAnnotations.last()->maIntervals.last()->mRight = tmp.toDouble();
		}
	    }
	    else if( line.contains("text = "))
	    {
		if(inInterval && maIntervalAnnotations.count() > 0 && maIntervalAnnotations.last()->maIntervals.count() > 0)
		{
		    QRegExp rx("\"(.*)\"");
		    rx.indexIn(line);
		    //			qDebug() << rx.capturedTexts();
		    if(rx.captureCount() < 1) { continue; }
		    QString tmp = rx.capturedTexts().at(1);
		    maIntervalAnnotations.last()->maIntervals.last()->mLabel = tmp;
		    //			qDebug() << tmp;
		}
	    }
	}
    }

    for(int i=count; i<maIntervalAnnotations.count(); i++)
    {
	mPlotDisplay->addAnnotation(new IntervalDisplayWidget(maIntervalAnnotations.at(i),mPlotDisplay->plotViews()->first(),this));
	addAnnotationMenu(maIntervalAnnotations.at(i));
    }

    /*
    for(int i=0; i<aIntervalAnnotations.count(); i++)
    {
	qDebug() << "Tier" << aIntervalAnnotations.at(i)->name;
	for(int j=0; j<aIntervalAnnotations.at(i)->aIntervals.count(); j++)
	{
	    qDebug() << "Interval" << aIntervalAnnotations.at(i)->aIntervals.at(j)->left << aIntervalAnnotations.at(i)->aIntervals.at(j)->right << aIntervalAnnotations.at(i)->aIntervals.at(j)->label;
	}
    }
*/
}

void SoundWidget::newRegression()
{
    RegressionDialog *rd = new RegressionDialog(&maWaveformData, &maSpectrogramData,this);
    connect(rd,SIGNAL(regressionObject(RegressionModel*)),this,SLOT(addRegression(RegressionModel*)));
    rd->exec();
}


void SoundWidget::openProject()
{
    close();

    QString fileName;
    fileName= QFileDialog::getOpenFileName(this, tr("Open Project"), "", tr("Project files (*.xml)"));
    if(!fileName.isNull())
	readFromFile(fileName);
}

void SoundWidget::save()
{
    if( mCurrentFilename == "")
	saveAs();
    else
	writeProjectToFile(mCurrentFilename);
}

void SoundWidget::saveAs()
{
    QString filename;
    filename= QFileDialog::getSaveFileName(this, tr("XML File"), "", tr("XML File (*.xml)"));
    if(filename.isNull()) { return; }
    writeProjectToFile(filename);
}

// private methods

void SoundWidget::readFromFile(QString filename)
{
    QFileInfo info(filename);
    if(!info.exists())
    {
	QMessageBox::critical(this,"Error","The file "+filename+" does not exist.");
	return;
    }

    QFile file(filename);
    if(! file.open(QFile::ReadOnly | QFile::Text) ) { QMessageBox::critical(this,"Error","The file "+filename+" could not be opened."); return; }
    QXmlStreamReader xml(&file);

    QFile binaryfile(info.completeBaseName() + ".bin");
    binaryfile.open(QIODevice::ReadOnly);
    QDataStream binaryin(&binaryfile);
    binaryin.setByteOrder(QDataStream::LittleEndian);
    binaryin.setFloatingPointPrecision(QDataStream::DoublePrecision);

    while (!xml.atEnd())
    {
	if(xml.readNext() == QXmlStreamReader::StartElement)
	{

	    QString name = xml.name().toString();
	    //	qDebug() << xml.name();
	    //	continue;

	    if( name == "interface-settings")
	    {
		double tMax = readXmlElement(xml,"time-max").toDouble();
		double tMin = readXmlElement(xml,"time-min").toDouble();
		double leftPos = readXmlElement(xml,"left-position").toDouble();
		double rightPos = readXmlElement(xml,"right-position").toDouble();

		mPlotDisplay->setTimeMinMax(tMin,tMax);
		mPlotDisplay->setTimeAxes(leftPos,rightPos);
	    }
	    else if( name == "waveform")
	    {

		QString name = readXmlElement(xml,"label");
		size_t fs = readXmlElement(xml,"sample-frequency").toInt();
		size_t nsam = readXmlElement(xml,"number-of-samples").toInt();

		double *x = (double*)malloc(sizeof(double)*nsam);
		double *y = (double*)malloc(sizeof(double)*nsam);
		if(x==NULL || y==NULL) { qDebug() << "Memory allocation error (x & y)."; return; }
		for(quint32 i=0; i<nsam; i++)
		{
		    binaryin >> *(x+i);
		}
		for(quint32 i=0; i<nsam; i++)
		{
		    binaryin >> *(y+i);
		}

		maWaveformData << new WaveformData(name, x, y, nsam, fs);
	    }
	    else if( name == "spectrogram" )
	    {
		QString name = readXmlElement(xml,"label");

		xml.readNextStartElement(); if(xml.name().toString() != "window-length") { qDebug() << "Line " << xml.lineNumber() << ", Column " << xml.columnNumber() << ": " << "File format error: " << xml.name(); return; }
		double windowLength = xml.readElementText().toDouble();

		xml.readNextStartElement(); if(xml.name().toString() != "time-step") { qDebug() << "Line " << xml.lineNumber() << ", Column " << xml.columnNumber() << ": " << "File format error: " << xml.name(); return; }
		double timeStep = xml.readElementText().toDouble();

		xml.readNextStartElement(); if(xml.name().toString() != "number-of-time-frames") { qDebug() << "Line " << xml.lineNumber() << ", Column " << xml.columnNumber() << ": " << "File format error: " << xml.name(); return; }
		size_t nFrames = xml.readElementText().toInt();

		xml.readNextStartElement(); if(xml.name().toString() != "number-of-frequency-bins") { qDebug() << "Line " << xml.lineNumber() << ", Column " << xml.columnNumber() << ": " << "File format error: " << xml.name(); return; }
		size_t nFreqBins= xml.readElementText().toInt();

		double spec_min = 999999999, spec_max = -999999999;

		double *times = (double*)malloc(sizeof(double)*nFrames);
		double *frequencies = (double*)malloc(sizeof(double)*nFreqBins);
		double *data = (double*)malloc(sizeof(double)*nFrames*nFreqBins);
		if(times==NULL || frequencies==NULL || data==NULL) { qDebug() << "Memory allocation error (times, frequencies, data)."; return; }

		for(quint32 i=0; i<nFrames; i++)
		{
		    binaryin >> *(times+i);
		}
		for(quint32 i=0; i<nFreqBins; i++)
		{
		    binaryin >> *(frequencies+i);
		}
		for(quint32 i=0; i<nFrames*nFreqBins; i++)
		{
		    binaryin >> *(data+i);
		    if( *(data+i) < spec_min ) { spec_min = *(data+i); }
		    if( *(data+i) > spec_max ) { spec_max = *(data+i); }
		}

		maSpectrogramData << new SpectrogramData(name, data, times, nFrames, frequencies, nFreqBins, spec_min, spec_max , windowLength, timeStep);
	    }
	    else if( name == "plot" )
	    {
		QString name = xml.attributes().value("name").toString();
		bool secondary = xml.attributes().value("secondary-axis").toString().toInt();
		int height = xml.attributes().value("height").toString().toInt();

		mPlotDisplay->addPlotView(new PlotViewWidget(name),name);
		mPlotDisplay->plotViews()->last()->setHasSecondaryAxis(secondary);
		mPlotDisplay->plotViews()->last()->setHeight(height);
	    }
	    else if( name == "curve" )
	    {
		QString name = xml.attributes().value("name").toString();
		bool secondary = xml.attributes().value("secondary-axis").toString().toInt();
		int index = xml.attributes().value("index").toString().toInt();

		while( xml.readNext() != QXmlStreamReader::StartElement );
		if(xml.name().toString() != "symbol-color") { qDebug() << "Line " << xml.lineNumber() << ", Column " << xml.columnNumber() << ": " << "File format error: " << xml.name() << "Expecting symbol-color"; return; }
		QColor symbolColor( xml.attributes().value("r").toString().toInt(), xml.attributes().value("g").toString().toInt() , xml.attributes().value("b").toString().toInt() );

		while( xml.readNext() != QXmlStreamReader::StartElement );
		if(xml.name().toString() != "symbol-fill-color") { qDebug() << "Line " << xml.lineNumber() << ", Column " << xml.columnNumber() << ": " << "File format error: " << xml.name(); return; }
		QColor symbolFillColor( xml.attributes().value("r").toString().toInt(), xml.attributes().value("g").toString().toInt() , xml.attributes().value("b").toString().toInt() );

		while( xml.readNext() != QXmlStreamReader::StartElement );
		if(xml.name().toString() != "symbol-style") { qDebug() << "Line " << xml.lineNumber() << ", Column " << xml.columnNumber() << ": " << "File format error: " << xml.name(); return; }
		int symbolStyle = xml.readElementText().toInt();

		while( xml.readNext() != QXmlStreamReader::StartElement );
		if(xml.name().toString() != "symbol-size") { qDebug() << "Line " << xml.lineNumber() << ", Column " << xml.columnNumber() << ": " << "File format error: " << xml.name(); return; }
		int symbolSize = xml.readElementText().toInt();

		while( xml.readNext() != QXmlStreamReader::StartElement );
		if(xml.name().toString() != "line-color") { qDebug() << "Line " << xml.lineNumber() << ", Column " << xml.columnNumber() << ": " << "File format error: " << xml.name(); return; }
		QColor lineColor( xml.attributes().value("r").toString().toInt(), xml.attributes().value("g").toString().toInt() , xml.attributes().value("b").toString().toInt() );

		while( xml.readNext() != QXmlStreamReader::StartElement );
		if(xml.name().toString() != "line-style") { qDebug() << "Line " << xml.lineNumber() << ", Column " << xml.columnNumber() << ": " << "File format error: " << xml.name(); return; }
		int lineStyle = xml.readElementText().toInt();

		while( xml.readNext() != QXmlStreamReader::StartElement );
		if(xml.name().toString() != "line-width") { qDebug() << "Line " << xml.lineNumber() << ", Column " << xml.columnNumber() << ": " << "File format error: " << xml.name(); return; }
		int lineWidth = xml.readElementText().toInt();

		while( xml.readNext() != QXmlStreamReader::StartElement );
		if(xml.name().toString() != "antialiased") { qDebug() << "Line " << xml.lineNumber() << ", Column " << xml.columnNumber() << ": " << "File format error: " << xml.name(); return; }
		bool antialiased = xml.readElementText().toInt();

		if(index >= maWaveformData.length())
		{
		    QMessageBox::critical(this,tr("Error"),tr("Evidently the file has been corrupted somehow. Quitting..."));
		    return;
		}

		// where do I set the name? -- it looks like the name is just redundant information
		mPlotDisplay->plotViews()->last()->addCurveData( maWaveformData.at(index), secondary );

		mPlotDisplay->plotViews()->last()->curves()->last()->setPen(QPen(lineColor,lineWidth));
		mPlotDisplay->plotViews()->last()->curves()->last()->setStyle((QwtPlotCurve::CurveStyle)lineStyle);
		mPlotDisplay->plotViews()->last()->curves()->last()->setRenderHint(QwtPlotItem::RenderAntialiased,antialiased);

        QwtSymbol * sym = new QwtSymbol;
        sym->setBrush(QBrush(symbolFillColor));
        sym->setPen(QPen(symbolColor));
        sym->setSize(symbolSize);
        sym->setStyle((QwtSymbol::Style)symbolStyle);
		mPlotDisplay->plotViews()->last()->curves()->last()->setSymbol(sym);
	    }
	    else if(name=="spectrogram-plot")
	    {
		QString name = xml.attributes().value("name").toString();
		int index = xml.attributes().value("index").toString().toInt();

		while( xml.readNext() != QXmlStreamReader::StartElement );
		if(xml.name().toString() != "frequency-lower-bound") { qDebug() << "Line " << xml.lineNumber() << ", Column " << xml.columnNumber() << ": " << "File format error: " << xml.name(); return; }
		int lowerbound = xml.readElementText().toInt();

		while( xml.readNext() != QXmlStreamReader::StartElement );
		if(xml.name().toString() != "frequency-upper-bound") { qDebug() << "Line " << xml.lineNumber() << ", Column " << xml.columnNumber() << ": " << "File format error: " << xml.name(); return; }
		int upperbound = xml.readElementText().toInt();

		mPlotDisplay->plotViews()->last()->addSpectrogramData( maSpectrogramData.at(index) );

		QwtLinearScaleEngine engine;
		mPlotDisplay->plotViews()->last()->plot()->setAxisScaleDiv(QwtPlot::yLeft,QwtScaleDiv( engine.divideScale( lowerbound, upperbound ,10, 10) ));
	    }
	    else if(name=="regression")
	    {
		RegressionModel *tmp = new RegressionModel;
		tmp->setName( xml.attributes().value("name").toString() );
		tmp->setInterceptTerm( xml.attributes().value("intercept-term").toString().toInt() );
		addRegression(tmp);
	    }
	    else if(name=="dependent-spectrogram")
	    {
		maRegressions.last()->setDependentSpectrogram( maSpectrogramData.at( xml.readElementText().toInt() ) );
	    }
	    else if(name=="dependent-waveform")
	    {
		maRegressions.last()->mDependent << maWaveformData.at(xml.readElementText().toInt());
	    }
	    else if(name=="independent-waveform")
	    {
		maRegressions.last()->mSimple << maWaveformData.at(xml.readElementText().toInt());
	    }
	    else if(name=="independent-interaction")
	    {
		maRegressions.last()->mInteraction << new InteractionEffect;
	    }
	    else if(name=="interaction-member")
	    {
		maRegressions.last()->mInteraction.last()->members << maWaveformData.at(xml.readElementText().toInt());
	    }
	    else if(name=="interval-annotation")
	    {
		maIntervalAnnotations << new IntervalAnnotation;
		maIntervalAnnotations.last()->mName = xml.attributes().value("name").toString();

		mPlotDisplay->addAnnotation(new IntervalDisplayWidget(maIntervalAnnotations.last(),mPlotDisplay->plotViews()->at(0),0));
		addAnnotationMenu(maIntervalAnnotations.last());
	    }
	    else if(name=="interval")
	    {
		maIntervalAnnotations.last()->maIntervals << new Interval(xml.attributes().value("label").toString(), xml.attributes().value("left").toString().toDouble(), xml.attributes().value("right").toString().toDouble());
	    }
	}
    }

    if(maWaveformData.length()>0)
    {
	this->setWindowTitle(maWaveformData.at(0)->name());
	mPlotDisplay->setTimeMinMax(maWaveformData.at(0)->tMin(),maWaveformData.at(0)->tMax());
    }

    mCloseAction->setEnabled(true);

    mCurrentFilename = filename;
    //    plotDisplay->drawVerticalLayout();
}

QString SoundWidget::readXmlElement(QXmlStreamReader &reader, QString elementname)
{
    reader.readNextStartElement();
    if(reader.name().toString() != elementname)
    {
	qDebug() << "Line " << reader.lineNumber() << ", Column " << reader.columnNumber() << ": " << "File format error: " << reader.name() << "was found, but I was expecting" << elementname;
	return "";
    }
    else
    {
	return reader.readElementText();
    }
}

void SoundWidget::writeProjectToFile(QString filename)
{
    // open the binary file
    QFileInfo info(filename);
    QFile binaryfile(info.completeBaseName() + ".bin");
    binaryfile.open(QIODevice::WriteOnly);
    QDataStream binaryout(&binaryfile);   // we will serialize the data into the file
    binaryout.setByteOrder(QDataStream::LittleEndian);
    binaryout.setFloatingPointPrecision(QDataStream::DoublePrecision);

    // open the xml file
    QFile xmlfile(filename);
    xmlfile.open(QFile::WriteOnly | QFile::Text);
    QXmlStreamWriter xs(&xmlfile);

    xs.setAutoFormatting(true);
    xs.writeStartDocument();
    xs.setCodec("UTF-8");

    xs.writeStartElement("root");

    xs.writeStartElement("interface-settings");

    xs.writeTextElement("time-max",QString::number(mPlotDisplay->getTMax()));
    xs.writeTextElement("time-min",QString::number(mPlotDisplay->getTMin()));
    xs.writeTextElement("left-position",QString::number(mPlotDisplay->getLeftPos()));
    xs.writeTextElement("right-position",QString::number(mPlotDisplay->getRightPos()));

    xs.writeEndElement(); // interface-settings

    xs.writeStartElement("waveform-data");
    for(int i=0; i<maWaveformData.count(); i++)
    {
	xs.writeStartElement("waveform");
	xs.writeAttribute("id",QString::number(i));

	xs.writeTextElement("label",maWaveformData.at(i)->name());
	xs.writeTextElement("sample-frequency",QString::number(maWaveformData.at(i)->getSamplingFrequency()));
	xs.writeTextElement("number-of-samples",QString::number(maWaveformData.at(i)->getNSamples() ));

	xs.writeEndElement(); // waveform

	for(quint32 j=0; j<maWaveformData.at(i)->getNSamples(); j++)
	{
        binaryout << maWaveformData.at(i)->xData().at(j);
	}
	for(quint32 j=0; j<maWaveformData.at(i)->getNSamples(); j++)
	{
        binaryout << maWaveformData.at(i)->yData().at(j);
	}
    }

    xs.writeEndElement(); // waveform-data

    xs.writeStartElement("spectrogram-data");
    for(int i=0; i<maSpectrogramData.count(); i++)
    {
	xs.writeStartElement("spectrogram");
	xs.writeAttribute("id",QString::number(i));

	xs.writeTextElement("label",maSpectrogramData.at(i)->name());

	xs.writeTextElement("window-length",QString::number(maSpectrogramData.at(i)->getWindowLength()));
	xs.writeTextElement("time-step",QString::number(maSpectrogramData.at(i)->getTimeStep()));
	xs.writeTextElement("number-of-time-frames",QString::number(maSpectrogramData.at(i)->getNTimeSteps()));
	xs.writeTextElement("number-of-frequency-bins",QString::number(maSpectrogramData.at(i)->getNFrequencyBins()));

	xs.writeEndElement(); // spectrogram

	for(quint32 j=0; j<maSpectrogramData.at(i)->getNTimeSteps(); j++)
	{
	    binaryout << maSpectrogramData.at(i)->getTimeFromIndex(j);
	}
	for(quint32 j=0; j< maSpectrogramData.at(i)->getNFrequencyBins(); j++)
	{
	    binaryout << maSpectrogramData.at(i)->getFrequencyFromIndex(j);
	}
	for(quint32 j=0; j<maSpectrogramData.at(i)->getNTimeSteps() * maSpectrogramData.at(i)->getNFrequencyBins(); j++)
	{
	    binaryout << maSpectrogramData.at(i)->flatdata(j);
	}
    }
    xs.writeEndElement(); // spectrogram-data

    xs.writeStartElement("plots");
    for(int i=0; i<mPlotDisplay->plotViews()->count(); i++)
    {
	xs.writeStartElement("plot");
	xs.writeAttribute("id",QString::number(i));
	xs.writeAttribute("name",mPlotDisplay->plotViews()->at(i)->name());
	if( mPlotDisplay->plotViews()->at(i)->hasSecondaryAxis() )
	    xs.writeAttribute("secondary-axis", "1" );
	else
	    xs.writeAttribute("secondary-axis", "0" );
	xs.writeAttribute("height",QString::number(mPlotDisplay->plotViews()->at(i)->height()));

	for(int j=0; j< mPlotDisplay->plotViews()->at(i)->spectrograms()->length(); j++)
	{
	    xs.writeStartElement("spectrogram-plot");
	    xs.writeAttribute("index", QString::number( maSpectrogramData.indexOf(mPlotDisplay->plotViews()->at(i)->spectrogramData(j) ) ) );
	    xs.writeAttribute("name", mPlotDisplay->plotViews()->at(i)->spectrogramData(j)->name());

	    xs.writeTextElement("frequency-lower-bound",QString::number( mPlotDisplay->plotViews()->at(i)->plot()->axisScaleDiv(QwtPlot::yLeft).lowerBound() ));
	    xs.writeTextElement("frequency-upper-bound",QString::number( mPlotDisplay->plotViews()->at(i)->plot()->axisScaleDiv(QwtPlot::yLeft).upperBound() ));
	    xs.writeEndElement();
	}

	for(int j=0; j< mPlotDisplay->plotViews()->at(i)->curves()->length(); j++)
	{
	    xs.writeStartElement("curve");
	    xs.writeAttribute("index",QString::number( maWaveformData.indexOf(mPlotDisplay->plotViews()->at(i)->curveData(j)) ));
	    xs.writeAttribute("name", mPlotDisplay->plotViews()->at(i)->curveData(j)->name() );
	    if( mPlotDisplay->plotViews()->at(i)->curves()->at(j)->yAxis() == QwtPlot::yLeft )
		xs.writeAttribute("secondary-axis", "0" );
	    else
		xs.writeAttribute("secondary-axis", "1" );

	    xs.writeEmptyElement("symbol-color");
        xs.writeAttribute("r",QString::number(mPlotDisplay->plotViews()->at(i)->curves()->at(j)->symbol()->pen().color().red()));
        xs.writeAttribute("g",QString::number(mPlotDisplay->plotViews()->at(i)->curves()->at(j)->symbol()->pen().color().green()));
        xs.writeAttribute("b",QString::number(mPlotDisplay->plotViews()->at(i)->curves()->at(j)->symbol()->pen().color().blue()));

	    xs.writeEmptyElement("symbol-fill-color");
        xs.writeAttribute("r",QString::number(mPlotDisplay->plotViews()->at(i)->curves()->at(j)->symbol()->brush().color().red()));
        xs.writeAttribute("g",QString::number(mPlotDisplay->plotViews()->at(i)->curves()->at(j)->symbol()->brush().color().green()));
        xs.writeAttribute("b",QString::number(mPlotDisplay->plotViews()->at(i)->curves()->at(j)->symbol()->brush().color().blue()));

        xs.writeTextElement("symbol-style",QString::number(mPlotDisplay->plotViews()->at(i)->curves()->at(j)->symbol()->style()));

        xs.writeTextElement("symbol-size",QString::number(mPlotDisplay->plotViews()->at(i)->curves()->at(j)->symbol()->size().height()));

	    xs.writeEmptyElement("line-color");
        xs.writeAttribute("r",QString::number(mPlotDisplay->plotViews()->at(i)->curves()->at(j)->pen().color().red()));
	    xs.writeAttribute("g",QString::number(mPlotDisplay->plotViews()->at(i)->curves()->at(j)->pen().color().green()));
	    xs.writeAttribute("b",QString::number(mPlotDisplay->plotViews()->at(i)->curves()->at(j)->pen().color().blue()));

	    xs.writeTextElement("line-style",QString::number(mPlotDisplay->plotViews()->at(i)->curves()->at(j)->style()));

	    xs.writeTextElement("line-width",QString::number(mPlotDisplay->plotViews()->at(i)->curves()->at(j)->pen().width()));

	    xs.writeTextElement("antialiased",QString::number(mPlotDisplay->plotViews()->at(i)->curves()->at(j)->testRenderHint(QwtPlotItem::RenderAntialiased)));

	    xs.writeEndElement();
	}

	xs.writeEndElement(); // plot
    }
    xs.writeEndElement(); // plots

    xs.writeStartElement("interval-annotations");
    for(int i=0; i<maIntervalAnnotations.count(); i++)
    {
	xs.writeStartElement("interval-annotation");
	xs.writeAttribute("name",maIntervalAnnotations.at(0)->mName);

	for(int j=0; j<maIntervalAnnotations.at(0)->maIntervals.count(); j++)
	{

	    xs.writeEmptyElement("interval");
	    xs.writeAttribute("label",maIntervalAnnotations.at(0)->maIntervals.at(j)->mLabel);
	    xs.writeAttribute("left",QString::number(maIntervalAnnotations.at(0)->maIntervals.at(j)->mLeft));
	    xs.writeAttribute("right",QString::number(maIntervalAnnotations.at(0)->maIntervals.at(j)->mRight));
	}
	xs.writeEndElement(); // interval-annotation
    }
    xs.writeEndElement(); // interval-annotations


    xs.writeStartElement("regressions");
    for(int i=0; i<maRegressions.count(); i++)
    {
	xs.writeStartElement("regression");
	xs.writeAttribute("name",maRegressions.at(i)->name());
	xs.writeAttribute("intercept-term",QString::number( maRegressions.at(i)->hasIntercept() ) );
	xs.writeAttribute("spectrogram-mode",QString::number( maRegressions.at(i)->dependentIsSpectrogram() ) );

	xs.writeStartElement("dependent");
	if( maRegressions.at(i)->dependentIsSpectrogram() )
	    xs.writeTextElement( "dependent-spectrogram", QString::number( maSpectrogramData.indexOf( maRegressions.at(i)->mDependentSpectrogram ) ) );
	else
	    for(int j=0; j<maRegressions.at(i)->mDependent.count(); j++)
		xs.writeTextElement( "dependent-waveform", QString::number( maWaveformData.indexOf( maRegressions.at(i)->mDependent.at(j) ) ) );

	xs.writeEndElement(); // dependent

	xs.writeStartElement("independent");

	xs.writeStartElement("simple");
	for(int j=0; j<maRegressions.at(i)->mSimple.count(); j++)
	    xs.writeTextElement("independent-waveform",QString::number( maWaveformData.indexOf( maRegressions.at(i)->mSimple.at(j) ) ));
	xs.writeEndElement(); // simple

	xs.writeStartElement("interaction");
	for(int j=0; j<maRegressions.at(i)->mInteraction.count(); j++)
	{
	    xs.writeStartElement("independent-interaction");
	    for(int k=0; k<maRegressions.at(i)->mInteraction.at(j)->members.count(); k++)
		xs.writeTextElement("interaction-member", QString::number( maWaveformData.indexOf( maRegressions.at(i)->mInteraction.at(j)->members.at(k) ) ) );
	    xs.writeEndElement(); // independent-interaction
	}
	xs.writeEndElement(); // interaction

	xs.writeEndElement(); // independent

	xs.writeEndElement(); // regression
    }
    xs.writeEndElement(); // regressions

    xs.writeEndElement(); // root

    xs.writeEndDocument();

    xmlfile.close();
    binaryfile.close();
}

void SoundWidget::setupActions()
{
    mImportSoundAction = new QAction(tr("Import sound to create waveform"),this);
    mImportSoundAction->setShortcut(QKeySequence("Ctrl+I"));
    connect(mImportSoundAction,SIGNAL(triggered()),this,SLOT(importSound()));

    mCloseAction = new QAction(tr("Close Sound"),this);
    mCloseAction->setShortcut(QKeySequence("Ctrl+F4"));
    mCloseAction->setEnabled(false);
    connect(mCloseAction,SIGNAL(triggered()),this,SLOT(close()));

    mOpenProjectAction = new QAction(tr("Open Sound"),this);
    mOpenProjectAction->setShortcut(QKeySequence("Ctrl+O"));
    connect(mOpenProjectAction,SIGNAL(triggered()),this,SLOT(openProject()));

    mSaveProjectAction = new QAction(tr("Save Sound"),this);
    mSaveProjectAction->setShortcut(QKeySequence("Ctrl+S"));
    connect(mSaveProjectAction,SIGNAL(triggered()),this,SLOT(save()));

    mSaveProjectAsAction = new QAction(tr("Save Sound As"),this);
    mSaveProjectAsAction->setShortcut(QKeySequence("Ctrl+Shift+S"));
    connect(mSaveProjectAsAction,SIGNAL(triggered()),this,SLOT(saveAs()));

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
    mAnnotationMenu = mMenuBar->addMenu(tr("Annotations"));
    mScriptingMenu = mMenuBar->addMenu(tr("Scripting"));
}

void SoundWidget::setupScripting()
{
    mScriptEngine = new QScriptEngine;

    // set up the connections to receive spectrograms & waveforms that plugins emit
    // this is, technically, broader than just the scripting environment, but I want to make sure that this connection is made before the others
    for(int i=0; i < mMainWnd->w2s()->count(); i++ )
	connect( mMainWnd->w2s()->at(i), SIGNAL(spectrogramCreated(SpectrogramData*)), this, SLOT(addSpectrogram(SpectrogramData*)) );
    for(int i=0; i < mMainWnd->s2s()->count(); i++ )
	connect( mMainWnd->s2s()->at(i), SIGNAL(spectrogramCreated(SpectrogramData*)), this, SLOT(addSpectrogram(SpectrogramData*)) );
    for(int i=0; i < mMainWnd->s2w()->count(); i++ )
	connect( mMainWnd->s2w()->at(i), SIGNAL(waveformCreated(WaveformData*)), this, SLOT(addWaveform(WaveformData*)) );
    for(int i=0; i < mMainWnd->w2w()->count(); i++ )
	connect( mMainWnd->w2w()->at(i), SIGNAL(waveformCreated(WaveformData*)), this, SLOT(addWaveform(WaveformData*)) );

    // update the script environment when the script requests it
    connect(this,SIGNAL(requestScriptDataRefresh()),this,SLOT(setupScriptEnvironment()));

    connect(this,SIGNAL(scriptDataChanged()),this,SLOT(setupScriptEnvironment()));
}

void SoundWidget::regressionMenuAction(QAction *action)
{
    int actiontype = action->data().toInt();
    int index = maRegressionMenus.indexOf(  qobject_cast<QMenu*>(action->parentWidget()) );


    if(actiontype < 1000) // edit
    {
	RegressionDialog *rd = new RegressionDialog(&maWaveformData, &maSpectrogramData,this);
	rd->setFromRegression(maRegressions.at(index));
	connect(rd,SIGNAL(regressionObject(RegressionModel*)),this,SLOT(addRegression(RegressionModel*)));
	rd->exec();
    }
    else if(actiontype < 2000) // delete
    {
	delete maRegressions.at(index);
	maRegressions.removeAt(index);
	mRegressionMenu->removeAction(maRegressionMenus.at(index)->menuAction());
	maRegressionMenus.removeAt(index);
    }
}

void SoundWidget::annotationMenuAction(QAction *action)
{
    int actiontype = action->data().toInt();
    int index = maAnnotationMenus.indexOf( qobject_cast<QMenu*>(action->parentWidget()) );

    if(actiontype < 1000) // visible
    {
	if(action->isChecked())
	    mPlotDisplay->annotations()->at(index)->show();
	else
	    mPlotDisplay->annotations()->at(index)->hide();
    }
    else if(actiontype < 2000) // delete
    {
	delete maIntervalAnnotations.at(index);
	maIntervalAnnotations.removeAt(index);
	mAnnotationMenu->removeAction(maAnnotationMenus.at(index)->menuAction());
	maAnnotationMenus.removeAt(index);

	delete mPlotDisplay->annotations()->at(index);
	mPlotDisplay->annotations()->removeAt(index);

	mPlotDisplay->drawVerticalLayout();
    }
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
    qScriptRegisterMetaType(mScriptEngine, waveformArrayToScriptValue, waveformArrayFromScriptValue);
    qScriptRegisterMetaType(mScriptEngine, spectrogramArrayToScriptValue, spectrogramArrayFromScriptValue);

    QStringList waveformNames;
    for(int i=0; i<maWaveformData.count(); i++)
	waveformNames << maWaveformData.at(i)->name();
    mScriptEngine->globalObject().setProperty("waveformNames", mScriptEngine->toScriptValue(waveformNames));

    QStringList spectrogramNames;
    for(int i=0; i<maSpectrogramData.count(); i++)
	spectrogramNames << maSpectrogramData.at(i)->name();
    mScriptEngine->globalObject().setProperty("spectrogramNames", mScriptEngine->toScriptValue(spectrogramNames));

    mScriptEngine->globalObject().setProperty("waveforms", mScriptEngine->toScriptValue(maWaveformData));
    mScriptEngine->globalObject().setProperty("spectrograms", mScriptEngine->toScriptValue(maSpectrogramData));

    mScriptEngine->globalObject().setProperty("widget", mScriptEngine->newQObject(this));

    // add the w2s plugins as objects
    for(int i=0; i<mMainWnd->w2s()->count(); i++)
    {
	QObject *someObject = mMainWnd->w2s()->at(i);
	QScriptValue objectValue = mScriptEngine->newQObject(someObject);
	mScriptEngine->globalObject().setProperty(mMainWnd->w2s()->at(i)->scriptName(), objectValue);
    }

    // add the w2w plugins as objects
    for(int i=0; i<mMainWnd->w2w()->count(); i++)
    {
	QObject *someObject = mMainWnd->w2w()->at(i);
	QScriptValue objectValue = mScriptEngine->newQObject(someObject);
	mScriptEngine->globalObject().setProperty(mMainWnd->w2w()->at(i)->scriptName(), objectValue);
    }

    // add the s2s plugins as objects
    for(int i=0; i<mMainWnd->s2s()->count(); i++)
    {
	QObject *someObject = mMainWnd->s2s()->at(i);
	QScriptValue objectValue = mScriptEngine->newQObject(someObject);
	mScriptEngine->globalObject().setProperty(mMainWnd->s2s()->at(i)->scriptName(), objectValue);
    }

    // add the s2w plugins as objects
    for(int i=0; i<mMainWnd->s2w()->count(); i++)
    {
	QObject *someObject = mMainWnd->s2w()->at(i);
	QScriptValue objectValue = mScriptEngine->newQObject(someObject);
	mScriptEngine->globalObject().setProperty(mMainWnd->s2w()->at(i)->scriptName(), objectValue);

//        qDebug() << "added" << mainWnd->s2w()->at(i)->scriptName() << engine->globalObject().property(mainWnd->s2w()->at(i)->scriptName()).isQObject();
    }
}

void SoundWidget::removeWaveform(int index)
{
    if( !(index < maWaveformData.count()) ) { return; }
    // first delete it from all the plots
    for(int i=0; i<mPlotDisplay->plotViews()->count(); i++) // every plot
    {
	for(int j=0; j<mPlotDisplay->plotViews()->at(i)->curves()->count(); j++) // every plot item
	{
	    if( mPlotDisplay->plotViews()->at(i)->curveData(j) == maWaveformData.at(index) ) // see if we're going to delete this curve's data
	    {
		mPlotDisplay->plotViews()->at(i)->removeItemAt(j); // if so, remove the curve
		j--; // back up one since the j-th item is one we haven't looked at yet
	    }
	}
    }
    // then delete the data itself
    delete maWaveformData.takeAt(index);
}

void SoundWidget::removeSpectrogram(int index)
{
    if( !(index < maSpectrogramData.count()) ) { return; }
    // first delete it from all the plots
    for(int i=0; i<mPlotDisplay->plotViews()->count(); i++) // every plot item
    {
	for(int j=0; j<mPlotDisplay->plotViews()->at(i)->spectrograms()->count(); j++) // every spectrogram
	{
	    if( mPlotDisplay->plotViews()->at(i)->spectrogramData(j) == maSpectrogramData.at(index) ) // see if we're going to delete this spectrograms's data
	    {
		mPlotDisplay->plotViews()->at(i)->removeItemAt(j); // if so, remove the spectrogram
		j--; // back up one since the j-th item is one we haven't looked at yet
	    }
	}
    }
    // then delete the data itself
    delete maSpectrogramData.takeAt(index);
}
