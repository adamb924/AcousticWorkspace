#include "interfaces.h"
#include <QMessageBox>
#include <QFileInfo>
#include <QtDebug>

WaveformData::WaveformData(QString name, double *x, double *y, size_t nsam, size_t fs) :
    QObject(), QwtPointArrayData(x, y, nsam),
    mLabel(name),
    mFs(fs)
{
    mSafeLabel = mLabel;
    mSafeLabel.replace(QRegExp("[\\W]*"),"");

    mPeriod = 1.0 / fs;

    calculateMinMax();
}

WaveformData::WaveformData(const WaveformData& other) : QObject(), QwtPointArrayData(other)
{
    mLabel = other.mLabel;
    mFs = other.mFs;
    mPeriod = other.mPeriod;
    mMaximum = other.mMaximum;
    mMinimum = other.mMinimum;
    mSafeLabel = other.mSafeLabel;
}

void WaveformData::calculateMinMax()
{
    mMinimum = 99999999999.0f;
    mMaximum = -99999999999.0f;
    for(quint32 i=0; i<yData().size(); i++)
    {
        if( yData().at(i) < mMinimum ) { mMinimum = yData().at(i); }
        if( yData().at(i) > mMaximum ) { mMaximum = yData().at(i); }
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
    if(mTimes != 0) { free(mTimes); }
    mTimes = x;
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
    return (double)mFs;
}

double WaveformData::getNyquistFrequency() const
{
    return floor((double)mFs/2);
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
    return QRectF( tMin() , mMinimum, tMax()-tMin(), mMaximum-mMinimum );
}

bool WaveformData::checkCongruentWith(const WaveformData *other) const
{
    if( tMin() == other->tMin() && tMax() == other->tMax() && getNSamples() == other->getNSamples() )
	return true;
    else
	return false;
}

void WaveformData::setName(QString n)
{
    mLabel = n;
    mSafeLabel = n;
    mSafeLabel.replace(QRegExp("[\\W]*"),"");
//    qDebug() << label << safeLabel;
}
