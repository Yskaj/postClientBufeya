#include "database.h"

database::database() {
  if (!QFile::exists("bufeya")) {
    createDBTable();
  } else {
    openDataBase();
  }
}

void database::openDataBase() {
  db = QSqlDatabase::addDatabase("QSQLITE");
  db.setDatabaseName("bufeya");
  db.open();
}

void database::createDBTable() {
  openDataBase();
  QSqlQuery q1(db);

  q1.exec("CREATE TABLE settings("
          "lastLetters INTEGER,"
          "dontChecked INTEGER,"
          "loopTime INTEGER,"
          "login VARCHAR(100),"
          "pass VARCHAR(100));");

  q1.exec("INSERT INTO settings(login,pass,loopTime) VALUES "
          "('*****','**************',15000)");

  q1.exec("CREATE TABLE orders("
          "ID INTEGER PRIMARY KEY AUTOINCREMENT,"
          "name VARCHAR(100),"
          "phone VARCHAR(12),"
          "email VARCHAR(100),"
          "status INTEGER);");
  q1.exec("CREATE TABLE comments("
          "userId INTEGER,"
          "comment VARCHAR(100));");
}

int database::getLettersCount() {
  QSqlQuery q1(db);
  q1.exec("SELECT lastLetters FROM settings;");
  q1.first();
  return q1.value(0).toInt();
}

int database::getDontCheked() {
  QSqlQuery q1(db);
  q1.exec("SELECT lastLetters FROM settings;");
  q1.first();
  return q1.value(0).toInt();
}

void database::setLettersCount(long letters) {
  updateSettings("lastLetters", letters);
}

void database::setDontCheked(long check) {
  updateSettings("dontChecked", check);
}

void database::setRequestTime(int time) {
  updateSettings("loopTime", time * 1000);
}

void database::setLogin(QString login) {
  QSqlQuery q1(db);
  q1.exec(QString("UPDATE settings SET login = '%1';").arg(login));
}

void database::setPass(QString pass) {
  QSqlQuery q1(db);
  q1.exec(QString("UPDATE settings SET pass = '%1';").arg(pass));
}

int database::getRequestTime() {
  QSqlQuery q1(db);
  q1.exec("SELECT loopTime FROM settings;");
  q1.first();
  return q1.value(0).toInt();
}

int database::getRTime() {
  QSqlQuery q1(db);
  q1.exec("SELECT loopTime FROM settings;");
  q1.first();
  return q1.value(0).toInt() / 1000;
}

void database::updateSettings(QString field, long date) {
  QSqlQuery q1(db);
  q1.exec(QString("UPDATE settings SET %1 = %2 where "
                  "login = '%3';")
              .arg(field)
              .arg(date)
              .arg(this->login));
}

int database::clientCount() {
  QSqlQuery q1(db);
  q1.exec("SELECT count (*) FROM orders;");
  q1.first();
  return q1.value(0).toInt();
}

QPair<QString, QString> database::getLoginPass() {
  QSqlQuery q1(db);
  q1.exec("SELECT login,pass FROM settings;");
  q1.first();
  QPair<QString, QString> pairLogin;
  login = q1.value(0).toString();
  pairLogin.first = login;
  pairLogin.second = q1.value(1).toString();
  return pairLogin;
}

Client database::getUser(QString nameQ, QString phoneQ, QString emailQ) {
  QSqlQuery q1(db);
  q1.exec(QString("SELECT * FROM orders where name = '%1' and phone = '%2' and "
                  "email = '%3';")
              .arg(nameQ)
              .arg(phoneQ)
              .arg(emailQ));
  q1.first();
  int IdQ = q1.value(0).toInt();
  QString name = q1.value(1).toString();
  QString phone = q1.value(2).toString();
  QString email = q1.value(3).toString();
  bool status = q1.value(4).toBool();
  Client *user = new Client(name, phone, email, status, IdQ);
  user->Id = IdQ;
  user->listK = getComments(IdQ);
  return *user;
}

Client database::getUser(int Id) {
  QSqlQuery q1(db);
  q1.exec(QString("SELECT * FROM orders where id = '%1';").arg(Id));
  q1.first();
  return getUser(q1.value(1).toString(), q1.value(2).toString(),
                 q1.value(3).toString());
}

QStringList *database::getComments(int Id) {
  QSqlQuery q1(db);
  q1.exec(QString("SELECT comment FROM comments where userId = '%1';").arg(Id));
  QStringList *comments = new QStringList();
  while (q1.next()) {
    comments->append(q1.value(0).toString());
  }
  return comments;
}

QList<QStringList> database::getUsersList() {
  QSqlQuery q1(db);
  q1.exec("SELECT * FROM orders;");
  QList<QStringList> listOrders;
  while (q1.next()) {
    QStringList order;
    for (int i = 0; i < 4; i++) {
      order.append(q1.value(i).toString());
    }
    listOrders.append(order);
  }
  return listOrders;
}

int database::setUser(Client *client) {
  QSqlQuery q1(db);
  q1.prepare("INSERT INTO orders(name,phone,email,status) "
             "VALUES(?,?,?,?);");
  q1.addBindValue(client->name);
  q1.addBindValue(client->phoneNumber);
  q1.addBindValue(client->email);
  q1.addBindValue(client->status);
  q1.exec();
  q1.exec(
      QString("SELECT Id FROM orders where name = '%1';").arg(client->name));
  return q1.value(0).toInt();
}

void database::setComment(int userId, QString comment) {
  QSqlQuery q1(db);
  q1.prepare("INSERT INTO comments(userId,comment) "
             "VALUES(?,?);");
  q1.addBindValue(userId);
  q1.addBindValue(comment);
  q1.exec();
}

void database::updateClient(QString id, QString name, QString phone,
                            QString email) {

  QSqlQuery q1(db);
  QString ss =
      QString("UPDATE orders SET name = '%2', phone = '%3', email = '%4' where "
              "id = '%1';")
          .arg(id)
          .arg(name)
          .arg(phone)
          .arg(email);
  q1.exec(ss);
}
