#include <QApplication>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QFileDialog>
#include <QVBoxLayout>
#include "portablexmap.h"
#include "mainwindow.h"


MainWindow::MainWindow( QWidget *parent)
    : QMainWindow( parent)
{
    QWidget      *zoneCentrale  = new QWidget;
    QFileDialog  *fileDialog    = new QFileDialog( this, Qt::Dialog);
    QMenu        *menuFichier   = menuBar()->addMenu( tr( "&Fichier"));
    QAction      *actionOuvrir  = new QAction( tr( "&Ouvrir"), this);
    QAction      *actionQuitter = new QAction( tr( "&Quitter"), this);
    QVBoxLayout  *layout        = new QVBoxLayout;
    PortableXMap *portableXmap  = new PortableXMap;

    menuFichier->addAction( actionOuvrir);
    menuFichier->addAction( actionQuitter);

    connect( actionOuvrir , SIGNAL( triggered())           , fileDialog  , SLOT(open()));
    connect( fileDialog   , SIGNAL( fileSelected( QString)), portableXmap, SLOT( traiteFichier( QString)));
    connect( actionQuitter, SIGNAL(triggered())            , qApp        , SLOT(quit()));

    layout->addWidget( portableXmap);
    zoneCentrale->setLayout( layout);

    setCentralWidget( zoneCentrale);
}


MainWindow::~MainWindow()
{
    
}
