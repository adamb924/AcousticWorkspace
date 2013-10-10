/*!
  \class SpectrogramSettingsDialog
  \ingroup DataEntry
  \brief A dialog for changing the settings of a spectrogram.

  The class is constructed with a pointer to the spectrogram plot item that will be modified, and a pointer to the QwtPlot that will be replotted after changes are made.
*/

#ifndef SPECTROGRAMSETTINGS_H
#define SPECTROGRAMSETTINGS_H

#include <QWidget>
#include <QDialog>

class QwtPlot;
class QwtPlotSpectrogram;

class SpectrogramSettingsDialog : public QDialog
{
    Q_OBJECT
public:
    //! \brief Constructs and displays the dialog based on the given parameters
    /*!
      \param curve The spectrogram plot item that will be modified
      \param parentPlot The plot the spectrogram plot item belongs to
    */
    explicit SpectrogramSettingsDialog(QwtPlotSpectrogram *spectrogram, QwtPlot *parentPlot, QWidget *parent = 0);

private:
    QwtPlot *plot;

signals:

public slots:
    void setFrequencyUpperbound(QString text);
    void setFrequencyLowerbound(QString text);

private slots:
    void setColorMap();
};

#endif // SPECTROGRAMSETTINGS_H
