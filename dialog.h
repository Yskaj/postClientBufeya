#ifndef DIALOG_H
#define DIALOG_H

#include <QCloseEvent>
#include <QDialog>
#include <database.h>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog {
  Q_OBJECT
protected:
  void resizeEvent(QResizeEvent *event);
  void closeEvent(QCloseEvent *event);

public:
  explicit Dialog(QWidget *parent = nullptr);
  ~Dialog();

private:
  Ui::Dialog *ui;
};

#endif // DIALOG_H
