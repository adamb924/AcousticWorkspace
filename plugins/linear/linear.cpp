#include <QtGui>
#include <QtDebug>

#include "linear.h"
#include <dataentrydialog.h>
#include "gsl/gsl_fit.h"
#include <waveformdata.h>
#include <spectrogramdata.h>

LinearPlugin::LinearPlugin()
{
//    sound = data;

    pluginnames << "Slope";
    pluginnames << "Intercept";

    settingsLabels << "From (Hz)";
    settingsValues << 1000;
    settingsLabels << "To (Hz)";
    settingsValues << 5000;
}

QString LinearPlugin::name() const
{
    return "Linear Library";
}

QString LinearPlugin::scriptName() const
{
    return "linearLibrary";
}

QStringList LinearPlugin::names() const
{
    return pluginnames;
}

LinearPlugin* LinearPlugin::copy() const
{
    // this needs to be better
    return new LinearPlugin();
}

void LinearPlugin::settings(int i)
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

void LinearPlugin::calculate(QString name, SpectrogramData *data)
{
    int index = pluginnames.indexOf(name);
    if(index != -1)
	calculate(index, data);
}

void LinearPlugin::calculate(int index, SpectrogramData *data)
{
    size_t nframes = data->getNTimeSteps();
    double c0, c1, cov00, cov01, cov11, sumsq;
    double *values = (double*)malloc(sizeof(double)*nframes);
    double *times = (double*)malloc(sizeof(double)*nframes);

    int startindex = data->frequencyBinBelow(settingsValues.at(0).toInt());
    int length = data->frequencyBinBelow(settingsValues.at(1).toInt()) - data->frequencyBinBelow(settingsValues.at(0).toInt());
    if(length<2)
    {
	qDebug() << "length is less than 2 somehow";
	return;
    }

    qDebug() << data->frequencyBinBelow(settingsValues.at(0).toInt()) << data->frequencyBinBelow(settingsValues.at(1).toInt()) << startindex << length;

    switch(index)
    {
    case 0: // slope
	for(quint32 i=0; i<nframes; i++)
	{
	    gsl_fit_linear(data->pfrequencies()+startindex, 1, data->pdata() + data->getNFrequencyBins() *i, 1, length, &c0, &c1, &cov00, &cov01, &cov11, &sumsq);
	    *(times+i) = data->getTimeFromIndex(i);
	    *(values+i) = c1;
	}
	emit waveformCreated( new WaveformData("Slope F:"+settingsValues.at(0).toString() + " T:" + settingsValues.at(1).toString(),times,values,nframes,0) );
	break;
    case 1: // intercept
	for(quint32 i=0; i<nframes; i++)
	{
	    gsl_fit_linear(data->pfrequencies()+startindex, 1, data->pdata() + data->getNFrequencyBins()*i, 1, length, &c0, &c1, &cov00, &cov01, &cov11, &sumsq);
	    *(times+i) = data->getTimeFromIndex(i);
	    *(values+i) = c0;
	}
	emit waveformCreated( new WaveformData("Intercept F:"+settingsValues.at(0).toString() + " T:" + settingsValues.at(1).toString(),times,values,nframes,0) );
	break;
    default:
	break;
    }
}

void LinearPlugin::setParameter(QString label, QVariant value)
{
    int index = settingsLabels.indexOf(label);
    if(index != -1)
	settingsValues[index] = value;
}

