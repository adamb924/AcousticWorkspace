#ifndef SOUNDVIEW_H
#define SOUNDVIEW_H

#include <QList>
#include "plotparameters.h"

class SoundView
{
public:
    SoundView();

    QList<PlotParameters*> * plotParameters();
    const QList<PlotParameters *> *plotParameters() const;

    double tMax() const;
    void setTMax(double value);

    double tMin() const;
    void setTMin(double value);

    double leftPos() const;
    void setLeftPos(double value);

    double rightPos() const;
    void setRightPos(double value);

private:
    QList<PlotParameters*> mPlotparameters;
    double mTMax;
    double mTMin;
    double mLeftPos;
    double mRightPos;
};

#endif // SOUNDVIEW_H
