#include "RepositoryModel.h"
#include "FileItem.h"

#include <QDebug>
#include <QDir>
#include <QIcon>
#include <QSettings>
#include <QString>
#include <QUrl>


RepositoryModel::RepositoryModel()
    : m_currentlyScanned( 0 ), m_ftp( 0 )
{
    QSettings settings;
    m_rootItem = new FileItem( 0, QFileInfo(), 0 );
    m_fsWatcher = new QFileSystemWatcher( this );

    unsigned int count = settings.beginReadArray( "local-repository" );
    for( unsigned int i=0; i<count; ++i )
    {
        settings.setArrayIndex( i );
        QString path = settings.value( "path" ).toString();
        FileItem* item = new FileItem( 0, path, 0 );
        connect( item, SIGNAL(statusChanged(FileItem::Status)),
                 this, SLOT(fileItemStatusChanged(FileItem::Status)) );
        m_rootItem->addChild( item );
        m_fsWatcher->addPath( path );
    }

    resetFtpConnection();
}


RepositoryModel::~RepositoryModel()
{
    QSettings settings;
    QStringList lst;
    for( int i=0; i<m_rootItem->getRowCount(); ++i )
    {
        QString localPath = m_rootItem->child( i )->getLocalPath();
        if( !localPath.isEmpty() )
            lst.append( localPath );
    }
    settings.beginWriteArray( "local-repository", lst.count() );
    for( int i=0; i<lst.count(); ++i )
    {
        settings.setArrayIndex( i );
        qDebug() << "Writing local path '" << lst[i] << "'";
        settings.setValue( "path", lst[i] );
    }
    settings.endArray();
}


void RepositoryModel::addFile( const QFileInfo &file )
{
    FileItem* item = new FileItem( 0, file, 0 );
    connect( item, SIGNAL(statusChanged(FileItem::Status)),
             this, SLOT(fileItemStatusChanged(FileItem::Status)));
    addFile( item, m_rootItem );
}


void RepositoryModel::addFile(FileItem *file, FileItem *parent)
{
    QModelIndex parentIndex;
    if( parent==0 )
        parent = m_rootItem;

    if( parent!=m_rootItem )
        parentIndex = createIndex( parent->getRowNumber(), 0, parent );
    emit beginInsertRows( parentIndex, parent->getRowCount(), parent->getRowCount() );
    parent->addChild( file );
    emit endInsertRows();
}


QVariant RepositoryModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if( orientation == Qt::Horizontal && role==Qt::DisplayRole && section==0 )
        return QString( "Local Files" );
    return QVariant();
}


int RepositoryModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}

QVariant RepositoryModel::data(const QModelIndex &index, int role) const
{
    if( !index.isValid() )
        return QVariant();

    FileItem* item = static_cast<FileItem*>( index.internalPointer() );
    if( role == Qt::DisplayRole )
        return item->getFileName();
    else if( role==Qt::DecorationRole )
    {
        if( item->isFolder() )
        {
            switch ( item->status() )
            {
            case FileItem::Error:
                return QIcon( ":/Icons/folder-error" );
            case FileItem::LocallyAdded:
                return QIcon( ":/Icons/folder-localyadded" );
            case FileItem::LocallyModified:
                return QIcon( ":/Icons/folder-locallymodified" );
            case FileItem::LocallyRemoved:
                return QIcon( ":/Icons/folder-locallyremoved" );
            case FileItem::NeedsUpdate:
                return QIcon( ":/Icons/folder-remotelymodified" );
            case FileItem::OnlyOnServer:
                return QIcon( ":/Icons/folder-onlyonserver" );
            case FileItem::Unknown:
                return QIcon( ":/Icons/folder-unknown" );
            case FileItem::UpToDate:
                return QIcon( ":/Icons/folder" );
            }
        }
        else
            return QIcon( ":/Icons/audiofile");
    }
    else if( role==Qt::ToolTipRole )
        return "A tooltip stuffy .. ";
    else if( role == Qt::StatusTipRole )
        return "A statustip stuffy .. ";
    return QVariant();
}

QModelIndex RepositoryModel::index( int row, int column, const QModelIndex &parent ) const
{
    if( !hasIndex( row, column, parent ))
        return QModelIndex();

    FileItem* parentItem;
    if( !parent.isValid() )
        parentItem = m_rootItem;
    else
        parentItem = static_cast<FileItem*>( parent.internalPointer() );

    FileItem* child = parentItem->child( row );
    if( child )
        return createIndex( row, column, child );
    return QModelIndex();
}


