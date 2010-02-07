#include "MCMainWindow.h"
#include "ui_MCMainWindow.h"

#include "FtpRepositoryModel.h"
#include "LocalRepositoryModel.h"
#include "PreferencesDialog.h"

#include <QDir>
#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>

MCMainWindow::MCMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MCMainWindow)
{
    ui->setupUi(this);
    ui->m_localRepositoryView->setModel( new RepositoryModel() );
    ui->m_remoteRepositoryView->setModel( new FtpRepositoryModel() );
}

MCMainWindow::~MCMainWindow()
{
    QAbstractItemModel* model = ui->m_localRepositoryView->model();
    ui->m_localRepositoryView->setModel( 0 );
    delete model;
    model = ui->m_remoteRepositoryView->model();
    ui->m_remoteRepositoryView->setModel( 0 );
    delete model;
    delete ui;
}

void MCMainWindow::loadSettings()
{

}

void MCMainWindow::saveSettings()
{
    QSettings s;
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



void MCMainWindow::on_actionAdd_Project_Folder_triggered()
{
    QSettings s;
    QString dir = QFileDialog::getExistingDirectory( this, "Select project directory", s.value( "last-directory" ).toString() );
    if( !dir.isNull() )
    {
        RepositoryModel* localModel = static_cast<RepositoryModel*>( ui->m_localRepositoryView->model() );
        localModel->addFile( QFileInfo( dir ) );
        s.setValue( "last-directory", dir );
    }
}




void MCMainWindow::on_actionRemove_Share_triggered()
{
    QModelIndex currIx = ui->m_localRepositoryView->currentIndex();
    QString name = ui->m_localRepositoryView->model()->data( currIx, Qt::DisplayRole ).toString();
    QMessageBox msgbox;
    msgbox.setText( QString( "Are you sure you want to stop the collaboration of %0?" ).arg( name ) );
    msgbox.setInformativeText( "Stopping collaborating a project means it will get deleted from the server" );
    msgbox.setStandardButtons( QMessageBox::Yes | QMessageBox::No );
    msgbox.exec();
    if( msgbox.clickedButton() == msgbox.button( QMessageBox::Yes ) )
    {
        ui->m_localRepositoryView->model()->removeRow( currIx.row(), currIx.parent() );
    }
}


void MCMainWindow::on_actionPreferences_triggered()
{
    PreferencesDialog* dlg = new PreferencesDialog( this );
    dlg->exec();
    delete dlg;
}
