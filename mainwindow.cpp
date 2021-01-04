/**********************************************************************
 *  mainwindow.cpp
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
#include "ui_mainwindow.h"
#include "version.h"

#include <QFile>
#include <QDir>
#include <QRadioButton>
#include <QTimer>
#include <QTextEdit>

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MainWindow)
{
    qDebug().noquote() << QCoreApplication::applicationName() << "version:" << VERSION;
    ui->setupUi(this);
    setWindowFlags(Qt::Window); // for the close, min and max buttons
    if (ui->buttonApply->icon().isNull()) {
        ui->buttonApply->setIcon(QIcon(":/icons/dialog-ok.svg"));
    }
    this->setWindowTitle(tr("MX Select Sound"));
    this->adjustSize();
}

MainWindow::~MainWindow()
{
    delete ui;
}

// Util function for getting bash command output and error code
Result MainWindow::runCmd(QString cmd)
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

// Get the list of sound cards
QStringList MainWindow::listCards()
{
    QStringList card_list;
    QString cards = runCmd("cat /proc/asound/cards 2>/dev/null | sed -n -r 's/[0-9 ]+\\[//p' |  sed 's/\\s*\\]:/:/'").output;
    if (cards.size() == 0) {
        QMessageBox::critical(0, tr("MX Select Sound"),
          tr("No sound cards/devices were found."));
    } else {
        card_list = cards.split("\n");
        ui->comboBox->addItems(card_list);
    }
    return card_list;
}

// Get default card
QString MainWindow::getDefault()
{
    QString prev_card;
    QString default_card = tr("none");

    Result cmd = runCmd("set -o pipefail; sed -n -r 's/^\\s*defaults.pcm.!card\\s+(.*)/\\1/p' ~/.asoundrc | head -n 1");
    if (cmd.exitCode == 0) {
        prev_card = cmd.output.toUtf8();

        for ( int i = 0; i < ui->comboBox->count(); i++ ) {
            if (prev_card == ui->comboBox->itemText(i).section(":", 0, 0).toUtf8()) {
                default_card = prev_card;
              }
          }
      }

    qDebug() << "Default sound card:" << default_card;
    if ( default_card == tr("none")) {
        ui->buttonTest->setEnabled(false);
      } else {
        ui->buttonTest->setEnabled(true);
      }

    ui->labelCurrent->setText(default_card);
    return default_card;
}

//// slots ////
// Apply button clicked
void MainWindow::on_buttonApply_clicked()
{
    QString selected = ui->comboBox->currentText().section(":", 0, 0);
    QFile asoundrc;

    asoundrc.setFileName(QDir::homePath() + "/.asoundrc");
    if(asoundrc.open(QFile::WriteOnly|QFile::Text)) {
        asoundrc.write(QString("defaults.pcm.!card %1\ndefaults.ctl.!card %1").arg(selected).toUtf8());
        asoundrc.close();
        getDefault();
    }
}

// About button clicked
void MainWindow::on_buttonAbout_clicked()
{
    this->hide();
    QMessageBox msgBox(QMessageBox::NoIcon,
                       tr("About MX Select Sound"), "<p align=\"center\"><b><h2>" +
                       tr("MX Select Sound") + "</h2></b></p><p align=\"center\">" + tr("Version: ") + VERSION + "</p><p align=\"center\"><h3>" +
                       tr("Program for selecting the default sound card in MX Linux") +
                       "</h3></p><p align=\"center\"><a href=\"http://mxlinux.org\">http://mxlinux.org</a><br /></p><p align=\"center\">" +
                       tr("Copyright (c) MX Linux") + "<br /><br /></p>", 0, this);
    QPushButton *btnLicense = msgBox.addButton(tr("License"), QMessageBox::HelpRole);
    QPushButton *btnChangelog = msgBox.addButton(tr("Changelog"), QMessageBox::HelpRole);
    QPushButton *btnCancel = msgBox.addButton(tr("Cancel"), QMessageBox::NoRole);
    btnCancel->setIcon(QIcon::fromTheme("window-close"));

    msgBox.exec();

    if (msgBox.clickedButton() == btnLicense) {
        system("mx-viewer file:///usr/share/doc/mx-select-sound/license.html '" + tr("MX Select Sound").toUtf8() + " " + tr("License").toUtf8() + "'");
    } else if (msgBox.clickedButton() == btnChangelog) {
        QDialog *changelog = new QDialog(this);
        changelog->resize(600, 500);

        QTextEdit *text = new QTextEdit;
        text->setReadOnly(true);
        text->setText(runCmd("zless /usr/share/doc/" + QFileInfo(QCoreApplication::applicationFilePath()).fileName()  + "/changelog.gz").output);

        QPushButton *btnClose = new QPushButton(tr("&Close"));
        btnClose->setIcon(QIcon::fromTheme("window-close"));
        connect(btnClose, &QPushButton::clicked, changelog, &QDialog::close);

        QVBoxLayout *layout = new QVBoxLayout;
        layout->addWidget(text);
        layout->addWidget(btnClose);
        changelog->setLayout(layout);
        changelog->exec();
    }
    this->show();
}

// Help button clicked
void MainWindow::on_buttonHelp_clicked()
{
    QLocale locale;
    QString lang = locale.bcp47Name();

    QString url = "/usr/share/doc/mx-select-sound/mx-select-sound.html";

    if (lang.startsWith("fr")) {
        url = "https://mxlinux.org/wiki/help-files/help-mx-carte-son";
    }

    QString cmd = QString("mx-viewer %1 '%2'&").arg(url).arg(tr("MX Select Sound"));
    system(cmd.toUtf8());
}

// Test default sound card
void MainWindow::on_buttonTest_clicked()
{
    int exitCode = runCmd("speaker-test -c 2 -t wav -l 2").exitCode;
    if (exitCode != 0) {
        QMessageBox::critical(0, tr("MX Select Sound"),
          tr("Could not play test sound."));
    }
}
