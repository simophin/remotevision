#include "connectdialog.h"
#include "ui_connectdialog.h"

#include "ipvalidator.h"

class ConnectDialog::Impl {
public:
    IPValidator validator;
};

ConnectDialog::ConnectDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConnectDialog),
    d(new Impl)
{
    ui->setupUi(this);
    init();
}

ConnectDialog::~ConnectDialog()
{
    delete ui;
}

void ConnectDialog::init()
{
    ui->ipEdit->setValidator(&d->validator);
}

void ConnectDialog::on_btnConnect_clicked()
{

}
