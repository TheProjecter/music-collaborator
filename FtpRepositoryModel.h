#ifndef FTPREPOSITORYMODEL_H
#define FTPREPOSITORYMODEL_H

#include <QAbstractItemModel>
#include <QFtp>

class FtpFileItem;

class FtpRepositoryModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    FtpRepositoryModel();
    ~FtpRepositoryModel();

    int columnCount( const QModelIndex & parent = QModelIndex() ) const;
    QVariant data( const QModelIndex & index, int role = Qt::DisplayRole ) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    QModelIndex	index ( int row, int column, const QModelIndex & parent = QModelIndex() ) const;
    QModelIndex	parent ( const QModelIndex & index ) const;
    int	rowCount( const QModelIndex & parent = QModelIndex() ) const;

protected slots:
    void ftpCommandFinished( int id, bool error );
    void ftpFileListing( const QUrlInfo& urlinfo );

protected:
    void resetFtpConnection();
    void refreshFileListing();
    void scanFile( FtpFileItem* );
    FtpFileItem* findNextUnScannedItem( FtpFileItem* );

    FtpFileItem*        m_rootItem;
    FtpFileItem*        m_currentlyScannedFile;
    QFtp*               m_ftpConnection;
};

#endif // FTPREPOSITORYMODEL_H
