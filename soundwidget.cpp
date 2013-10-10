#include "soundwidget.h"


#include <qwt_plot.h>
#include <qwt_plot_marker.h>
#include <qwt_plot_curve.h>
#include <qwt_legend.h>
#include <qwt_data.h>
#include <qwt_text.h>
#include <qwt_math.h>
#include <qwt_color_map.h>
#include <qwt_scale_engine.h>
#include <qwt_text_label.h>
#include <qwt_slider.h>
#include <qwt_symbol.h>
#include <qwt_plot_curve.h>
#include <qwt_scale_engine.h>

#include <QtGui>
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
	QWidget(parent)
{
    mainWnd = wnd;

    currentFilename = "";

    QVBoxLayout *layout = new QVBoxLayout;
    plotDisplay = new PlotDisplayAreaWidget;
    layout->addWidget(plotDisplay);

    resize(600,400);

    plotDisplay->setTimeAxes(0.0f,0.4f);

    this->setLayout(layout);

    menuBar = new QMenuBar(this);
    layout->setMenuBar(menuBar);
    setupMenus();
    setupActions();
    setupScripting();

    this->setWindowTitle(tr("Acoustic Workspace"));
}

SoundWidget::~SoundWidget()
{
    qDeleteAll(aWaveformData.begin(), aWaveformData.end());
    qDeleteAll(aSpectrogramData.begin(), aSpectrogramData.end());
    qDeleteAll(aRegressions.begin(), aRegressions.end());

    aWaveformData.clear();
    aSpectrogramData.clear();

    if(engine != 0)
	delete engine;
}

// Private slots

void SoundWidget::addAnnotationMenu(IntervalAnnotation *annotation)
{
    QMenu *tmp = annotationMenu->addMenu(annotation->name);
    QAction *visible = new QAction(tr("Visible"),tmp);
    visible->setCheckable(true);
    visible->setChecked(true);
    visible->setData(0);

    QAction *deleteAction = new QAction(tr("Delete"),tmp);
    deleteAction->setData(1000);

    tmp->addAction(visible);
    tmp->addAction(deleteAction);
    aAnnotationMenus << tmp;

    connect(tmp,SIGNAL(triggered(QAction*)),this,SLOT(annotationMenuAction(QAction*)));
}

void SoundWidget::addRegression(RegressionModel *regression)
{
    aRegressions << regression;
    int index = aRegressions.count() - 1;

    QMenu *tmp = regressionMenu->addMenu(regression->name());
    QAction *editRegression = new QAction(tr("Edit"),tmp);
    editRegression->setData(index);
    QAction *deleteRegression = new QAction(tr("Delete"),tmp);
    deleteRegression->setData(1000+index);
    tmp->addAction(editRegression);
    tmp->addAction(deleteRegression);
    aRegressionMenus << tmp;

    connect(tmp,SIGNAL(triggered(QAction*)),this,SLOT(regressionMenuAction(QAction*)));

    emit scriptDataChanged();
}

void SoundWidget::addSpectrogram(SpectrogramData *data)
{
    // the need for this test arises because the plugins live in the MainWindow, and emitting a signal from a plugin, while advantageous for other reasons, sends the signal to all of the child windows
    QMdiSubWindow* tmp = qobject_cast<QMdiSubWindow*>(this->parentWidget());
    if(tmp == 0) { return; }

    if( this->parentWidget() == tmp->mdiArea()->currentSubWindow() )
	aSpectrogramData << data;

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
	aWaveformData << data;

//    qDebug() << "SoundWidget::addWaveform here2";

    emit scriptDataChanged();

//    qDebug() << "SoundWidget::addWaveform here3";
}

void SoundWidget::close()
{
    if(plotDisplay != 0) { delete plotDisplay; }
    plotDisplay = new PlotDisplayAreaWidget;
    this->layout()->addWidget(plotDisplay);

    qDeleteAll(aWaveformData.begin(), aWaveformData.end());
    aWaveformData.clear();

    qDeleteAll(aSpectrogramData.begin(), aSpectrogramData.end());
    aSpectrogramData.clear();

    qDeleteAll(aRegressionMenus.begin(), aRegressionMenus.end());
    aRegressionMenus.clear();

    this->setWindowTitle(tr("Acoustic Workspace"));
    closeAction->setEnabled(false);
}

