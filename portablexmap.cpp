#include <QFile>
#include <QPainter>
#include <QTextStream>

#include "portablexmap.h"


PortableXMap::PortableXMap(QWidget *parent) : QWidget(parent)
{
    nomFichier  = "";
    typeFichier = TYPE_INCONNU;
    x           = 0;
    y           = 0;
}


void PortableXMap::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    int      i;
    int      j;

    if (typeFichier != TYPE_INCONNU)
    {
        setFixedSize(x, y);

        for (i = 0; i < x; i++)
        {
            for (j = 0; j < y; j++)
            {
                painter.setPen(QColor(image[i][j][0], image[i][j][1], image[i][j][2]));
                painter.drawPoint(i, j);
            }
        }
    }
}


void PortableXMap::traiteFichier(QString string)
{
    QFile       fichier(string);
    QString     ligne;
    QTextStream texte(&fichier);
    char        type;

    nomFichier = string;
    if (fichier.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        ligne = texte.readLine();
        type  = ligne[1].toAscii();

        do
        {
            ligne = texte.readLine();
        } while (ligne[0] == QChar('#'));

        QTextStream(&ligne) >> x >> y;

        switch (type)
        {
            case '1':
                traiteFichierP1(&texte);
                break;

            case '2':
                traiteFichierP2(&texte);
                break;

            case '3':
                traiteFichierP3(&texte);
                break;

            case '4':
                traiteFichierP4(&texte);
                break;

            case '5':
                traiteFichierP5(&texte);
                break;

            case '6':
                traiteFichierP6(&texte);
                break;

            default:
                typeFichier = TYPE_INCONNU;
                break;
        }

        if (typeFichier != TYPE_INCONNU)
            update();

        fichier.close();
    }
}


void PortableXMap::traiteFichierP1(QTextStream *texte)
{
    QString ligne;
    int     i;
    int     j;
    int     valeur;

    typeFichier = TYPE_PBM_ASCII;

    for (j = 0; j < y; j++)
    {
        for (i = 0; i < x; i++)
        {
            *texte >> valeur;

            if (valeur)
                image[i][j][0] = image[i][j][1] = image[i][j][2] = 255;
            else
                image[i][j][0] = image[i][j][1] = image[i][j][2] = 0;
        }
    }
}


void PortableXMap::traiteFichierP2(QTextStream *texte)
{
    QString ligne;
    int     i;
    int     j;

    typeFichier = TYPE_PGM_ASCII;

    ligne = texte->readLine();
    QTextStream(&ligne) >> maxVal;

    for (j = 0; j < y; j++)
    {
        for (i = 0; i < x; i++)
        {
            *texte >> image[i][j][0];

            image[i][j][0] = (image[i][j][0] * 255) / maxVal;

            image[i][j][2] = image[i][j][1] = image[i][j][0];
        }
    }
}


void PortableXMap::traiteFichierP3(QTextStream *texte)
{
    QString ligne;
    int     i;
    int     j;

    typeFichier = TYPE_PPM_ASCII;

    ligne = texte->readLine();
    QTextStream(&ligne) >> maxVal;

    for (j = 0; j < y; j++)
    {
        for (i = 0; i < x; i++)
        {
            *texte >> image[i][j][0];
            *texte >> image[i][j][1];
            *texte >> image[i][j][2];

            image[i][j][0] = (image[i][j][0] * 255) / maxVal;
            image[i][j][1] = (image[i][j][1] * 255) / maxVal;
            image[i][j][2] = (image[i][j][2] * 255) / maxVal;
        }
    }
}


void PortableXMap::traiteFichierP4(QTextStream *texte)
{
    QString       ligne;
    int           i;
    int           j;
    int           u;
    int           v;
    unsigned char valeur;
    qint64        pos;
    QFile         fichier(nomFichier);
    QDataStream   data(&fichier);

    typeFichier = TYPE_PBM_BIN;

    pos = texte->pos();

    if (fichier.open(QIODevice::ReadOnly))
    {
        data.skipRawData(pos);

        i = j = 0;
        for (u = 0; u < x * y / 8; u++)
        {
            data >> valeur;
            for (v = 7; v >= 0; v--)
            {
                if ((valeur >> v) & 0x01)
                    image[i][j][0] = image[i][j][1] = image[i][j][2] = 255;
                else
                    image[i][j][0] = image[i][j][1] = image[i][j][2] = 0;

                i++;
                if (i == x)
                {
                    i = 0;
                    j++;
                }
            }
        }


        /*for ( j = 0 ; j < y ; j++ )
        {
            for ( i = 0 ; i < x ; i++ )
            {
                data >> valeur;

                if ( valeur )
                    image[i][j][0] = image[i][j][1] = image[i][j][2] = 255;
                else
                    image[i][j][0] = image[i][j][1] = image[i][j][2] = 0;
            }
        }*/

        fichier.close();
    }
}


void PortableXMap::traiteFichierP5(QTextStream *texte)
{
    QString       ligne;
    int           i;
    int           j;
    unsigned char valeur;
    qint64        pos;
    QFile         fichier(nomFichier);
    QDataStream   data(&fichier);

    typeFichier = TYPE_PGM_BIN;

    ligne = texte->readLine();
    QTextStream(&ligne) >> maxVal;

    pos = texte->pos();

    if (fichier.open(QIODevice::ReadOnly))
    {
        data.skipRawData(pos);

        for (j = 0; j < y; j++)
        {
            for (i = 0; i < x; i++)
            {
                data >> valeur;
                image[i][j][0] = ((unsigned char) valeur) * 255 / maxVal;
                image[i][j][0] = (unsigned char) valeur;
                image[i][j][2] = image[i][j][1] = image[i][j][0];
            }
        }

        fichier.close();
    }
}

void PortableXMap::traiteFichierP6(QTextStream *texte)
{
    QString       ligne;
    int           i;
    int           j;
    unsigned char valeur;
    qint64        pos;
    QFile         fichier(nomFichier);
    QDataStream   data(&fichier);

    typeFichier = TYPE_PPM_BIN;

    ligne = texte->readLine();
    QTextStream(&ligne) >> maxVal;

    pos = texte->pos();

    if (fichier.open(QIODevice::ReadOnly))
    {
        data.skipRawData(pos);
        for (j = 0; j < y; j++)
        {
            for (i = 0; i < x; i++)
            {
                data >> valeur;
                image[i][j][0] = valeur;
                data >> valeur;
                image[i][j][1] = valeur;
                data >> valeur;
                image[i][j][2] = valeur;

                image[i][j][0] = (image[i][j][0] * 255) / maxVal;
                image[i][j][1] = (image[i][j][1] * 255) / maxVal;
                image[i][j][2] = (image[i][j][2] * 255) / maxVal;
            }
        }

        fichier.close();
    }
}
