#include "plotparameters.h"

PlotParameters::PlotParameters()
{
}

const QList<CurveParameters *> *PlotParameters::curveParameters() const
{
    return &mCurveParameters;
}

QList<CurveParameters *> *PlotParameters::curveParameters()
{
    return &mCurveParameters;
}
QString PlotParameters::name() const
{
    return mName;
}

void PlotParameters::setName(const QString &value)
{
    mName = value;
}
bool PlotParameters::hasSecondaryAxis() const
{
    return mHasSecondaryAxis;
}

void PlotParameters::setHasSecondaryAxis(bool value)
{
    mHasSecondaryAxis = value;
}
int PlotParameters::height() const
{
    return mHeight;
}

void PlotParameters::setHeight(int value)
{
    mHeight = value;
}

QList<SpectrogramParameters *> PlotParameters::spectrogramParameters()
{
    return mSpectrogramParameters;
}

const QList<SpectrogramParameters *> PlotParameters::spectrogramParameters() const
{
    return mSpectrogramParameters;
}

