#ifndef LOCALREPOSITORYMODEL_H
#define LOCALREPOSITORYMODEL_H

#include <QAbstractItemModel>
#include <QFileInfo>
#include <QStringList>

class FileItem {
public:
    FileItem( FileItem* parent, const QString& path, int rowNumber );
    int getRowNumber() const { return m_rownumber; }
    FileItem* child( int n );
    FileItem* parent() { return m_parent; }
    QString path();
    void addPath( const QString& );
protected:
    int                     m_rownumber;
    QFileInfo               m_fileInfo;
    FileItem*               m_parent;
    QHash<int,FileItem*>    m_children;
};

class LocalRepositoryModel : public QAbstractItemModel
{
public:
    LocalRepositoryModel();
    ~LocalRepositoryModel();

    void addFile( const QString& file );
    int columnCount( const QModelIndex & parent = QModelIndex() ) const;
    QVariant data( const QModelIndex & index, int role = Qt::DisplayRole ) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    QModelIndex	index ( int row, int column, const QModelIndex & parent = QModelIndex() ) const;
    QModelIndex	parent ( const QModelIndex & index ) const;
    int	rowCount ( const QModelIndex & parent = QModelIndex() ) const;

protected:
    FileItem*   m_rootItem;
    QStringList m_stringList;
};

#endif // LOCALREPOSITORYMODEL_H
