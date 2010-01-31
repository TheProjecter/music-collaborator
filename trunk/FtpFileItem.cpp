#include "FtpFileItem.h"

FtpFileItem::FtpFileItem( FtpFileItem *parent, const QUrlInfo &info, int rowNumber )
    : m_rownumber( rowNumber ), m_urlInfo( info ), m_parent( parent ), m_scanned( false )
{
}

FtpFileItem* FtpFileItem::child(int n)
{
    if( m_children.size() > n )
    {
        return m_children[n];
    }
    return 0;
}

void FtpFileItem::addFile(const QUrlInfo &urlInfo )
{
    FtpFileItem* fi = new FtpFileItem( this, urlInfo, m_children.size() );
    m_children[ m_children.size() ] = fi;
}


QString FtpFileItem::getFullPath()
{
    QString path = m_urlInfo.name();
    FtpFileItem* p = m_parent;
    while( p !=0 )
    {
        QString pPath = p->urlInfo().name();
        if( !pPath.isEmpty() )
            path = QString( "%1/%2" ).arg( p->urlInfo().name() ).arg( path );
        p = p->parent();
    }
    return path;
}


FtpFileItem* FtpFileItem::getNextSibling()
{
    if( parent()!=0 )
    {
        return parent()->child( m_rownumber+1 );
    }
    return 0;
}

