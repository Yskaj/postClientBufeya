#ifndef DATABASE_H
#define DATABASE_H
#include "client.h"
#include <QDebug>
#include <QFile>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>

class database : public QObject {
  Q_OBJECT
public:
  database();
  QSqlDatabase db;
  void openDataBase();
  void createDBTable();
  int getLettersCount();
  int getDontCheked();
  void setLettersCount(long);
  void setDontCheked(long);
  void setRequestTime(int);
  void setLogin(QString);
  void setPass(QString);
  int getRequestTime();
  int getRTime();
  void updateSettings(QString, long);
  int clientCount();

  QPair<QString, QString> getLoginPass();
  Client getUser(QString, QString, QString);
  Client getUser(int Id);
  QStringList *getComments(int Id);
  QList<QStringList> getUsersList();
  int setUser(Client *client);
  void setComment(int, QString);
  void updateClient(QString, QString, QString, QString);

private:
  QString login;
};

#endif // DATABASE_H
