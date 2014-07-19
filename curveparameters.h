#ifndef CURVEPARAMETERS_H
#define CURVEPARAMETERS_H

#include <QPen>
#include <QBrush>

#include <qwt_plot_curve.h>
#include <qwt_plot_item.h>
#include <qwt_symbol.h>

class WaveformData;

class CurveParameters
{
public:
    CurveParameters();
    /// @todo probably delete this
    CurveParameters(QPen pen, QwtPlotCurve::CurveStyle curveStyle, bool antialiased);

    QPen pen() const;
    void setPen(const QPen &value);

    QwtPlotCurve::CurveStyle curveStyle() const;
    void setCurveStyle(const QwtPlotCurve::CurveStyle &value);

    bool antialiased() const;
    void setAntialiased(bool value);

    QBrush symbolBrush() const;
    void setSymbolBrush(const QBrush &value);

    QPen symbolPen() const;
    void setSymbolPen(const QPen &value);

    int symbolSize() const;
    void setSymbolSize(int value);

    QwtSymbol::Style symbolStyle() const;
    void setSymbolStyle(const QwtSymbol::Style &value);

    bool isSecondary() const;
    void setIsSecondary(bool value);

    WaveformData *waveformData() const;
    void setWaveformData(WaveformData *value);

private:
    WaveformData *mWaveformData;

    QPen mPen;
    QwtPlotCurve::CurveStyle mCurveStyle;
    bool mAntialiased;
    bool mIsSecondary;

    QBrush mSymbolBrush;
    QPen mSymbolPen;
    int mSymbolSize;
    QwtSymbol::Style mSymbolStyle;

};

#endif // PLOTDISPLAYPARAMETERS_H
