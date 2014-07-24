#include "intervalannotation.h"

#include "interval.h"

#include <QFile>
#include <QTextStream>
#include <QString>
#include <QtDebug>
#include <QRegExp>

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
    if( maIntervals.count() != other.maIntervals.count() ) { return false; }
    for(int i=0; i< maIntervals.count(); i++)
	if( maIntervals.at(i)->mLabel != other.maIntervals.at(i)->mLabel )
	    return false;
    return true;
}

bool IntervalAnnotation::operator!=(const IntervalAnnotation &other) const
{
  return !(*this == other);
}
