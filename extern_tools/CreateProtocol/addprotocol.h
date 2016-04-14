#ifndef ADDPROTOCOL_H
#define ADDPROTOCOL_H

#include <QMainWindow>

namespace Ui {
class addprotocol;
}

class addprotocol : public QMainWindow
{
    Q_OBJECT

public:
    explicit addprotocol(QWidget *parent = 0);
    ~addprotocol();

private:
    Ui::addprotocol *ui;
};

#endif // ADDPROTOCOL_H

 /* vim: set expandtab ts=2 sw=2 sts=2 tw=100: */
