
#include "MCMainWindow.h"

#include <QApplication>

int main( int arc, char** argv )
{
    QApplication app( arc, argv );
    app.setApplicationName( "Music Collaborator" );
    app.setApplicationVersion( "0.1" );
    app.setOrganizationName( "Svansoft" );
    app.setOrganizationDomain( "svans.net" );

    QObject::connect( &app, SIGNAL(lastWindowClosed()), &app, SLOT(quit()) );

    MCMainWindow mainWindow;
    mainWindow.show();

    return app.exec();
}
