#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QThread>

class QString;

class Controller : public QObject
{
  Q_OBJECT

public:
  Controller(QObject* parent = 0);

public slots:
  void setupWeidu(const QString& weiduPath, QString gamePath);
  void weiduVersion(const QString& version);

signals:
  void weiduFailedValidation(const QString& weiduPath);
  void getVersion();

private:
  QThread* workerThread;

};

#endif // CONTROLLER_H
