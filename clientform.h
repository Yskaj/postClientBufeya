#ifndef CLIENTFORM_H
#define CLIENTFORM_H

#include <QPlainTextEdit>
#include <QWidget>
#include <client.h>
#include <serverexchange.h>

namespace Ui {
class ClientForm;
}

class ClientForm : public QWidget {
  Q_OBJECT
protected:
  void resizeEvent(QResizeEvent *event);

public:
  explicit ClientForm(QWidget *parent = nullptr);
  ClientForm(QWidget *parent = nullptr, serverExchange *sE = nullptr,
             QString name = "", QString number = "", QString email = "");
  ~ClientForm();

private slots:
  void on_newCommentButton_clicked();
  void on_saveButton_clicked();
signals:
  void updateClient();

private:
  Ui::ClientForm *ui;
  void setData(QString, QString, QString);
  serverExchange *sExch;
  int Id;
  QString name;
  QString phone;
  QString email;
  Client *client;
};

#endif // CLIENTFORM_H
