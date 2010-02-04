#include "CompressionTask.h"

void CompressionTask::run()
{
    m_compressedData = qCompress( m_uncompressedData, 9 );
}
