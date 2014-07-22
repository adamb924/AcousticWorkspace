#include "sound.h"

#include <QtWidgets>

#include "curveparameters.h"
#include "spectrogramparameters.h"

#include "waveformdata.h"
#include "spectrogramdata.h"
#include "regression.h"
#include "intervalannotation.h"

Sound::Sound(const QString & filename, QObject *parent) :
    QObject(parent),
    mFilename(filename),
    mReadState(Sound::NoAttempt)
{
    readFromFile(mFilename);
}

Sound::Sound(WaveformData *sound, QObject *parent) :
    QObject(parent),
    mReadState(Sound::Success)
{
    addWaveform(sound);
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

QString Sound::name() const
{
    return QFileInfo(mFilename).fileName();
}

QList<WaveformData *> *Sound::waveformData()
{
    return &maWaveformData;
}

QList<SpectrogramData *> *Sound::spectrogramData()
{
    return &maSpectrogramData;
}

const QList<WaveformData *> *Sound::waveformData() const
{
    return &maWaveformData;
}

const QList<SpectrogramData *> *Sound::spectrogramData() const
{
    return &maSpectrogramData;
}

const QList<IntervalAnnotation *> *Sound::intervals() const
{
    return &maIntervalAnnotations;
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

                mSoundView.setTMax(tMax);
                mSoundView.setTMin(tMin);
                mSoundView.setLeftPos(leftPos);
                mSoundView.setRightPos(rightPos);
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
                }

                maSpectrogramData << new SpectrogramData(name, data, times, nFrames, frequencies, nFreqBins, windowLength, timeStep);
            }
            else if( name == "plot" )
            {
                QString name = xml.attributes().value("name").toString();
                bool secondary = xml.attributes().value("secondary-axis").toString().toInt();
                int height = xml.attributes().value("height").toString().toInt();

                mSoundView.plotParameters()->append( new PlotParameters );
                mSoundView.plotParameters()->last()->setName(name);
                mSoundView.plotParameters()->last()->setHasSecondaryAxis(secondary);
                mSoundView.plotParameters()->last()->setHeight(height);
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

                CurveParameters *cp = new CurveParameters;
                cp->setWaveformData(maWaveformData.at(index));
                cp->setIsSecondary(secondary);
                cp->setPen(QPen(lineColor,lineWidth));
                cp->setCurveStyle((QwtPlotCurve::CurveStyle)lineStyle);
                cp->setAntialiased(antialiased);
                cp->setSymbolBrush(QBrush(symbolFillColor));
                cp->setSymbolPen(QPen(symbolColor));
                cp->setSymbolSize(symbolSize);
                cp->setSymbolStyle((QwtSymbol::Style)symbolStyle);
                mSoundView.plotParameters()->last()->curveParameters()->append(cp);
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

                SpectrogramParameters *sp = new SpectrogramParameters;
                sp->setName(name);
                sp->setLowerBound(lowerbound);
                sp->setUpperBound(upperbound);
                sp->setSpectrogramData(maSpectrogramData.at(index));
                mSoundView.plotParameters()->last()->spectrogramParameters()->append(sp);
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
            }
            else if(name=="interval")
            {
                maIntervalAnnotations.last()->maIntervals << new Interval(xml.attributes().value("label").toString(), xml.attributes().value("left").toString().toDouble(), xml.attributes().value("right").toString().toDouble());
            }
        }
    }

    mFilename = filename;

    mReadState = Sound::Success;
}

