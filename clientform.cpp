#include "clientform.h"
#include "ui_clientform.h"

ClientForm::ClientForm(QWidget *parent)
    : QWidget(parent), ui(new Ui::ClientForm) {
  ui->setupUi(this);
}
ClientForm::ClientForm(QWidget *parent, serverExchange *sExch, QString name,
                       QString number, QString email)
    : QWidget(parent), ui(new Ui::ClientForm) {
  ui->setupUi(this);
  this->setWindowTitle("Клиент");
  this->sExch = sExch;
  setData(name, number, email);
}

void ClientForm::resizeEvent(QResizeEvent *event) {
  ui->verticalLayoutWidget->resize(this->size());
}

ClientForm::~ClientForm() { delete ui; }

void ClientForm::setData(QString name, QString phoneNumber, QString email) {
  client = new Client();
  *client = sExch->db->getUser(name, phoneNumber, email);
  ui->nameEdit->setText(client->name);
  ui->phoneEdit->setText(client->phoneNumber);
  ui->emailEdit->setText(client->email);
  this->name = client->name;
  this->phone = client->phoneNumber;
  this->email = client->email;
  QStringListModel *model = new QStringListModel(*(client->listK));
  ui->CommentsList->setModel(model);
}

void ClientForm::on_newCommentButton_clicked() {
  QWidget *comment = new QWidget();
  comment->setWindowModality(Qt::WindowModal);
  comment->setWindowTitle("Новый комментарий");
  QVBoxLayout *vbl = new QVBoxLayout();
  QPlainTextEdit *pte = new QPlainTextEdit();
  QPushButton *butSave = new QPushButton("Сохранить");
  vbl->addWidget(pte);
  vbl->addWidget(butSave);
  comment->setLayout(vbl);
  comment->show();
  connect(butSave, &QPushButton::clicked, [=] {
    sExch->db->setComment(client->Id, pte->toPlainText());
    comment->close();
    setData(client->name, client->phoneNumber, client->email);
  });
}

void ClientForm::on_saveButton_clicked() {
  if (ui->nameEdit->text() != name || ui->phoneEdit->text() != phone ||
      ui->emailEdit->text() != email) {
    QtConcurrent::run([=]() {
      sExch->db->updateClient(QString::number(client->Id), ui->nameEdit->text(),
                              ui->phoneEdit->text(), ui->emailEdit->text());
      emit updateClient();
    });
  }
  emit this->close();
}
