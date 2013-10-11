#include "curvesettingsdialog.h"

#include <qwt/qwt_symbol.h>
#include <qwt/qwt_plot_curve.h>
#include <qwt/qwt_plot.h>

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

CurveSettingsDialog::CurveSettingsDialog(QwtPlotCurve *curve, QwtPlot *parentPlot, QWidget *parent) : QDialog(parent)
{
    this->curve = curve;
    this->parent = parentPlot;

//    qDebug() << this->curve->pen().color();

    QVBoxLayout *vlayout = new QVBoxLayout;

    vlayout->addWidget(new QLabel(tr("Symbol Settings")));

    symbolColor = new QPushButton(iconFromColor(curve->symbol()->pen().color()),"Symbol Color");
    connect(symbolColor,SIGNAL(clicked()),this,SLOT(SymbolBorderColor()));
    vlayout->addWidget(symbolColor,0,Qt::AlignHCenter);

    symbolFillColor = new QPushButton(iconFromColor(curve->symbol()->brush().color()),"Symbol Fill Color");
    connect(symbolFillColor,SIGNAL(clicked()),this,SLOT(SymbolFillColor()));
    vlayout->addWidget(symbolFillColor,0,Qt::AlignHCenter);

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

    lineColor = new QPushButton(iconFromColor(curve->pen().color() ),"Line Color");
    connect(lineColor,SIGNAL(clicked()),this,SLOT(LineColor()));
    vlayout->addWidget(lineColor,0,Qt::AlignHCenter);

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
    QColor col = cd.getColor(curve->symbol()->pen().color());
    if(!col.isValid()) { return; }

    curve->setSymbol( new QwtSymbol( curve->symbol()->style() , curve->symbol()->brush(), curve->symbol()->pen() , curve->symbol()->size() ) );
    symbolColor->setIcon(iconFromColor(col));

    parent->replot();
}

void CurveSettingsDialog::SymbolFillColor()
{
    QColorDialog cd;
    QColor col = cd.getColor(curve->symbol()->brush().color());
    if(!col.isValid()) { return; }

    curve->setSymbol( new QwtSymbol( curve->symbol()->style() , QBrush(col), curve->symbol()->pen() , curve->symbol()->size() ) );
    symbolFillColor->setIcon(iconFromColor(col));

    parent->replot();
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
    curve->setSymbol( new QwtSymbol( (QwtSymbol::Style)index , curve->symbol()->brush(), curve->symbol()->pen() , curve->symbol()->size() ) );
    parent->replot();
}

void CurveSettingsDialog::SymbolSizeChange(int size)
{
    curve->setSymbol( new QwtSymbol( curve->symbol()->style() , curve->symbol()->brush(), curve->symbol()->pen() , QSize(size,size) ) );
    parent->replot();
}

void CurveSettingsDialog::LineColor()
{
    QColorDialog cd;
    QColor col = cd.getColor(curve->pen().color());
    if(!col.isValid()) { return; }
    lineColor->setIcon(iconFromColor(col));
    curve->setPen(QPen(col));
    parent->replot();
}

void CurveSettingsDialog::LineStyleChanged(int index)
{
    curve->setStyle((QwtPlotCurve::CurveStyle)(index-1));
    parent->replot();
}

void CurveSettingsDialog::LineSizeChange(int size)
{
    curve->setPen(QPen( curve->pen().color(), size ));
    parent->replot();
}

void CurveSettingsDialog::LineAntialiased(bool value)
{
    curve->setRenderHint(QwtPlotItem::RenderAntialiased,value);
    parent->replot();
}
