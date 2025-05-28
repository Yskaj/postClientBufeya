#ifndef SERVEREXCHANGE_H
#define SERVEREXCHANGE_H
#include "client.h"
#include "database.h"
#include <QAbstractSocket>
#include <QDebug>
#include <QSslSocket>
#include <QString>
#include <QTimer>
#include <QtConcurrent/QtConcurrent>

class serverExchange : public QObject {
  Q_OBJECT
public:
  serverExchange();
  QString hostName = "imap.yandex.ru";
  int hostPort = 993;
  QSslSocket *socket;
  database *db;
  QTimer *timer;
  int getTimeRequest();
  void connectToHost();
  void connecters();
  void getLoginPass();
  void setSettings(QString, QString, QString);
public slots:
  void connectError();
  void socketReadyRead();
  void socketEncrypted();
  void processRequest();
  void responseLooper();
  void reqLoop();
signals:
  void newOrder(Client *);
};

#endif // SERVEREXCHANGE_H
