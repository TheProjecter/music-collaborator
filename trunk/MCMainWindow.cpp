#include "MCMainWindow.h"
#include "ui_MCMainWindow.h"

MCMainWindow::MCMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MCMainWindow)
{
    ui->setupUi(this);
}

MCMainWindow::~MCMainWindow()
{
    delete ui;
}

void MCMainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
