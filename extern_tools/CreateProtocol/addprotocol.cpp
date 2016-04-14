#include "addprotocol.h"
#include "ui_addprotocol.h"

addprotocol::addprotocol(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::addprotocol)
{
    ui->setupUi(this);
}

addprotocol::~addprotocol()
{
    delete ui;
}

 /* vim: set expandtab ts=2 sw=2 sts=2 tw=100: */
