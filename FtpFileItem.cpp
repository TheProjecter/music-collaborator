#include "FtpFileItem.h"

FtpFileItem::FtpFileItem( FtpFileItem *parent, const QUrlInfo &info, int rowNumber )
    : m_rownumber( rowNumber ), m_urlInfo( info ), m_parent( parent )
{
}

FtpFileItem* FtpFileItem::child(int n)
{
    if( m_urlInfo.isValid() && m_urlInfo.isDir() )
    {
    }
    return 0;
}

void FtpFileItem::addFile(const QUrlInfo &urlInfo )
{
    FtpFileItem* fi = new FtpFileItem( this, urlInfo, m_children.size() );
    m_children[ m_children.size() ] = fi;
}
