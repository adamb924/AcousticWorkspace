#include "spectrogramsettingsdialog.h"

#include <qwt_plot.h>
#include <qwt_plot_spectrogram.h>
#include <qwt_scale_engine.h>

#include <QLineEdit>
#include <QVBoxLayout>
#include <QLabel>
#include <QDoubleValidator>
#include <QPushButton>
#include <QtDebug>

SpectrogramSettingsDialog::SpectrogramSettingsDialog(QwtPlotSpectrogram *spectrogram, QwtPlot *parentPlot, QWidget *parent) :
    QDialog(parent)
{
    plot = parentPlot;

    QVBoxLayout *layout = new QVBoxLayout;

    QLineEdit *fromFrequency = new QLineEdit;
    QLineEdit *toFrequency = new QLineEdit;

    fromFrequency->setValidator(new QDoubleValidator(spectrogram->boundingRect().bottom(),spectrogram->boundingRect().top(),2,this));
    toFrequency->setValidator(new QDoubleValidator(spectrogram->boundingRect().bottom(),spectrogram->boundingRect().top(),2,this));

    fromFrequency->setText(QString::number(parentPlot->axisScaleDiv(QwtPlot::yLeft)->lowerBound()));
    toFrequency->setText(QString::number(parentPlot->axisScaleDiv(QwtPlot::yLeft)->upperBound()));

    connect(fromFrequency,SIGNAL(textEdited(QString)),this,SLOT(setFrequencyLowerbound(QString)));
    connect(toFrequency,SIGNAL(textEdited(QString)),this,SLOT(setFrequencyUpperbound(QString)));

    QPushButton *colorMapButton = new QPushButton(tr("Edit Color Map"));
    connect(colorMapButton,SIGNAL(clicked()),this,SLOT(setColorMap()));

    layout->addWidget(new QLabel(tr("From frequency (Hz)")));
    layout->addWidget(fromFrequency);
    layout->addWidget(new QLabel(tr("To frequency (Hz)")));
    layout->addWidget(toFrequency);

    this->setLayout(layout);
}

void SpectrogramSettingsDialog::setFrequencyUpperbound(QString text)
{
    QwtLinearScaleEngine engine;
    plot->setAxisScaleDiv(QwtPlot::yLeft,QwtScaleDiv( engine.divideScale(plot->axisScaleDiv(QwtPlot::yLeft)->lowerBound(), text.toDouble() ,10, 10) ));
    plot->replot();
}

void SpectrogramSettingsDialog::setFrequencyLowerbound(QString text)
{
    QwtLinearScaleEngine engine;
    plot->setAxisScaleDiv(QwtPlot::yLeft,QwtScaleDiv( engine.divideScale(text.toDouble(), plot->axisScaleDiv(QwtPlot::yLeft)->upperBound(),10, 10) ));
    plot->replot();
}

void SpectrogramSettingsDialog::setColorMap()
{
    qDebug() << "SpectrogramSettings::setColorMap";
}
