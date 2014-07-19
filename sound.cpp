#include "sound.h"

#include <QtWidgets>

#include "waveformdata.h"
#include "spectrogramdata.h"
#include "regression.h"
#include "intervalannotation.h"

Sound::Sound(const QString & filename, QObject *parent) :
    QObject(parent),
    mCurrentFilename(filename),
    mReadState(Sound::NoAttempt)
{
    readFromFile(mCurrentFilename);
}

Sound::~Sound()
{
    qDeleteAll(maWaveformData);
    qDeleteAll(maSpectrogramData);
    qDeleteAll(maRegressions);
    qDeleteAll(maIntervalAnnotations);
}

Sound::ReadState Sound::readState() const
{
    return mReadState;
}

void Sound::readFromFile(const QString & filename)
{
    QFileInfo info(filename);
    if(!info.exists())
    {
        mReadState = Sound::Error;
        return;
    }

    QFile file(filename);
    if(! file.open(QFile::ReadOnly | QFile::Text) )
    {
        mReadState = Sound::Error;
        return;
    }
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

                /// @todo handle this information
                //                mPlotDisplay->setTimeMinMax(tMin,tMax);
                //                mPlotDisplay->setTimeAxes(leftPos,rightPos);
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

                /// @todo handle this information
                //                mPlotDisplay->addPlotView(new PlotViewWidget(name),name);
                //                mPlotDisplay->plotViews()->last()->setHasSecondaryAxis(secondary);
                //                mPlotDisplay->plotViews()->last()->setHeight(height);
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
                    mReadState = Sound::Error;
                    return;
                }

                // where do I set the name? -- it looks like the name is just redundant information
                /// @todo handle this information
                //                mPlotDisplay->plotViews()->last()->addCurveData( maWaveformData.at(index), secondary );

                //                mPlotDisplay->plotViews()->last()->curves()->last()->setPen(QPen(lineColor,lineWidth));
                //                mPlotDisplay->plotViews()->last()->curves()->last()->setStyle((QwtPlotCurve::CurveStyle)lineStyle);
                //                mPlotDisplay->plotViews()->last()->curves()->last()->setRenderHint(QwtPlotItem::RenderAntialiased,antialiased);

                //                QwtSymbol * sym = new QwtSymbol;
                //                sym->setBrush(QBrush(symbolFillColor));
                //                sym->setPen(QPen(symbolColor));
                //                sym->setSize(symbolSize);
                //                sym->setStyle((QwtSymbol::Style)symbolStyle);
                //                mPlotDisplay->plotViews()->last()->curves()->last()->setSymbol(sym);
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

                /// @todo handle this information
                //                mPlotDisplay->plotViews()->last()->addSpectrogramData( maSpectrogramData.at(index) );

                //                QwtLinearScaleEngine engine;
                //                mPlotDisplay->plotViews()->last()->plot()->setAxisScaleDiv(QwtPlot::yLeft,QwtScaleDiv( engine.divideScale( lowerbound, upperbound ,10, 10) ));
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

                /// @todo handle this information
                //                mPlotDisplay->addAnnotation(new IntervalDisplayWidget(maIntervalAnnotations.last(),mPlotDisplay->plotViews()->at(0),0));
                //                addAnnotationMenu(maIntervalAnnotations.last());
            }
            else if(name=="interval")
            {
                maIntervalAnnotations.last()->maIntervals << new Interval(xml.attributes().value("label").toString(), xml.attributes().value("left").toString().toDouble(), xml.attributes().value("right").toString().toDouble());
            }
        }
    }

    if(maWaveformData.length()>0)
    {
        /// @todo handle this information
        //        setWindowTitle(maWaveformData.at(0)->name());
        //        mPlotDisplay->setTimeMinMax(maWaveformData.at(0)->tMin(),maWaveformData.at(0)->tMax());
    }

    mCurrentFilename = filename;

    mReadState = Sound::Success;
}

QString Sound::readXmlElement(QXmlStreamReader &reader, QString elementname)
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

void Sound::addRegression(RegressionModel *regression)
{
    maRegressions << regression;

    /// @todo Recover this functionality
//    int index = maRegressions.count() - 1;
//    QMenu *tmp = mRegressionMenu->addMenu(regression->name());
//    QAction *editRegression = new QAction(tr("Edit"),tmp);
//    editRegression->setData(index);
//    QAction *deleteRegression = new QAction(tr("Delete"),tmp);
//    deleteRegression->setData(1000+index);
//    tmp->addAction(editRegression);
//    tmp->addAction(deleteRegression);
//    maRegressionMenus << tmp;
//    connect(tmp,SIGNAL(triggered(QAction*)),this,SLOT(regressionMenuAction(QAction*)));

    emit scriptDataChanged();
}
