#ifndef LOCALREPOSITORYMODEL_H
#define LOCALREPOSITORYMODEL_H

#include <QAbstractItemModel>
#include <QFileInfo>

class LocalRepositoryModel : public QAbstractItemModel
{
public:
    LocalRepositoryModel();
    void addFile( const QFileInfo& file );
    int columnCount( const QModelIndex & parent = QModelIndex() ) const;
    QVariant data( const QModelIndex & index, int role = Qt::DisplayRole ) const;
    QModelIndex	index ( int row, int column, const QModelIndex & parent = QModelIndex() ) const;
    QModelIndex	parent ( const QModelIndex & index ) const;
    int	rowCount ( const QModelIndex & parent = QModelIndex() ) const;
protected:
    QList<QFileInfo>    m_fileInfoList;
};

#endif // LOCALREPOSITORYMODEL_H
