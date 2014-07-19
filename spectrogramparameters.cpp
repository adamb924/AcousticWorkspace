#include "spectrogramparameters.h"

SpectrogramParameters::SpectrogramParameters()
{
}
QString SpectrogramParameters::name() const
{
    return mName;
}

void SpectrogramParameters::setName(const QString &value)
{
    mName = value;
}
int SpectrogramParameters::upperBound() const
{
    return mUpperBound;
}

void SpectrogramParameters::setUpperBound(int value)
{
    mUpperBound = value;
}
int SpectrogramParameters::lowerBound() const
{
    return mLowerBound;
}

void SpectrogramParameters::setLowerBound(int value)
{
    mLowerBound = value;
}
SpectrogramData *SpectrogramParameters::spectrogramData() const
{
    return mSpectrogramData;
}

void SpectrogramParameters::setSpectrogramData(SpectrogramData *value)
{
    mSpectrogramData = value;
}




