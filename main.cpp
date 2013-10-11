/*!
 \defgroup Regression Regression-related classes
 \defgroup MajorDialog Dialogs that provide major program functionality
 \defgroup GUI GUI- and display-related classes
 \defgroup DataEntry Minor data-entry dialogs
 \defgroup SupportWidgets Support classes & widgets
 \defgroup Data Data classes
 \defgroup Plugin Plugin interface classes
 \defgroup Annotation Annotation-related classes
 */

#include <QtWidgets/QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
