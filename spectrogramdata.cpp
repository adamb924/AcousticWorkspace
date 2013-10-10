#include <qwt_raster_data.h>
#include <qwt_double_rect.h>

#include "fftw3.h"
#include <math.h>

#include "spectrogramdata.h"

#include <QtDebug>
#include <QTime>
#include <QRegExp>

SpectrogramData::SpectrogramData()
{
    data = 0;
    times = 0;
    frequencies = 0;
}

SpectrogramData::SpectrogramData(QString n, double *data, double *times, size_t nFrames, double *frequencies, size_t nFreqBins, double spec_min, double spec_max , double windowLength, double timeStep)
{
    this->label = n;
    safeLabel = n;
    safeLabel.replace(QRegExp("[\\W]*"),"");

    this->data = data;
    this->times = times;
    this->frequencies = frequencies;
    this->spec_min = spec_min;
    this->spec_max = spec_max;
    this->windowLength = windowLength;
    this->timeStep = timeStep;
    this->nFrames = nFrames;
    this->nFreqBins = nFreqBins;

//    qDebug() << "SpectrogramData::SpectrogramData" << boundingRect();
}

SpectrogramData::~SpectrogramData()
{
    if(data) { free(data); }
    if(times){ free(times); }
    if(frequencies) { free(frequencies); }
}

QwtDoubleRect SpectrogramData::boundingRect() const
{
    return QwtDoubleRect( getTimeFromIndex(0) , getFrequencyFromIndex(0), getTimeFromIndex(nFrames-1)-getTimeFromIndex(0), getFrequencyFromIndex(nFreqBins-1)-getFrequencyFromIndex(0) );
}

SpectrogramData* SpectrogramData::copy() const
{
    SpectrogramData *copy = new SpectrogramData;

    copy->windowLength = this->windowLength;
    copy->timeStep = this->timeStep;
    copy->windowLengthInSamples = this->windowLengthInSamples;
    copy->timeStepInSamples = this->timeStepInSamples;
    copy->nFrames = this->nFrames;
    copy->nFreqBins = this->nFreqBins;
    copy->spec_max = this->spec_max;
    copy->safeLabel = this->safeLabel;

    quint32 i;
    copy->times = (double*)malloc(sizeof(double)*nFrames);
    for(i=0; i<nFrames; i++)
    {
	*(copy->times+i) = *(this->times+i);
    }

    copy->frequencies = (double*)malloc(sizeof(double)*nFreqBins);
    for(i=0; i<nFreqBins; i++)
    {
	*(copy->frequencies+i) = *(this->frequencies+i);
    }

    copy->data = (double*)malloc(sizeof(double)*nFreqBins*nFrames);
    for(i=0; i<nFreqBins*nFrames; i++)
    {
	*(copy->data+i) = *(this->data+i);
    }

    return copy;
}

QwtDoubleInterval SpectrogramData::range() const
{
    return QwtDoubleInterval(spec_min, spec_max);
}

double SpectrogramData::dataAt(quint32 t, quint32 f) const
{
    Q_CHECK_PTR(data);
    return *(data + t*nFreqBins + f);
}

double SpectrogramData::flatdata(quint32 i) const
{
    Q_CHECK_PTR(data);
    return *(data+i);
}

double* SpectrogramData::pdata() const
{
    Q_CHECK_PTR(data);
    return data;
}

double* SpectrogramData::pfrequencies() const
{
    Q_CHECK_PTR(frequencies);
    return frequencies;
}


bool SpectrogramData::inTimeRange(double t) const
{
    if( t <= *(times+nFrames-1) && t >= *(times) )
	return true;
    else
	return false;
}

bool SpectrogramData::inFrequencyRange(double f) const
{
    if( f >= 0 && f <= *(frequencies+nFreqBins-1) )
	return true;
    else
	return false;
}

quint32 SpectrogramData::timeStepAbove(double t) const
{
    if(!inTimeRange(t)) { return 0; }
    for(quint32 i=0; i<nFrames; i++)
    {
	if( *(times+i) > t ) {return i;}
    }
    return 0;
}

quint32 SpectrogramData::timeStepBelow(double t) const
{
    //	if(!inTimeRange(t)) { return 0; }
    for(quint32 i=0; i<nFrames; i++)
    {
	if( *(times+i) > t ) {return i-1;}
    }
    return 0;
}

quint32 SpectrogramData::frequencyBinAbove(double t) const
{
    //	if(!inFrequencyRange(t)) { return 0; }
    for(quint32 i=0; i<nFreqBins; i++)
    {
	if( *(frequencies+i) > t ) {return i;}
    }
    return nFreqBins-1;
}

quint32 SpectrogramData::frequencyBinBelow(double t) const
{
    //	if(!inFrequencyRange(t)) { return 0; }
    for(quint32 i=0; i<nFreqBins; i++)
    {
	if( *(frequencies+i) > t ) {return i-1;}
    }
    return nFreqBins-1;
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

    double x1 = *(times + ix1);
    double x2 = *(times + ix2);
    double y1 = *(frequencies + iy1);
    double y2 = *(frequencies + iy2);
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
    return label;
}

QString SpectrogramData::safeName() const
{
    return safeLabel;
}

void SpectrogramData::setName(QString n)
{
    label = n;
    safeLabel = n;
    safeLabel.replace(QRegExp("[\\W]*"),"");
}

double SpectrogramData::getWindowLength() const
{
    return windowLength;
}

double SpectrogramData::getTimeStep() const
{
    return timeStep;
}

quint32 SpectrogramData::getNTimeSteps() const
{
    return nFrames;
}

quint32 SpectrogramData::getNFrequencyBins() const
{
    return nFreqBins;
}

double SpectrogramData::getTimeFromIndex(int i) const
{
    Q_CHECK_PTR(times);
    return *(times+i);
}

double SpectrogramData::getFrequencyFromIndex(int i) const
{
    return *(frequencies+i);
}
