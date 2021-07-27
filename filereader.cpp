#include "filereader.h"

FileReader::FileReader(QString path)
{
    m_curIdx = 0;

    QFile file(path);
    if(!file.exists()) return;

    if(file.open(QIODevice::ReadOnly))
    {
        QString data = QString::fromUtf8(file.readAll());
        m_dataList = data.split(",");
    }
}

int FileReader::getLux()
{
    if(m_curIdx >= m_dataList.count()) return -1;

    return m_dataList.at(m_curIdx++).toInt();
}
