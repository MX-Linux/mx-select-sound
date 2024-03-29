/**********************************************************************
 *  mainwindow.h
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
#pragma once

#include <QMessageBox>
#include <QProcess>

namespace Ui
{
class MainWindow;
}

struct Result {
    int exitCode;
    QString output;
};

class MainWindow : public QDialog
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

    Result runCmd(const QString &cmd);

    QString getDefault();
    QStringList listCards();
    void setConnections();

private slots:
    static void pushHelp_clicked();
    void pushAbout_clicked();
    void pushApply_clicked();
    void pushTest_clicked();

private:
    Ui::MainWindow *ui;
    QProcess *proc {};
    QString output;
};
