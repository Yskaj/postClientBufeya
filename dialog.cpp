#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) : QDialog(parent), ui(new Ui::Dialog) {
  ui->setupUi(this);
  this->setAttribute(Qt::WA_Disabled, true);
  ui->verticalLayoutWidget->setSizePolicy(QSizePolicy::Expanding,
                                          QSizePolicy::Expanding);
  ui->verticalLayoutWidget->resize(this->size());
  ui->requestTime->setText(
      QString(QString::number(database::getRTime()) + " сек."));
  this->setVisible(false);
}
void Dialog::resizeEvent(QResizeEvent *event) {
  ui->verticalLayoutWidget->resize(this->size());
  QWidget::resizeEvent(event);
}

void Dialog::closeEvent(QCloseEvent *event) {
  event->ignore();
  this->hide();
}

Dialog::~Dialog() { delete ui; }
