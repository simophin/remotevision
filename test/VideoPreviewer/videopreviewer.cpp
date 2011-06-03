#include "videopreviewer.h"
#include "ui_videopreviewer.h"

VideoPreviewer::VideoPreviewer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VideoPreviewer)
{
    ui->setupUi(this);
}

VideoPreviewer::~VideoPreviewer()
{
    delete ui;
}
