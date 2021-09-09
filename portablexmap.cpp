//------------------------------------------------------------------------------
//! @file portablexmap.cpp
//! @brief Definition file of the PortableXMap class
//! @author Fabrice Cochet
//! @version 1.0
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Include(s):

#include <QFile>
#include <QPainter>
#include <QTextStream>

#include "portablexmap.h"


//------------------------------------------------------------------------------
// Public Method(s):

//------------------------------------------------------------------------------
//! CEdgeDetection constructor for a given parent widget
//!
//! @param parent The parent widget
//!
//! @return The created object
//------------------------------------------------------------------------------

PortableXMap::PortableXMap(QWidget *parent) : QWidget(parent),
                                              m_fileName(""),
                                              m_fileType(TYPE_UNKNOWN),
                                              m_x(0),
                                              m_y(0),
                                              m_maxVal(0),
                                              m_image({0})
{
}


//------------------------------------------------------------------------------
// Public Slot Method(s):

//------------------------------------------------------------------------------
//! The slot method in order to process a file by its name
//!
//! @param fileName The file name
//!
//! @return _
//------------------------------------------------------------------------------
void PortableXMap::processFile(QString fileName)
{
    QFile       file(fileName);
    QTextStream text(&file);

    m_fileName = fileName;
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QString line;
        char    type;

        line = text.readLine();
        type = line.at(1).toLatin1();

        do
        {
            line = text.readLine();
        } while (line[0] == QChar('#'));

        QTextStream(&line) >> m_x >> m_y;

        switch (type)
        {
            case '1':
                processFileP1(&text);
                break;

            case '2':
                processFileP2(&text);
                break;

            case '3':
                processFileP3(&text);
                break;

            case '4':
                processFileP4(&text);
                break;

            case '5':
                processFileP5(&text);
                break;

            case '6':
                processFileP6(&text);
                break;

            default:
                m_fileType = TYPE_UNKNOWN;
                break;
        }

        if (m_fileType != TYPE_UNKNOWN)
            update();

        file.close();
    }
}


//------------------------------------------------------------------------------
// Protected Method(s):

//------------------------------------------------------------------------------
//! The paint event hook
//!
//! @param _ ?
//!
//! @return _
//------------------------------------------------------------------------------
void PortableXMap::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    if (m_fileType != TYPE_UNKNOWN)
    {
        setFixedSize(m_x, m_y);

        for (int i = 0; i < m_x; i++)
        {
            for (int j = 0; j < m_y; j++)
            {
                painter.setPen(QColor(m_image[i][j][0], m_image[i][j][1], m_image[i][j][2]));
                painter.drawPoint(i, j);
            }
        }
    }
}


//------------------------------------------------------------------------------
// Private Method(s):

//------------------------------------------------------------------------------
//! Process a P1 file by its content
//!
//! @param text The content to process
//!
//! @return _
//------------------------------------------------------------------------------
void PortableXMap::processFileP1(QTextStream *text)
{
    QString line;
    int     value;

    m_fileType = TYPE_PBM_ASCII;

    for (int j = 0; j < m_y; j++)
    {
        for (int i = 0; i < m_x; i++)
        {
            *text >> value;

            if (value)
                m_image[i][j][0] = m_image[i][j][1] = m_image[i][j][2] = 255;
            else
                m_image[i][j][0] = m_image[i][j][1] = m_image[i][j][2] = 0;
        }
    }
}


//------------------------------------------------------------------------------
//! Process a P2 file by its content
//!
//! @param text The content to process
//!
//! @return _
//------------------------------------------------------------------------------
void PortableXMap::processFileP2(QTextStream *text)
{
    QString line;

    m_fileType = TYPE_PGM_ASCII;

    line = text->readLine();
    QTextStream(&line) >> m_maxVal;

    for (int j = 0; j < m_y; j++)
    {
        for (int i = 0; i < m_x; i++)
        {
            *text >> m_image[i][j][0];

            m_image[i][j][0] = (m_image[i][j][0] * 255) / m_maxVal;

            m_image[i][j][2] = m_image[i][j][1] = m_image[i][j][0];
        }
    }
}


