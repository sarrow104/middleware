#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "finddialog.h"
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
};

#endif // MAINWINDOW_H
