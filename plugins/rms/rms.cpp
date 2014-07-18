#include <QtGui>
#include <QtDebug>

#include "rms.h"
#include "dataentrydialog.h"
#include <waveformdata.h>

RmsPlugin::RmsPlugin()
{
//    sound = data;
    pluginnames << "RMS";

    settingsLabels << "Window length (ms)";
    settingsValues << 7.5;

    settingsLabels << "Time step (ms)";
    settingsValues << 0.35;

    times = 0;
    values = 0;
}

QString RmsPlugin::name() const
{
    return "RMS Library";
}

QStringList RmsPlugin::names() const
{
    return pluginnames;
}

RmsPlugin* RmsPlugin::copy() const
{
    // this needs to be better
    return new RmsPlugin();
}

void RmsPlugin::settings(int i)
{
    Q_UNUSED(i);
    DataEntryDialog dew(&settingsLabels, &settingsValues, "", 0);
    if( dew.exec() == QDialog::Accepted)
    {
	for(int i=0; i<settingsValues.count(); i++)
	{
	    settingsValues.replace(i, dew.values()->at(i));
	}
    }
}

void RmsPlugin::calculate(int i, WaveformData *data)
{
    Q_UNUSED(i);
    double windowLength = settingsValues.at(0).toDouble()/1000.0f;
    double timeStep = settingsValues.at(1).toDouble() / 1000.0f;
    quint32 s_wl = windowLength * data->getSamplingFrequency();
    quint32 s_ts = timeStep * data->getSamplingFrequency();

    nframes = (quint32)floor( (data->length()-windowLength) / timeStep );
    if( floor( (data->length()-windowLength) / windowLength ) == ( (data->length()-windowLength) / windowLength )) { nframes++; } // if there happened to be exactly one windowLenth left over

//    qDebug() << data->length() << windowLength << nframes << data->ns() << s_wl;

    double *times = (double*)malloc(sizeof(double)*nframes);
    double *values = (double*)malloc(sizeof(double)*nframes);

    double rms;
    for(quint32 i=0; i<nframes; i ++) // i is expressed in time steps
    {
	*(times+i) = windowLength/2 + i*timeStep;

	rms = 0.0f;
	for(quint32 j=0; j<s_wl; j++)
	{
	    quint32 tmp = i*s_ts+j;
        rms += data->yData()[tmp] * data->yData()[tmp];
	}
	*(values+i) = sqrt(rms/s_wl);
    }

    QString suggested_label = "RMS WL:" + settingsValues.at(0).toString() + " TS:" + settingsValues.at(1).toString();

    emit waveformCreated( new WaveformData(suggested_label, times, values, nframes, (size_t)1.0f/windowLength ) );
}

QString RmsPlugin::scriptName() const
{
    return "rmsLibrary";
}

void RmsPlugin::calculate(QString name, WaveformData *data)
{
    int index = pluginnames.indexOf(name);
    if(index != -1)
	calculate(index, data);
}

void RmsPlugin::setParameter(QString label, QVariant value)
{
    int index = settingsLabels.indexOf(label);
    if(index != -1)
	settingsValues[index] = value;
}

