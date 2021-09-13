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
                                              m_width(0),
                                              m_height(0),
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

        // Ignore comment lines
        do
        {
            line = text.readLine();
        } while (line[0] == QChar('#'));

        QTextStream(&line) >> m_width >> m_height;

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
        setFixedSize(m_width, m_height);

        for (int x = 0; x < m_width; x++)
        {
            for (int y = 0; y < m_height; y++)
            {
                painter.setPen(QColor(m_image[x][y][0], m_image[x][y][1], m_image[x][y][2]));
                painter.drawPoint(x, y);
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
    m_fileType = TYPE_PBM_ASCII;

    for (int y = 0; y < m_height; y++)
    {
        for (int x = 0; x < m_width; x++)
        {
            int value;

            *text >> value;

            m_image[x][y][0] = m_image[x][y][1] = m_image[x][y][2] = value ? 255 : 0;
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
    m_fileType = TYPE_PGM_ASCII;

    *text >> m_maxVal;

    for (int y = 0; y < m_height; y++)
    {
        for (int x = 0; x < m_width; x++)
        {
            int gray;

            *text >> gray;

            m_image[x][y][2] = m_image[x][y][1] = m_image[x][y][0] = gray * 255 / m_maxVal;
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
    m_fileType = TYPE_PPM_ASCII;

    *text >> m_maxVal;

    for (int y = 0; y < m_height; y++)
    {
        for (int x = 0; x < m_width; x++)
        {
            int R, G, B;

            *text >> R >> G >> B;

            m_image[x][y][0] = R * 255 / m_maxVal;
            m_image[x][y][1] = G * 255 / m_maxVal;
            m_image[x][y][2] = B * 255 / m_maxVal;
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
        int x = 0;
        int y = 0;

        data.skipRawData(pos);

        for (int u = 0; u < m_width * m_height / 8; u++)
        {
            unsigned char value;

            data >> value;
            for (int v = 7; v >= 0; v--)
            {
                m_image[x][y][0] = m_image[x][y][1] = m_image[x][y][2] = ((value >> v) & 0x01) ? 255 : 0;

                x++;
                if (x == m_width)
                {
                    x = 0;
                    y++;
                }
            }
        }

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

        for (int y = 0; y < m_height; y++)
        {
            for (int x = 0; x < m_width; x++)
            {
                unsigned char gray;

                data >> gray;

                m_image[x][y][0] = ((unsigned char) gray) * 255 / m_maxVal;
                m_image[x][y][0] = (unsigned char) gray;
                m_image[x][y][2] = m_image[x][y][1] = m_image[x][y][0];
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
        for (int y = 0; y < m_height; y++)
        {
            for (int x = 0; x < m_width; x++)
            {
                unsigned char R, G, B;

                data >> R >> G >> B;

                m_image[x][y][0] = R * 255 / m_maxVal;
                m_image[x][y][1] = G * 255 / m_maxVal;
                m_image[x][y][2] = B * 255 / m_maxVal;
            }
        }

        file.close();
    }
}
