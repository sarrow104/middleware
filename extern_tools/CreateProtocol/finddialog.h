#ifndef FINDDIALOG_H
#define FINDDIALOG_H

#include <QDialog>

namespace Ui {
class FindDialog;
}

class QLineEdit;

class FindDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FindDialog(QWidget *parent = 0);
    ~FindDialog();
    bool event(QEvent *e);
private slots:
    void search();
private:
    Ui::FindDialog *ui;
};

#endif // FINDDIALOG_H
