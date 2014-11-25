#ifndef MAINCENTRALWIDGET_H
#define MAINCENTRALWIDGET_H

#include <QWidget>

class QHBoxLayout;
class ModList;
class ModTree;

class MainCentralWidget : public QWidget
{
  Q_OBJECT

public:
  MainCentralWidget(QWidget* parent = 0);

private:
  ModList* availableMods;
  ModTree* queuedMods;
  ModTree* installedMods;
};

#endif // MAINCENTRALWIDGET_H