void SoundWidget::launchDataManager()
{
    DataManagerDialog *dm = new DataManagerDialog(mainWnd->w2w(), mainWnd->w2s(), mainWnd->s2w(), mainWnd->s2s(), &aWaveformData,&aSpectrogramData,this);
    connect(dm, SIGNAL(removeWaveform(int)),this,SLOT(removeWaveform(int)));
    connect(dm, SIGNAL(removeSpectrogram(int)),this,SLOT(removeSpectrogram(int)));

//    dm->setModal(false);
//    dm->show();

    dm->exec();

//    qDebug() << "end of SoundWidget::launchDataManager";
}

void SoundWidget::launchPlotManager()
{
    PlotManagerDialog pm(plotDisplay->plotViews(),&aWaveformData,&aSpectrogramData);
    connect(&pm,SIGNAL(addProsody(PlotViewWidget*,QString)),this->plotDisplay,SLOT(addPlotView(PlotViewWidget*,QString)));
    pm.exec();
    disconnect(&pm,SIGNAL(addProsody(PlotViewWidget*,QString)),this->plotDisplay,SLOT(addPlotView(PlotViewWidget*,QString)));

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
    SNDFILE *sndFile = sf_open(fileName.toAscii(), SFM_READ, &sndInfo);
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
    aWaveformData << sound;

    PlotViewWidget *tmp = new PlotViewWidget("Waveform");
    plotDisplay->addPlotView( tmp ,"Waveform");

    tmp->addCurveData(sound, false, Qt::blue);

    tmp->curves()->last()->setStyle(QwtPlotCurve::Dots);
    tmp->curves()->last()->setRenderHint(QwtPlotItem::RenderAntialiased,false);

    plotDisplay->setTimeMinMax(sound->tMin(),sound->tMax());

    closeAction->setEnabled(true);

    this->setWindowTitle(sound->name());
}


