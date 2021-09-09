//------------------------------------------------------------------------------
//! @file portablexmap.h
//! @brief Header file of the PortableXMap class
//! @author Fabrice Cochet
//! @version 1.0
//------------------------------------------------------------------------------

#ifndef PORTABLEXMAP_H
#define PORTABLEXMAP_H

//------------------------------------------------------------------------------
// Include(s) :

#include <QTextStream>
#include <QWidget>


//------------------------------------------------------------------------------
// Enumeration(s) :

enum
{
    TYPE_UNKNOWN,
    TYPE_PBM_ASCII,
    TYPE_PGM_ASCII,
    TYPE_PPM_ASCII,
    TYPE_PBM_BIN,
    TYPE_PGM_BIN,
    TYPE_PPM_BIN
};


//------------------------------------------------------------------------------
// Classe(s) :

class PortableXMap : public QWidget
{
    Q_OBJECT

public:
    PortableXMap(QWidget *parent = 0);

protected:
    void paintEvent(QPaintEvent *event);

private:
    QString        m_fileName;
    unsigned char  m_fileType;
    unsigned short m_width;
    unsigned short m_height;
    int            m_maxVal;
    int            m_image[2000][2000][3];

    void processFileP1(QTextStream *text);
    void processFileP2(QTextStream *text);
    void processFileP3(QTextStream *text);
    void processFileP4(QTextStream *text);
    void processFileP5(QTextStream *text);
    void processFileP6(QTextStream *text);

signals:

public slots:
    void processFile(QString fileName);
};

#endif // PORTABLEXMAP_H
