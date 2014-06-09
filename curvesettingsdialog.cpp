#include "curvesettingsdialog.h"

#include <qwt_symbol.h>
#include <qwt_plot_curve.h>
#include <qwt_plot.h>

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QColorDialog>
#include <QPushButton>
#include <QBrush>
#include <QPen>
#include <QSize>
#include <QIcon>
#include <QColor>
#include <QComboBox>
#include <QStringList>
#include <QSpinBox>
#include <QtDebug>
#include <QCheckBox>

CurveSettingsDialog::CurveSettingsDialog(QwtPlotCurve *curve, QwtPlot *parentPlot, QWidget *parent)
    : QDialog(parent), mCurve(curve), mParent(parentPlot)
{
    QVBoxLayout *vlayout = new QVBoxLayout;

    vlayout->addWidget(new QLabel(tr("Symbol Settings")));

    mSymbolColor = new QPushButton(iconFromColor(curve->symbol()->pen().color()),"Symbol Color");
    connect(mSymbolColor,SIGNAL(clicked()),this,SLOT(SymbolBorderColor()));
    vlayout->addWidget(mSymbolColor,0,Qt::AlignHCenter);

    mSymbolFillColor = new QPushButton(iconFromColor(curve->symbol()->brush().color()),"Symbol Fill Color");
    connect(mSymbolFillColor,SIGNAL(clicked()),this,SLOT(SymbolFillColor()));
    vlayout->addWidget(mSymbolFillColor,0,Qt::AlignHCenter);

    QComboBox *symbolStyle = new QComboBox();
    QStringList styles;
    styles << "NoSymbol" << "Ellipse" << "Rect" << "Diamond" << "Triangle" << "DTriangle" << "UTriangle" << "LTriangle" << "RTriangle" << "Cross" << "XCross" << "HLine" << "VLine" << "Star1" << "Star2" << "Hexagon" << "StyleCnt";
    symbolStyle->addItems(styles);
    connect(symbolStyle,SIGNAL(currentIndexChanged(int)),this,SLOT(SymbolStyleChanged(int)));
    vlayout->addWidget(symbolStyle);

    QSpinBox *symbolSize = new QSpinBox();
    symbolSize->setMinimum(0);
    symbolSize->setMaximum(100);
    symbolSize->setValue(curve->symbol()->pen().width());
    connect(symbolSize,SIGNAL(valueChanged(int)),this,SLOT(SymbolSizeChange(int)));
    vlayout->addWidget(symbolSize);

    vlayout->addWidget(new QLabel(tr("Line Settings")));

    mLineColor = new QPushButton(iconFromColor(curve->pen().color() ),"Line Color");
    connect(mLineColor,SIGNAL(clicked()),this,SLOT(LineColor()));
    vlayout->addWidget(mLineColor,0,Qt::AlignHCenter);

    QComboBox *lineStyle = new QComboBox();
    styles.clear();
    styles << "NoCurve" << "Lines" << "Sticks" << "Steps" << "Dots";
    lineStyle->addItems(styles);
    lineStyle->setCurrentIndex(curve->style());
    connect(lineStyle,SIGNAL(currentIndexChanged(int)),this,SLOT(LineStyleChanged(int)));
    vlayout->addWidget(lineStyle);

    QSpinBox *lineSize = new QSpinBox();
    lineSize->setMinimum(1);
    lineSize->setMaximum(100);
    lineSize->setValue(curve->pen().width());
    connect(lineSize,SIGNAL(valueChanged(int)),this,SLOT(LineSizeChange(int)));
    vlayout->addWidget(lineSize);

    QCheckBox *antialiased = new QCheckBox(tr("Antialiased"));
    antialiased->setChecked(curve->testRenderHint(QwtPlotItem::RenderAntialiased));
    connect(antialiased,SIGNAL(toggled(bool)),this,SLOT(LineAntialiased(bool)));
    vlayout->addWidget(antialiased);

    this->setLayout(vlayout);
}

void CurveSettingsDialog::SymbolBorderColor()
{
    QColorDialog cd;
    QColor col = cd.getColor(mCurve->symbol()->pen().color());
    if(!col.isValid()) { return; }

    mCurve->setSymbol( new QwtSymbol( mCurve->symbol()->style() , mCurve->symbol()->brush(), mCurve->symbol()->pen() , mCurve->symbol()->size() ) );
    mSymbolColor->setIcon(iconFromColor(col));

    mParent->replot();
}

void CurveSettingsDialog::SymbolFillColor()
{
    QColorDialog cd;
    QColor col = cd.getColor(mCurve->symbol()->brush().color());
    if(!col.isValid()) { return; }

    mCurve->setSymbol( new QwtSymbol( mCurve->symbol()->style() , QBrush(col), mCurve->symbol()->pen() , mCurve->symbol()->size() ) );
    mSymbolFillColor->setIcon(iconFromColor(col));

    mParent->replot();
}

QIcon CurveSettingsDialog::iconFromColor(QColor col)
{
    QPixmap pm(16,16);
    pm.fill(col);
    return QIcon(pm);
}

void CurveSettingsDialog::SymbolStyleChanged(int index)
{
    index--;
    mCurve->setSymbol( new QwtSymbol( (QwtSymbol::Style)index , mCurve->symbol()->brush(), mCurve->symbol()->pen() , mCurve->symbol()->size() ) );
    mParent->replot();
}

void CurveSettingsDialog::SymbolSizeChange(int size)
{
    mCurve->setSymbol( new QwtSymbol( mCurve->symbol()->style() , mCurve->symbol()->brush(), mCurve->symbol()->pen() , QSize(size,size) ) );
    mParent->replot();
}

void CurveSettingsDialog::LineColor()
{
    QColorDialog cd;
    QColor col = cd.getColor(mCurve->pen().color());
    if(!col.isValid()) { return; }
    mLineColor->setIcon(iconFromColor(col));
    mCurve->setPen(QPen(col));
    mParent->replot();
}

void CurveSettingsDialog::LineStyleChanged(int index)
{
    mCurve->setStyle((QwtPlotCurve::CurveStyle)(index-1));
    mParent->replot();
}

void CurveSettingsDialog::LineSizeChange(int size)
{
    mCurve->setPen(QPen( mCurve->pen().color(), size ));
    mParent->replot();
}

void CurveSettingsDialog::LineAntialiased(bool value)
{
    mCurve->setRenderHint(QwtPlotItem::RenderAntialiased,value);
    mParent->replot();
}
