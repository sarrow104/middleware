#ifndef SHOWPROTOCOL_H
#define SHOWPROTOCOL_H

#include <QMainWindow>

namespace Ui {
class showprotocol;
}

class showprotocol : public QMainWindow
{
    Q_OBJECT

public:
    explicit showprotocol(QWidget *parent = 0);
    ~showprotocol();

private:
    Ui::showprotocol *ui;
};

#endif // SHOWPROTOCOL_H
