#ifndef CONNECTDIALOG_H
#define CONNECTDIALOG_H

#include <QtGui/QDialog>

namespace Ui {
    class ConnectDialog;
}

class ConnectDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConnectDialog(QWidget *parent = 0);
    ~ConnectDialog();

protected:
    void init();

private slots:
    void on_btnConnect_clicked();
    void resetUI();
    void onVideoPreviewerDestroyed(QObject *);
    void onRemoteConnectSuccess();
    void onRemoteConnectError(QString);

private:
    Ui::ConnectDialog *ui;

    class Impl;
    Impl *d;
};

#endif // CONNECTDIALOG_H
