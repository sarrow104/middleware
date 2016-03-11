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
