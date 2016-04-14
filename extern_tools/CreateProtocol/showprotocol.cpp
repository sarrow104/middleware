#include "showprotocol.h"
#include "ui_showprotocol.h"

showprotocol::showprotocol(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::showprotocol)
{
    ui->setupUi(this);
    ui->table->resize(350, 200);


    //ui->table->setSelectionBehavior ( QAbstractItemView::SelectRows); //设置选择行为，以行为单位
    ui->table->setSelectionMode ( QAbstractItemView::SingleSelection); //设置选择模式，选择单行
    ui->table->setEditTriggers(QAbstractItemView::NoEditTriggers|QAbstractItemView::DoubleClicked);
    ui->table->verticalHeader()->hide();
    this->show();
    connect(ui->table,SIGNAL(itemClicked(QTableWidgetItem*)),this,SLOT(clieck_item(QTableWidgetItem*)));
}

showprotocol::~showprotocol()
{
    delete ui;
}

void showprotocol::set(int acolumn, QStringList aheader)
{
    ui->table->setRowCount(1);     //设置行数
    m_maxcolumn = acolumn;
    aheader << "";
    ui->table->setColumnCount(acolumn+1);   //设置列数
    ui->table->setHorizontalHeaderLabels(aheader);
}

void showprotocol::clieck_item(QTableWidgetItem* Item)
{
    int r = Item->row();
    int c = Item->column();
   if( c == m_maxcolumn)
    {
        //点击编辑按钮
        //Item->setFlags(Qt::ItemFlag);


    }
    else
    {

    }
}

 /* vim: set expandtab ts=2 sw=2 sts=2 tw=100: */
