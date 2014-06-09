/*! \class DataEntryDialog
    \ingroup DataEntry
    \brief A dialog class for entering simple parameters.

    This class provides a convenient way to collect parameters from users. There is no data checking. The QVariant data class is used for maximum flexibility.
  */

#ifndef DATAENTRYWIDGET_H
#define DATAENTRYWIDGET_H

#include <QDialog>
#include <QWidget>
#include <QList>
#include <QLineEdit>
#include <QList>
#include <QVariant>

class QStringList;

class DataEntryDialog : public QDialog
{
Q_OBJECT
public:
    //! \brief Constructs and displays the dialog
    /*!
      \param f A list of the names of the values to be entered.
      \param v A list of values with which the dialog is initialized, and which are subsequently modified.
      \param label A string to be displayed at the top of the dialog. If the string is empty no text is displayed.
      */
    explicit DataEntryDialog(const QStringList* f, QList<QVariant>* v, QString label, QWidget *parent);

    QList<QLineEdit*> mEdits;
    QList<QVariant> mValues;

signals:

public slots:
    //! \brief Copies the values from the QLineEdits into list of QVariants
    void accept();

};

#endif // DATAENTRYWIDGET_H
