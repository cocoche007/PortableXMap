//------------------------------------------------------------------------------
//! @file MainWindow.h
//! @brief Header file of the MainWindow class
//! @author Fabrice Cochet
//! @version 1.0
//------------------------------------------------------------------------------

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

//------------------------------------------------------------------------------
// Include(s) :

#include <QMainWindow>


//------------------------------------------------------------------------------
// Classe(s) :

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = NULL);
    ~MainWindow();
};

#endif // MAINWINDOW_H
