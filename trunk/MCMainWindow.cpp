#include "MCMainWindow.h"
#include "ui_MCMainWindow.h"
#include "PreferencesDialog.h"

#include <QDir>
#include <QFileDialog>
#include <QSettings>

MCMainWindow::MCMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MCMainWindow)
{
    ui->setupUi(this);

    ui->m_localRepositoryView->setModel( &m_localModel );
}

MCMainWindow::~MCMainWindow()
{
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
        m_localModel.addFile( QFileInfo( dir ) );
        s.setValue( "last-directory", dir );
    }
}

void MCMainWindow::on_actionAdd_Project_File_triggered()
{
    QSettings s;
    QString dir = QFileDialog::getOpenFileName( this, "Select project file", s.value( "last-directory" ).toString() );
    if( !dir.isNull() )
    {

        s.setValue( "last-directory", dir );
    }
}

void MCMainWindow::on_actionPreferences_triggered()
{
    PreferencesDialog* dlg = new PreferencesDialog( this );
    if( dlg->exec() )
    {

    }
    delete dlg;
}
