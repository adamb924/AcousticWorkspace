#ifndef REGRESSION_H
#define REGRESSION_H

#include <QList>

#include "regressiondialog.h"

class SpectrogramData;
class WaveformData;
class RegressionListItem;
class RegressionInteractionListItem;

/*!
  \class InteractionEffect
  \ingroup Regression
  \brief A class for storing interaction terms in a RegressionModel.

  This class stores pointers to WaveformData objects, for example pointers to A, B, and C to represent the A-by-B-by-C interaction.
*/
class InteractionEffect
{
public:
    InteractionEffect();

    //! \brief The members variable is public out of convenience.
    QList<WaveformData*> members;

    //! \brief Return the name of the interaction, which is the concatenated list of names of the WaveformData objects, delimited by a colon (:)
    QString name();
    //! \brief Return the safe name of the interaction, which is the name() of the interaction with whitespace characters removed
    QString safeName();
};

/*!
  \class RegressionModel
  \ingroup Regression
  \brief A class for representing a linear regression model

  This class stores information needed for a linear regression model. The dependent variable can either be a set of WaveformData object or a SpectrogramData object. Independent variables must be WaveformData objects. fit() fits the regression model, and R() generates R code and data files for the model.
*/
class RegressionModel
{
public:
    //! \brief The class must be initialized by subsequent calls to member functions.
    RegressionModel();
    ~RegressionModel();

    //! \brief Get the name of the regression.
    QString name() const;
    //! \brief Set the name of the regression
    void setName(QString n);

    //! \brief Return true if the dependent variable(s) in the regression is a SpectrogramData object.
    bool dependentIsSpectrogram() const;

    //! \brief Return true if the regression has an intercept term.
    bool hasIntercept() const;

    //! \brief Set whether the regression has an intercept term.
    void setInterceptTerm(bool value);

    //! \brief Set the dependent variable of the regression to a particular SpectrogramData object.
    void setDependentSpectrogram(SpectrogramData *dep);

    //! \brief Add dependent variables to the regression.
    /*!
      For convenience this function takes a list of pointers to RegressionListItem objects, which are used in the RegressionDialog class.
      \param dep A list of pointers to dependent variables.
      */
    void addDependentVariables(QList<RegressionListItem*> dep);

    //! \brief Add independent variables to the regression
    /*!
      For convenience this function takes a list of pointers to RegressionListItem objects and RegressionInteractionListItem objects, which are used in the RegressionDialog class.
      \param dep A list of pointers to dependent variables.
      */
    void addIndependentVariables(QList<RegressionListItem*> simple, QList<RegressionInteractionListItem*> interaction);

    //! \brief Fit the regression with FFTW routines, and display a brief summary of the results.
    void fit();

    //! \brief Generate data files and R code to run the regressons externally.
    void R();

    QList<WaveformData*> dependent;
    SpectrogramData *dependentSpectrogram;

    QList<WaveformData*> simple;
    QList<InteractionEffect*> interaction;
private:
    bool interceptTerm;
    bool spectrogramMode;
    QString label;
};

#endif // REGRESSION_H
