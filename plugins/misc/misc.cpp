#include <QtGui>
#include <QtDebug>

#include "misc.h"
#include <dataentrydialog.h>
#include "gsl/gsl_fit.h"
#include <gsl/gsl_vector.h>
#include <gsl/gsl_statistics_double.h>
#include <spectrogramdata.h>
#include <waveformdata.h>


MiscPlugin::MiscPlugin()
{
//    sound = data;

    pluginnames << "Peak Value";
    pluginnames << "Median Energy";

    settingsLabels << "From (Hz)";
    settingsValues << 1000;
    settingsLabels << "To (Hz)";
    settingsValues << 5000;
}

QString MiscPlugin::name() const
{
    return "Miscellaneous Library";
}

QString MiscPlugin::scriptName() const
{
    return "miscLibrary";
}

QStringList MiscPlugin::names() const
{
    return pluginnames;
}

MiscPlugin* MiscPlugin::copy() const
{
    // this needs to be better
    return new MiscPlugin();
}

void MiscPlugin::settings(int i)
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

void MiscPlugin::calculate(QString name, SpectrogramData *data)
{
    int index = pluginnames.indexOf(name);
    if(index != -1)
	calculate(index, data);
}

void MiscPlugin::calculate(int index, SpectrogramData *data)
{
    size_t nframes = data->getNTimeSteps();
    double *values = (double*)malloc(sizeof(double)*nframes);
    double *times = (double*)malloc(sizeof(double)*nframes);

    QList<WaveformData*> ret;

    quint32 startindex = data->frequencyBinBelow(settingsValues.at(0).toInt());
    quint32 length = data->frequencyBinBelow(settingsValues.at(1).toInt()) - data->frequencyBinBelow(settingsValues.at(0).toInt());
    if(length<2)
    {
	qDebug() << "length is less than 2 somehow";
	return;
    }

    switch(index)
    {
    case 0: // peak
	for(quint32 i=0; i<nframes; i++)
	{
	    *(times+i) = data->getTimeFromIndex(i);
	    *(values+i) = data->getFrequencyFromIndex( gsl_stats_max_index( data->pdata() +  i*data->getNFrequencyBins()+startindex, 1, length ) );
	}
	ret << new WaveformData("Peak F:"+settingsValues.at(0).toString() + " T:" + settingsValues.at(1).toString(),times,values,nframes,0);
	break;
    case 1: // median
	for(quint32 i=0; i<nframes; i++)
	{
	    gsl_vector *cs = gsl_vector_alloc(length);

	    gsl_vector_set(cs, 0, data->dataAt(i,startindex));

	    for(quint32 j=startindex+1; j<startindex+length; j++)
		gsl_vector_set(cs, j-startindex, data->dataAt(i,j) + gsl_vector_get(cs, j-startindex-1) );
	    double middle = gsl_vector_get(cs, length-1)/2;

	    double median = 0;
	    for(quint32 j=0; j<length; j++)
	    {
		if( gsl_vector_get(cs, j) > middle )
		{
		    if(j==0)
			median = data->getFrequencyFromIndex(startindex+j);
		    else
			median = (data->getFrequencyFromIndex(startindex+j) + data->getFrequencyFromIndex(startindex+j-1))/2;
		    break;
		}
	    }

	    *(times+i) = data->getTimeFromIndex(i);
	    *(values+i) = median;

	    gsl_vector_free(cs);
	}
	emit waveformCreated( new WaveformData("Median F:"+settingsValues.at(0).toString() + " T:" + settingsValues.at(1).toString(),times,values,nframes,0) );
	break;
    default:
	break;
    }
}

void MiscPlugin::setParameter(QString label, QVariant value)
{
    int index = settingsLabels.indexOf(label);
    if(index != -1)
	settingsValues[index] = value;
}

