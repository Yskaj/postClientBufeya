#ifndef CLIENT_H
#define CLIENT_H
#include <QString>

class Client {
public:
  Client();
  Client(QString name, QString phoneNumber, QString email, bool status, int Id);
  Client(QString name, QString phoneNumber, QString email);
  bool status = 0;
  int Id;
  QString name;
  QString phoneNumber;
  QString email;
  QStringList *listK;
  QStringList getClientDate();
  QString toString();
  QStringList getComments();
  void addComment(QString str);
  QString getLastComment();
  ~Client(){};
};

#endif // CLIENT_H
