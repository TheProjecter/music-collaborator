#ifndef FTPFtpFileItem_H
#define FTPFtpFileItem_H

#include <QUrlInfo>
#include <QHash>

class FtpFileItem
{
public:
    FtpFileItem( FtpFileItem* parent, const QUrlInfo& info, int rowNumber );
    int getRowNumber() const { return m_rownumber; }
    int getRowCount() const { return m_children.size(); }
    FtpFileItem* child( int n );
    FtpFileItem* parent() { return m_parent; }
    const QUrlInfo& urlInfo() const { return m_urlInfo; }
    void addFile( const QUrlInfo& );
protected:
    int                     m_rownumber;
    QUrlInfo                m_urlInfo;
    FtpFileItem*            m_parent;
    QHash<int,FtpFileItem*> m_children;
};

#endif // FTPFtpFileItem_H
