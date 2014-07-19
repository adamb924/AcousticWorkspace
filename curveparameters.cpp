#include "curveparameters.h"

CurveParameters::CurveParameters()
{

}

CurveParameters::CurveParameters(QPen pen, QwtPlotCurve::CurveStyle curveStyle, bool antialiased) :
    mPen(pen),
    mCurveStyle(curveStyle),
    mAntialiased(antialiased)
{
}

QPen CurveParameters::pen() const
{
    return mPen;
}

void CurveParameters::setPen(const QPen &value)
{
    mPen = value;
}

QwtPlotCurve::CurveStyle CurveParameters::curveStyle() const
{
    return mCurveStyle;
}

void CurveParameters::setCurveStyle(const QwtPlotCurve::CurveStyle &value)
{
    mCurveStyle = value;
}
bool CurveParameters::antialiased() const
{
    return mAntialiased;
}

void CurveParameters::setAntialiased(bool value)
{
    mAntialiased = value;
}

QBrush CurveParameters::symbolBrush() const
{
    return mSymbolBrush;
}

void CurveParameters::setSymbolBrush(const QBrush &value)
{
    mSymbolBrush = value;
}

QPen CurveParameters::symbolPen() const
{
    return mSymbolPen;
}

void CurveParameters::setSymbolPen(const QPen &value)
{
    mSymbolPen = value;
}
int CurveParameters::symbolSize() const
{
    return mSymbolSize;
}

void CurveParameters::setSymbolSize(int value)
{
    mSymbolSize = value;
}

QwtSymbol::Style CurveParameters::symbolStyle() const
{
    return mSymbolStyle;
}

void CurveParameters::setSymbolStyle(const QwtSymbol::Style &value)
{
    mSymbolStyle = value;
}
bool CurveParameters::isSecondary() const
{
    return mIsSecondary;
}

void CurveParameters::setIsSecondary(bool value)
{
    mIsSecondary = value;
}
WaveformData *CurveParameters::waveformData() const
{
    return mWaveformData;
}

void CurveParameters::setWaveformData(WaveformData *value)
{
    mWaveformData = value;
}








