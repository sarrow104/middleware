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
