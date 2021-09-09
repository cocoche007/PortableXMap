//------------------------------------------------------------------------------
//! @file MainWindow.cpp
//! @brief Definition file of the MainWindow class
//! @author Fabrice Cochet
//! @version 1.0
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Include(s) :

#include <QAction>
#include <QApplication>
#include <QFileDialog>
#include <QMenu>
#include <QMenuBar>
#include <QVBoxLayout>

#include "mainwindow.h"
#include "portablexmap.h"


//------------------------------------------------------------------------------
// Public Method(s):

//------------------------------------------------------------------------------
//! MainWindow constructor
//!
//! @param parent The parent widget
//!
//! @return The created object
//------------------------------------------------------------------------------
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QWidget *     centralZone  = new QWidget;
    QFileDialog * fileDialog   = new QFileDialog(this, Qt::Dialog);
    QMenu *       fileMenu     = menuBar()->addMenu(tr("&File"));
    QAction *     openAction   = new QAction(tr("&Open"), this);
    QAction *     quitAction   = new QAction(tr("&Quit"), this);
    QVBoxLayout * layout       = new QVBoxLayout;
    PortableXMap *portableXmap = new PortableXMap;

    fileMenu->addAction(openAction);
    fileMenu->addAction(quitAction);

    connect(openAction, SIGNAL(triggered()), fileDialog, SLOT(open()));
    connect(fileDialog, SIGNAL(fileSelected(QString)), portableXmap, SLOT(processFile(QString)));
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));

    layout->addWidget(portableXmap);
    centralZone->setLayout(layout);

    setCentralWidget(centralZone);
}


//------------------------------------------------------------------------------
//! Destructor
//!
//! @param _
//!
//! @return _
//------------------------------------------------------------------------------
MainWindow::~MainWindow()
{
}
