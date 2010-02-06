#include "LocalRepositoryModel.h"
#include "FileItem.h"

#include <QDebug>
#include <QDir>
#include <QIcon>
#include <QSettings>
#include <QString>


LocalRepositoryModel::LocalRepositoryModel()
{
    QSettings settings;
    m_rootItem = new FileItem( 0, QFileInfo(), 0 );
    m_fsWatcher = new QFileSystemWatcher( this );

    unsigned int count = settings.beginReadArray( "local-repository" );
    for( unsigned int i=0; i<count; ++i )
    {
        settings.setArrayIndex( i );
        QString path = settings.value( "path" ).toString();
        m_rootItem->addFile( path );
        m_fsWatcher->addPath( path );
    }
}


LocalRepositoryModel::~LocalRepositoryModel()
{
    QSettings settings;
    settings.beginWriteArray( "local-repository", m_rootItem->getRowCount() );
    for( int i=0; i<m_rootItem->getRowCount(); ++i )
    {
        settings.setArrayIndex( i );
        settings.setValue( "path", m_rootItem->child( i )->fileInfo().absoluteFilePath() );
    }
    settings.endArray();
}


void LocalRepositoryModel::addFile( const QFileInfo &file )
{
    emit beginInsertRows( QModelIndex(), m_rootItem->getRowCount(),
                          m_rootItem->getRowCount() );
    m_rootItem->addFile( file );
    emit endInsertRows();
}


QVariant LocalRepositoryModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if( orientation == Qt::Horizontal && role==Qt::DisplayRole && section==0 )
        return QString( "Local Files" );
    return QVariant();
}


int LocalRepositoryModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}

QVariant LocalRepositoryModel::data(const QModelIndex &index, int role) const
{
    if( !index.isValid() )
        return QVariant();

    FileItem* item = static_cast<FileItem*>( index.internalPointer() );
    if( role == Qt::DisplayRole )
        return item->fileInfo().fileName();
    else if( role==Qt::DecorationRole )
    {
        if( item->fileInfo().isDir() )
            return QIcon( ":/Icons/folder" );
        else
            return QIcon( ":/Icons/audiofile");
    }
    else if( role==Qt::ToolTipRole )
        return "A tooltip stuffy .. ";
    else if( role == Qt::StatusTipRole )
        return "A statustip stuffy .. ";
    else
        return QVariant();
}

QModelIndex LocalRepositoryModel::index( int row, int column, const QModelIndex &parent ) const
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


QModelIndex LocalRepositoryModel::parent(const QModelIndex &child) const
{
    if( !child.isValid() )
        return QModelIndex();
    FileItem* childItem = static_cast<FileItem*>( child.internalPointer() );
    FileItem* parentItem = childItem->parent();
    if( parentItem==0 || parentItem == m_rootItem )
        return QModelIndex();
    return createIndex( parentItem->getRowNumber(), 0, parentItem );
}


int LocalRepositoryModel::rowCount(const QModelIndex &parent) const
{
    FileItem* parentItem;
    if( !parent.isValid() )
        parentItem = m_rootItem;
    else
        parentItem = static_cast<FileItem*>( parent.internalPointer() );
    return parentItem->getRowCount();
}
