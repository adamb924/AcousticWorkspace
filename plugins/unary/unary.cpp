#include <QtGui>
#include <QtDebug>

#include "unary.h"
#include "dataentrydialog.h"

#include <waveformdata.h>

UnaryPlugin::UnaryPlugin()
{
//    sound = data;
    pluginnames << "Log10";
    pluginnames << "Ln";
    pluginnames << "Negative";

}

UnaryPlugin::~UnaryPlugin()
{
}

QString UnaryPlugin::name() const
{
    return "Unary Operations Library";
}

QString UnaryPlugin::scriptName() const
{
	return "unaryOperationsLibrary";
}

QStringList UnaryPlugin::names() const
{
    return pluginnames;
}

UnaryPlugin* UnaryPlugin::copy() const
{
    // this needs to be better
    return new UnaryPlugin();
}

void UnaryPlugin::settings(int i)
{
    Q_UNUSED(i);
}

void UnaryPlugin::calculate(QString name, WaveformData *data)
{
    int index = pluginnames.indexOf(name);
    if(index != -1)
	calculate(index, data);
}

void UnaryPlugin::calculate(int index, WaveformData *data)
{
    QString suggested_label;

    size_t nframes = data->getNSamples();
    double *values = (double*)malloc(sizeof(double)*nframes);
    double *times = (double*)malloc(sizeof(double)*nframes);
    double samplingFreq = data->getSamplingFrequency();

    switch(index)
    {
    case 0: // log 10
	for(quint32 i=0; i<nframes; i++)
	{
        *(values+i) = log10(data->yData()[i]);
        *(times+i) = data->xData()[i];
	}
	suggested_label = "log10(" + data->name() + ")";
	break;
    case 1: // ln
	for(quint32 i=0; i<nframes; i++)
	{
        *(values+i) = log(data->yData()[i]);
        *(times+i) = data->xData()[i];
	}
	suggested_label = "ln(" + data->name() + ")";
	break;
    case 2: // negative
	for(quint32 i=0; i<nframes; i++)
	{
        *(values+i) = -1*data->yData()[i];
        *(times+i) = data->xData()[i];
	}
	suggested_label = "neg(" + data->name() + ")";
	break;
    }

    emit waveformCreated(new WaveformData(suggested_label, times, values, nframes, samplingFreq));
}

void UnaryPlugin::setParameter(QString label, QVariant value)
{
    Q_UNUSED(label);
    Q_UNUSED(value);
}

