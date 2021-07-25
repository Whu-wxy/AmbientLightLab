#include "lightfilter.h"

lightfilter::lightfilter(int numTh)
{
    m_numTh = numTh;
    m_lastStable = -1;
}

#include <QDebug>
int lightfilter::stableLux(int lux)
{
    m_luxQue.enqueue(lux);
    if(m_luxQue.size() > m_numTh)
    {
        int stableLux = stablize(m_luxQue);

        //        qDebug()<<"stable1: "<<stableLux;
        if(m_lastStable == -1)
        {
            m_lastStable = stableLux;
            return m_lastStable;
        }
        else
        {
            int res = filt(m_lastStable, stableLux);
            //            qDebug()<<"stable2: "<<res;
            m_lastStable = res;
            return res;
        }
    }
    else
        return -1;
}


int lightfilter::stablize(QQueue<int>& que)
{
    int mean = 0;
    int count = 0;
    while(!que.empty())
    {
        int lux = que.head();
        mean += lux;
        count++;
        que.dequeue();
    }

    mean /= count;
    return mean;
}

int lightfilter::filt(int stableLux, int newLux)
{
    return stableLux * 0.2 + newLux * 0.8;
}
