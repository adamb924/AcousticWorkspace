#include "intervalannotation.h"

#include <QFile>
#include <QTextStream>
#include <QString>
#include <QtDebug>
#include <QRegExp>

Interval::Interval(QString label, double left, double right)
{
    this->mLabel = label;
    this->mLeft = left;
    this->mRight = right;
}

// this seems too simple
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

IntervalAnnotation::IntervalAnnotation()
{
}

QString IntervalAnnotation::toString()
{
    QString ret;
    for(int i=0; i<maIntervals.count(); i++)
	ret += maIntervals.at(i)->mLabel + " ";
    return ret;
}

bool IntervalAnnotation::operator==(const IntervalAnnotation &other) const
{
    if( this->maIntervals.count() != other.maIntervals.count() ) { return false; }
    for(int i=0; i< maIntervals.count(); i++)
	if( maIntervals.at(i)->mLabel != other.maIntervals.at(i)->mLabel )
	    return false;
    return true;
}

bool IntervalAnnotation::operator!=(const IntervalAnnotation &other) const
{
  return !(*this == other);
}
