#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QKeyEvent>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    fd = new FindDialog(this);
    sp = new showprotocol(this);
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

