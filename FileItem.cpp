#include "FileItem.h"

#include <QDebug>
#include <QDir>

FileItem::FileItem( FileItem *parent, const QFileInfo &fileInfo, int rowNumber )
    : m_rownumber( rowNumber ), m_fileInfo( fileInfo ), m_parent( parent ),
    m_status( Unknown ), m_scanned( false )
{
    if( m_fileInfo.isDir() )
    {
        QDir dir( m_fileInfo.absoluteFilePath() );
        QFileInfoList lst = dir.entryInfoList( QDir::AllEntries | QDir::NoDotAndDotDot );
        for( int i=0; i<lst.size(); ++i )
        {
            FileItem* fi = new FileItem( this, lst[i].absoluteFilePath(), i );
            m_children[i] = fi;
        }
    }
}



FileItem* FileItem::child( int n )
{
    if( m_children.contains( n ) )
        return m_children[n];

    if( m_fileInfo.isDir() )
    {
        QStringList lst = m_fileInfo.dir().entryList( QDir::AllEntries | QDir::NoDotAndDotDot );
        if( n < lst.size() )
        {
            FileItem* item = new FileItem( this, lst[n], n );
            m_children[n] = item;
            return item;
        }
    }
    return 0;
}



FileItem* FileItem::child( const QString& name )
{
    for( int i=0; i<m_children.count(); ++i )
    {
        FileItem* child = m_children[i];
        if( child->getFileName() == name )
            return child;
    }
    return 0;
}


int FileItem::getRowCount() const
{
    return m_children.size();
}


QString FileItem::getFileName() const
{
    QString fname = m_fileInfo.fileName();
    if( fname.isEmpty() )
        fname = m_remoteFileInfo.name();
    return fname;
}


bool FileItem::isFolder()
{
    if( m_fileInfo.filePath().isEmpty() )
        return m_remoteFileInfo.isDir();
    else
        return m_fileInfo.isDir();
    return false;
}


void FileItem::addChild( FileItem *child )
{
    child->setParent( this );
    child->m_parent = this;
    m_children[ m_children.count() ] = child;
    child->m_rownumber = m_children.count()-1;
}





QString FileItem::getRepositoryPath() const
{
    QString path = getFileName();
    FileItem* p = m_parent;
    while( p !=0 )
    {
        QString pPath = p->getFileName();
        if( !pPath.isEmpty() )
            path = QString( "%1/%2" ).arg( pPath ).arg( path );
        p = p->parent();
    }
    return path;
}


QString FileItem::getLocalPath() const
{
    return m_fileInfo.absoluteFilePath();
}


FileItem* FileItem::getNextUnScanned()
{
    FileItem* unScanned = 0;
    if( !isScanned() )
        unScanned = this;

    for( int i=0; i<getRowCount() && unScanned==0; ++i )
        unScanned = m_children[i]->getNextUnScanned();

    return unScanned;
}


void FileItem::scanComplete()
{
    setScanned( true );
    if( m_remoteFileInfo.name().isEmpty() )
    {
        if( !m_fileInfo.absoluteFilePath().isEmpty() )
            setStatus( LocallyAdded );
        else
            setStatus( Error );
    }
    else
    {
        if( m_fileInfo.absoluteFilePath().isEmpty() )
            setStatus( OnlyOnServer );
        else
            setStatus( Error );
    }
}


void FileItem::updateRemoteStatus( const QUrlInfo &urlinfo )
{
    m_remoteFileInfo = urlinfo;
    if( !urlinfo.isDir() )
        scanComplete();
}


void FileItem::setStatus( Status s )
{
    m_status = s;
    qDebug() << "FileItem status of" << getRepositoryPath() << "set to" << s;
    emit statusChanged( m_status );
}
