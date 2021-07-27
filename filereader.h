#ifndef FILEREADER_H
#define FILEREADER_H

#include <QStringList>
#include <QFile>

class FileReader
{
public:
    FileReader(QString path);
    int getLux();
    bool hasData() {return (m_dataList.count()>0); }
    bool finished() {return (m_curIdx == m_dataList.count()); }
    void reset() {m_curIdx = 0; }

private:
    int m_curIdx;
    QStringList m_dataList;
};

#endif // FILEREADER_H
