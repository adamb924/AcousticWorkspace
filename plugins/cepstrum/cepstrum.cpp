#include <QtGui>
#include <QtDebug>

#include <fftw3.h>

#include "cepstrum.h"
#include "dataentrydialog.h"
#include <spectrogramdata.h>
#include <waveformdata.h>

CepstrumPlugin::CepstrumPlugin()
{
//    sound = data;

    pluginnames << "Cepstrum";

    settingsLabels << "Number of cepstral coefficients";
    settingsValues << 20;
}

QString CepstrumPlugin::name() const
{
    return "Cepstrum Library";
}

QString CepstrumPlugin::scriptName() const
{
    return "cepstrumLibrary";
}


QStringList CepstrumPlugin::names() const
{
    return pluginnames;
}

CepstrumPlugin* CepstrumPlugin::copy() const
{
    // this needs to be better
    return new CepstrumPlugin();
}

void CepstrumPlugin::settings(int i)
{
    DataEntryDialog dew(&settingsLabels, &settingsValues, "", 0);
    if( dew.exec() == QDialog::Accepted)
    {
	for(int i=0; i<settingsValues.count(); i++)
	{
        settingsValues.replace(i, dew.values()->at(i));
	}
    }
}

void CepstrumPlugin::calculate(QString name, SpectrogramData *data)
{
    int index = pluginnames.indexOf(name);
    if(index != -1)
	calculate(index, data);
}

void CepstrumPlugin::calculate(int index, SpectrogramData *data)
{
    size_t nFrames = data->getNTimeSteps();
    size_t windowLengthInSamples = data->getNFrequencyBins();
    size_t ncoeff = settingsValues.at(0).toInt();

    QList<double*> coeff;

//    qDebug() << nFrames << windowLengthInSamples << ncoeff;

    double *in = (double*)fftw_malloc(sizeof(double)*windowLengthInSamples);
    fftw_complex *out = (fftw_complex*)fftw_malloc(sizeof(fftw_complex)*windowLengthInSamples);
    fftw_plan theplan = fftw_plan_dft_r2c_1d(windowLengthInSamples, in, out, FFTW_ESTIMATE);

    // allocate space for all of the coefficients
    for(quint32 i = 0; i < ncoeff; i++)
	coeff << (double*)malloc(sizeof(double)*nFrames);

    for(quint32 j = 0; j < nFrames; j++)
    {
	for(quint32 i=0; i<windowLengthInSamples; i++)
	    *(in+i) = data->dataAt(j,i);
	fftw_execute(theplan);
	for(quint32 i=0; i<ncoeff; i++)
	    *(coeff.at(i)+j) = log( (out+i)[0][0]*(out+i)[0][0] + (out+i)[0][1]*(out+i)[0][1] );
//	*(coeff.at(i)+j) = (out+i)[0][0]*(out+i)[0][0] + (out+i)[0][1]*(out+i)[0][1];
    }

    fftw_free( in );
    fftw_free( out );
    fftw_destroy_plan(theplan);

    for(quint32 i = 0; i < ncoeff; i++)
    {
	double *times = (double*)malloc(sizeof(double)*nFrames);
	for(quint32 j=0; j<nFrames; j++)
	    *(times+j) = data->getTimeFromIndex(j);
	emit waveformCreated(new WaveformData("CC "+QString::number(i+1),times,coeff.at(i),nFrames,0));
    }
}

void CepstrumPlugin::setParameter(QString label, QVariant value)
{
    int index = settingsLabels.indexOf(label);
    if(index != -1)
	settingsValues[index] = value;
}

