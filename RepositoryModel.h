#ifndef RepositoryModel_H
#define RepositoryModel_H

#include "FileItem.h"

#include <QAbstractItemModel>
#include <QFileInfo>
#include <QFileSystemWatcher>
#include <QFtp>


class RepositoryModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    RepositoryModel();
    ~RepositoryModel();

    void addFile( const QFileInfo& file );
    void addFile( FileItem* file, FileItem* parent=0 );
    int columnCount( const QModelIndex & parent = QModelIndex() ) const;
    QVariant data( const QModelIndex & index, int role = Qt::DisplayRole ) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    QModelIndex	index ( int row, int column, const QModelIndex & parent = QModelIndex() ) const;
    QModelIndex	parent ( const QModelIndex & index ) const;
    int	rowCount( const QModelIndex & parent = QModelIndex() ) const;
    bool removeRow(int row, const QModelIndex &parent);

protected slots:
    void fileItemStatusChanged( FileItem::Status );
    void ftpStateChanged( int );
    void ftpFileListing( const QUrlInfo& urlinfo );
    void ftpCommandFinished( int, bool );

protected:
    void resetFtpConnection();
    void scanFile( FileItem* );

    FileItem*           m_rootItem;
    FileItem*           m_currentlyScanned;
    QFileSystemWatcher* m_fsWatcher;
    QFtp*               m_ftp;
};

#endif // RepositoryModel_H
