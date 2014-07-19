#ifndef SPECTROGRAMPARAMETERS_H
#define SPECTROGRAMPARAMETERS_H

class SpectrogramData;

#include <QString>

class SpectrogramParameters
{
public:
    SpectrogramParameters();

    QString name() const;
    void setName(const QString &value);

    int upperBound() const;
    void setUpperBound(int value);

    int lowerBound() const;
    void setLowerBound(int value);

    SpectrogramData *spectrogramData() const;
    void setSpectrogramData(SpectrogramData *value);

private:
    SpectrogramData * mSpectrogramData;
    QString mName;
    int mUpperBound;
    int mLowerBound;
};

#endif // SPECTROGRAMPARAMETERS_H
