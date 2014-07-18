#include <QtGui>
#include <QtDebug>
#include <QProgressDialog>

#include <waveformdata.h>
#include <spectrogramdata.h>

#include "spectrogram.h"
#include <dataentrydialog.h>
#include <fftw3.h>

SpectrogramPlugin::SpectrogramPlugin()
{
//    sound = data;
    window = 0;

    pluginNames << "Spectrogram";

    settingsLabels << "Window length (ms)";
    settingsValues << 30;
    settingsLabels << "Time step (ms)";
    settingsValues << 5;
}

QString SpectrogramPlugin::name() const
{
    return "Spectrogram Library";
}

QString SpectrogramPlugin::scriptName() const
{
    return "spectrogramLibrary";
}

QStringList SpectrogramPlugin::names() const
{
    return pluginNames;
}

SpectrogramPlugin* SpectrogramPlugin::copy() const
{
    // this needs to be better
    return new SpectrogramPlugin();
}

void SpectrogramPlugin::settings(int i)
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

void SpectrogramPlugin::calculate(QString name, WaveformData *data)
{
    int index = pluginNames.indexOf(name);
    if(index != -1)
	calculate(index, data);
}


void SpectrogramPlugin::calculate(int index, WaveformData *data)
{
    Q_UNUSED(index);
    WaveformData *sound = data;

    //				    x, y, width, height
//
    double windowLength = settingsValues.at(0).toDouble() / 1000.0f;
    double timeStep = settingsValues.at(1).toDouble() / 1000.0f;
    size_t windowLengthInSamples = windowLength * sound->getSamplingFrequency();
    size_t timeStepInSamples = timeStep * sound->getSamplingFrequency();

    // BEWARE BEWARE BEWARE //
//    windowLengthInSamples *= 2;
    // the above line is motivated by Praat's practice of using twice as long a window as is specified

    double spec_min, spec_max;

    // quint32 nFrames, nFreqBins
    size_t nFrames = (quint32)floor( (sound->length()-windowLength) / timeStep );
    if( floor( (sound->length()-windowLength) / windowLength ) == ( (sound->length()-windowLength) / windowLength )) { nFrames++; } // if there happened to be exactly one windowLenth left over
    size_t nFreqBins = (quint32)floor(windowLengthInSamples/2.0f);

//    qDebug() << nFrames << sound->length() << windowLength << timeStep;

    // Gaussian window
    double *filter = (double*)malloc(sizeof(double)*windowLengthInSamples);
    double wls = windowLengthInSamples;
    for(quint32 i=0; i<windowLengthInSamples; i++)
    {
	double n = (-1*wls/2) + i;
	*(filter+i) = exp( -0.5 * (2.5*n / (wls/2))*(2.5*n / (wls/2)) );
    }

    // time frames
    double *times = (double*)malloc(sizeof(double)*nFrames);
    for(quint32 i=0; i<nFrames; i++)
    {
	*(times+i) = windowLength/2 + i*timeStep;
    }

    // frequency bins
    double *frequencies = (double*)malloc(sizeof(double)*nFreqBins);
    for(quint32 i=0; i<nFreqBins; i++)
    {
	*(frequencies+i) = i*(sound->getNyquistFrequency()/nFreqBins);
    }

    double *spec = (double*)malloc(sizeof(double)*nFrames*nFreqBins);

    double *in = (double*)fftw_malloc(sizeof(double)*windowLengthInSamples);
    fftw_complex *out = (fftw_complex*)fftw_malloc(sizeof(fftw_complex)*windowLengthInSamples);
    fftw_plan theplan = fftw_plan_dft_r2c_1d(windowLengthInSamples, in, out, FFTW_ESTIMATE);

    quint32 startSample = 0;

    QProgressDialog progress("Calculating spectrogram...", QString(), 0, nFrames, 0);
    progress.setWindowModality(Qt::WindowModal);

    spec_max = 0.0f;
    spec_min = 99999999999.0f;
    for(quint32 j = 0; j < nFrames; j++)
    {

	progress.setValue(j);

	// put a segment of waveform into in
	startSample = j * timeStepInSamples;
	for(quint32 i=0; i<windowLengthInSamples; i++)
	{
        *(in+i) = sound->yData()[startSample+i]  * *(filter+i);
	}
	fftw_execute(theplan);
	// change to the absolute value, and also keep track of the minimum and maximum values
	for(quint32 i=0; i<nFreqBins; i++)
	{
	    *(spec + j*nFreqBins + i) = (out+i)[0][0]*(out+i)[0][0] + (out+i)[0][1]*(out+i)[0][1];
	    if( *(spec + j*nFreqBins + i) > spec_max) { spec_max = *(spec + j*nFreqBins + i); }
	    if( *(spec + j*nFreqBins + i) < spec_min) { spec_min = *(spec + j*nFreqBins + i); }
	}
    }

    double log_spec_max = -1 * log( spec_min / spec_max );

    for(quint32 i=0; i<nFrames*nFreqBins; i++)
    {
	*(spec+i) = log( *(spec+i) / spec_max ) + log_spec_max;
    }

    fftw_free( in );
    fftw_free( out );
    fftw_destroy_plan(theplan);

//    qDebug() << spec << times << frequencies;
//    qDebug() << spec_min << spec_max << windowLength << timeStep << nFrames << nFreqBins;
/*
    FILE *fid = fopen("spec.bin","wb");
    fwrite(spec,sizeof(double),nFrames*nFreqBins,fid);
    fclose(fid);
*/
    free(filter);

    QString suggested_label = "Spectrogram WL:" + settingsValues.at(0).toString() + " TS:" + settingsValues.at(1).toString();

    emit spectrogramCreated(new SpectrogramData(suggested_label, spec, times, nFrames, frequencies, nFreqBins, spec_min, spec_max , windowLength, timeStep));

//    QList<SpectrogramData*> ret;
//    ret << new SpectrogramData(suggested_label, spec, times, nFrames, frequencies, nFreqBins, spec_min, spec_max , windowLength, timeStep);
//    ret.last()->setBoundingRect(QwtDoubleRect( sound->tMin(), 0.0f , (sound->tMax()-sound->tMin()), (double)sound->getNyquistFrequency()  ));
//    return ret;
}

void SpectrogramPlugin::setParameter(QString label, QVariant value)
{
    int index = settingsLabels.indexOf(label);
    if(index != -1)
	settingsValues[index] = value;
}