void SoundWidget::importTextGrid()
{
    if( plotDisplay->plotViews()->count() <1 )
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
    bool inInterval = false;
    int count = aIntervalAnnotations.count();
    QFile data(fileName);
    if (data.open(QFile::ReadOnly)) {
	QTextStream in(&data);

	while( !in.atEnd() )
	{
	    QString line = in.readLine();
	    //		qDebug() << line;
	    if( line.contains("class = \"IntervalTier\""))
	    {
		aIntervalAnnotations << new IntervalAnnotation;
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
		    aIntervalAnnotations.last()->name = rx.capturedTexts().at(1);
	    }
	    else if( line.contains(QRegExp("intervals \\[\\d*\\]:")))
	    {
		//		    qDebug() << "Interval" << line;
		if(inInterval)
		    aIntervalAnnotations.last()->aIntervals << new Interval;
	    }
	    else if( line.contains("xmin = "))
	    {
		if(inInterval && aIntervalAnnotations.count() > 0 && aIntervalAnnotations.last()->aIntervals.count() > 0)
		{
		    QRegExp rx("xmin = (\\d*\\.\\d*)"); // will this get the decimal?
		    rx.indexIn(line);
		    //			qDebug() << rx.capturedTexts();
		    if(rx.captureCount() < 1) { continue; }
		    QString tmp = rx.capturedTexts().at(1);
		    //			qDebug() << tmp.toDouble();
		    aIntervalAnnotations.last()->aIntervals.last()->left = tmp.toDouble();
		}
	    }
	    else if( line.contains("xmax = "))
	    {
		if(inInterval && aIntervalAnnotations.count() > 0 && aIntervalAnnotations.last()->aIntervals.count() > 0)
		{
		    QRegExp rx("xmax = (\\d*\\.\\d*)"); // will this get the decimal?
		    rx.indexIn(line);
		    //			qDebug() << rx.capturedTexts() << rx.captureCount();
		    if(rx.captureCount() < 1) { continue; }
		    QString tmp = rx.capturedTexts().at(1);
		    //			qDebug() << tmp.toDouble();
		    aIntervalAnnotations.last()->aIntervals.last()->right = tmp.toDouble();
		}
	    }
	    else if( line.contains("text = "))
	    {
		if(inInterval && aIntervalAnnotations.count() > 0 && aIntervalAnnotations.last()->aIntervals.count() > 0)
		{
		    QRegExp rx("\"(.*)\"");
		    rx.indexIn(line);
		    //			qDebug() << rx.capturedTexts();
		    if(rx.captureCount() < 1) { continue; }
		    QString tmp = rx.capturedTexts().at(1);
		    aIntervalAnnotations.last()->aIntervals.last()->label = tmp;
		    //			qDebug() << tmp;
		}
	    }
	}
    }

    for(int i=count; i<aIntervalAnnotations.count(); i++)
    {
	plotDisplay->addAnnotation(new IntervalDisplayWidget(aIntervalAnnotations.at(i),plotDisplay->plotViews()->first(),this));
	addAnnotationMenu(aIntervalAnnotations.at(i));
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
    RegressionDialog *rd = new RegressionDialog(&aWaveformData, &aSpectrogramData,this);
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
    if( currentFilename == "")
	saveAs();
    else
	writeProjectToFile(currentFilename);
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

		plotDisplay->setTimeMinMax(tMin,tMax);
		plotDisplay->setTimeAxes(leftPos,rightPos);
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

		aWaveformData << new WaveformData(name, x, y, nsam, fs);
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

		aSpectrogramData << new SpectrogramData(name, data, times, nFrames, frequencies, nFreqBins, spec_min, spec_max , windowLength, timeStep);
	    }
	    else if( name == "plot" )
	    {
		QString name = xml.attributes().value("name").toString();
		bool secondary = xml.attributes().value("secondary-axis").toString().toInt();
		int height = xml.attributes().value("height").toString().toInt();

		plotDisplay->addPlotView(new PlotViewWidget(name),name);
		plotDisplay->plotViews()->last()->setHasSecondaryAxis(secondary);
		plotDisplay->plotViews()->last()->setHeight(height);
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

		if(index >= aWaveformData.length())
		{
		    QMessageBox::critical(this,tr("Error"),tr("Evidently the file has been corrupted somehow. Quitting..."));
		    return;
		}

		// where do I set the name? -- it looks like the name is just redundant information
		plotDisplay->plotViews()->last()->addCurveData( aWaveformData.at(index), secondary );

		plotDisplay->plotViews()->last()->curves()->last()->setPen(QPen(lineColor,lineWidth));
		plotDisplay->plotViews()->last()->curves()->last()->setStyle((QwtPlotCurve::CurveStyle)lineStyle);
		plotDisplay->plotViews()->last()->curves()->last()->setRenderHint(QwtPlotItem::RenderAntialiased,antialiased);

		QwtSymbol sym = plotDisplay->plotViews()->last()->curves()->last()->symbol();
		sym.setBrush(QBrush(symbolFillColor));
		sym.setPen(QPen(symbolColor));
		sym.setSize(symbolSize);
		sym.setStyle((QwtSymbol::Style)symbolStyle);
		plotDisplay->plotViews()->last()->curves()->last()->setSymbol(sym);
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

		plotDisplay->plotViews()->last()->addSpectrogramData( aSpectrogramData.at(index) );

		QwtLinearScaleEngine engine;
		plotDisplay->plotViews()->last()->plot()->setAxisScaleDiv(QwtPlot::yLeft,QwtScaleDiv( engine.divideScale( lowerbound, upperbound ,10, 10) ));
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
		aRegressions.last()->setDependentSpectrogram( aSpectrogramData.at( xml.readElementText().toInt() ) );
	    }
	    else if(name=="dependent-waveform")
	    {
		aRegressions.last()->dependent << aWaveformData.at(xml.readElementText().toInt());
	    }
	    else if(name=="independent-waveform")
	    {
		aRegressions.last()->simple << aWaveformData.at(xml.readElementText().toInt());
	    }
	    else if(name=="independent-interaction")
	    {
		aRegressions.last()->interaction << new InteractionEffect;
	    }
	    else if(name=="interaction-member")
	    {
		aRegressions.last()->interaction.last()->members << aWaveformData.at(xml.readElementText().toInt());
	    }
	    else if(name=="interval-annotation")
	    {
		aIntervalAnnotations << new IntervalAnnotation;
		aIntervalAnnotations.last()->name = xml.attributes().value("name").toString();

		plotDisplay->addAnnotation(new IntervalDisplayWidget(aIntervalAnnotations.last(),plotDisplay->plotViews()->at(0),0));
		addAnnotationMenu(aIntervalAnnotations.last());
	    }
	    else if(name=="interval")
	    {
		aIntervalAnnotations.last()->aIntervals << new Interval(xml.attributes().value("label").toString(), xml.attributes().value("left").toString().toDouble(), xml.attributes().value("right").toString().toDouble());
	    }
	}
    }

    if(aWaveformData.length()>0)
    {
	this->setWindowTitle(aWaveformData.at(0)->name());
	plotDisplay->setTimeMinMax(aWaveformData.at(0)->tMin(),aWaveformData.at(0)->tMax());
    }

    closeAction->setEnabled(true);

    currentFilename = filename;
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

    xs.writeTextElement("time-max",QString::number(plotDisplay->getTMax()));
    xs.writeTextElement("time-min",QString::number(plotDisplay->getTMin()));
    xs.writeTextElement("left-position",QString::number(plotDisplay->getLeftPos()));
    xs.writeTextElement("right-position",QString::number(plotDisplay->getRightPos()));

    xs.writeEndElement(); // interface-settings

    xs.writeStartElement("waveform-data");
    for(int i=0; i<aWaveformData.count(); i++)
    {
	xs.writeStartElement("waveform");
	xs.writeAttribute("id",QString::number(i));

	xs.writeTextElement("label",aWaveformData.at(i)->name());
	xs.writeTextElement("sample-frequency",QString::number(aWaveformData.at(i)->getSamplingFrequency()));
	xs.writeTextElement("number-of-samples",QString::number(aWaveformData.at(i)->getNSamples() ));

	xs.writeEndElement(); // waveform

	for(quint32 j=0; j<aWaveformData.at(i)->getNSamples(); j++)
	{
	    binaryout << aWaveformData.at(i)->getTimeAtSample(j);
	}
	for(quint32 j=0; j<aWaveformData.at(i)->getNSamples(); j++)
	{
	    binaryout << aWaveformData.at(i)->getYAtSample(j);
	}
    }

    xs.writeEndElement(); // waveform-data

    xs.writeStartElement("spectrogram-data");
    for(int i=0; i<aSpectrogramData.count(); i++)
    {
	xs.writeStartElement("spectrogram");
	xs.writeAttribute("id",QString::number(i));

	xs.writeTextElement("label",aSpectrogramData.at(i)->name());

	xs.writeTextElement("window-length",QString::number(aSpectrogramData.at(i)->getWindowLength()));
	xs.writeTextElement("time-step",QString::number(aSpectrogramData.at(i)->getTimeStep()));
	xs.writeTextElement("number-of-time-frames",QString::number(aSpectrogramData.at(i)->getNTimeSteps()));
	xs.writeTextElement("number-of-frequency-bins",QString::number(aSpectrogramData.at(i)->getNFrequencyBins()));

	xs.writeEndElement(); // spectrogram

	for(quint32 j=0; j<aSpectrogramData.at(i)->getNTimeSteps(); j++)
	{
	    binaryout << aSpectrogramData.at(i)->getTimeFromIndex(j);
	}
	for(quint32 j=0; j< aSpectrogramData.at(i)->getNFrequencyBins(); j++)
	{
	    binaryout << aSpectrogramData.at(i)->getFrequencyFromIndex(j);
	}
	for(quint32 j=0; j<aSpectrogramData.at(i)->getNTimeSteps() * aSpectrogramData.at(i)->getNFrequencyBins(); j++)
	{
	    binaryout << aSpectrogramData.at(i)->flatdata(j);
	}
    }
    xs.writeEndElement(); // spectrogram-data

    xs.writeStartElement("plots");
    for(int i=0; i<plotDisplay->plotViews()->count(); i++)
    {
	xs.writeStartElement("plot");
	xs.writeAttribute("id",QString::number(i));
	xs.writeAttribute("name",plotDisplay->plotViews()->at(i)->name());
	if( plotDisplay->plotViews()->at(i)->hasSecondaryAxis() )
	    xs.writeAttribute("secondary-axis", "1" );
	else
	    xs.writeAttribute("secondary-axis", "0" );
	xs.writeAttribute("height",QString::number(plotDisplay->plotViews()->at(i)->height()));

	for(int j=0; j< plotDisplay->plotViews()->at(i)->spectrograms()->length(); j++)
	{
	    xs.writeStartElement("spectrogram-plot");
	    xs.writeAttribute("index", QString::number( aSpectrogramData.indexOf(plotDisplay->plotViews()->at(i)->spectrogramData(j) ) ) );
	    xs.writeAttribute("name", plotDisplay->plotViews()->at(i)->spectrogramData(j)->name());

	    xs.writeTextElement("frequency-lower-bound",QString::number( plotDisplay->plotViews()->at(i)->plot()->axisScaleDiv(QwtPlot::yLeft)->lowerBound() ));
	    xs.writeTextElement("frequency-upper-bound",QString::number( plotDisplay->plotViews()->at(i)->plot()->axisScaleDiv(QwtPlot::yLeft)->upperBound() ));
	    xs.writeEndElement();
	}

	for(int j=0; j< plotDisplay->plotViews()->at(i)->curves()->length(); j++)
	{
	    xs.writeStartElement("curve");
	    xs.writeAttribute("index",QString::number( aWaveformData.indexOf(plotDisplay->plotViews()->at(i)->curveData(j)) ));
	    xs.writeAttribute("name", plotDisplay->plotViews()->at(i)->curveData(j)->name() );
	    if( plotDisplay->plotViews()->at(i)->curves()->at(j)->yAxis() == QwtPlot::yLeft )
		xs.writeAttribute("secondary-axis", "0" );
	    else
		xs.writeAttribute("secondary-axis", "1" );

	    xs.writeEmptyElement("symbol-color");
	    xs.writeAttribute("r",QString::number(plotDisplay->plotViews()->at(i)->curves()->at(j)->symbol().pen().color().red()));
	    xs.writeAttribute("g",QString::number(plotDisplay->plotViews()->at(i)->curves()->at(j)->symbol().pen().color().green()));
	    xs.writeAttribute("b",QString::number(plotDisplay->plotViews()->at(i)->curves()->at(j)->symbol().pen().color().blue()));

	    xs.writeEmptyElement("symbol-fill-color");
	    xs.writeAttribute("r",QString::number(plotDisplay->plotViews()->at(i)->curves()->at(j)->symbol().brush().color().red()));
	    xs.writeAttribute("g",QString::number(plotDisplay->plotViews()->at(i)->curves()->at(j)->symbol().brush().color().green()));
	    xs.writeAttribute("b",QString::number(plotDisplay->plotViews()->at(i)->curves()->at(j)->symbol().brush().color().blue()));

	    xs.writeTextElement("symbol-style",QString::number(plotDisplay->plotViews()->at(i)->curves()->at(j)->symbol().style()));

	    xs.writeTextElement("symbol-size",QString::number(plotDisplay->plotViews()->at(i)->curves()->at(j)->symbol().size().height()));

	    xs.writeEmptyElement("line-color");
	    xs.writeAttribute("r",QString::number(plotDisplay->plotViews()->at(i)->curves()->at(j)->pen().color().red()));
	    xs.writeAttribute("g",QString::number(plotDisplay->plotViews()->at(i)->curves()->at(j)->pen().color().green()));
	    xs.writeAttribute("b",QString::number(plotDisplay->plotViews()->at(i)->curves()->at(j)->pen().color().blue()));

	    xs.writeTextElement("line-style",QString::number(plotDisplay->plotViews()->at(i)->curves()->at(j)->style()));

	    xs.writeTextElement("line-width",QString::number(plotDisplay->plotViews()->at(i)->curves()->at(j)->pen().width()));

	    xs.writeTextElement("antialiased",QString::number(plotDisplay->plotViews()->at(i)->curves()->at(j)->testRenderHint(QwtPlotItem::RenderAntialiased)));

	    xs.writeEndElement();
	}

	xs.writeEndElement(); // plot
    }
    xs.writeEndElement(); // plots

    xs.writeStartElement("interval-annotations");
    for(int i=0; i<aIntervalAnnotations.count(); i++)
    {
	xs.writeStartElement("interval-annotation");
	xs.writeAttribute("name",aIntervalAnnotations.at(0)->name);

	for(int j=0; j<aIntervalAnnotations.at(0)->aIntervals.count(); j++)
	{

	    xs.writeEmptyElement("interval");
	    xs.writeAttribute("label",aIntervalAnnotations.at(0)->aIntervals.at(j)->label);
	    xs.writeAttribute("left",QString::number(aIntervalAnnotations.at(0)->aIntervals.at(j)->left));
	    xs.writeAttribute("right",QString::number(aIntervalAnnotations.at(0)->aIntervals.at(j)->right));
	}
	xs.writeEndElement(); // interval-annotation
    }
    xs.writeEndElement(); // interval-annotations


    xs.writeStartElement("regressions");
    for(int i=0; i<aRegressions.count(); i++)
    {
	xs.writeStartElement("regression");
	xs.writeAttribute("name",aRegressions.at(i)->name());
	xs.writeAttribute("intercept-term",QString::number( aRegressions.at(i)->hasIntercept() ) );
	xs.writeAttribute("spectrogram-mode",QString::number( aRegressions.at(i)->dependentIsSpectrogram() ) );

	xs.writeStartElement("dependent");
	if( aRegressions.at(i)->dependentIsSpectrogram() )
	    xs.writeTextElement( "dependent-spectrogram", QString::number( aSpectrogramData.indexOf( aRegressions.at(i)->dependentSpectrogram ) ) );
	else
	    for(int j=0; j<aRegressions.at(i)->dependent.count(); j++)
		xs.writeTextElement( "dependent-waveform", QString::number( aWaveformData.indexOf( aRegressions.at(i)->dependent.at(j) ) ) );

	xs.writeEndElement(); // dependent

	xs.writeStartElement("independent");

	xs.writeStartElement("simple");
	for(int j=0; j<aRegressions.at(i)->simple.count(); j++)
	    xs.writeTextElement("independent-waveform",QString::number( aWaveformData.indexOf( aRegressions.at(i)->simple.at(j) ) ));
	xs.writeEndElement(); // simple

	xs.writeStartElement("interaction");
	for(int j=0; j<aRegressions.at(i)->interaction.count(); j++)
	{
	    xs.writeStartElement("independent-interaction");
	    for(int k=0; k<aRegressions.at(i)->interaction.at(j)->members.count(); k++)
		xs.writeTextElement("interaction-member", QString::number( aWaveformData.indexOf( aRegressions.at(i)->interaction.at(j)->members.at(k) ) ) );
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
    importSoundAction = new QAction(tr("Import sound to create waveform"),this);
    importSoundAction->setShortcut(QKeySequence("Ctrl+I"));
    connect(importSoundAction,SIGNAL(triggered()),this,SLOT(importSound()));

    closeAction = new QAction(tr("Close Sound"),this);
    closeAction->setShortcut(QKeySequence("Ctrl+F4"));
    closeAction->setEnabled(false);
    connect(closeAction,SIGNAL(triggered()),this,SLOT(close()));

    openProjectAction = new QAction(tr("Open Sound"),this);
    openProjectAction->setShortcut(QKeySequence("Ctrl+O"));
    connect(openProjectAction,SIGNAL(triggered()),this,SLOT(openProject()));

    saveProjectAction = new QAction(tr("Save Sound"),this);
    saveProjectAction->setShortcut(QKeySequence("Ctrl+S"));
    connect(saveProjectAction,SIGNAL(triggered()),this,SLOT(save()));

    saveProjectAsAction = new QAction(tr("Save Sound As"),this);
    saveProjectAsAction->setShortcut(QKeySequence("Ctrl+Shift+S"));
    connect(saveProjectAsAction,SIGNAL(triggered()),this,SLOT(saveAs()));

    dataManagerAction = new QAction(tr("Data Manager"),this);
    dataManagerAction->setShortcut(QKeySequence("Ctrl+D"));
    connect(dataManagerAction,SIGNAL(triggered()),this,SLOT(launchDataManager()));

    plotManagerAction = new QAction(tr("Plot Manager"),this);
    plotManagerAction->setShortcut(QKeySequence("Ctrl+M"));
    connect(plotManagerAction,SIGNAL(triggered()),this,SLOT(launchPlotManager()));

    newSoundAction = new QAction(tr("New Sound (new window)"),this);
    newSoundAction->setShortcut(QKeySequence("Ctrl+N"));
    //    MdiArea* mdi = qobject_cast<MdiArea*>(mainWnd->centralWidget());
    //    qDebug() << mainWnd->mdi();
    //    connect(newSoundAction,SIGNAL(triggered()),mainWnd->mdi(),SLOT(newSoundWindow()));

    newRegressionAction = new QAction(tr("New regression"),this);
    connect(newRegressionAction,SIGNAL(triggered()),this,SLOT(newRegression()));
    newRegressionAction->setShortcut(QKeySequence("Ctrl+R"));

    importTextGridAction = new QAction(tr("Import TextGrid"),this);
    connect(importTextGridAction,SIGNAL(triggered()),this,SLOT(importTextGrid()));
    importTextGridAction->setShortcut(QKeySequence("Ctrl+T"));

    fileMenu->addAction(openProjectAction);
    fileMenu->addAction(saveProjectAction);
    fileMenu->addAction(saveProjectAsAction);
    fileMenu->addAction(closeAction);
    fileMenu->addSeparator();
    fileMenu->addAction(importSoundAction);
    fileMenu->addAction(importTextGridAction);
    //    fileMenu->addSeparator();
    //    fileMenu->addAction(newSoundAction);

    optionsMenu->addAction(dataManagerAction);
    optionsMenu->addAction(plotManagerAction);
    optionsMenu->addSeparator();

    visibilityMenu->addSeparator();

    regressionMenu->addAction(newRegressionAction);
    regressionMenu->addSeparator();

    QAction* runScriptAction = new QAction(tr("Run script..."),this);
    scriptingMenu->addAction(runScriptAction);
    connect(runScriptAction,SIGNAL(triggered()),this,SLOT(runScript()));
}

void SoundWidget::setupMenus()
{
    fileMenu = menuBar->addMenu(tr("File"));
    optionsMenu = menuBar->addMenu(tr("Options"));
    visibilityMenu = menuBar->addMenu(tr("Display"));
    visibilityMenu->setEnabled(false);
    regressionMenu = menuBar->addMenu(tr("Regressions"));
    annotationMenu = menuBar->addMenu(tr("Annotations"));
    scriptingMenu = menuBar->addMenu(tr("Scripting"));
}

void SoundWidget::setupScripting()
{
    engine = new QScriptEngine;

    // set up the connections to receive spectrograms & waveforms that plugins emit
    // this is, technically, broader than just the scripting environment, but I want to make sure that this connection is made before the others
    for(int i=0; i < mainWnd->w2s()->count(); i++ )
	connect( mainWnd->w2s()->at(i), SIGNAL(spectrogramCreated(SpectrogramData*)), this, SLOT(addSpectrogram(SpectrogramData*)) );
    for(int i=0; i < mainWnd->s2s()->count(); i++ )
	connect( mainWnd->s2s()->at(i), SIGNAL(spectrogramCreated(SpectrogramData*)), this, SLOT(addSpectrogram(SpectrogramData*)) );
    for(int i=0; i < mainWnd->s2w()->count(); i++ )
	connect( mainWnd->s2w()->at(i), SIGNAL(waveformCreated(WaveformData*)), this, SLOT(addWaveform(WaveformData*)) );
    for(int i=0; i < mainWnd->w2w()->count(); i++ )
	connect( mainWnd->w2w()->at(i), SIGNAL(waveformCreated(WaveformData*)), this, SLOT(addWaveform(WaveformData*)) );

    // update the script environment when the script requests it
    connect(this,SIGNAL(requestScriptDataRefresh()),this,SLOT(setupScriptEnvironment()));

    connect(this,SIGNAL(scriptDataChanged()),this,SLOT(setupScriptEnvironment()));
}

void SoundWidget::regressionMenuAction(QAction *action)
{
    int actiontype = action->data().toInt();
    int index = aRegressionMenus.indexOf((QMenu*)action->parentWidget());

    if(actiontype < 1000) // edit
    {
	RegressionDialog *rd = new RegressionDialog(&aWaveformData, &aSpectrogramData,this);
	rd->setFromRegression(aRegressions.at(index));
	connect(rd,SIGNAL(regressionObject(RegressionModel*)),this,SLOT(addRegression(RegressionModel*)));
	rd->exec();
    }
    else if(actiontype < 2000) // delete
    {
	delete aRegressions.at(index);
	aRegressions.removeAt(index);
	regressionMenu->removeAction(aRegressionMenus.at(index)->menuAction());
	aRegressionMenus.removeAt(index);
    }
}

void SoundWidget::annotationMenuAction(QAction *action)
{
    int actiontype = action->data().toInt();
    int index = aAnnotationMenus.indexOf((QMenu*)action->parentWidget());

    if(actiontype < 1000) // visible
    {
	if(action->isChecked())
	    plotDisplay->annotations()->at(index)->show();
	else
	    plotDisplay->annotations()->at(index)->hide();
    }
    else if(actiontype < 2000) // delete
    {
	delete aIntervalAnnotations.at(index);
	aIntervalAnnotations.removeAt(index);
	annotationMenu->removeAction(aAnnotationMenus.at(index)->menuAction());
	aAnnotationMenus.removeAt(index);

	delete plotDisplay->annotations()->at(index);
	plotDisplay->annotations()->removeAt(index);

	plotDisplay->drawVerticalLayout();
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

    QScriptSyntaxCheckResult check = engine->checkSyntax(code);
    if( check.state() != QScriptSyntaxCheckResult::Valid )
    {
        QMessageBox::critical(this,tr("Script error"),tr("Line %1, Column %2: %3").arg(check.errorLineNumber()).arg(check.errorColumnNumber()).arg(check.errorMessage()));
        return;
    }

    QScriptValue result = engine->evaluate(code,"error.log");
    if (engine->hasUncaughtException()) {
        int line = engine->uncaughtExceptionLineNumber();
        qDebug() << "uncaught exception at line" << line << ":" << result.toString();
    }
}

void SoundWidget::setupScriptEnvironment()
{
    qScriptRegisterMetaType(engine, waveformArrayToScriptValue, waveformArrayFromScriptValue);
    qScriptRegisterMetaType(engine, spectrogramArrayToScriptValue, spectrogramArrayFromScriptValue);

    QStringList waveformNames;
    for(int i=0; i<aWaveformData.count(); i++)
	waveformNames << aWaveformData.at(i)->name();
    engine->globalObject().setProperty("waveformNames", engine->toScriptValue(waveformNames));

    QStringList spectrogramNames;
    for(int i=0; i<aSpectrogramData.count(); i++)
	spectrogramNames << aSpectrogramData.at(i)->name();
    engine->globalObject().setProperty("spectrogramNames", engine->toScriptValue(spectrogramNames));

    engine->globalObject().setProperty("waveforms", engine->toScriptValue(aWaveformData));
    engine->globalObject().setProperty("spectrograms", engine->toScriptValue(aSpectrogramData));

    engine->globalObject().setProperty("widget", engine->newQObject(this));

    // add the w2s plugins as objects
    for(int i=0; i<mainWnd->w2s()->count(); i++)
    {
	QObject *someObject = mainWnd->w2s()->at(i);
	QScriptValue objectValue = engine->newQObject(someObject);
	engine->globalObject().setProperty(mainWnd->w2s()->at(i)->scriptName(), objectValue);
    }

    // add the w2w plugins as objects
    for(int i=0; i<mainWnd->w2w()->count(); i++)
    {
	QObject *someObject = mainWnd->w2w()->at(i);
	QScriptValue objectValue = engine->newQObject(someObject);
	engine->globalObject().setProperty(mainWnd->w2w()->at(i)->scriptName(), objectValue);
    }

    // add the s2s plugins as objects
    for(int i=0; i<mainWnd->s2s()->count(); i++)
    {
	QObject *someObject = mainWnd->s2s()->at(i);
	QScriptValue objectValue = engine->newQObject(someObject);
	engine->globalObject().setProperty(mainWnd->s2s()->at(i)->scriptName(), objectValue);
    }

    // add the s2w plugins as objects
    for(int i=0; i<mainWnd->s2w()->count(); i++)
    {
	QObject *someObject = mainWnd->s2w()->at(i);
	QScriptValue objectValue = engine->newQObject(someObject);
	engine->globalObject().setProperty(mainWnd->s2w()->at(i)->scriptName(), objectValue);

//        qDebug() << "added" << mainWnd->s2w()->at(i)->scriptName() << engine->globalObject().property(mainWnd->s2w()->at(i)->scriptName()).isQObject();
    }
}

void SoundWidget::removeWaveform(int index)
{
    if( !(index < aWaveformData.count()) ) { return; }
    // first delete it from all the plots
    for(int i=0; i<plotDisplay->plotViews()->count(); i++) // every plot
    {
	for(int j=0; j<plotDisplay->plotViews()->at(i)->curves()->count(); j++) // every plot item
	{
	    if( plotDisplay->plotViews()->at(i)->curveData(j) == aWaveformData.at(index) ) // see if we're going to delete this curve's data
	    {
		plotDisplay->plotViews()->at(i)->removeItemAt(j); // if so, remove the curve
		j--; // back up one since the j-th item is one we haven't looked at yet
	    }
	}
    }
    // then delete the data itself
    delete aWaveformData.takeAt(index);
}

void SoundWidget::removeSpectrogram(int index)
{
    if( !(index < aSpectrogramData.count()) ) { return; }
    // first delete it from all the plots
    for(int i=0; i<plotDisplay->plotViews()->count(); i++) // every plot item
    {
	for(int j=0; j<plotDisplay->plotViews()->at(i)->spectrograms()->count(); j++) // every spectrogram
	{
	    if( plotDisplay->plotViews()->at(i)->spectrogramData(j) == aSpectrogramData.at(index) ) // see if we're going to delete this spectrograms's data
	    {
		plotDisplay->plotViews()->at(i)->removeItemAt(j); // if so, remove the spectrogram
		j--; // back up one since the j-th item is one we haven't looked at yet
	    }
	}
    }
    // then delete the data itself
    delete aSpectrogramData.takeAt(index);
}
