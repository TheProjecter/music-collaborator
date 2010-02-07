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
        Unknown,
        UpToDate,
        LocallyModified,
        LocallyAdded,
        LocallyRemoved,
        NeedsUpdate,
        OnlyOnServer,
        Error
    };

    FileItem( FileItem* parent, const QFileInfo& path, int rowNumber );
    int getRowNumber() const { return m_rownumber; }
    int getRowCount() const;
    FileItem* child( int n );
    FileItem* parent() { return m_parent; }

    const QFileInfo& fileInfo() const { return m_fileInfo; }
    void addFile( const QFileInfo& );

signals:
    void statusChanged( Status );

protected:
    int                     m_rownumber;
    QFileInfo               m_fileInfo;
    QUrlInfo                m_remoteFileInfo;
    FileItem*               m_parent;
    QHash<int,FileItem*>    m_children;
    Status                  m_status;
};

#endif // FILEITEM_H
