#include "client.h"
#include <QStringList>

Client::Client() {}

Client::Client(QString name, QString phoneNumber, QString email, bool status,
               int Id) {
  this->name = name;
  this->phoneNumber = phoneNumber;
  this->email = email;
  this->status = status;
  this->Id = Id;
}

Client::Client(QString name, QString phoneNumber, QString email) {
  this->name = name;
  this->phoneNumber = phoneNumber;
  this->email = email;
}

QStringList Client::getClientDate() {
  return QStringList() << name << phoneNumber << email
                       << QString::number(this->Id);
}

QString Client::toString() {
  return QString(name + '\n' + phoneNumber + '\n' + email);
}

QStringList Client::getComments() { return *listK; }

void Client::addComment(QString str) { listK->append(str); }

QString Client::getLastComment() { return listK->last(); }
