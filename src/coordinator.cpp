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

#include "coordinator.h"
#include "controller.h"
#include "datamanager.h"
#include "weidumanager.h"

#include <QFile>

class QJsonDocument;

Coordinator::Coordinator(QObject* parent) :
  QObject(parent), dataManager(new DataManager(this)),
  controller(new Controller(this))
{
  connect(this, &Coordinator::weiduPathSignal,
          controller, &Controller::setupWeidu);
  connect(controller, &Controller::weiduFailedValidation,
          this, &Coordinator::weiduFailedValidation);
  connect(controller, &Controller::newWeiduManager,
          this, &Coordinator::newWeiduManager);
  connect(controller, &Controller::confirmedWeiduPath,
          dataManager, &DataManager::confirmedWeiduPath);
  connect(dataManager, &DataManager::storedWeiduPath,
          this, &Coordinator::weiduPath);
  connect(this, &Coordinator::restoreStateSignal,
          dataManager, &DataManager::restoreState);
  connect(dataManager, &DataManager::processQueues,
          controller, &Controller::processQueues);
  connect(dataManager, &DataManager::getLog,
          controller, &Controller::readLog);
  connect(controller, &Controller::logFile,
          dataManager, &DataManager::logFile);
  connect(controller, &Controller::componentList,
          dataManager, &DataManager::componentList);
  connect(this, &Coordinator::createModDistArchive,
          dataManager, &DataManager::createModDistArchive);
}

void Coordinator::weiduPath(const QString& path)
{
  emit weiduPathSignal(path);
}

void Coordinator::weiduFailedValidation(const QString& weiduPath)
{
  emit weiduFailedValidationSignal(weiduPath);
}

void Coordinator::newWeiduManager(const WeiduManager* manager)
{
  connect(dataManager, &DataManager::newGamePath,
          manager, &WeiduManager::newGamePath);
  connect(dataManager, &DataManager::eeLang,
          manager, &WeiduManager::eeLang);
  provideGamePath();
  emit installerAvailable(true);
}

void Coordinator::provideGamePath()
{
  dataManager->emitCurrentGamePath();
}

void Coordinator::restoreState()
{
  emit restoreStateSignal();
}

void Coordinator::initWeidu(const QString& weidu)
{
  QFile f(weidu);
  if (f.exists()) {
    weiduPath(weidu);
  } else {
    dataManager->restoreWeidu();
  }
}
