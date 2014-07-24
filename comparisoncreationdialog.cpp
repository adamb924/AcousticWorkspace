#include "comparisoncreationdialog.h"
#include "ui_comparisoncreationdialog.h"

#include <QComboBox>

#include "sound.h"
#include "intervalannotation.h"
#include "waveformdata.h"
#include "comparisonschema.h"

ComparisonCreationDialog::ComparisonCreationDialog(QList<Sound*> sounds, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ComparisonCreationDialog),
    mSounds(sounds)
{
    ui->setupUi(this);

    populatePrimarySounds();

    connect(ui->primarySoundCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(populatePrimaryIntervals()));
    connect(ui->secondarySoundCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(populateSecondaryIntervals()));
    connect(ui->primarySoundCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(populateSecondarySounds()));
    connect(ui->intervalAlignmentBox, SIGNAL(toggled(bool)), this, SLOT(setIntervalAlignment(bool)));

    connect(ui->secondarySoundCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(layoutComparisonWidgets()));

    populatePrimarySounds();
    layoutComparisonWidgets();
}

ComparisonCreationDialog::~ComparisonCreationDialog()
{
    delete ui;
}

ComparisonSchema ComparisonCreationDialog::comparisonSchema() const
{
    ComparisonSchema schema;
    schema.setIntervalComparison( ui->intervalAlignmentBox->isChecked() );
    schema.setIntervals( primarySound()->intervals()->at( ui->primaryIntervalCombo->currentData().toInt() ), secondarySound()->intervals()->at( ui->secondaryIntervalCombo->currentData().toInt() ) );
    schema.setStretchAlgorithm( ui->stretchAlgorithmCombo->currentText() );

    const QList<WaveformData *> *pWaveforms = primaryWaveforms();
    const QList<WaveformData *> *sWaveforms = secondaryWaveforms();
    for( int i=0; i<pWaveforms->count(); i++ )
    {
        int rightIndex = mRightWidgets.at(i)->currentData().toInt();
        if( rightIndex > -1 )
        {
            schema.addWaveformPair( WaveformPair( pWaveforms->at(i) , sWaveforms->at(rightIndex) ) );
        }
    }

    return schema;
}

void ComparisonCreationDialog::populatePrimarySounds()
{
    ui->primarySoundCombo->clear();
    for(int i=0; i<mSounds.count(); i++)
    {
        ui->primarySoundCombo->addItem( mSounds.at(i)->name(), i );
    }
    populateSecondarySounds();
}

void ComparisonCreationDialog::populateSecondarySounds()
{
    ui->secondarySoundCombo->clear();
    for(int i=0; i<mSounds.count(); i++)
    {
        if( mSounds.at(i)->name() != ui->primarySoundCombo->currentText() )
        {
            ui->secondarySoundCombo->addItem( mSounds.at(i)->name(), i );
        }
    }
}

void ComparisonCreationDialog::populatePrimaryIntervals()
{
    int index = ui->primarySoundCombo->currentData().toInt();
    if( index > -1 && index < mSounds.count() )
    {
        ui->primaryIntervalCombo->clear();
        for(int i=0; i<mSounds.at(index)->intervals()->count(); i++)
        {
            ui->primaryIntervalCombo->addItem( mSounds.at(index)->intervals()->at(i)->mName );
        }
    }
}

void ComparisonCreationDialog::populateSecondaryIntervals()
{
    int index = ui->secondarySoundCombo->currentData().toInt();
    if( index > -1 && index < mSounds.count() )
    {
        ui->secondaryIntervalCombo->clear();
        for(int i=0; i<mSounds.at(index)->intervals()->count(); i++)
        {
            ui->secondaryIntervalCombo->addItem( mSounds.at(index)->intervals()->at(i)->mName );
        }
    }
}

void ComparisonCreationDialog::setIntervalAlignment(bool value)
{
    ui->primaryIntervalCombo->setEnabled(value);
    ui->secondaryIntervalCombo->setEnabled(value);
    ui->stretchAlgorithmCombo->setEnabled(value);
}

void ComparisonCreationDialog::layoutComparisonWidgets()
{
    qDeleteAll(mLeftWidgets);
    mLeftWidgets.clear();
    qDeleteAll(mRightWidgets);
    mRightWidgets.clear();

    int pIndex = ui->primarySoundCombo->currentData().toInt();
    if( pIndex > -1 && pIndex < mSounds.count() )
    {
        const QList<WaveformData *> *pWaveforms = mSounds.at(pIndex)->waveformData();
        for( int i=0; i<pWaveforms->count(); i++ )
        {
            QLabel * leftWidget = new QLabel( pWaveforms->at(i)->name() );
            QComboBox * rightWidget = secondaryCurveCombo();

            int possibleMatch = rightWidget->findText(pWaveforms->at(i)->name());
            if( possibleMatch > -1 )
            {
                rightWidget->setCurrentIndex(possibleMatch);
            }

            ui->curveComparisonLayout->addWidget(leftWidget, i, 0);
            ui->curveComparisonLayout->addWidget(rightWidget, i, 1);

            mLeftWidgets.append( leftWidget );
            mRightWidgets.append( rightWidget );
        }
    }
}

QComboBox *ComparisonCreationDialog::secondaryCurveCombo()
{
    QComboBox *combo = new QComboBox;
    combo->addItem(tr("<None>"), -1);
    int sIndex = ui->secondarySoundCombo->currentData().toInt();
    if( sIndex > -1 && sIndex < mSounds.count() )
    {
        const QList<WaveformData *> *sWaveforms = mSounds.at(sIndex)->waveformData();
        for( int i=0; i<sWaveforms->count(); i++ )
        {
            combo->addItem( sWaveforms->at(i)->name() , i );
        }
    }
    return combo;
}

const QList<WaveformData *> * ComparisonCreationDialog::primaryWaveforms() const
{
    int pIndex = ui->primarySoundCombo->currentData().toInt();
    if( pIndex > -1 && pIndex < mSounds.count() )
    {
        return mSounds.at(pIndex)->waveformData();
    }
    else
    {
        return new const QList<WaveformData *>();
    }
}

const QList<WaveformData *> *ComparisonCreationDialog::secondaryWaveforms() const
{
    int sIndex = ui->secondarySoundCombo->currentData().toInt();
    if( sIndex > -1 && sIndex < mSounds.count() )
    {
        return mSounds.at(sIndex)->waveformData();
    }
    else
    {
        return new const QList<WaveformData *>();
    }
}

const Sound *ComparisonCreationDialog::primarySound() const
{
    int pIndex = ui->primarySoundCombo->currentData().toInt();
    if( pIndex > -1 && pIndex < mSounds.count() )
    {
        return mSounds.at(pIndex);
    }
    else
    {
        return 0;
    }
}

const Sound *ComparisonCreationDialog::secondarySound() const
{
    int sIndex = ui->secondarySoundCombo->currentData().toInt();
    if( sIndex > -1 && sIndex < mSounds.count() )
    {
        return mSounds.at(sIndex);
    }
    else
    {
        return 0;
    }
}
