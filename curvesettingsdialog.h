/*!
  \class CurveSettingsDialog
  \ingroup DataEntry
  \brief A dialog for changing the settings of a curve.

  The class is constructed with a pointer to the curve plot item that will be modified, and a pointer to the QwtPlot that will be replotted after changes are made.
*/

#ifndef CURVESETTINGS_H
#define CURVESETTINGS_H

#include <QWidget>
#include <QDialog>
#include <qwt/qwt_symbol.h>

class QPushButton;
class QBrush;
class QPen;
class QSize;
class QwtCurve;
class QIcon;
class QColor;
class QwtPlotCurve;
class QwtPlot;

class CurveSettingsDialog : public QDialog
{
    Q_OBJECT
public:
    //! \brief Constructs and displays the dialog based on the given parameters
    /*!
      \param curve The curve that will be modified
      \param parentPlot The plot the curve belongs to
    */
    explicit CurveSettingsDialog(QwtPlotCurve *curve, QwtPlot *parentPlot, QWidget *parent);
private:
    QPushButton *symbolColor;
    QPushButton *lineColor;
    QPushButton *symbolFillColor;

    //! \brief Return a QIcon that is a square filled with the specified color (convenience function)
    QIcon iconFromColor(QColor col);

    QwtPlotCurve *curve;
    QwtPlot *parent;

private slots:
    //! \brief Prompts the user to select a color for the symbol borders, and modifies the curve accordingly
    void SymbolBorderColor();

    //! \brief Prompts the user to select a color for the symbol fills, and modifies the curve accordingly
    void SymbolFillColor();

    //! \brief Modifies the curve make the symbol style the \a index-th in the symbol style QComboBox
    void SymbolStyleChanged(int index);

    //! \brief Modifies the curve make the symbol size equal to \a index
    void SymbolSizeChange(int size);

    //! \brief Prompts the user to select a color for the lines, and modifies the curve accordingly
    void LineColor();

    //! \brief Modifies the curve make the symbol style the \a index-th in the line style QComboBox
    void LineStyleChanged(int index);

    //! \brief Modifies the curve make the line size (width) equal to \a index
    void LineSizeChange(int size);

    //! \brief If \a value is true, makes the curve antialiased, otherwise makes it aliased
    void LineAntialiased(bool value);
};

#endif // CURVESETTINGS_H
