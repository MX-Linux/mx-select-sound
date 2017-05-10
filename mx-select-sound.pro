# **********************************************************************
# * Copyright (C) 2015 MX Authors
# *
# * Authors: Adrian
# *          MX Linux <http://mxlinux.org>
# *
# * This file is part of mx-select-sound.
# *
# * mx-select-sound is free software: you can redistribute it and/or modify
# * it under the terms of the GNU General Public License as published by
# * the Free Software Foundation, either version 3 of the License, or
# * (at your option) any later version.
# *
# * mx-select-sound is distributed in the hope that it will be useful,
# * but WITHOUT ANY WARRANTY; without even the implied warranty of
# * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# * GNU General Public License for more details.
# *
# * You should have received a copy of the GNU General Public License
# * along with mx-select-sound.  If not, see <http://www.gnu.org/licenses/>.
# **********************************************************************/

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = mx-select-sound
TEMPLATE = app


SOURCES += main.cpp\
        mxselectsound.cpp

HEADERS  += mxselectsound.h

FORMS    += mxselectsound.ui

TRANSLATIONS += translations/mx-select-sound_ca.ts \
                translations/mx-select-sound_cs.ts \
                translations/mx-select-sound_de.ts \
                translations/mx-select-sound_el.ts \
                translations/mx-select-sound_es.ts \
                translations/mx-select-sound_fr.ts \
                translations/mx-select-sound_it.ts \
                translations/mx-select-sound_ja.ts \
                translations/mx-select-sound_kk.ts \
                translations/mx-select-sound_lt.ts \
                translations/mx-select-sound_nl.ts \
                translations/mx-select-sound_pl.ts \
                translations/mx-select-sound_pt.ts \
                translations/mx-select-sound_ro.ts \
                translations/mx-select-sound_ru.ts \
                translations/mx-select-sound_sv.ts \
                translations/mx-select-sound_tr.ts

RESOURCES += \
    images.qrc


