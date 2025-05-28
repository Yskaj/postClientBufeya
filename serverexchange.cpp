#include "serverexchange.h"

serverExchange::serverExchange() {
  socket = new QSslSocket;
  db = new database();
  connecters();
  connectToHost();
}

int serverExchange::getTimeRequest() { return db->getRequestTime() / 1000; }

void serverExchange::connecters() {
  connect(socket, &QSslSocket::readyRead, this,
          &serverExchange::socketReadyRead);
  connect(socket, &QSslSocket::encrypted, this,
          &serverExchange::socketEncrypted);
  connect(socket,
          QOverload<const QList<QSslError> &>::of(&QSslSocket::sslErrors), this,
          &serverExchange::connectError);
  connect(socket, &QSslSocket::errorOccurred, this,
          &serverExchange::connectError);
  timer = new QTimer(this);
}

void serverExchange::setSettings(QString time, QString login, QString pass) {
  if (!time.isEmpty())
    db->setRequestTime(time.toInt());
  if (!login.isEmpty())
    db->setLogin(login);
  if (!pass.isEmpty())
    db->setPass(pass);
}
void serverExchange::connectError() { qDebug() << socket->errorString(); }

void serverExchange::connectToHost() {
  socket->connectToHostEncrypted(hostName, hostPort);
}

void serverExchange::socketReadyRead() {
  QString response = socket->readAll();
  QString startResponse = response.left(5);
  if (QString::compare(startResponse, "* CAP", Qt::CaseInsensitive) == 0) {
    if (response.contains("OK login Completed")) {
      processRequest();
    }
  }
}

void serverExchange::socketEncrypted() {
  QPair logins = db->getLoginPass();
  QString request =
      QString("01 login %1 %2\r\n").arg(logins.first, logins.second);
  const char *msg = request.toStdString().c_str();
  socket->write(msg);
}

void serverExchange::processRequest() {
  disconnect(socket, &QSslSocket::readyRead, this,
             &serverExchange::socketReadyRead);
  connect(socket, &QSslSocket::readyRead, this,
          &serverExchange::responseLooper);
  connect(timer, &QTimer::timeout, this, &serverExchange::reqLoop);
  reqLoop();
}

void serverExchange::responseLooper() {
  QtConcurrent::run([=]() {
    QString answer = socket->readAll();
    if (answer.left(5) == "* FLA") { // Пришло количество писем или письмо?
      long existsLetters = answer.split("*")[2].split(" ")[1].toLong();
      long wasLetters = db->getLettersCount();
      if (!wasLetters) { // Заполняет БД начальными значениями при первом
                         // запуске
        db->setLettersCount(existsLetters);
        db->setDontCheked(0);
        return;
      }
      long lettersCount = existsLetters - wasLetters;
      db->setLettersCount(existsLetters);
      if (lettersCount > 0) {
        while (lettersCount) {
          QString strMsg =
              QString("02 fetch %1 BODY[]\r\n").arg(existsLetters--);
          const char *msg = strMsg.toStdString().c_str();
          socket->write(msg);
          lettersCount--;
        }
      }
    } else if (answer.contains("From: bufeya.ru")) {
      QStringList letter = answer.split("\r\n");
      Client *client = new Client(letter[letter.size() - 5].remove(
                                      letter[letter.size() - 5].size() - 2, 2),
                                  letter[letter.size() - 4].remove(
                                      letter[letter.size() - 4].size() - 2, 2),
                                  letter[letter.size() - 3]);
      db->setUser(client);
      emit newOrder(client);
    }
  });
}
void serverExchange::reqLoop() {
  socket->write("01 select INBOX\r\n");
  timer->start(db->getRequestTime());
}
