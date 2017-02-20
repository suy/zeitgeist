#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class Coordinator;
class DataManager;
class GameWindow;
class SettingsWindow;
class MainCentralWidget;

class QCloseEvent;
class QLabel;
class QAction;
class QMenu;

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(Coordinator* coordinator);
  ~MainWindow();

protected:
  void closeEvent(QCloseEvent* event);

private slots:
  void showGameWindow();
  void showSettingsWindow();
  void updateNameOfCurrentGame(const QString& name);

signals:
  void saveState();

private:
  void createStatusBar();
  void createActions();
  void createMenus();

  const Coordinator* coordinator;
  const DataManager* dataManager;
  MainCentralWidget* mainCentralWidget;
  QLabel* statusBarGameLabel;
  QLabel* statusBarCurrentGame;
  QAction* programAboutAction;
  QAction* programSettingsAction;
  QAction* programQuitAction;
  QAction* gameEditAction;
  QAction* gameRefreshAction;
  QAction* gameInstallAction;
  QAction* gameUnqueueAction;
  QAction* gameUninstallAction;
  QAction* gameProcessAction;
  QMenu* programMenu;
  QMenu* gameMenu;
  GameWindow* gameWindow;
  SettingsWindow* settingsWindow;
};

#endif // MAINWINDOW_H
