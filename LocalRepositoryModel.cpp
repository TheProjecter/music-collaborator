#include "LocalRepositoryModel.h"

#include <QDir>
#include <QSettings>



FileItem::FileItem(FileItem *parent, const QString &path, int rowNumber)
    : m_rownumber( rowNumber ), m_parent( parent )
{
    m_fileInfo = QFileInfo( path );
}

QString FileItem::path()
{
    return m_fileInfo.absoluteFilePath();
}


FileItem* FileItem::child(int n)
{
    if( m_children.contains( n ) )
        return m_children[n];

    if( m_fileInfo.isDir() )
    {
        QStringList lst = m_fileInfo.dir().entryList( QDir::AllEntries );
        if( n < lst.size() )
        {
            FileItem* item = new FileItem( this, lst[n], n );
            m_children[n] = item;
            return item;
        }
    }
    return 0;
}


void FileItem::addPath(const QString &path )
{
    FileItem* fi = new FileItem( this, path, m_children.size() );
    m_children[ m_children.size() ] = fi;
}



LocalRepositoryModel::LocalRepositoryModel()
{
    QSettings settings;
    m_rootItem = new FileItem( 0, "", 0 );

    unsigned int count = settings.beginReadArray( "local-repository" );
    for( unsigned int i=0; i<count; ++i )
    {
        settings.setArrayIndex( i );
        m_rootItem->addPath( settings.value( "path" ).toString() );
        m_stringList.append( settings.value( "path" ).toString() );
    }
}

LocalRepositoryModel::~LocalRepositoryModel()
{
    QSettings settings;
    settings.beginWriteArray( "local-repository", m_stringList.size() );
    for( int i=0; i<m_stringList.size(); ++i )
    {
        settings.setArrayIndex( i );
        settings.setValue( "path", m_stringList.at( i ) );
    }
    settings.endArray();
}


void LocalRepositoryModel::addFile( const QString &file )
{
    emit beginInsertRows( QModelIndex(), 0, m_stringList.size()+1 );
    m_stringList.append( file );
    m_rootItem->addPath( file );
    emit endInsertRows();
}


QVariant LocalRepositoryModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if( orientation == Qt::Horizontal && section==0 )
        return QString( "Local Project" );
    return QVariant();
}


int LocalRepositoryModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}

QVariant LocalRepositoryModel::data(const QModelIndex &index, int role) const
{
    if( index.column()==0 && index.row()<m_stringList.size() )
        return m_stringList.at( index.row() );
    return QVariant();
}

QModelIndex LocalRepositoryModel::index( int row, int column, const QModelIndex &parent ) const
{
    if( column==0 && row<m_stringList.count() )
    return QModelIndex();
}

QModelIndex LocalRepositoryModel::parent(const QModelIndex &index) const
{
    return QModelIndex();
}

int LocalRepositoryModel::rowCount(const QModelIndex &parent) const
{
    return m_stringList.size();
}
