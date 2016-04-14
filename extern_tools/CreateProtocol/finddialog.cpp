#include "finddialog.h"
#include "ui_finddialog.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QToolButton>
#include <QSplitter>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QKeyEvent>

FindDialog::FindDialog(QWidget *parent) :
    QDialog(parent, Qt::FramelessWindowHint),
    ui(new Ui::FindDialog)
{
    ui->setupUi(this);
    QString runPath = QCoreApplication::applicationDirPath();

    ui->pSearchButton->setCursor(Qt::PointingHandCursor);
    ui->pSearchButton->setFixedSize(22, 22);
    ui->pSearchButton->setToolTip(QStringLiteral(""));
    ui->pSearchButton->setStyleSheet("QPushButton{border-image:url(../CreateProtocol/images/search1.png);}");
    /** 设置输入框无边框 */
    ui->m_pSearchLineEdit->setStyleSheet("QLineEdit{border-width:0;border-style:outset}");
    //ui->m_pSearchLineEdit->setReadOnly(true);设置为只读
    /** 防止文本框输入内容位于按钮之下 */
    QMargins margins = ui->m_pSearchLineEdit->textMargins();
    ui->m_pSearchLineEdit->setTextMargins(margins.left(), margins.top(), ui->pSearchButton->width(), margins.bottom());
    ui->m_pSearchLineEdit->setPlaceholderText(QStringLiteral("请输入搜索内容"));

    ui->pSearchLayout->addStretch();
    ui->pSearchLayout->addWidget(ui->pSearchButton);
    ui->pSearchLayout->setSpacing(0);
    ui->pSearchLayout->setContentsMargins(0, 0, 0, 0);
    ui->m_pSearchLineEdit->setLayout(ui->pSearchLayout);

    connect(ui->pSearchButton, SIGNAL(clicked(bool)), this, SLOT(search()));
 }

FindDialog::~FindDialog()
{
    delete ui;
}


void FindDialog::search()
{
    QString strText = ui->m_pSearchLineEdit->text();
    if (!strText.isEmpty())
    {
        /** 搜索逻辑 */
       // QMessageBox::information(this, QStringLiteral("搜索"), QStringLiteral("搜索内容为%1").arg(strText));
    }
}


bool FindDialog::event(QEvent *e)
{
    if( e->type() == QEvent::KeyRelease )
    {
        QKeyEvent *event = static_cast<QKeyEvent*>(e);
        if (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_F)
        {
            static bool lshow = false;

            if(lshow)
            {
                this->hide();
                lshow = false;
            }
            else
            {
                this->show();
                lshow = true;
            }
        }
    }
    return true;
}

 /* vim: set expandtab ts=2 sw=2 sts=2 tw=100: */
