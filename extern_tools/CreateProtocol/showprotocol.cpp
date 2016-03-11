#include "showprotocol.h"
#include "ui_showprotocol.h"

showprotocol::showprotocol(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::showprotocol)
{
    ui->setupUi(this);
    QStringList headerLabels;
    headerLabels << "C1" << "C2";
    ui->table2->setHorizontalHeaderLabels(headerLabels);
    this->show();
}

showprotocol::~showprotocol()
{
    delete ui;
}
