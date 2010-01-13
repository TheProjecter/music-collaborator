#ifndef MUSICCOLLABORATORDIALOG_H
#define MUSICCOLLABORATORDIALOG_H

#include <QDialog>

namespace Ui {
    class MusicCollaboratorDialog;
}

class MusicCollaboratorDialog : public QDialog {
    Q_OBJECT
public:
    MusicCollaboratorDialog(QWidget *parent = 0);
    ~MusicCollaboratorDialog();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::MusicCollaboratorDialog *ui;
};

#endif // MUSICCOLLABORATORDIALOG_H
