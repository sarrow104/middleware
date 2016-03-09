#include "finddialog.h"
#include "ui_finddialog.h"

FindDialog::FindDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FindDialog)
{
    this->grabKeyboard();
    ui->setupUi(this);
    ui->FindButton->setDefault(false);
    ui->FindButton->setEnabled(false);
    connect(
               ui->FindEdit,
               SIGNAL(textChanged(const QString&)),
               this,
               SLOT(enableFindButton(const QString&))
            );
}

FindDialog::~FindDialog()
{
    delete ui;
}


void FindDialog::enableFindButton(const QString& aitext)
{
    bool ltxtnotnull = !aitext.isEmpty();
    ui->FindButton->setDefault(ltxtnotnull);
    ui->FindButton->setEnabled(ltxtnotnull);
}
