#include <QtGui>
#include <QtDebug>

#include "centroid.h"
#include <dataentrydialog.h>
#include <spectrogramdata.h>
#include <waveformdata.h>

CentroidPlugin::CentroidPlugin()
{
//    sound = data;

    pluginnames << "Centroid";

    settingsLabels << "From (Hz)";
    settingsValues << 1000;
    settingsLabels << "To (Hz)";
    settingsValues << 5000;
}

CentroidPlugin::~CentroidPlugin()
{
}

QString CentroidPlugin::name() const
{
    return "Centroid Library";
}

QString CentroidPlugin::scriptName() const
{
    return "centroidLibrary";
}

QStringList CentroidPlugin::names() const
{
    return pluginnames;
}

CentroidPlugin* CentroidPlugin::copy() const
{
    // this needs to be better
    return new CentroidPlugin();
}

void CentroidPlugin::settings(int i)
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

void CentroidPlugin::calculate(QString name, SpectrogramData *data)
{
    int index = pluginnames.indexOf(name);
    if(index != -1)
	calculate(index, data);
}

void CentroidPlugin::calculate(int index, SpectrogramData *data)
{
    Q_UNUSED(index);
    size_t nframes = data->getNTimeSteps();

    double *values = (double*)malloc(sizeof(double)*nframes);
    double *times = (double*)malloc(sizeof(double)*nframes);
    double tmp, sum;

    quint32 begin = data->frequencyBinBelow( settingsValues.at(0).toDouble() );
    quint32 end = data->frequencyBinAbove( settingsValues.at(1).toDouble() );

    for(quint32 i=0; i<nframes; i++)
    {
	*(times+i) = data->getTimeFromIndex(i);
	tmp = 0;
	sum = 0.0f;
	for(quint32 j=begin; j<end; j++)
	{
	    tmp += data->getFrequencyFromIndex(j) * data->dataAt(i,j);
	    sum += data->dataAt(i,j);
	}
	*(values+i) = tmp / sum;
    }

    emit waveformCreated(new WaveformData("Centroid F:"+settingsValues.at(0).toString() + " T:" + settingsValues.at(1).toString(),times,values,nframes,0));
}

void CentroidPlugin::setParameter(QString label, QVariant value)
{
    int index = settingsLabels.indexOf(label);
    if(index != -1)
	settingsValues[index] = value;
}
