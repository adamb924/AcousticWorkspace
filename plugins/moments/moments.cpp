#include <QtGui>
#include <QtDebug>

#include "moments.h"
#include <dataentrydialog.h>
#include <fftw3.h>
#include <gsl/gsl_statistics_double.h>
#include <waveformdata.h>
#include <spectrogramdata.h>

MomentsPlugin::MomentsPlugin()
{
    pluginnames << "Variance";
    pluginnames << "Skewness";
    pluginnames << "Kurtosis";

    settingsLabels << "From (Hz)";
    settingsValues << 1000;
    settingsLabels << "To (Hz)";
    settingsValues << 5000;
}

QString MomentsPlugin::name() const
{
    return "Spectral Moments Library";
}

QStringList MomentsPlugin::names() const
{
    return pluginnames;
}

MomentsPlugin* MomentsPlugin::copy() const
{
    // this needs to be better
    return new MomentsPlugin();
}

void MomentsPlugin::settings(int i)
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

void MomentsPlugin::calculate(int index, SpectrogramData *data)
{
    size_t nframes = data->getNTimeSteps();

    double *values = (double*)malloc(sizeof(double)*nframes);
    double *times = (double*)malloc(sizeof(double)*nframes);

    quint32 begin = data->frequencyBinBelow( settingsValues.at(0).toDouble() );
    quint32 end = data->frequencyBinAbove( settingsValues.at(1).toDouble() );

    switch(index)
    {
    case 0: // variance
	for(quint32 i=0; i<nframes; i++)
	{
	    *(times+i) = data->getTimeFromIndex(i);
	    *(values+i) = gsl_stats_variance( data->pdata() + i*data->getNFrequencyBins() + begin, 1, end-begin);
	}
	emit waveformCreated( new WaveformData("Variance F:"+settingsValues.at(0).toString() + " T:" + settingsValues.at(1).toString(),times,values,nframes,0) );
	break;
    case 1: // skewness
	for(quint32 i=0; i<nframes; i++)
	{
	    *(times+i) = data->getTimeFromIndex(i);
	    *(values+i) = gsl_stats_skew( data->pdata() + i*data->getNFrequencyBins() + begin, 1, end-begin);
	}
	emit waveformCreated( new WaveformData("Skewness F:"+settingsValues.at(0).toString() + " T:" + settingsValues.at(1).toString(),times,values,nframes,0) );
	break;
    case 2: // kurtosis
	for(quint32 i=0; i<nframes; i++)
	{
	    *(times+i) = data->getTimeFromIndex(i);
	    *(values+i) = gsl_stats_kurtosis( data->pdata() + i*data->getNFrequencyBins() + begin, 1, end-begin);
	}
	emit waveformCreated( new WaveformData("Kurtosis F:"+settingsValues.at(0).toString() + " T:" + settingsValues.at(1).toString(),times,values,nframes,0) );
	break;
    default:
	break;
    }
}

QString MomentsPlugin::scriptName() const
{
    return "momentsLibrary";
}

void MomentsPlugin::calculate(QString name, SpectrogramData *data)
{
    int index = pluginnames.indexOf(name);
    if(index != -1)
	calculate(index, data);
}

void MomentsPlugin::setParameter(QString label, QVariant value)
{
    int index = settingsLabels.indexOf(label);
    if(index != -1)
	settingsValues[index] = value;
}

