#include <QtGui>
#include <QtDebug>

#include "cepstrum_spectrogram.h"
#include "dataentrydialog.h"
#include <fftw3.h>
#include <spectrogramdata.h>

CepstrumSpectrogramPlugin::CepstrumSpectrogramPlugin()
{
//    sound = data;

    pluginnames << "Cepstrum Spectrogram";

    settingsLabels << "Number of cepstral coefficients";
    settingsValues << 20;
}

QString CepstrumSpectrogramPlugin::scriptName() const
{
    return "cepstrumLibrarySV";
}


QString CepstrumSpectrogramPlugin::name() const
{
    return "Cepstrum Library (Spectrogram version)";
}

QStringList CepstrumSpectrogramPlugin::names() const
{
    return pluginnames;
}

CepstrumSpectrogramPlugin* CepstrumSpectrogramPlugin::copy() const
{
    // this needs to be better
    return new CepstrumSpectrogramPlugin();
}

void CepstrumSpectrogramPlugin::settings(int i)
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

void CepstrumSpectrogramPlugin::calculate(QString name, SpectrogramData *data)
{
    int index = pluginnames.indexOf(name);
    if(index != -1)
	calculate(index, data);
}

void CepstrumSpectrogramPlugin::calculate(int index, SpectrogramData *data)
{
    Q_UNUSED(index);
    qDebug() << "CepstrumSpectrogramPlugin::calculate begin";

    size_t nFrames = data->getNTimeSteps();
    size_t nCoefficients = settingsValues.at(0).toString().toInt();

    // time frames
    double *times = (double*)malloc(sizeof(double)*nFrames);
    for(quint32 i=0; i<nFrames; i++)
	*(times+i) = data->getTimeFromIndex(i);

    // frequency bins -- not really, they're cepstral coefficients
    double *coefficientindices = (double*)malloc(sizeof(double)*nCoefficients);
    for(quint32 i=0; i<nCoefficients; i++)
	*(coefficientindices+i) = i;

    double *spec = (double*)malloc(sizeof(double)*nFrames*nCoefficients);

    size_t windowLengthInSamples = data->getNFrequencyBins();

    qDebug() << "CepstrumSpectrogramPlugin::calculate";

//    qDebug() << nFrames << windowLengthInSamples << ncoeff;

    double *in = (double*)fftw_malloc(sizeof(double)*windowLengthInSamples);
    fftw_complex *out = (fftw_complex*)fftw_malloc(sizeof(fftw_complex)*windowLengthInSamples);
    fftw_plan theplan = fftw_plan_dft_r2c_1d(windowLengthInSamples, in, out, FFTW_ESTIMATE);

    double spec_min, spec_max;
    spec_max = 0.0f;
    spec_min = 99999999999.0f;
    for(quint32 j = 0; j < nFrames; j++)
    {
	for(quint32 i=0; i<windowLengthInSamples; i++)
	    *(in+i) = data->dataAt(j,i);
	fftw_execute(theplan);
	for(quint32 i=0; i<nCoefficients; i++)
	{
	    *(spec + j*nCoefficients + i) = (out+i)[0][0]*(out+i)[0][0] + (out+i)[0][1]*(out+i)[0][1];
	    if( *(spec + j*nCoefficients + i) > spec_max) { spec_max = *(spec + j*nCoefficients + i); }
	    if( *(spec + j*nCoefficients + i) < spec_min) { spec_min = *(spec + j*nCoefficients + i); }
	}
    }

    fftw_free( in );
    fftw_free( out );
    fftw_destroy_plan(theplan);

    // 10/19/2011 test: doing the log of the cepstral coefficients
    double log_spec_max = -1 * log( spec_min / spec_max );
    for(quint32 i=0; i<nFrames*nCoefficients; i++)
    {
	*(spec+i) = log( *(spec+i) / spec_max ) + log_spec_max;
    }

    QString suggested_label = "Cepstral Spectrogram NC:" + settingsValues.at(0).toString();

    qDebug() << "CepstrumSpectrogramPlugin::calculate end";

    emit spectrogramCreated(new SpectrogramData(suggested_label, spec, times, nFrames, coefficientindices, nCoefficients, data->getWindowLength(), data->getTimeStep()));
}

void CepstrumSpectrogramPlugin::setParameter(QString label, QVariant value)
{
    int index = settingsLabels.indexOf(label);
    if(index != -1)
	settingsValues[index] = value;
}

