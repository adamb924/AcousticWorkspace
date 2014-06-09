#ifndef REGRESSIONDIALOG_H
#define REGRESSIONDIALOG_H

#include <QDialog>
#include <QList>
#include <QListWidgetItem>

#include "waveformdata.h"
#include "spectrogramdata.h"
#include "regression.h"

class InteractionEffect;
class QCheckBox;


/*!
  \class RegressionListItem
  \ingroup Regression
  \brief A subclass of QListWidgetItem for items that represent WaveformData objects in regressions.

  This class is used for particularly in RegressionDialog.
*/
class RegressionListItem : public QListWidgetItem
{
public:
    //! \brief Initialize the member variable
    explicit RegressionListItem( WaveformData *data , QListWidget * parent = 0, int type = Type );

    //! \brief Return a pointer to the object's WaveformData
    WaveformData* data();

    //! \brief Return a the name of the item (the name of the object's WaveformData member)
    QString text() const;

private:
    WaveformData *mWaveformData;
};

/*!
  \class RegressionSpectrogramListItem
  \ingroup Regression
  \brief A subclass of QListWidgetItem for items that represent SpectrogramData objects in regressions.

  This class is used for particularly in RegressionDialog. In this usage, RegressionSpectrogramListItem happen always to be dependent variables.
*/
class RegressionSpectrogramListItem : public QListWidgetItem
{
public:
    explicit RegressionSpectrogramListItem( SpectrogramData *data , QListWidget * parent = 0, int type = Type );

    //! \brief Return a pointer to the SpectrogramData member
    SpectrogramData* data();

    //! \brief Return a the name of the item (the name of the object's spectrogramData member)
    QString text() const;

private:
    SpectrogramData *mSpectrogramData;
};

/*!
  \class RegressionInteractionListItem
  \ingroup Regression
  \brief A subclass of QListWidgetItem for items that represent InteractionEffect objects (interaction effects) in regressions.

  This class is used for particularly in RegressionDialog. The interaction effects are obviously for dependent variables.
*/
class RegressionInteractionListItem : public QListWidgetItem
{
public:
    explicit RegressionInteractionListItem( InteractionEffect *data , QListWidget * parent = 0, int type = Type );

    //! \brief Return a pointer to the InteractionEffect member
    InteractionEffect* interaction();

    //! \brief Return a readable version of the name of the InteractionEffect
    QString text() const;

private:
    InteractionEffect *mInteractionData;
};

class RegressionModel;

/*!
  \class RegressionDialog
  \ingroup Regression
  \brief A dialog class that provides a user interface for construction regression models.

  This class is used for particularly in RegressionDialog.
*/
class RegressionDialog : public QDialog
{
    Q_OBJECT
public:
    //! \brief Constructs the dialog, using the data available to the model
    RegressionDialog(QList<WaveformData*> *wfd, QList<SpectrogramData*> *spd, QWidget *parent = 0);

    //! \brief Initialize the dialog's widgets from an existing RegressionModel object
    void setFromRegression(RegressionModel *model);

private:
    //! \brief Return a list of the checked dependent variables (WaveformData objects)
    /*!
      \param excluding Do not return this item with the list
      */
    QList<RegressionListItem*> checkedDependent(RegressionListItem *excluding);

    //! \brief Return a list of the checked simple effects
    /*!
      \param excluding Do not return this item with the list
      */
    QList<RegressionListItem*> checkedSimple(RegressionListItem *excluding);

    //! \brief Return a list of the checked interaction terms
    /*!
      \param excluding Do not return this item with the list
      */
    QList<RegressionInteractionListItem*> checkedInteraction(RegressionListItem *excluding);

    //! \brief Add all of the interaction effects to the QListWidget of interaction effects
    /*!
      \param list Add all combinations from list list.
      \param n Add from 2-way through n-way interaction terms.
      */
    void addNWayInteractions(QList<RegressionListItem*> *list, int n);

    QListWidget *mDependentList;
    QListWidget *mDependentSpectrogramList;
    QListWidget *mIndependentSimpleList;
    QListWidget *mIndependentInteractionList;

    QLineEdit *mNameEdit;
    QCheckBox *mInterceptTerm;

signals:
    //! \brief Emit a regression model (associated with a slot in SoundWidget)
    void regressionObject(RegressionModel *model);

public slots:

private slots:
    //! \brief Adds all 2-way interactions to the QListWidget of interaction effects.
    void int2x();

    //! \brief Adds all 3-way interactions to the QListWidget of interaction effects.
    void int3x();

    //! \brief Adds all possible interactions to the QListWidget of interaction effects.
    void intAll();

    //! \brief Adds all 2-way interactions to the QListWidget of interaction effects, for a particular simple effect.
    void int2xSpecific();

    //! \brief When a simple effect is checked, this function checks that the times of the effect are congruent with the dependent variable and the other independent variables.
    void independentSimpleChanged(QListWidgetItem* item);

    //! \brief When a dependent variable is checked, this function checks that the times of the variable are congruent with the dependent variables and the independent variables.
    void dependentChanged(QListWidgetItem* item);

    //! \brief When a dependent spectrogram is checked, this function checks that the times of the variable are congruent with the independent variables.
    void dependentSpectrogramChanged(QListWidgetItem* item);

    //! \brief Assembles a RegressionModel object, and calls the function to fit it and display a summary.
    void calculateRegression();

    //! \brief Assembles a RegressionModel object, and calls the function to generate R-code and data files.
    void createRDataCode();

    //! \brief Assembles a RegressionModel object and emits the regressionObject signal so that it can be stored.
    void saveRegression();
};

#endif // REGRESSIONDIALOG_H
