#ifndef COMPARISONCREATIONDIALOG_H
#define COMPARISONCREATIONDIALOG_H

#include <QDialog>
#include <QList>
#include <QSet>

class Sound;
class QComboBox;
class QLabel;
class ComparisonSchema;
class WaveformData;

namespace Ui {
class ComparisonCreationDialog;
}

class ComparisonCreationDialog : public QDialog
{
    Q_OBJECT

public:
    ComparisonCreationDialog(QList<Sound*> sounds, QWidget *parent = 0);
    ~ComparisonCreationDialog();

    ComparisonSchema comparisonSchema() const;

private slots:
    void populatePrimarySounds();
    void populateSecondarySounds();
    void populatePrimaryIntervals();
    void populateSecondaryIntervals();
    void setIntervalAlignment(bool value);
    void layoutComparisonWidgets();

private:
    QComboBox * secondaryCurveCombo();
    const QList<WaveformData *> *primaryWaveforms() const;
    const QList<WaveformData *> *secondaryWaveforms() const;
    const Sound *primarySound() const;
    const Sound *secondarySound() const;

    Ui::ComparisonCreationDialog *ui;

    QList<Sound*> mSounds;
    QList<QLabel*> mLeftWidgets;
    QList<QComboBox*> mRightWidgets;
};

#endif // COMPARISONCREATIONDIALOG_H
