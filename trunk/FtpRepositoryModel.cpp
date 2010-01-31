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
    QVariant v;
    if( role == Qt::DisplayRole )
        v = item->urlInfo().name();
    else if( role==Qt::DecorationRole )
        v = QIcon( ":/folder" );
    else if( role==Qt::ToolTipRole )
        v = "A tooltip stuffy .. ";
    else if( role == Qt::StatusTipRole )
        v = "A statustip stuffy .. ";
    else
        v = QVariant();
    //qDebug() << "Returning"<<v.toString()<<"from data()";
    return v;
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
    int rc = parentItem->getRowCount();
    //qDebug() << "Returning" << rc << "from rowCount";
    return rc;
}


void FtpRepositoryModel::ftpCommandFinished( int id, bool error )
{
    if( m_ftpConnection->currentCommand()==QFtp::List )
    {
        if( !error )
        {
            qDebug() << "Directory listing successful for file" << m_currentlyScannedFile->urlInfo().name();
            m_currentlyScannedFile->setScanned( true );
            m_currentlyScannedFile = findNextUnScannedItem( m_currentlyScannedFile );
            if( m_currentlyScannedFile )
                scanFile( m_currentlyScannedFile );
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
    qDebug() << "Got listing:" << urlinfo.name()<<". Adding as item"<<m_currentlyScannedFile->getRowCount() <<
            "to" << m_currentlyScannedFile->urlInfo().name();

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
    //m_ftpConnection->list();
    scanFile( m_rootItem );
}

void FtpRepositoryModel::scanFile( FtpFileItem *fileItem )
{
    qDebug() << "Starting scan of file" << fileItem->getFullPath();
    m_currentlyScannedFile = fileItem;
    m_ftpConnection->list( m_currentlyScannedFile->getFullPath() );
}

FtpFileItem* FtpRepositoryModel::findNextUnScannedItem(FtpFileItem *fi )
{
    if( fi!=0 && fi->isScanned() )
    {
        qDebug() << "findNextUnScannedItem: Testing" << fi->urlInfo().name() << "for scanned";
        FtpFileItem* tempitem = findNextUnScannedItem( fi->getFirstChild() );
        if( tempitem==0 || tempitem->isScanned() )
        {
            for( ; fi!=0; fi = fi->getNextSibling() )
            {
                tempitem = findNextUnScannedItem( fi->getNextSibling() );
                if( tempitem!= 0 && !tempitem->isScanned() )
                    return tempitem;
            }
        }
        else if( tempitem )
            return tempitem;
    }
    else
    {
        if( fi==0 )
            qDebug() << "findNextUnScannedItem: Item was 0 returning it";
        else
            qDebug() << "findNextUnScannedItem: Item" << fi->urlInfo().name() << "is unscanned, returning it";
        return fi;
    }
    return 0;
}
