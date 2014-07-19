#include "soundview.h"

SoundView::SoundView()
{
}

QList<PlotParameters *> *SoundView::plotParameters()
{
    return &mPlotparameters;
}

const QList<PlotParameters *> *SoundView::plotParameters() const
{
    return &mPlotparameters;
}
double SoundView::tMax() const
{
    return mTMax;
}

void SoundView::setTMax(double value)
{
    mTMax = value;
}
double SoundView::tMin() const
{
    return mTMin;
}

void SoundView::setTMin(double value)
{
    mTMin = value;
}
double SoundView::leftPos() const
{
    return mLeftPos;
}

void SoundView::setLeftPos(double value)
{
    mLeftPos = value;
}
double SoundView::rightPos() const
{
    return mRightPos;
}

void SoundView::setRightPos(double value)
{
    mRightPos = value;
}




