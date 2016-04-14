#ifndef SHOWPROTOCOL_H
#define SHOWPROTOCOL_H

#include <QMainWindow>

namespace Ui {
class showprotocol;
}

class QTableWidgetItem;


class showprotocol : public QMainWindow
{
    Q_OBJECT

public:
    explicit showprotocol(QWidget *parent = 0);
    ~showprotocol();
    void set(int acolumn, QStringList aheader);
private slots:
    void clieck_item(QTableWidgetItem*);

private:
    Ui::showprotocol *ui;
    int m_maxcolumn ;
};

#endif // SHOWPROTOCOL_H

 /* vim: set expandtab ts=2 sw=2 sts=2 tw=100: */
