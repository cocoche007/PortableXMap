#ifndef PORTABLEXMAP_H
#define PORTABLEXMAP_H

#include <QWidget>
#include <QTextStream>


enum
{
    TYPE_INCONNU,
    TYPE_PBM_ASCII,
    TYPE_PGM_ASCII,
    TYPE_PPM_ASCII,
    TYPE_PBM_BIN,
    TYPE_PGM_BIN,
    TYPE_PPM_BIN
};


class PortableXMap : public QWidget
{
    Q_OBJECT

public :
    PortableXMap( QWidget *parent = 0);

protected :
    void paintEvent( QPaintEvent *event);

private :
    QString        nomFichier;
    unsigned char  typeFichier;
    unsigned short x;
    unsigned short y;
    int            maxVal;
    int            image[2000][2000][3];

    void traiteFichierP1( QTextStream *texte);
    void traiteFichierP2( QTextStream *texte);
    void traiteFichierP3( QTextStream *texte);
    void traiteFichierP4( QTextStream *texte);
    void traiteFichierP5( QTextStream *texte);
    void traiteFichierP6( QTextStream *texte);

signals :
    
public slots :
    void traiteFichier( QString string);
};

#endif // PORTABLEXMAP_H
