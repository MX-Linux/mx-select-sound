/**********************************************************************
 *  main.cpp
 **********************************************************************
 * Copyright (C) 2015 MX Authors
 *
 * Authors: Adrian
 *          MX Linux <http://mxlinux.org>
 *
 * This file is part of mx-select-sound.
 *
 * mx-select-sound is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * mx-select-sound is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with mx-select-sound.  If not, see <http://www.gnu.org/licenses/>.
 **********************************************************************/
#include "mainwindow.h"

#include <QApplication>
#include <QIcon>
#include <QLibraryInfo>
#include <QLocale>
#include <QTranslator>

#include <unistd.h>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QApplication::setWindowIcon(QIcon::fromTheme(QApplication::applicationName()));

    QTranslator qtTran;
    if (qtTran.load(QLocale::system(), QStringLiteral("qt"), QStringLiteral("_"),
                    QLibraryInfo::location(QLibraryInfo::TranslationsPath)))
        QApplication::installTranslator(&qtTran);

    QTranslator qtBaseTran;
    if (qtBaseTran.load("qtbase_" + QLocale::system().name(), QLibraryInfo::location(QLibraryInfo::TranslationsPath)))
        QApplication::installTranslator(&qtBaseTran);

    QTranslator appTran;
    if (appTran.load(QApplication::applicationName() + "_" + QLocale::system().name(),
                     "/usr/share/" + QApplication::applicationName() + "/locale"))
        QApplication::installTranslator(&appTran);

    MainWindow w;
    if (w.listCards().empty()) {
        return EXIT_SUCCESS;
    } else {
        w.show();
        w.getDefault();
        return QApplication::exec();
    }
}
