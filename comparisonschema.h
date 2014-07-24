#ifndef COMPARISONSCHEMA_H
#define COMPARISONSCHEMA_H

#include <QList>

#include "waveformdata.h"

class IntervalAnnotation;

class ComparisonSchema
{
public:
    ComparisonSchema();

    WaveformPairList waveforms() const;
    void addWaveformPair(WaveformPair value);

    bool intervalComparison() const;
    void setIntervalComparison(bool value);

    QPair<const IntervalAnnotation*,const IntervalAnnotation*> intervals() const;
    void setIntervals(const IntervalAnnotation*first, const IntervalAnnotation*second);

    QString stretchAlgorithm() const;
    void setStretchAlgorithm(const QString &value);

private:
    WaveformPairList mWaveforms;
    bool mIntervalComparison;
    QPair<const IntervalAnnotation*,const IntervalAnnotation*> mIntervals;
    QString mStretchAlgorithm;
};

#endif // COMPARISONSCHEMA_H
