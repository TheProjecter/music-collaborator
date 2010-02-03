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
    bool isScanned() const { return m_scanned; }
    QString getFullPath();
    FtpFileItem* getNextSibling();
    FtpFileItem* getFirstChild() { if( m_children.size()>0 ) return m_children[0]; return 0; }
    void setScanned( bool scanned ) { m_scanned=scanned; }

protected:
    int                     m_rownumber;
    QUrlInfo                m_urlInfo;
    FtpFileItem*            m_parent;
    QHash<int,FtpFileItem*> m_children;
    bool                    m_scanned;
};

#endif // FTPFtpFileItem_H
