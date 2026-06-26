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

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QRadioButton>
#include <QTextStream>

#include "about.h"
#include "cardutils.h"

#ifndef VERSION
    #define VERSION "?.?.?.?"
#endif

MainWindow::MainWindow(QWidget *parent)
    : QDialog(parent),
      ui(new Ui::MainWindow)
{
    qDebug().noquote() << QCoreApplication::applicationName() << "version:" << VERSION;
    ui->setupUi(this);
    setConnections();
    setWindowFlags(Qt::Window); // for the close, min and max buttons
    if (ui->pushApply->icon().isNull()) {
        ui->pushApply->setIcon(QIcon(":/icons/dialog-ok.svg"));
    }
    setWindowTitle(tr("MX Select Sound"));
    adjustSize();
}

MainWindow::~MainWindow()
{
    delete ui;
}

// Util function for getting bash command output and error code
Result MainWindow::runCmd(const QString &cmd)
{
    QProcess proc(this);
    proc.setProcessChannelMode(QProcess::MergedChannels);
    proc.start(QStringLiteral("/bin/bash"), {"-c", cmd});
    proc.waitForFinished(-1);
    Result result = {proc.exitCode(), proc.readAll().trimmed()};
    return result;
}

// Get the list of sound cards
QStringList MainWindow::listCards()
{
    QStringList card_list;
    QFile file(QStringLiteral("/proc/asound/cards"));
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream stream(&file);
        card_list = CardUtils::parseCards(stream.readAll());
    }
    if (card_list.isEmpty()) {
        QMessageBox::critical(this, tr("MX Select Sound"), tr("No sound cards/devices were found."));
    } else {
        ui->comboBox->addItems(card_list);
    }
    return card_list;
}

// Get default card
QString MainWindow::getDefault()
{
    QString default_card = tr("none");

    QFile asoundrc(QDir::homePath() + QStringLiteral("/.asoundrc"));
    if (asoundrc.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream stream(&asoundrc);
        const QString prev_card = CardUtils::parseDefaultCard(stream.readAll());
        for (int i = 0; i < ui->comboBox->count(); i++) {
            if (prev_card == ui->comboBox->itemText(i).section(QStringLiteral(":"), 0, 0)) {
                default_card = prev_card;
                break;
            }
        }
    }

    qDebug() << "Default sound card:" << default_card;
    ui->pushTest->setDisabled(default_card == tr("none"));
    ui->labelCurrent->setText(default_card);
    return default_card;
}

void MainWindow::setConnections()
{
    connect(ui->pushApply, &QPushButton::clicked, this, &MainWindow::pushApply_clicked);
    connect(ui->pushAbout, &QPushButton::clicked, this, &MainWindow::pushAbout_clicked);
    connect(ui->pushHelp, &QPushButton::clicked, this, &MainWindow::pushHelp_clicked);
    connect(ui->pushTest, &QPushButton::clicked, this, &MainWindow::pushTest_clicked);
}

void MainWindow::pushApply_clicked()
{
    QString selected = ui->comboBox->currentText().section(QStringLiteral(":"), 0, 0);
    QFile asoundrc;

    asoundrc.setFileName(QDir::homePath() + "/.asoundrc");
    if (asoundrc.open(QFile::WriteOnly | QFile::Text)) {
        asoundrc.write(QStringLiteral("defaults.pcm.!card %1\ndefaults.ctl.!card %1").arg(selected).toUtf8());
        asoundrc.close();
        getDefault();
    }
}

void MainWindow::pushAbout_clicked()
{
    displayAboutMsgBox(
        tr("About MX Select Sound"),
        "<p align=\"center\"><b><h2>" + tr("MX Select Sound") + "</h2></b></p><p align=\"center\">" + tr("Version: ")
            + VERSION + "</p><p align=\"center\"><h3>" + tr("Program for selecting the default sound card in MX Linux")
            + "</h3></p><p align=\"center\"><a href=\"http://mxlinux.org\">http://mxlinux.org</a><br /></p>"
              "<p align=\"center\">"
            + tr("Copyright (c) MX Linux") + "<br /><br /></p>",
        QStringLiteral("/usr/share/doc/mx-select-sound/license.html"), tr("%1 License").arg(windowTitle()));
}

void MainWindow::pushHelp_clicked()
{
    displayDoc(QStringLiteral("mx-select-sound.html"), tr("%1 Help").arg(tr("MX Select Sound")), true);
}

// Test default sound card
void MainWindow::pushTest_clicked()
{
    const int exitCode = runCmd(QStringLiteral("speaker-test -c 2 -t wav -l 2")).exitCode;
    if (exitCode != 0) {
        QMessageBox::critical(this, tr("MX Select Sound"), tr("Could not play test sound."));
    }
}
