#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QKeyEvent>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    fd = new FindDialog(this);
    QStringList header;
    header<<"协议号"<<"协议名"<<"";
    sp = new showprotocol(this);
    sp->set(2,header);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete fd;
}

bool MainWindow::event(QEvent *e)
{
    if( e->type() == QEvent::KeyRelease )
    {
        QKeyEvent *event = static_cast<QKeyEvent*>(e);
        if (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_F)
        {
            static bool lshow = true;

            if(lshow)
            {
                fd->show();
                lshow = false;
            }
            else
            {
                fd->hide();
                lshow = true;
            }
        }
    }
    return true;
}


 /* vim: set expandtab ts=2 sw=2 sts=2 tw=100: */
