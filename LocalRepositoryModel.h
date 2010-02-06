#ifndef LOCALREPOSITORYMODEL_H
#define LOCALREPOSITORYMODEL_H

#include <QAbstractItemModel>
#include <QFileInfo>
#include <QFileSystemWatcher>

class FileItem;

class LocalRepositoryModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    LocalRepositoryModel();
    ~LocalRepositoryModel();

    void addFile( const QFileInfo& file );
    int columnCount( const QModelIndex & parent = QModelIndex() ) const;
    QVariant data( const QModelIndex & index, int role = Qt::DisplayRole ) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    QModelIndex	index ( int row, int column, const QModelIndex & parent = QModelIndex() ) const;
    QModelIndex	parent ( const QModelIndex & index ) const;
    int	rowCount( const QModelIndex & parent = QModelIndex() ) const;
    bool removeRow(int row, const QModelIndex &parent);

protected:
    FileItem*           m_rootItem;
    QFileSystemWatcher* m_fsWatcher;
};

#endif // LOCALREPOSITORYMODEL_H
