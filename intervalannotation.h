/*! \class IntervalAnnotation
    \ingroup Annotation
    \brief A data class for representing multiple time-domain annotations.

    The class represents the an interval annotation tier with a list of Interval objects. There are convenience functions to convert the tier to a string, clip the interval, and check for equality (of labels) with other tiers.
  */

#ifndef INTERVALANNOTATION_H
#define INTERVALANNOTATION_H

#include <QList>
#include <QString>

class Interval;

class IntervalAnnotation
{
public:
    IntervalAnnotation();

    //! \brief Return a string consisted of all of the labels of the intervals on the tier, delimited by a space.
    QString toString();

    //! \brief Return true if the \a other has the same number of intervals and same labels as \a this, otherwise return false.
    bool operator==(const IntervalAnnotation &other) const;

    //! \brief Return false if the \a other has the same number of intervals and same labels as \a this, otherwise return true.
    bool operator!=(const IntervalAnnotation &other) const;

    QString mName;
    QList<Interval*> maIntervals;

    //! \brief Return an interval tier identical to the current one, but clipped to the range [\a start,\a end].
    QList<Interval*> clip(double start, double end);
};

#endif // INTERVALANNOTATION_H
