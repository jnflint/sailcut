/*
 * Copyright (C) 1993-2006 Robert & Jeremy Laine
 * See AUTHORS file for a full list of contributors.
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "sailcutqt.h"
#include <QFile>
#include <QDir>
#include <QTranslator>
#include "sailreader-xml.h"
#include "sailwriter-xml.h"

using namespace std;

/**
 * Creates an new Sailcut application.
 *
 * @param argc
 * @param argv
 */
CSailApp::CSailApp(int argc, char* argv[]) :
        QApplication(argc, argv)
{
    // create and install translators
    transApp = new QTranslator( this );
    transQt = new QTranslator( this );

    // the file containing the user's preferences
    prefsfile = QDir::home().filePath(".sailcutrc");
}


/**
 * Tries to locate the Sailcut Handbook.
 */
QString CSailApp::findHandbook(const QString locale)
{
    QString handbook;
    QStringList locales(locale);
    if (locale != "en")
        locales.append("en");
    QStringList docdirs("../doc");
#ifdef SAILCUT_DOCDIR

    docdirs.append(QString(SAILCUT_DOCDIR) + QDir::separator() + "html");
#endif

    // look for handbook in different languages
    for (int i = 0; i < locales.size(); i++)
    {
        for (int d = 0; d < docdirs.size(); d++)
        {
            handbook = docdirs.at(d) + QDir::separator() + locales.at(i) + QDir::separator() + "index.html";
            if (QFile::exists(handbook))
                return handbook;
        }
    }

    return "";
}


/**
 * Loads translation strings for a given language.
 */
void CSailApp::loadTranslation(const QString locale)
{
    // translation file for Qt
    transQt->load( QString( "qt_" ) + locale, "." );
    installTranslator(transQt);

    // translation file for application strings
    QString qm = QString("sailcut.") + locale;
    QStringList datadirs("ts");
#ifdef SAILCUT_DATADIR

    datadirs.append(QString(SAILCUT_DATADIR));
#endif

    int d = 0;
    while ((d < datadirs.size()) &&
            (!transApp->load(qm, datadirs.at(d))))
    {
        d++;
    }
    installTranslator(transApp);
}


/**
 * Try to read the user's preferences
 */
void CSailApp::readPrefs()
{
    try
    {
        if (QFile(prefsfile).exists())
            prefs = CPrefsXmlReader("prefs").read(prefsfile);
    }
    catch (CException e)
    {
        qDebug("Could not read preferences");
    }
}


/**
 * Try to write the user's preferences.
 */
void CSailApp::writePrefs()
{
    try
    {
        CPrefsXmlWriter(prefs,"prefs").write(prefsfile);
    }
    catch (CException e)
    {
        qDebug("Could not write preferences");
    }
}