/**********************************************************************
 *  mxselectsound.cpp
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


#include "mxselectsound.h"
#include "ui_mxselectsound.h"

#include <QFile>
#include <QDir>
#include <QRadioButton>
#include <QTimer>

//#include <QDebug>

mxselectsound::mxselectsound(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::mxselectsound)
{
    ui->setupUi(this);
    setup();
}

mxselectsound::~mxselectsound()
{
    delete ui;
}

// setup versious items first time program runs
void mxselectsound::setup()
{
    version = getVersion("mx-select-sound");
    this->setWindowTitle(tr("MX Select Sound"));
    listCards();
    getDefault();
}

// Util function for getting bash command output and error code
Result mxselectsound::runCmd(QString cmd)
{
    QEventLoop loop;
    proc = new QProcess(this);
    proc->setReadChannelMode(QProcess::MergedChannels);
    connect(proc, SIGNAL(finished(int)), &loop, SLOT(quit()));
    proc->start("/bin/bash", QStringList() << "-c" << cmd);
    loop.exec();
    disconnect(proc, SIGNAL(finished(int)), 0, 0);
    Result result = {proc->exitCode(), proc->readAll().trimmed()};
    delete proc;
    return result;
}

// Get version of the program
QString mxselectsound::getVersion(QString name)
{
    QString cmd = QString("dpkg -l %1 | awk 'NR==6 {print $3}'").arg(name);
    return runCmd(cmd).output;
}

// Get the list of sound cards
QStringList mxselectsound::listCards()
{
    QString cards = runCmd("cat /proc/asound/cards | sed -n -r 's/[0-9 ]+\\[//p' |  sed 's/\\s*\\]:/:/'").output;
    QStringList card_list = cards.split("\n");
    if (card_list.size() == 0) {
        QMessageBox::critical(0, tr("MX Select Sound"),
          tr("No sound cards/devices were found."));
        QTimer::singleShot(50, qApp, SLOT(quit()));
    } else if (card_list.size() == 1) {
        QMessageBox::critical(0, tr("MX Select Sound"),
          tr("Only one sound card was found."));
        QTimer::singleShot(50, qApp, SLOT(quit()));
    } else {
        ui->comboBox->addItems(card_list);
    }
    return card_list;
}

// Get default card
QString mxselectsound::getDefault()
{
    QString prev_card;
    Result cmd = runCmd("set -o pipefail; sed -n -r 's/^\\s*defaults.pcm.!card\\s+(.*)/\\1/p' ~/.asoundrc | head -n 1");
    if (cmd.exitCode == 0) {
        prev_card = cmd.output;
        ui->labelCurrent->setText(prev_card);
    } else {
        ui->labelCurrent->setText(tr("none"));
    }
    return prev_card;
}

//// slots ////
// Apply button clicked
void mxselectsound::on_buttonApply_clicked()
{
    QString selected = ui->comboBox->currentText().section(":", 0, 0);
    QFile asoundrc;

    asoundrc.setFileName(QDir::homePath() + "/.asoundrc");
    if(asoundrc.open(QFile::WriteOnly|QFile::Text)) {
        asoundrc.write(QString("defaults.pcm.!card %1 \ndefaults.ctl.!card %1").arg(selected).toUtf8());
        asoundrc.close();
        getDefault();
    }
}

// About button clicked
void mxselectsound::on_buttonAbout_clicked()
{
    QMessageBox msgBox(QMessageBox::NoIcon,
                       tr("About MX Select Sound"), "<p align=\"center\"><b><h2>" +
                       tr("MX Select Sound") + "</h2></b></p><p align=\"center\">" + tr("Version: ") + version + "</p><p align=\"center\"><h3>" +
                       tr("Program for selecting the default sound card in MX Linux") +
                       "</h3></p><p align=\"center\"><a href=\"http://www.mepiscommunity.org/mx\">http://www.mepiscommunity.org/mx</a><br /></p><p align=\"center\">" +
                       tr("Copyright (c) MX Linux") + "<br /><br /></p>", 0, this);
    msgBox.addButton(tr("Cancel"), QMessageBox::AcceptRole); // because we want to display the buttons in reverse order we use counter-intuitive roles.
    msgBox.addButton(tr("License"), QMessageBox::RejectRole);
    if (msgBox.exec() == QMessageBox::RejectRole) {
        system("mx-viewer file:///usr/share/doc/mx-select-sound/license.html '" + tr("MX Select Sound").toUtf8() + " " + tr("License").toUtf8() + "'");
    }
}

// Help button clicked
void mxselectsound::on_buttonHelp_clicked()
{
    QString cmd = QString("mx-viewer http://www.mepiscommunity.org/user_manual_mx15/mxum.html#sound '%1'").arg(tr("MX Select Sound"));
    system(cmd.toUtf8());
}

// Test default sound card
void mxselectsound::on_buttonTest_clicked()
{
    int exitCode = runCmd("speaker-test -c 2 -t wav -l 2").exitCode;
    if (exitCode != 0) {
        QMessageBox::critical(0, tr("MX Select Sound"),
          tr("Could not play test sound."));
    }
}
