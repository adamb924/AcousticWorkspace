#include "regressiondialog.h"


RegressionListItem::RegressionListItem( WaveformData *data , QListWidget * parent, int type ) : QListWidgetItem(parent,type)
{
    waveformData = data;
    this->setFlags(Qt::ItemIsSelectable|Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);
    this->setCheckState(Qt::Unchecked);
    this->setText(text());
}

WaveformData* RegressionListItem::data()
{
    return waveformData;
}

QString RegressionListItem::text () const
{
    return waveformData->name();
}

RegressionInteractionListItem::RegressionInteractionListItem( InteractionEffect *data , QListWidget * parent, int type ) : QListWidgetItem(parent,type)
{
    interactionData = data;
    this->setFlags(Qt::ItemIsSelectable|Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);
    this->setCheckState(Qt::Unchecked);
    this->setText(text());
}

InteractionEffect* RegressionInteractionListItem::interaction()
{
    return interactionData;
}

QString RegressionInteractionListItem::text () const
{
    if( interactionData->members.length() == 0 ) { return "<empty>"; }
    QString retval = interactionData->members.at(0)->name();
    for(int i=1; i<interactionData->members.length(); i++)
	retval += " x " + interactionData->members.at(i)->name();

    return retval;
}

RegressionSpectrogramListItem::RegressionSpectrogramListItem( SpectrogramData *data , QListWidget * parent, int type) : QListWidgetItem(parent,type)
{
    spectrogramData = data;
    this->setFlags(Qt::ItemIsSelectable|Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);
    this->setCheckState(Qt::Unchecked);
    this->setText(text());
}

SpectrogramData* RegressionSpectrogramListItem::data()
{
    return spectrogramData;
}

QString RegressionSpectrogramListItem::text () const
{
    return spectrogramData->name();
}
