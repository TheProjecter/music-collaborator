#ifndef FILEITEM_H
#define FILEITEM_H

#include <QFileInfo>
#include <QHash>
#include <QString>

/**
  * @brief Represents an Item Entity for the LocalRepositoryModel
  */
class FileItem
{
public:
    FileItem( FileItem* parent, const QFileInfo& path, int rowNumber );
    int getRowNumber() const { return m_rownumber; }
    int getRowCount() const;
    FileItem* child( int n );
    FileItem* parent() { return m_parent; }
    //QString path();
    const QFileInfo& fileInfo() const { return m_fileInfo; }
    void addFile( const QFileInfo& );
protected:
    int                     m_rownumber;
    QFileInfo               m_fileInfo;
    FileItem*               m_parent;
    QHash<int,FileItem*>    m_children;
};

#endif // FILEITEM_H
