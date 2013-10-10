#include "intervalannotation.h"

#include <QFile>
#include <QTextStream>
#include <QString>
#include <QtDebug>
#include <QRegExp>

Interval::Interval(QString label, double left, double right)
{
    this->label = label;
    this->left = left;
    this->right = right;
}

// this seems too simple
bool Interval::inRange(double start, double end)
{
    if( end < left || start > right )
	return false;
    else
	return true;
}

Interval* Interval::clip(double start, double end)
{
    if( left > start )
	start = left;
    if( end > right )
	end = right;
    return new Interval(label,start,end);
}

IntervalAnnotation::IntervalAnnotation()
{
}

QString IntervalAnnotation::toString()
{
    QString ret;
    for(int i=0; i<aIntervals.count(); i++)
	ret += aIntervals.at(i)->label + " ";
    return ret;
}

bool IntervalAnnotation::operator==(const IntervalAnnotation &other) const
{
    if( this->aIntervals.count() != other.aIntervals.count() ) { return false; }
    for(int i=0; i< aIntervals.count(); i++)
	if( aIntervals.at(i)->label != other.aIntervals.at(i)->label )
	    return false;
    return true;
}

bool IntervalAnnotation::operator!=(const IntervalAnnotation &other) const
{
  return !(*this == other);
}
