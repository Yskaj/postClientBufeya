#include "widget.h"
#include "ui_widget.h"
#include <QNetworkProxy>

Widget::Widget(QWidget *parent) : QWidget(parent), ui(new Ui::Widget) {

  ui->setupUi(this);
  this->setWindowTitle("");
  QPixmap pm = QPixmap(1, 1);
  pm.fill(QColor(0, 0, 0, 0));
  this->setWindowIcon(QIcon(pm));
  this->setAttribute(Qt::WA_QuitOnClose, true);
  this->ui->verticalLayoutWidget->setSizePolicy(QSizePolicy::Expanding,
                                                QSizePolicy::Expanding);
  ui->verticalLayoutWidget->resize(this->size());
  ui->listOrders->hide();
  connectOrderSigal();
  iconInTray();
  CreateClientsTable();
  startExchange();
}
void Widget::resizeEvent(QResizeEvent *event) {
  ui->verticalLayoutWidget->resize(this->size());
  ui->listOrders->resize(this->width(), this->height() - 110);
  QWidget::resizeEvent(event);
}
void Widget::iconInTray() // иконка в трее
{
  trayIcon = new QSystemTrayIcon(this);
  trayIcon->setIcon(QIcon(":/new/img/res/favicon.ico"));
  connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this,
          SLOT(onTrayIconActivated(QSystemTrayIcon::ActivationReason)));
  QMenu *menuForTray = new QMenu(this);
  QAction *quitAction = new QAction(("Выход"), this);
  viewWindow = new QAction(("Развернуть окно"), this);
  connect(viewWindow, &QAction::triggered, [=]() {
    viewWindow->setVisible(false);
    this->show();
  });
  connect(quitAction, SIGNAL(triggered()), this, SLOT(close()));
  menuForTray->addAction(quitAction);
  trayIcon->setContextMenu(menuForTray);
  trayIcon->show();
}

void Widget::onTrayIconActivated(
    QSystemTrayIcon::ActivationReason reason) // нажатие иконки в трее
{
  switch (reason) {
  case QSystemTrayIcon::Trigger:
    if (!this->isVisible()) {
      viewWindow->setVisible(false);
      this->show();
      this->activateWindow();
    } else {
      viewWindow->setVisible(true);
      this->hide();
    }
    break;
  default:
    break;
  }
}
void Widget::closeEvent(QCloseEvent *event) // закрытие приложения
{
  if (this->isVisible()) {
    event->ignore();
    this->hide();
  } else {
    event->accept();
  }
}

Widget::~Widget() { delete ui; }

void Widget::on_listOrders_doubleClicked(const QModelIndex &index) {
  QList lUser(ui->listOrders->selectedItems());
  clientForm =
      new ClientForm(this->parentWidget(), &(this->sExch), lUser[0]->text(),
                     lUser[1]->text(), lUser[2]->text());
  connect(clientForm, &ClientForm::updateClient, this,
          &Widget::updateClientTable);
  clientForm->show();
}

void Widget::newOrderSigal(Client *client) {
  ui->ClientMainLabel->setText(client->toString());
  if (!this->isVisible() || this->isMinimized()) {
    this->setVisible(true);
    this->showNormal();
    updateClientTable();
    ui->stackedWidget->setCurrentIndex(0);
  }
}
void Widget::updateClientTable() { this->CreateClientsTable(); }

void Widget::startExchange() {}

void Widget::connectOrderSigal() {
  connect(&sExch, SIGNAL(newOrder(Client *)), this,
          SLOT(newOrderSigal(Client *)));
}

void Widget::on_settings_button_clicked() {
  QWidget *sett = new QWidget();
  sett->setWindowTitle("Настройки");
  sett->setAttribute(Qt::WA_DeleteOnClose, true);
  QPixmap pm = QPixmap(1, 1);
  pm.fill(QColor(0, 0, 0, 0));
  sett->setWindowIcon(QIcon(pm));
  QVBoxLayout *generalField = new QVBoxLayout;
  QHBoxLayout *b1 = new QHBoxLayout;
  QHBoxLayout *b2 = new QHBoxLayout;
  QHBoxLayout *b3 = new QHBoxLayout;
  QHBoxLayout *b4 = new QHBoxLayout;
  QLabel *timeR = new QLabel("Частота обновления запроса(сек.): ");
  ti = new QLineEdit();
  ti->setText(QString::number(sExch.getTimeRequest()));
  b1->addWidget(timeR);
  b1->addWidget(ti);
  b1->setMargin(20);
  generalField->addLayout(b1);
  QLabel *log = new QLabel("Логин:   ");
  login = new QLineEdit();
  b2->addWidget(log);
  b2->addWidget(login);
  b2->setMargin(20);
  generalField->addLayout(b2);
  QLabel *passL = new QLabel("Пароль: ");
  pass = new QLineEdit();
  b3->addWidget(passL);
  b3->addWidget(pass);
  b3->setMargin(20);
  generalField->addLayout(b3);
  QSpacerItem *sp =
      new QSpacerItem(20, 40, QSizePolicy::Expanding, QSizePolicy::Fixed);
  b4->addItem(sp);
  QPushButton *butSave = new QPushButton("Сохранить");
  connect(butSave, &QPushButton::clicked, [=]() {
    QString rTime = ti->text();
    QString loginEdit = login->text();
    QString passEdit = pass->text();
    sExch.setSettings(rTime, loginEdit, passEdit);
    sett->close();
  });
  b4->addWidget(butSave);
  generalField->addLayout(b4);
  sett->setLayout(generalField);
  sett->show();
  ui->settings_button->setEnabled(false);
  connect(sett, &QWidget::destroyed,
          [=]() { ui->settings_button->setEnabled(true); });
}

void Widget::on_client_list_button_clicked() {
  ui->stackedWidget->setCurrentIndex(1);
  if (!isTableCreated) {
    isTableCreated = true;
    CreateClientsTable();
  }
}

void Widget::CreateClientsTable() {
  lo = ui->listOrders;
  ui->listOrders->setColumnCount(3);
  ui->listOrders->horizontalHeader()->setVisible(true);
  ui->listOrders->setHorizontalHeaderLabels(
      QStringList() << tr("ФИО") << tr("Номер") << tr("e-mail"));
  lo->setRowCount(0);
  lo->clear();
  lo->resize(this->size());
  lo->horizontalHeader()->setStretchLastSection(true);
  lo->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
  lo->setSelectionBehavior(QAbstractItemView::SelectRows);
  lo->horizontalHeader()->setVisible(true);
  int clientCount = sExch.db->clientCount();
  for (int i = 0; i < clientCount; i++) {
    lo->insertRow(i);
    for (int j = 0; j < 3; j++) {
      lo->setItem(
          i, j,
          new QTableWidgetItem(sExch.db->getUser(i + 1).getClientDate()[j]));
    }
  }
  ui->listOrders->sortItems(0, Qt::DescendingOrder);
  ui->listOrders->show();
}
