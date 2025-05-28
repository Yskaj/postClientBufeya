#ifndef WIDGET_H
#define WIDGET_H
#include <QAction>
#include <QCloseEvent>
#include <QDialog>
#include <QLineEdit>
#include <QMainWindow>
#include <QMenu>
#include <QMessageBox>
#include <QMouseEvent>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QProcess>
#include <QPushButton>
#include <QSizeGrip>
#include <QSqlTableModel>
#include <QSslSocket>
#include <QSystemTrayIcon>
#include <QTableWidget>
#include <QThread>
#include <QWidget>
#include <client.h>
#include <clientform.h>
#include <clientwidget.h>
#include <dialog.h>
#include <serverexchange.h>

namespace Ui {
class Widget;
}
class Widget : public QWidget {
  Q_OBJECT
protected:
  void resizeEvent(QResizeEvent *event) override;
  void closeEvent(QCloseEvent *event) override;

public:
  explicit Widget(QWidget *parent = 0);
  ~Widget() override;
  serverExchange sExch;

public slots:
  void newOrderSigal(Client *);
  void updateClientTable();
  void onTrayIconActivated(QSystemTrayIcon::ActivationReason reason);

private slots:
  void connectOrderSigal();
  void on_listOrders_doubleClicked(const QModelIndex &index);
  void on_settings_button_clicked();
  void on_client_list_button_clicked();

private:
  Ui::Widget *ui;
  QSystemTrayIcon *trayIcon;
  void startExchange();
  QLineEdit *login;
  QLineEdit *pass;
  QLineEdit *ti;
  QTableWidget *lo;
  void CreateClientsTable();
  bool isTableCreated = false;
  QSqlTableModel *model;
  ClientForm *clientForm;
  void iconInTray();
  QAction *viewWindow;
};

#endif // WIDGET_H