QModelIndex RepositoryModel::parent(const QModelIndex &child) const
{
    if( !child.isValid() )
        return QModelIndex();
    FileItem* childItem = static_cast<FileItem*>( child.internalPointer() );
    FileItem* parentItem = childItem->parent();
    if( parentItem==0 || parentItem == m_rootItem )
        return QModelIndex();
    return createIndex( parentItem->getRowNumber(), 0, parentItem );
}


int RepositoryModel::rowCount(const QModelIndex &parent) const
{
    FileItem* parentItem;
    if( !parent.isValid() )
        parentItem = m_rootItem;
    else
        parentItem = static_cast<FileItem*>( parent.internalPointer() );
    return parentItem->getRowCount();
}


void RepositoryModel::fileItemStatusChanged( FileItem::Status status )
{
    FileItem* item = static_cast<FileItem*>( sender() );
    if( item )
    {
        QModelIndex ix = createIndex( item->getRowNumber(), 0, item );
        emit dataChanged( ix, ix );
    }
    else
    {
        qWarning() << "Unable to cast fileStatus sender to FileItem";
    }
}


void RepositoryModel::ftpFileListing( const QUrlInfo &urlinfo )
{
    qDebug() << "Listing returned" << urlinfo.name() << "for item" << m_currentlyScanned->getRepositoryPath();
    QString itemName = urlinfo.name().right( urlinfo.name().length() -
                                             m_currentlyScanned->getRepositoryPath().length() );
    if( urlinfo.name() == m_currentlyScanned->getRepositoryPath() )
    {
        return;
    }
    FileItem* childItem = m_currentlyScanned->child( itemName );
    if( childItem==0 )
    {
        childItem = new FileItem( 0, QFileInfo(), 0 );
        connect( childItem, SIGNAL(statusChanged(FileItem::Status)),
                 this, SLOT(fileItemStatusChanged(FileItem::Status)) );
        addFile( childItem, m_currentlyScanned );
    }
    childItem->updateRemoteStatus( urlinfo );
}


void RepositoryModel::ftpStateChanged( int state )
{
    if( state == QFtp::LoggedIn )
    {
        scanFile( m_rootItem );
    }
}


void RepositoryModel::ftpCommandFinished( int id, bool error )
{
    if( m_ftp->currentCommand() == QFtp::ConnectToHost )
    {
    }
    else if( m_ftp->currentCommand() == QFtp::Login )
    {
    }
    else if( m_ftp->currentCommand() == QFtp::List )
    {
        if( error )
        {
            qDebug() << m_ftp->errorString();
        }
        else
        {
            qDebug() << "Listing command successful for " << m_currentlyScanned->getRepositoryPath();
        }
        m_currentlyScanned->scanComplete();
        m_currentlyScanned->setScanned( true );
        QString lastScanned = m_currentlyScanned->getRepositoryPath();
        m_currentlyScanned = m_rootItem->getNextUnScanned();
        qDebug() << "Got" << ( m_currentlyScanned==0?"NULL":m_currentlyScanned->getRepositoryPath() )
                << "as next unscanned after" << lastScanned;
        if( m_currentlyScanned )
            scanFile( m_currentlyScanned );
    }
}


void RepositoryModel::resetFtpConnection()
{
    if( m_ftp!=0 )
    {
        m_ftp->abort();
        m_ftp->deleteLater();
        m_ftp = 0;
    }

    m_ftp = new QFtp();
    connect( m_ftp, SIGNAL(commandFinished(int,bool)), this, SLOT(ftpCommandFinished(int,bool)));
    connect( m_ftp, SIGNAL(stateChanged(int)), this, SLOT(ftpStateChanged(int)) );
    connect( m_ftp, SIGNAL(listInfo(QUrlInfo)), this, SLOT(ftpFileListing(QUrlInfo)) );

    QSettings settings;
    QUrl url( settings.value( "ftp-server" ).toString() );
    if( !url.isValid() || url.scheme().toLower() != QLatin1String( "ftp" ))
        m_ftp->connectToHost( settings.value( "ftp-server" ).toString(), 21 );
    else
        m_ftp->connectToHost( url.host(), url.port( 21 ) );
    m_ftp->login( settings.value( "ftp-user" ).toString(),
                            settings.value( "ftp-password" ).toString() );
    QString dir = settings.value( "ftp-dir" ).toString();
    if( !dir.isEmpty() )
        m_ftp->cd( dir );
}





void RepositoryModel::scanFile( FileItem *fileItem )
{
    m_currentlyScanned = fileItem;
    m_ftp->list( m_currentlyScanned->getRepositoryPath() );
}
