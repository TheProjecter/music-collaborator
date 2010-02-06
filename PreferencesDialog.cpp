#include "PreferencesDialog.h"
#include "ui_PreferencesDialog.h"

#include <QtNetwork/QFtp>
#include <QUrl>
#include <QMessageBox>
#include <QSettings>

PreferencesDialog::PreferencesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PreferencesDialog), m_ftp( 0 )
{
    ui->setupUi(this);

    QSettings settings;
    ui->edtPassword->setText( settings.value( "ftp-password" ).toString() );
    ui->edtServerURL->setText( settings.value( "ftp-server" ).toString() );
    ui->edtUsername->setText( settings.value( "ftp-user" ).toString() );
}

PreferencesDialog::~PreferencesDialog()
{
    delete ui;
}


void PreferencesDialog::accept()
{
    QSettings settings;
    settings.setValue( "ftp-password", ui->edtPassword->text() );
    settings.setValue( "ftp-user", ui->edtUsername->text() );
    settings.setValue( "ftp-server", ui->edtServerURL->text() );
    QDialog::accept();
}

void PreferencesDialog::changeEvent(QEvent *e)
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


void PreferencesDialog::resetFtp()
{
    if( m_ftp!=0 )
    {
        m_ftp->abort();
        m_ftp->deleteLater();
        m_ftp = 0;
    }

    m_ftp = new QFtp( this );
    connect( m_ftp, SIGNAL(commandFinished(int,bool)), this, SLOT(ftpCommandFinished(int,bool)));
}

void PreferencesDialog::on_testConnection_clicked()
{
    resetFtp();
    QUrl url( ui->edtServerURL->text() );
    if( !url.isValid() || url.scheme().toLower() != QLatin1String( "ftp" ))
    {
        m_ftp->connectToHost( ui->edtServerURL->text(), 21 );
    }
    else
    {
        m_ftp->connectToHost( url.host(), url.port( 21 ) );
    }
    m_ftp->login( ui->edtUsername->text(), ui->edtPassword->text() );
}






void PreferencesDialog::ftpCommandFinished( int id, bool error )
{
    if( m_ftp->currentCommand() == QFtp::ConnectToHost )
    {
        if( error )
        {
            QMessageBox msgbox;
            msgbox.setText( "Unable to connect to FTP server." );
            msgbox.setInformativeText( "Please verify that the server URL points to a valid FTP server." );
            msgbox.setDetailedText( m_ftp->errorString() );
            msgbox.setIconPixmap( QPixmap( ":/Icons/warning" ).scaled( 64, 64 ) );
            msgbox.exec();
        }
    }
    else if( m_ftp->currentCommand() == QFtp::Login )
    {
        if( error )
        {
            QMessageBox msgbox;
            msgbox.setText( "Unable to login to the FTP server." );
            msgbox.setInformativeText( "Please verify that the user credentials you specified are correct." );
            msgbox.setDetailedText( m_ftp->errorString() );
            msgbox.setIconPixmap( QPixmap( "://Icons/warning" ).scaled( 64, 64 ) );
            msgbox.exec();
        }
        else
        {
            QMessageBox msgbox;
            msgbox.setText( "Successfully connected to the FTP server!" );
            msgbox.setIconPixmap( QPixmap( ":/Icons/information" ).scaled( 64, 64 ) );
            msgbox.exec();
        }
    }
}
