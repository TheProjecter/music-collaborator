#ifndef FILEITEM_H
#define FILEITEM_H

#include <QFileInfo>
#include <QHash>
#include <QString>
#include <QUrlInfo>

/**
  * @brief Represents an Item Entity for the LocalRepositoryModel
  */
class FileItem : public QObject
{
    Q_OBJECT

public:
    enum Status {
        Unknown = 0,
        UpToDate = 1,
        LocallyModified = 2,
        LocallyAdded = 3,
        LocallyRemoved = 4,
        NeedsUpdate = 5,
        OnlyOnServer = 6,
        Error = 7
    };

    FileItem( FileItem* parent, const QFileInfo& path, int rowNumber );
    int getRowNumber() const { return m_rownumber; }
    int getRowCount() const;
    FileItem* child( int n );
    FileItem* child( const QString& name );
    FileItem* parent() { return m_parent; }

    void addChild( FileItem* child );
    QString getFileName() const;
    bool isFolder();

    //const QFileInfo& fileInfo() const { return m_fileInfo; }

    void updateRemoteStatus( const QUrlInfo& );

    QString getRepositoryPath() const;
    QString getLocalPath() const;

    Status status() const { return m_status; }
    void scanComplete();

    bool isScanned() const { return m_scanned; }
    void setScanned( bool scanned=true ) { m_scanned=scanned; }
    FileItem* getNextUnScanned();

signals:
    void statusChanged( FileItem::Status );

protected:
    void setStatus( Status );

    int                     m_rownumber;
    QFileInfo               m_fileInfo;
    QUrlInfo                m_remoteFileInfo;
    FileItem*               m_parent;
    QHash<int,FileItem*>    m_children;
    Status                  m_status;
    bool                    m_scanned;
};

#endif // FILEITEM_H
