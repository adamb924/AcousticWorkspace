#include <QtGui>
#include <QtDebug>

#include <waveformdata.h>
#include <spectrogramdata.h>

#include "spectralchange.h"
#include <dataentrydialog.h>
#include <fftw3.h>

SpectralChangePlugin::SpectralChangePlugin()
{
//    sound = data;

    pluginnames << "Spectral Change";

    settingsLabels << "From cepstral coefficient...";
    settingsValues << 1;
    settingsLabels << "To cepstral coefficient...";
    settingsValues << 10;
    settingsLabels << "N frames";
    settingsValues << 5;
}

SpectralChangePlugin::~SpectralChangePlugin()
{
}


QString SpectralChangePlugin::name() const
{
    return "Spectral Change Library";
}

QString SpectralChangePlugin::scriptName() const
{
    return "spectralChangeLibrary";
}

QStringList SpectralChangePlugin::names() const
{
    return pluginnames;
}

SpectralChangePlugin* SpectralChangePlugin::copy() const
{
    // this needs to be better
    return new SpectralChangePlugin();
}

void SpectralChangePlugin::settings(int i)
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

void SpectralChangePlugin::calculate(QString name, SpectrogramData *data)
{
    int index = pluginnames.indexOf(name);
    if(index != -1)
	calculate(index, data);
}


void SpectralChangePlugin::calculate(int index, SpectrogramData *data)
{
    Q_UNUSED(index);
    int fromCC = settingsValues.at(0).toInt();
    int toCC = settingsValues.at(1).toInt();
    int N = settingsValues.at(2).toInt();
    int nFrames = data->getNTimeSteps() - 2*N;

    QList<WaveformData*> retval;

    if(data->getNTimeSteps() <  (unsigned)(2*N + 1)) { return; }

    double *times = (double*)malloc(sizeof(double)*nFrames);
    double *values = (double*)malloc(sizeof(double)*nFrames);

    // waste a few bytes to keep things pretty
    double *a = (double*)malloc(sizeof(double)*toCC);
    for(int i=0; i<nFrames; i++)
    {
	*(times+i) = data->getTimeFromIndex(N+i); // was timeAt

	for(int j=fromCC; j<=toCC; j++) // for each cepstral coefficient
	{
	    *(a + j) = 0;
	    double denominator = 0;
	    for(int k= -1*N; k <= N; k++) // over the span of the window
	    {
		*(a + j) +=  data->dataAt(i+N+k,j)*k; // Ci(n) * n
		denominator += k*k;
	    }
	    *(a + j) /= denominator;
	}

	*(values+i) = 0;
	for(int j=fromCC; j<=toCC; j++)
	    *(values+i) += *(a+j) * *(a+j);
	*(values+i) /= toCC-fromCC+1;
    }
    free(a);

    QString suggestedName = "Furui86 CC" + settingsValues.at(0).toString() + "-" + settingsValues.at(1).toString() + " N:" + settingsValues.at(2).toString();
    double samplingFreq = 1 / (data->getTimeFromIndex(1)-data->getTimeFromIndex(0));
/*
    retval << new WaveformData(suggestedName, times, values, nFrames, samplingFreq);
    return retval;
*/
    emit waveformCreated(new WaveformData(suggestedName, times, values, nFrames, samplingFreq));
}

void SpectralChangePlugin::setParameter(QString label, QVariant value)
{
    int index = settingsLabels.indexOf(label);
    if(index != -1)
	settingsValues[index] = value;
}