//------------------------------------------------------------------------------
//! Process a P3 file by its content
//!
//! @param text The content to process
//!
//! @return _
//------------------------------------------------------------------------------
void PortableXMap::processFileP3(QTextStream *text)
{
    QString line;

    m_fileType = TYPE_PPM_ASCII;

    line = text->readLine();
    QTextStream(&line) >> m_maxVal;

    for (int j = 0; j < m_y; j++)
    {
        for (int i = 0; i < m_x; i++)
        {
            *text >> m_image[i][j][0];
            *text >> m_image[i][j][1];
            *text >> m_image[i][j][2];

            m_image[i][j][0] = (m_image[i][j][0] * 255) / m_maxVal;
            m_image[i][j][1] = (m_image[i][j][1] * 255) / m_maxVal;
            m_image[i][j][2] = (m_image[i][j][2] * 255) / m_maxVal;
        }
    }
}


//------------------------------------------------------------------------------
//! Process a P4 file by its content
//!
//! @param text The content to process
//!
//! @return _
//------------------------------------------------------------------------------
void PortableXMap::processFileP4(QTextStream *text)
{
    QString line;

    qint64      pos;
    QFile       file(m_fileName);
    QDataStream data(&file);

    m_fileType = TYPE_PBM_BIN;

    pos = text->pos();

    if (file.open(QIODevice::ReadOnly))
    {
        int i = 0;
        int j = 0;

        data.skipRawData(pos);

        for (int u = 0; u < m_x * m_y / 8; u++)
        {
            unsigned char value;

            data >> value;
            for (int v = 7; v >= 0; v--)
            {
                if ((value >> v) & 0x01)
                    m_image[i][j][0] = m_image[i][j][1] = m_image[i][j][2] = 255;
                else
                    m_image[i][j][0] = m_image[i][j][1] = m_image[i][j][2] = 0;

                i++;
                if (i == m_x)
                {
                    i = 0;
                    j++;
                }
            }
        }

        /*for ( j = 0 ; j < m_y ; j++ )
        {
            for ( i = 0 ; i < m_x ; i++ )
            {
                unsigned char value;

                data >> value;

                if ( value )
                    m_image[i][j][0] = m_image[i][j][1] = m_image[i][j][2] = 255;
                else
                    m_image[i][j][0] = m_image[i][j][1] = m_image[i][j][2] = 0;
            }
        }*/

        file.close();
    }
}


//------------------------------------------------------------------------------
//! Process a P5 file by its content
//!
//! @param text The content to process
//!
//! @return _
//------------------------------------------------------------------------------
void PortableXMap::processFileP5(QTextStream *text)
{
    QString     line;
    qint64      pos;
    QFile       file(m_fileName);
    QDataStream data(&file);

    m_fileType = TYPE_PGM_BIN;

    line = text->readLine();
    QTextStream(&line) >> m_maxVal;

    pos = text->pos();

    if (file.open(QIODevice::ReadOnly))
    {
        data.skipRawData(pos);

        for (int j = 0; j < m_y; j++)
        {
            for (int i = 0; i < m_x; i++)
            {
                unsigned char value;

                data >> value;
                m_image[i][j][0] = ((unsigned char) value) * 255 / m_maxVal;
                m_image[i][j][0] = (unsigned char) value;
                m_image[i][j][2] = m_image[i][j][1] = m_image[i][j][0];
            }
        }

        file.close();
    }
}


//------------------------------------------------------------------------------
//! Process a P6 file by its content
//!
//! @param text The content to process
//!
//! @return _
//------------------------------------------------------------------------------
void PortableXMap::processFileP6(QTextStream *text)
{
    QString     line;
    qint64      pos;
    QFile       file(m_fileName);
    QDataStream data(&file);

    m_fileType = TYPE_PPM_BIN;

    line = text->readLine();
    QTextStream(&line) >> m_maxVal;

    pos = text->pos();

    if (file.open(QIODevice::ReadOnly))
    {
        data.skipRawData(pos);
        for (int j = 0; j < m_y; j++)
        {
            for (int i = 0; i < m_x; i++)
            {
                unsigned char value;

                data >> value;
                m_image[i][j][0] = value;
                data >> value;
                m_image[i][j][1] = value;
                data >> value;
                m_image[i][j][2] = value;

                m_image[i][j][0] = (m_image[i][j][0] * 255) / m_maxVal;
                m_image[i][j][1] = (m_image[i][j][1] * 255) / m_maxVal;
                m_image[i][j][2] = (m_image[i][j][2] * 255) / m_maxVal;
            }
        }

        file.close();
    }
}
