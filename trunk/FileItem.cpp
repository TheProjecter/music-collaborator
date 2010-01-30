#include "FileItem.h"

#include <QDebug>
#include <QDir>

FileItem::FileItem(FileItem *parent, const QFileInfo &fileInfo, int rowNumber)
    : m_rownumber( rowNumber ), m_fileInfo( fileInfo ), m_parent( parent )
{
    if( m_fileInfo.isDir() )
    {
        QDir dir( m_fileInfo.absoluteFilePath() );
        QFileInfoList lst = dir.entryInfoList( QDir::AllEntries | QDir::NoDotAndDotDot );
        qDebug() << "Scanning" << lst.size() << "subfiles of" << m_fileInfo.absoluteFilePath();
        for( int i=0; i<lst.size(); ++i )
        {
            qDebug() << "Creating fileitem nr"<<i<<"for path" << lst.at( i ).absoluteFilePath();
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


int FileItem::getRowCount() const
{
    return m_children.size();
}


void FileItem::addFile( const QFileInfo &fileinfo )
{
    FileItem* fi = new FileItem( this, fileinfo, m_children.size() );
    m_children[ m_children.size() ] = fi;
}
