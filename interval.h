/*! \class Interval
    \ingroup Annotation
    \brief A data class for representing a single interval annotation in the time domain.

    The class provides data structure and convenience functions for a single time-interval.
  */

#ifndef INTERVAL_H
#define INTERVAL_H

#include <QString>

class Interval {
public:
    Interval();
    //! \brief Initialize the interval with label \a label, the left edge of the interval at \a left seconds, and the right edge at \a right seconds.
    Interval(QString label, double left, double right);

    //! \brief Return true if the interval overlaps with the range [\a start,\a end], otherwise return false.
    bool inRange(double start, double end);

    //! \brief Return an interval identical to the current one, but clipped to the range [\a start,\a end].
    Interval* clip(double start, double end);

    QString mLabel;
    double mLeft, mRight;
};


#endif // INTERVAL_H
