#ifndef COMPRESSIONTASK_H
#define COMPRESSIONTASK_H

#include <QByteArray>
#include <QRunnable>

class CompressionTask : public QRunnable
{
public:
    CompressionTask( const QByteArray& uncompressed )
        : m_uncompressedData( uncompressed ) {}
    void run();
    const QByteArray& getCompressedData() const { return m_compressedData; }

protected:
    QByteArray  m_uncompressedData;
    QByteArray  m_compressedData;
};

#endif // COMPRESSIONTASK_H
