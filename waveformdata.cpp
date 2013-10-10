#include "interfaces.h"
#include <QMessageBox>
#include <QFileInfo>
#include <QtDebug>

WaveformData::WaveformData() { }

WaveformData::WaveformData(QString name, double *x, double *y, size_t nsam, size_t fs)
{
    this->label = name;
    this->times = x;
    this->data = y;
    this->nsam = nsam;
    this->fs = fs;

    safeLabel = label;
    safeLabel.replace(QRegExp("[\\W]*"),"");

    period = 1.0 / fs;
    len = *(times+nsam-1) - *(times);

    calculateMinMax();
}

WaveformData* WaveformData::copy() const
{
    WaveformData *copy = new WaveformData;
    copy->label = label;
    copy->fs = fs;
    copy->nsam = nsam;
    copy->period = period;
    copy->len = len;
    copy->maximum = maximum;
    copy->minimum = minimum;
    copy->safeLabel = safeLabel;

    copy->times = (double*)malloc(sizeof(double)*nsam);
    Q_CHECK_PTR(copy->times);
    for(quint32 i=0; i < nsam; i++)
    {
	*(copy->times+i) = *(times+i);
    }

    copy->data = (double*)malloc(sizeof(double)*nsam);
    Q_CHECK_PTR(copy->data);
    for(quint32 i=0; i < nsam; i++)
    {
	*(copy->data+i) = *(data+i);
    }

    return copy;
}

void WaveformData::calculateMinMax()
{
    minimum = 99999999999.0f;
    maximum = -99999999999.0f;
    for(quint32 i=0; i<nsam; i++)
    {
	if( *(data+i) < minimum ) { minimum = *(data+i); }
	if( *(data+i) > maximum ) { maximum = *(data+i); }
    }
}

double* WaveformData::xData() const
{
    return times;
}

double* WaveformData::yData() const
{
    return data;
}

double WaveformData::x(size_t i) const
{
    return *(times+i);
}

double WaveformData::y(size_t i) const
{
    return *(data+i);
}

size_t WaveformData::size() const
{
    return nsam;
}

void WaveformData::setXData(double *x)
{
    if(times != 0) { free(times); }
    times = x;
}

void WaveformData::setXDataAt(int index, double x)
{
    *(times+index) = x;
}

void WaveformData::setYData(double *y)
{
    if(data != 0) { free(data); }
    data = y;
    calculateMinMax();
}



quint32 WaveformData::getNSamples() const
{
    return nsam;
}

double WaveformData::tMin() const
{
    Q_CHECK_PTR(times);
    return *(times+0);
}

double WaveformData::tMax() const
{
    Q_CHECK_PTR(times);
    return *(times+nsam-1);
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

double WaveformData::getYAtSample(quint32 i) const
{
    Q_CHECK_PTR(data);
    if(i < nsam)
    {
	return *(data+i);
    }
    else
    {
	return -999.0f;
    }
}

double WaveformData::getTimeAtSample(quint32 i) const
{
    Q_CHECK_PTR(times);
    if(i < nsam)
    {
	return *(times+i);
    }
    else
    {
	return -999.0f;
    }
}

size_t WaveformData::getSampleFromTime(double time)
{
    if( time <= tMin() ) { return 0; }
    if( time >= tMax() ) { return nsam-1; }
    for(size_t i=0; i<nsam-1; i++)
	if( getTimeAtSample(i+1) > time )
	    return i;
    return 99999999;
}

QwtDoubleRect WaveformData::boundingRect() const
{
    return QwtDoubleRect( tMin() , minimum, tMax()-tMin(), maximum-minimum );
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
