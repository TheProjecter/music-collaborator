#ifndef PREFERENCESDIALOG_H
#define PREFERENCESDIALOG_H

#include <QDialog>

class QFtp;

namespace Ui {
    class PreferencesDialog;
}

class PreferencesDialog : public QDialog {
    Q_OBJECT
public:
    PreferencesDialog(QWidget *parent = 0);
    ~PreferencesDialog();

public slots:
    void accept();

protected slots:
    void on_testConnection_clicked();
    void ftpCommandFinished( int, bool );

protected:
    void changeEvent(QEvent *e);
    void resetFtp();

private:
    Ui::PreferencesDialog *ui;
    int m_currentFtpCommand;
    QFtp*   m_ftp;
};

#endif // PREFERENCESDIALOG_H
