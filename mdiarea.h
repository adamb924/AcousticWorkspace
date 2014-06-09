/*!
  \class MdiArea
  \ingroup GUI
  \brief A subclass of QMdiArea.

  This class provides the MDI area for the application.
*/

#ifndef MDI_H
#define MDI_H

#include <QMdiArea>
#include <QList>

class MainWindow;
class SoundWidget;

class MdiArea : public QMdiArea
{
    Q_OBJECT
public:
    explicit MdiArea(QWidget *parent = 0);


private:
    MainWindow *mMainWnd; /*!< Pointer to the parent window */

    //! \brief Return a pointer to a list of pointers to SoundWidget objects.
    QList<SoundWidget*>* soundWindows();

signals:

public slots:
    //! \brief Create a new sound-display child window
    void newSoundWindow();

    //! \brief Create a new sound-comparison child window
    void newComparisonWindow();

};

#endif // MDI_H
