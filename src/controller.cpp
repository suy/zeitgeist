/*
 *  Zeitgeist - Graphical WeiDU frontend
 *  Copyright (C) 2017  Fredrik Lindgren

 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.

 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.

 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "controller.h"
#include "logreader.h"
#include "weidulog.h"
#include "weidumanager.h"

#include <QDebug>

Controller::Controller(QObject* parent) :
  QObject(parent), workerThread(new QThread(this)),
  readerThread(new QThread(this)), currentWeidu(QString()),
  weiduManager(nullptr)
{
  LogReader* reader = new LogReader(&weiduLog);
  connect(this, &Controller::readLog,
          reader, &LogReader::readLog);
  connect(reader, &LogReader::logFile,
          this, &Controller::logFile);
  connect(this, &Controller::terminateReader,
          reader, &LogReader::terminateReader);

  reader->moveToThread(readerThread);
  readerThread->start();
}

Controller::~Controller()
{
  weiduManager->deleteLater();
  emit terminateReader();
}

void Controller::setupWeidu(const QString& weiduPath)
{
  weiduManager->deleteLater();
  currentWeidu = weiduPath;
  weiduManager = new WeiduManager(weiduPath, &weiduLog);
  if (weiduManager->executable()) {
    qDebug() << "File" << weiduPath << "is executable";
    weiduManager->moveToThread(workerThread);
    connect(weiduManager, SIGNAL(quacks(bool)),
            this, SLOT(quacks(bool)));

    connect(weiduManager, SIGNAL(versionSignal(int)),
            this, SLOT(weiduVersion(int)));
    connect(weiduManager, SIGNAL(languageList(const QStringList&)),
            this, SIGNAL(languageList(const QStringList&)));
    connect(this, SIGNAL(weiduListLanguages(const QString&)),
            weiduManager, SLOT(getLanguageList(const QString&)));
    connect(this, SIGNAL(weiduListComponents(const QString&, int)),
            weiduManager, SLOT(getComponentList(const QString&, int)));
    connect(weiduManager, SIGNAL(componentList(const QString&, int,
                                               const QJsonDocument&)),
            this, SIGNAL(componentList(const QString&, int,
                                       const QJsonDocument&)));
    connect(this, SIGNAL(weiduInstall(WeiduLog*)),
            weiduManager, SLOT(install(WeiduLog*)));
    connect(this, SIGNAL(weiduUninstall(WeiduLog*, WeiduLog*)),
            weiduManager, SLOT(uninstall(WeiduLog*, WeiduLog*)));
    connect(weiduManager, SIGNAL(modStackChanged(const QString&)),
            this, SIGNAL(readLog(const QString&)));
    connect(weiduManager, SIGNAL(installTaskStarted()),
            this, SIGNAL(installTaskStarted()));
    connect(weiduManager, SIGNAL(installTaskEnded()),
            this, SIGNAL(installTaskEnded()));
    connect(weiduManager, SIGNAL(processOutput(const QString&)),
            this, SIGNAL(processOutput(const QString&)));
    connect(this, SIGNAL(processInput(const QString&)),
            weiduManager, SLOT(processInput(const QString&)));

    workerThread->start();
    weiduManager->quack();
  } else {
    qDebug() << "File" << weiduPath << "is not executable";
    delete weiduManager;
    weiduManager = nullptr;
    currentWeidu = QString();
    currentWeiduVersion = 0;
    emit weiduFailedValidation(weiduPath);
  }
}

void Controller::quacks(bool quacks)
{
  if (quacks) {
    qDebug() << "File quacks like a WeiDU";
    emit newWeiduManager(weiduManager);
    emit confirmedWeiduPath(currentWeidu);
    weiduManager->version();
  } else {
    qDebug() << "File fails to quack like a WeiDU";
    weiduManager->deleteLater();
    weiduManager = nullptr;
    QString tmpPath = currentWeidu;
    currentWeidu = QString();
    currentWeiduVersion = 0;
    emit weiduFailedValidation(tmpPath);
  }
}

void Controller::weiduVersion(int version)
{
  currentWeiduVersion = version;
  emit weiduVersionSignal(version);
}

void Controller::weiduCheck() const
{
  if (weiduManager) {
    emit confirmedWeiduPath(currentWeidu);
    emit weiduVersionSignal(currentWeiduVersion);
  }
}

void Controller::getLanguageList(const QString& tp2)
{
  emit weiduListLanguages(tp2);
}

void Controller::getComponentList(const QString& tp2, int index)
{
  emit weiduListComponents(tp2, index);
}

void Controller::processQueues(WeiduLog* install, WeiduLog* uninstall,
                               WeiduLog* log)
{
  /* WeiduLog objects are intended for WeiduManager */
  if (!uninstall->isEmpty()) {
    emit weiduUninstall(uninstall, log);
  }
  if (!install->isEmpty()) {
    emit weiduInstall(install);
  }
}
