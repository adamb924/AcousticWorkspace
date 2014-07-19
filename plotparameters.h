#ifndef PLOTPARAMETERS_H
#define PLOTPARAMETERS_H

#include <QList>
#include <QString>

#include "curveparameters.h"
#include "spectrogramparameters.h"

class PlotParameters
{
public:
    PlotParameters();

    const QList<CurveParameters*> * curveParameters() const;
    QList<CurveParameters *> *curveParameters();

    QString name() const;
    void setName(const QString &value);

    bool hasSecondaryAxis() const;
    void setHasSecondaryAxis(bool value);

    int height() const;
    void setHeight(int value);

    const QList<SpectrogramParameters *> spectrogramParameters() const;
    QList<SpectrogramParameters *> spectrogramParameters();

private:
    QString mName;
    bool mHasSecondaryAxis;
    int mHeight;
    QList<CurveParameters*> mCurveParameters;
    QList<SpectrogramParameters*> mSpectrogramParameters;
};

#endif // PLOTPARAMETERS_H
