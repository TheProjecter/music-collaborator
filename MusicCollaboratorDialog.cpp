#include "MusicCollaboratorDialog.h"
#include "ui_MusicCollaboratorDialog.h"

MusicCollaboratorDialog::MusicCollaboratorDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MusicCollaboratorDialog)
{
    ui->setupUi(this);
}

MusicCollaboratorDialog::~MusicCollaboratorDialog()
{
    delete ui;
}

void MusicCollaboratorDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
