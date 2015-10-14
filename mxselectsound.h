/**********************************************************************
 *  mxselectsound.h
 **********************************************************************
 * Copyright (C) 2015 MX Authors
 *
 * Authors: Adrian
 *          MX & MEPIS Community <http://forum.mepiscommunity.org>
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


#ifndef MXSELECTSOUND_H
#define MXSELECTSOUND_H

#include <QMessageBox>
#include <QProcess>

namespace Ui {
class mxselectsound;
}

struct Result {
    int exitCode;
    QString output;
};


class mxselectsound : public QDialog
{
    Q_OBJECT

protected:
    QProcess *proc;

public:
    explicit mxselectsound(QWidget *parent = 0);
    ~mxselectsound();

    Result runCmd(QString cmd);
    QString getVersion(QString name);

    QString version;
    QString output;

    void setup();
    QStringList listCards();
    QString getDefault();

private slots:
    void on_buttonApply_clicked();
    void on_buttonAbout_clicked();
    void on_buttonHelp_clicked();    
    void on_buttonTest_clicked();

private:
    Ui::mxselectsound *ui;
};


#endif // MXSNAPSHOT_H

