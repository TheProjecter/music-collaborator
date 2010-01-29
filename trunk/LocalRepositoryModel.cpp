#include "LocalRepositoryModel.h"

#include <QSettings>

LocalRepositoryModel::LocalRepositoryModel()
{
}

LocalRepositoryModel::~LocalRepositoryModel()
{
    QSettings settings;
}

void LocalRepositoryModel::addFile( const QString &file )
{
    emit beginInsertRows( QModelIndex(), 0, m_fileInfoList.size()+1 );
    m_fileInfoList.append( file );
    emit endInsertRows();
}

int LocalRepositoryModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}

QVariant LocalRepositoryModel::data(const QModelIndex &index, int role) const
{
    if( index.column()==0 && index.row()<m_fileInfoList.size() )
        return m_fileInfoList.at( index.row() ).absoluteFilePath();
    return QVariant();
}

QModelIndex LocalRepositoryModel::index(int row, int column, const QModelIndex &parent) const
{
    return QModelIndex();
}

QModelIndex LocalRepositoryModel::parent(const QModelIndex &index) const
{
    return QModelIndex();
}

int LocalRepositoryModel::rowCount(const QModelIndex &parent) const
{
    return m_fileInfoList.size();
}
