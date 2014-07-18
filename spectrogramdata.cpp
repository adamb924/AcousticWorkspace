#include <qwt_raster_data.h>

#include <math.h>

#include "spectrogramdata.h"

#include <QtDebug>
#include <QTime>
#include <QRegExp>

SpectrogramData::SpectrogramData() : mData(0), mTimes(0), mFrequencies(0), mSpecMin(-1.0f), mSpecMax(-1.0f), mWindowLength(-1.0f), mTimeStep(-1.0f)
{
}

SpectrogramData::SpectrogramData(QString n, double *data, double *times, size_t nFrames, double *frequencies, size_t nFreqBins, double spec_min, double spec_max , double windowLength, double timeStep)
     : mLabel(n), mData(data), mTimes(times), mFrequencies(frequencies), mSpecMin(spec_min), mSpecMax(spec_max), mWindowLength(windowLength), mTimeStep(timeStep), mNFrames(nFrames), mNFreqBins(nFreqBins)
{
    mSafeLabel = n;
    mSafeLabel.replace(QRegExp("[\\W]*"),"");
}

SpectrogramData::~SpectrogramData()
{
    if(mData) { free(mData); }
    if(mTimes){ free(mTimes); }
    if(mFrequencies) { free(mFrequencies); }
}

QRectF SpectrogramData::boundingRect() const
{
    return QRectF( getTimeFromIndex(0) , getFrequencyFromIndex(0), getTimeFromIndex(mNFrames-1)-getTimeFromIndex(0), getFrequencyFromIndex(mNFreqBins-1)-getFrequencyFromIndex(0) );
}

SpectrogramData* SpectrogramData::copy() const
{
    SpectrogramData *copy = new SpectrogramData;

    copy->mWindowLength = mWindowLength;
    copy->mTimeStep = mTimeStep;
    copy->mWindowLengthInSamples = mWindowLengthInSamples;
    copy->mTimeStepInSamples = mTimeStepInSamples;
    copy->mNFrames = mNFrames;
    copy->mNFreqBins = mNFreqBins;
    copy->mSpecMax = mSpecMax;
    copy->mSafeLabel = mSafeLabel;

    quint32 i;
    copy->mTimes = (double*)malloc(sizeof(double)*mNFrames);
    for(i=0; i<mNFrames; i++)
    {
	*(copy->mTimes+i) = *(mTimes+i);
    }

    copy->mFrequencies = (double*)malloc(sizeof(double)*mNFreqBins);
    for(i=0; i<mNFreqBins; i++)
    {
	*(copy->mFrequencies+i) = *(mFrequencies+i);
    }

    copy->mData = (double*)malloc(sizeof(double)*mNFreqBins*mNFrames);
    for(i=0; i<mNFreqBins*mNFrames; i++)
    {
	*(copy->mData+i) = *(mData+i);
    }

    return copy;
}

double SpectrogramData::dataAt(quint32 t, quint32 f) const
{
    Q_CHECK_PTR(mData);
    return *(mData + t*mNFreqBins + f);
}

double SpectrogramData::flatdata(quint32 i) const
{
    Q_CHECK_PTR(mData);
    return *(mData+i);
}

double* SpectrogramData::pdata() const
{
    Q_CHECK_PTR(mData);
    return mData;
}

double* SpectrogramData::pfrequencies() const
{
    Q_CHECK_PTR(mFrequencies);
    return mFrequencies;
}


bool SpectrogramData::inTimeRange(double t) const
{
    if( t <= *(mTimes+mNFrames-1) && t >= *(mTimes) )
	return true;
    else
	return false;
}

bool SpectrogramData::inFrequencyRange(double f) const
{
    if( f >= 0 && f <= *(mFrequencies+mNFreqBins-1) )
	return true;
    else
	return false;
}

quint32 SpectrogramData::timeStepAbove(double t) const
{
    if(!inTimeRange(t)) { return 0; }
    for(quint32 i=0; i<mNFrames; i++)
    {
	if( *(mTimes+i) > t ) {return i;}
    }
    return 0;
}

quint32 SpectrogramData::timeStepBelow(double t) const
{
    //	if(!inTimeRange(t)) { return 0; }
    for(quint32 i=0; i<mNFrames; i++)
    {
	if( *(mTimes+i) > t ) {return i-1;}
    }
    return 0;
}

quint32 SpectrogramData::frequencyBinAbove(double t) const
{
    //	if(!inFrequencyRange(t)) { return 0; }
    for(quint32 i=0; i<mNFreqBins; i++)
    {
	if( *(mFrequencies+i) > t ) {return i;}
    }
    return mNFreqBins-1;
}

quint32 SpectrogramData::frequencyBinBelow(double t) const
{
    //	if(!inFrequencyRange(t)) { return 0; }
    for(quint32 i=0; i<mNFreqBins; i++)
    {
	if( *(mFrequencies+i) > t ) {return i-1;}
    }
    return mNFreqBins-1;
}

double SpectrogramData::value(double x, double y) const
{
    if(!inTimeRange(x) || !inFrequencyRange(y) ) { return 0.0f; }
    return dataAt(timeStepBelow(x),frequencyBinBelow(y));
}

double SpectrogramData::bilinearInterpolation(double x, double y) const
{
    // http://en.wikipedia.org/wiki/Bilinear_interpolation
    quint32 ix1 = timeStepBelow(x);
    quint32 ix2 = timeStepAbove(x);
    quint32 iy1 = frequencyBinBelow(y);
    quint32 iy2 = frequencyBinAbove(y);

    if( ix1==0xff || ix2==0xff || iy1==0xff || iy2==0xff ) { return 0.0f; }

    double x1 = *(mTimes + ix1);
    double x2 = *(mTimes + ix2);
    double y1 = *(mFrequencies + iy1);
    double y2 = *(mFrequencies + iy2);
    double q11 = dataAt(ix1,iy1);
    double q12 = dataAt(ix1,iy2);
    double q21 = dataAt(ix2,iy1);
    double q22 = dataAt(ix2,iy2);

    return (q11*(x2-x)*(y2-y))/((x2-x1)*(y2-y1)) +
	    (q21*(x-x1)*(y2-y))/((x2-x1)*(y2-y1)) +
	    (q12*(x2-x)*(y-y1))/((x2-x1)*(y2-y1)) +
	    (q22*(x-x1)*(y-y1))/((x2-x1)*(y2-y1));
}

QString SpectrogramData::name() const
{
    return mLabel;
}

QString SpectrogramData::safeName() const
{
    return mSafeLabel;
}

void SpectrogramData::setName(QString n)
{
    mLabel = n;
    mSafeLabel = n;
    mSafeLabel.replace(QRegExp("[\\W]*"),"");
}

double SpectrogramData::getWindowLength() const
{
    return mWindowLength;
}

double SpectrogramData::getTimeStep() const
{
    return mTimeStep;
}

quint32 SpectrogramData::getNTimeSteps() const
{
    return mNFrames;
}

quint32 SpectrogramData::getNFrequencyBins() const
{
    return mNFreqBins;
}

double SpectrogramData::getTimeFromIndex(int i) const
{
    Q_CHECK_PTR(mTimes);
    return *(mTimes+i);
}

double SpectrogramData::getFrequencyFromIndex(int i) const
{
    return *(mFrequencies+i);
}
