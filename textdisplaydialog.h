/*!
  \class TextDisplayDialog
  \ingroup SupportWidgets
  \brief A data class for displaying text data.

  This class provides a simple interface for displaying text data.
*/

#ifndef TEXTDISPLAYDIALOG_H
#define TEXTDISPLAYDIALOG_H

#include <QDialog>
#include <QWidget>
#include <QPlainTextEdit>
#include <QHBoxLayout>

class TextDisplayDialog : public QDialog
{
    Q_OBJECT
public:
    /*!
      \brief Construct a dialog and display \a message.

      The constructor creates the dialog with a single QPlainTextEdit widget to display \a message.
      \param message The message to be displayed.
      \param parent The parent of the widget
      */
    TextDisplayDialog(QString message, QWidget *parent) : QDialog(parent)
    {
	QPlainTextEdit *txt = new QPlainTextEdit(message,this);
	QHBoxLayout *layout = new QHBoxLayout;
	layout->addWidget(txt);
	setLayout(layout);
    }
};

#endif // TEXTDISPLAYDIALOG_H
