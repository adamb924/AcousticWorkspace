#include "comparisonschema.h"

ComparisonSchema::ComparisonSchema()
{
}

bool ComparisonSchema::intervalComparison() const
{
    return mIntervalComparison;
}

void ComparisonSchema::setIntervalComparison(bool value)
{
    mIntervalComparison = value;
}

QPair<const IntervalAnnotation*,const IntervalAnnotation*> ComparisonSchema::intervals() const
{
    return mIntervals;
}

void ComparisonSchema::setIntervals(const IntervalAnnotation *first, const IntervalAnnotation *second)
{
    mIntervals = QPair<const IntervalAnnotation*,const IntervalAnnotation*>(first,second);
}

QString ComparisonSchema::stretchAlgorithm() const
{
    return mStretchAlgorithm;
}

void ComparisonSchema::setStretchAlgorithm(const QString &value)
{
    mStretchAlgorithm = value;
}

WaveformPairList ComparisonSchema::waveforms() const
{
    return mWaveforms;
}

void ComparisonSchema::addWaveformPair(WaveformPair value)
{
    mWaveforms.append(value);
}




