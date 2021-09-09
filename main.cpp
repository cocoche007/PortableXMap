//------------------------------------------------------------------------------
//! @file main.cpp
//! @brief File where the main method is defined
//! @author Fabrice Cochet
//! @version 1.0
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Include(s) :

#include <QApplication>

#include "mainwindow.h"


//------------------------------------------------------------------------------
//! Application start point
//!
//! @param argc The argument number
//! @param argv The argument list
//!
//! @return 0 if OK
//! @return <>0 otherwise
//------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
    QApplication application(argc, argv);
    MainWindow   mainWindow;

    mainWindow.show();

    return application.exec();
}
