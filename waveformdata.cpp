#include "interfaces.h"
#include <QMessageBox>
#include <QFileInfo>
#include <QtDebug>

WaveformData::WaveformData(QString name, double *x, double *y, size_t nsam, size_t fs) : QObject(), QwtPointArrayData(x, y, nsam)
{
    this->label = name;
    this->fs = fs;

    safeLabel = label;
    safeLabel.replace(QRegExp("[\\W]*"),"");

    period = 1.0 / fs;

    calculateMinMax();
}

WaveformData::WaveformData(const WaveformData& other) : QObject(), QwtPointArrayData(other)
{
    label = other.label;
    fs = other.fs;
    period = other.period;
    maximum = other.maximum;
    minimum = other.minimum;
    safeLabel = other.safeLabel;
}

void WaveformData::calculateMinMax()
{
    minimum = 99999999999.0f;
    maximum = -99999999999.0f;
    for(quint32 i=0; i<yData().size(); i++)
    {
        if( yData().at(i) < minimum ) { minimum = yData().at(i); }
        if( yData().at(i) > maximum ) { maximum = yData().at(i); }
    }
}

QPointF WaveformData::sample(size_t i) const
{
    return QPointF( xData().at(i) , yData().at(i) );
}

size_t WaveformData::size() const
{
    return xData().size();
}

void WaveformData::setXData(double *x)
{
    // TODO this is out of date. It's only used in comparisonwidget.cpp, so I don't know if that can be gotten around.
    if(times != 0) { free(times); }
    times = x;
}

quint32 WaveformData::getNSamples() const
{
    return xData().size();
}

double WaveformData::tMin() const
{
    return xData().first();
}

double WaveformData::tMax() const
{
    return xData().last();
}

double WaveformData::length() const
{
    return tMax() - tMin();
}

double WaveformData::getSamplingFrequency() const
{
    return (double)fs;
}

double WaveformData::getNyquistFrequency() const
{
    return floor((double)fs/2);
}

size_t WaveformData::getSampleFromTime(double time)
{
    if( time <= tMin() ) { return 0; }
    if( time >= tMax() ) { return xData().size()-1-1; }
    for(size_t i=0; i<xData().size()-1; i++)
        if( xData().at(i+1) > time )
            return i;
    return 99999999;
}

QRectF WaveformData::boundingRect() const
{
    return QRectF( tMin() , minimum, tMax()-tMin(), maximum-minimum );
}

bool WaveformData::checkCongruentWith(const WaveformData *other) const
{
    if( this->tMin() == other->tMin() && this->tMax() == other->tMax() && this->getNSamples() == other->getNSamples() )
	return true;
    else
	return false;
}

void WaveformData::setName(QString n)
{
    label = n;
    safeLabel = n;
    safeLabel.replace(QRegExp("[\\W]*"),"");
//    qDebug() << label << safeLabel;
}
