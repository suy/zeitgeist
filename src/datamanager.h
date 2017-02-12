#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <QObject>
#include <QHash>

class QSettings;
class QString;

class GameListModel;
class AvailableModsModel;
class InstalledModsModel;
class Game;

class DataManager : public QObject
{
  Q_OBJECT

public:
  DataManager(QObject* parent);
  QString getCurrentGamePath() const;

  GameListModel* gameListModel;
  AvailableModsModel* availableModsModel;
  InstalledModsModel* installedModsModel;

private slots:
  void useGame(const QString& path);

public slots:
  void saveState();
  void identifyCurrentGame() const;
  void refreshCurrentGame();

signals:
  void identityOfCurrentGame(const QString& name) const;

private:
  void restoreState();
  void saveGameList();
  void restoreGameList();
  void loadGame(const QString& path);

  QSettings* settings;
  Game* currentGame;

  const QString gameListSettingsName = "gameList";
};

#endif // DATAMANAGER_H
