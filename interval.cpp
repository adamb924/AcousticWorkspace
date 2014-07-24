#include "interval.h"

Interval::Interval()
{
}

Interval::Interval(QString label, double left, double right)
    : mLabel(label), mLeft(left), mRight(right)
{
}

bool Interval::inRange(double start, double end)
{
    if( end < mLeft || start > mRight )
        return false;
    else
        return true;
}

Interval* Interval::clip(double start, double end)
{
    if( mLeft > start )
        start = mLeft;
    if( end > mRight )
        end = mRight;
    return new Interval(mLabel,start,end);
}