void Sound::writeProjectToFile(const QString & filename)
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

    /// @todo replace this functionality
    //    xs.writeTextElement("time-max",QString::number(mPlotDisplay->getTMax()));
    //    xs.writeTextElement("time-min",QString::number(mPlotDisplay->getTMin()));
    //    xs.writeTextElement("left-position",QString::number(mPlotDisplay->getLeftPos()));
    //    xs.writeTextElement("right-position",QString::number(mPlotDisplay->getRightPos()));

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

    /// @todo replace this functionality
    //    xs.writeStartElement("plots");
    //    for(int i=0; i<mPlotDisplay->plotViews()->count(); i++)
    //    {
    //    xs.writeStartElement("plot");
    //    xs.writeAttribute("id",QString::number(i));
    //    xs.writeAttribute("name",mPlotDisplay->plotViews()->at(i)->name());
    //    if( mPlotDisplay->plotViews()->at(i)->hasSecondaryAxis() )
    //        xs.writeAttribute("secondary-axis", "1" );
    //    else
    //        xs.writeAttribute("secondary-axis", "0" );
    //    xs.writeAttribute("height",QString::number(mPlotDisplay->plotViews()->at(i)->height()));

    //    for(int j=0; j< mPlotDisplay->plotViews()->at(i)->spectrograms()->length(); j++)
    //    {
    //        xs.writeStartElement("spectrogram-plot");
    //        xs.writeAttribute("index", QString::number( maSpectrogramData.indexOf(mPlotDisplay->plotViews()->at(i)->spectrogramData(j) ) ) );
    //        xs.writeAttribute("name", mPlotDisplay->plotViews()->at(i)->spectrogramData(j)->name());

    //        xs.writeTextElement("frequency-lower-bound",QString::number( mPlotDisplay->plotViews()->at(i)->plot()->axisScaleDiv(QwtPlot::yLeft).lowerBound() ));
    //        xs.writeTextElement("frequency-upper-bound",QString::number( mPlotDisplay->plotViews()->at(i)->plot()->axisScaleDiv(QwtPlot::yLeft).upperBound() ));
    //        xs.writeEndElement();
    //    }

    //    for(int j=0; j< mPlotDisplay->plotViews()->at(i)->curves()->length(); j++)
    //    {
    //        xs.writeStartElement("curve");
    //        xs.writeAttribute("index",QString::number( maWaveformData.indexOf(mPlotDisplay->plotViews()->at(i)->curveData(j)) ));
    //        xs.writeAttribute("name", mPlotDisplay->plotViews()->at(i)->curveData(j)->name() );
    //        if( mPlotDisplay->plotViews()->at(i)->curves()->at(j)->yAxis() == QwtPlot::yLeft )
    //        xs.writeAttribute("secondary-axis", "0" );
    //        else
    //        xs.writeAttribute("secondary-axis", "1" );

    //        xs.writeEmptyElement("symbol-color");
    //        xs.writeAttribute("r",QString::number(mPlotDisplay->plotViews()->at(i)->curves()->at(j)->symbol()->pen().color().red()));
    //        xs.writeAttribute("g",QString::number(mPlotDisplay->plotViews()->at(i)->curves()->at(j)->symbol()->pen().color().green()));
    //        xs.writeAttribute("b",QString::number(mPlotDisplay->plotViews()->at(i)->curves()->at(j)->symbol()->pen().color().blue()));

    //        xs.writeEmptyElement("symbol-fill-color");
    //        xs.writeAttribute("r",QString::number(mPlotDisplay->plotViews()->at(i)->curves()->at(j)->symbol()->brush().color().red()));
    //        xs.writeAttribute("g",QString::number(mPlotDisplay->plotViews()->at(i)->curves()->at(j)->symbol()->brush().color().green()));
    //        xs.writeAttribute("b",QString::number(mPlotDisplay->plotViews()->at(i)->curves()->at(j)->symbol()->brush().color().blue()));

    //        xs.writeTextElement("symbol-style",QString::number(mPlotDisplay->plotViews()->at(i)->curves()->at(j)->symbol()->style()));

    //        xs.writeTextElement("symbol-size",QString::number(mPlotDisplay->plotViews()->at(i)->curves()->at(j)->symbol()->size().height()));

    //        xs.writeEmptyElement("line-color");
    //        xs.writeAttribute("r",QString::number(mPlotDisplay->plotViews()->at(i)->curves()->at(j)->pen().color().red()));
    //        xs.writeAttribute("g",QString::number(mPlotDisplay->plotViews()->at(i)->curves()->at(j)->pen().color().green()));
    //        xs.writeAttribute("b",QString::number(mPlotDisplay->plotViews()->at(i)->curves()->at(j)->pen().color().blue()));

    //        xs.writeTextElement("line-style",QString::number(mPlotDisplay->plotViews()->at(i)->curves()->at(j)->style()));

    //        xs.writeTextElement("line-width",QString::number(mPlotDisplay->plotViews()->at(i)->curves()->at(j)->pen().width()));

    //        xs.writeTextElement("antialiased",QString::number(mPlotDisplay->plotViews()->at(i)->curves()->at(j)->testRenderHint(QwtPlotItem::RenderAntialiased)));

    //        xs.writeEndElement();
    //    }

    //    xs.writeEndElement(); // plot
    //    }
    //    xs.writeEndElement(); // plots

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

void Sound::addSpectrogram(SpectrogramData *data)
{
    maSpectrogramData << data;
    emit scriptDataChanged();
}

void Sound::addWaveform(WaveformData *data)
{
    maWaveformData << data;
    emit scriptDataChanged();
}

void Sound::readTextGridFromFile(const QString & fileName)
{
    int count = maIntervalAnnotations.count();
    QFile data(fileName);
    if (data.open(QFile::ReadOnly))
    {
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

    /// @todo Update this functionality
//    for(int i=count; i<maIntervalAnnotations.count(); i++)
//    {
//        mPlotDisplay->addAnnotation(new IntervalDisplayWidget(maIntervalAnnotations.at(i),mPlotDisplay->plotViews()->first(),this));
//        addAnnotationMenu(maIntervalAnnotations.at(i));
    //    }
}

const SoundView *Sound::soundView() const
{
    return &mSoundView;
}

QString Sound::filename() const
{
    return mFilename;
}
