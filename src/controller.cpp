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
//  reader->moveToThread(readerThread);
  connect(workerThread, &QThread::finished,
          reader, &QObject::deleteLater);

  // This is a mess of signals being passed around objects, and to in the end forward
  // a WeiduLog*, which is sort of a data type, but inheriting QObject, and
  // sometimes but not always having a parent. It is messy enough to not be any
  // clear if it's breaking the whole thread safety or not (but probably yes,
  // as creating a QObject in a different thread of its parent is a big risk).
  // In the meantime, keep this as a means to be able to test with and without
  // the thread (for now the thread code is commented out, and the threads are
  // not used).
  connect(this, &Controller::readLog,
          reader, &LogReader::readLog);
  connect(reader, &LogReader::logFile,
          this, &Controller::logFile);

//  readerThread->start();
}

Controller::~Controller()
{
  if (weiduManager)
      weiduManager->deleteLater(); // Deleted indirectly, in case the thread is added back.
}

void Controller::setupWeidu(const QString& weiduPath)
{
  if (weiduManager)
      weiduManager->deleteLater(); // Deleted indirectly, in case the thread is added back.
  currentWeidu = weiduPath;
  weiduManager = new WeiduManager(weiduPath, &weiduLog);
  if (weiduManager->executable()) {
    qDebug() << "File" << weiduPath << "is executable";
//    weiduManager->moveToThread(workerThread);

    connect(weiduManager, &WeiduManager::quacks,
            this, &Controller::quacks);
    connect(weiduManager, &WeiduManager::versionSignal,
            this, &Controller::weiduVersion);
    connect(weiduManager, &WeiduManager::languageList,
            this, &Controller::languageList);
    connect(weiduManager, &WeiduManager::componentList,
            this, &Controller::componentList);
    connect(weiduManager, &WeiduManager::modStackChanged,
            this, &Controller::readLog);
    connect(weiduManager, &WeiduManager::installTaskStarted,
            this, &Controller::installTaskStarted);
    connect(weiduManager, &WeiduManager::installTaskEnded,
            this, &Controller::installTaskEnded);
    connect(weiduManager, &WeiduManager::processOutput,
            this, &Controller::processOutput);

    connect(this, &Controller::processInput,
            weiduManager, &WeiduManager::processInput);

//    workerThread->start();
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
  weiduManager->getLanguageList(tp2);
}

void Controller::getComponentList(const QString& tp2, int index)
{
  weiduManager->getComponentList(tp2, index);
}

void Controller::processQueues(WeiduLog* install, WeiduLog* uninstall,
                               WeiduLog* log)
{
  /* WeiduLog objects are intended for WeiduManager */
  if (!uninstall->isEmpty()) {
    weiduManager->uninstall(uninstall, log);
  }
  if (!install->isEmpty()) {
    weiduManager->install(install);
  }
}
