#include "dynamicLightFilter.h"
#include <QDebug>

dynamicLightFilter::dynamicLightFilter(int numTh)
{
    m_numTh = numTh;
    m_lastStable = -1;
}

int dynamicLightFilter::stableLux(int lux)
{
    m_luxQue.enqueue(lux);
    if(m_luxQue.size() > m_numTh)
    {
        double std = StandardDeviation(m_luxQue);
        double meanVal = mean(m_luxQue);
        double sk = skewness(m_luxQue);
        int stableLux = stablize(m_luxQue);
        // 离散系数
        qDebug()<<"dynamicLightFilter std: "<<std<<","<<"log std:"<<log(std)<<","<<"离散:"<<std / (meanVal+1);

        if(m_pOutChart)
        {
            QString str = QString("std:%1--log:%2--离散:%3--偏度:%4").arg(std,0,'g',3).arg(log(std),0,'g',3)
                    .arg((std/(meanVal+1)),0,'g',3).arg(sk,0,'g',3);
            m_pOutChart->appendString(str);
        }
//        return std;

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


int dynamicLightFilter::stablize(QQueue<int>& que)
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

int dynamicLightFilter::filt(int stableLux, int newLux)
{
    return stableLux * 0.2 + newLux * 0.8;
}
