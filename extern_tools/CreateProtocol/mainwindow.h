#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "finddialog.h"
#include "showprotocol.h"
#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    bool event(QEvent *event);

private:
    Ui::MainWindow *ui;
    FindDialog* fd;
    showprotocol* sp;
};

#endif // MAINWINDOW_H

 /* vim: set expandtab ts=2 sw=2 sts=2 tw=100: */
