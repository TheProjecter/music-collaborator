#include "FtpRepositoryModel.h"
#include "FtpFileItem.h"

#include <QDebug>
#include <QIcon>
#include <QSettings>
#include <QUrl>

FtpRepositoryModel::FtpRepositoryModel()
    : m_currentlyScannedFile(0), m_ftpConnection( 0 )
{
    m_rootItem = new FtpFileItem( 0, QUrlInfo(), 0 );
    refreshFileListing();
}

FtpRepositoryModel::~FtpRepositoryModel()
{
    if( m_ftpConnection!=0 )
    {
       m_ftpConnection->abort();
       m_ftpConnection->deleteLater();
    }
}


QVariant FtpRepositoryModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if( orientation == Qt::Horizontal && role==Qt::DisplayRole && section==0 )
        return QString( "Files on FTP server" );
    return QVariant();
}


int FtpRepositoryModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}

QVariant FtpRepositoryModel::data(const QModelIndex &index, int role) const
{
    if( !index.isValid() )
        return QVariant();

    FtpFileItem* item = static_cast<FtpFileItem*>( index.internalPointer() );
    if( role == Qt::DisplayRole )
    {
        return item->urlInfo().name();
    }
    else if( role==Qt::DecorationRole )
    {
        if( item->urlInfo().isDir() )
            return QIcon( ":/Icons/folder" );
        else
            return QIcon( ":/Icons/appicon" );
    }
    else if( role==Qt::ToolTipRole )
    {
        return "A tooltip stuffy .. ";
    }
    else if( role == Qt::StatusTipRole )
    {
        return "A statustip stuffy .. ";
    }
    return QVariant();
}


QModelIndex FtpRepositoryModel::index( int row, int column, const QModelIndex &parent ) const
{
    if( !hasIndex( row, column, parent ))
        return QModelIndex();

    FtpFileItem* parentItem;
    if( !parent.isValid() )
        parentItem = m_rootItem;
    else
        parentItem = static_cast<FtpFileItem*>( parent.internalPointer() );

    FtpFileItem* child = parentItem->child( row );
    if( child )
        return createIndex( row, column, child );
    return QModelIndex();
}


QModelIndex FtpRepositoryModel::parent(const QModelIndex &child) const
{
    if( !child.isValid() )
        return QModelIndex();
    FtpFileItem* childItem = static_cast<FtpFileItem*>( child.internalPointer() );
    FtpFileItem* parentItem = childItem->parent();
    if( parentItem==0 || parentItem == m_rootItem )
        return QModelIndex();
    return createIndex( parentItem->getRowNumber(), 0, parentItem );
}


int FtpRepositoryModel::rowCount( const QModelIndex &parent ) const
{
    FtpFileItem* parentItem;
    if( !parent.isValid() )
        parentItem = m_rootItem;
    else
        parentItem = static_cast<FtpFileItem*>( parent.internalPointer() );
    return parentItem->getRowCount();
}


void FtpRepositoryModel::ftpCommandFinished( int id, bool error )
{
    if( m_ftpConnection->currentCommand()==QFtp::List )
    {
        if( !error )
        {
            m_currentlyScannedFile->setScanned( true );
            m_currentlyScannedFile = findNextUnScannedItem( m_rootItem );

            if( m_currentlyScannedFile && !m_currentlyScannedFile->isScanned() )
            {
                scanFile( m_currentlyScannedFile );
            }
        }
        else
            qDebug() << "Directory listing failed";
    }

    if( error )
    {
        qDebug() << "Error in ftp command" << m_ftpConnection->currentCommand();
        qDebug() << ":"<<m_ftpConnection->errorString();
        switch( m_ftpConnection->currentCommand() )
        {
        case QFtp::SetProxy:
        case QFtp::SetTransferMode:
        case QFtp::ConnectToHost:
        case QFtp::List:
        case QFtp::Login:
        case QFtp::Close:
        case QFtp::Cd:
        case QFtp::Get:
        case QFtp::Put:
        case QFtp::Remove:
        case QFtp::Mkdir:
        case QFtp::Rmdir:
        case QFtp::Rename:
        case QFtp::RawCommand:
        case QFtp::None:
            break;
        }
    }
}


void FtpRepositoryModel::ftpFileListing(const QUrlInfo &urlinfo)
{
    QModelIndex index = QModelIndex();
    if( m_currentlyScannedFile != m_rootItem )
        index=createIndex( m_currentlyScannedFile->getRowNumber(), 0, m_currentlyScannedFile );

    beginInsertRows( index, m_currentlyScannedFile->getRowCount(),
                     m_currentlyScannedFile->getRowCount() );
    m_currentlyScannedFile->addFile( urlinfo );
    endInsertRows();
}



void FtpRepositoryModel::resetFtpConnection()
{
    if( m_ftpConnection!=0 )
    {
        m_ftpConnection->abort();
        m_ftpConnection->deleteLater();
        m_ftpConnection = 0;
    }
    m_ftpConnection = new QFtp( this );
    connect( m_ftpConnection, SIGNAL(commandFinished(int,bool)),
             this, SLOT(ftpCommandFinished(int,bool)));
    connect( m_ftpConnection, SIGNAL(listInfo(QUrlInfo)),
             this, SLOT(ftpFileListing(QUrlInfo)) );
}

void FtpRepositoryModel::refreshFileListing()
{
    QSettings settings;
    resetFtpConnection();
    QUrl url( settings.value( "ftp-server" ).toString() );
    if( !url.isValid() || url.scheme().toLower() != QLatin1String( "ftp" ))
        m_ftpConnection->connectToHost( settings.value( "ftp-server" ).toString(), 21 );
    else
        m_ftpConnection->connectToHost( url.host(), url.port( 21 ) );
    m_ftpConnection->login( settings.value( "ftp-user" ).toString(),
                            settings.value( "ftp-password" ).toString() );
    QString dir = settings.value( "ftp-dir" ).toString();
    if( !dir.isEmpty() )
        m_ftpConnection->cd( dir );
    scanFile( m_rootItem );
}

void FtpRepositoryModel::scanFile( FtpFileItem *fileItem )
{
    m_currentlyScannedFile = fileItem;
    m_ftpConnection->list( m_currentlyScannedFile->getFullPath() );
}

FtpFileItem* FtpRepositoryModel::findNextUnScannedItem(FtpFileItem *fi )
{
    if( !fi->isScanned() )
    {
        if( fi->urlInfo().isDir() )
            return fi;
        else
            fi->setScanned( true );
    }

    FtpFileItem* item;
    if( fi->getFirstChild() )
    {
        item = findNextUnScannedItem( fi->getFirstChild() );
        if( item && !item->isScanned() )
            return item;
    }
    if( fi->getNextSibling() )
    {
        item = findNextUnScannedItem( fi->getNextSibling() );
        if( item && !item->isScanned() )
            return item;
    }
    return 0;
}
