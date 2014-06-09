#include "regressiondialog.h"


RegressionListItem::RegressionListItem( WaveformData *data , QListWidget * parent, int type ) : QListWidgetItem(parent,type)
{
    mWaveformData = data;
    this->setFlags(Qt::ItemIsSelectable|Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);
    this->setCheckState(Qt::Unchecked);
    this->setText(text());
}

WaveformData* RegressionListItem::data()
{
    return mWaveformData;
}

QString RegressionListItem::text () const
{
    return mWaveformData->name();
}

RegressionInteractionListItem::RegressionInteractionListItem( InteractionEffect *data , QListWidget * parent, int type ) : QListWidgetItem(parent,type)
{
    mInteractionData = data;
    this->setFlags(Qt::ItemIsSelectable|Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);
    this->setCheckState(Qt::Unchecked);
    this->setText(text());
}

InteractionEffect* RegressionInteractionListItem::interaction()
{
    return mInteractionData;
}

QString RegressionInteractionListItem::text () const
{
    if( mInteractionData->members.length() == 0 ) { return "<empty>"; }
    QString retval = mInteractionData->members.at(0)->name();
    for(int i=1; i<mInteractionData->members.length(); i++)
	retval += " x " + mInteractionData->members.at(i)->name();

    return retval;
}

RegressionSpectrogramListItem::RegressionSpectrogramListItem( SpectrogramData *data , QListWidget * parent, int type) : QListWidgetItem(parent,type)
{
    mSpectrogramData = data;
    this->setFlags(Qt::ItemIsSelectable|Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);
    this->setCheckState(Qt::Unchecked);
    this->setText(text());
}

SpectrogramData* RegressionSpectrogramListItem::data()
{
    return mSpectrogramData;
}

QString RegressionSpectrogramListItem::text () const
{
    return mSpectrogramData->name();
}
