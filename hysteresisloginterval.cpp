#include "hysteresisloginterval.h"

HysteresisLogInterval::HysteresisLogInterval(int numTh)
{
    m_numTh = numTh;
    m_lastStable = -1;
    m_methodName = "HysteresisLogInterval";

}

HysteresisLogInterval::HysteresisLogInterval(LightChart* pChart, int numTh)
{
    m_pOutChart = pChart;
    m_numTh = numTh;
    m_lastStable = -1;
    m_methodName = "HysteresisLogInterval";
}

int HysteresisLogInterval::stableLux(int lux)
{
    m_luxQue.enqueue(lux);
    if(m_luxQue.size() >= m_numTh)
    {
        int meanVal = mean(m_luxQue);
        int stableLux = stablize(m_luxQue);
        if(lastMean == -1) lastMean = stableLux;
        else
        {
            qDebug()<<"last: "<<lastMean<<"---"<<"stableLux: "<<stableLux<<"---"<<"delta: "<<stableLux-lastMean;
        }

        //        qDebug()<<"stable1: "<<stableLux;
        if(m_lastStable == -1)
        {
            m_lastStable = stableLux;
            return m_lastStable;
        }
        else
        {
            double log_last = log(m_lastStable+1);
            double last_max = std::max(0.0, log_last*1.1);
            double last_min = std::max(log(10), log_last-log_last*0.1);

            int res = filt(m_lastStable, stableLux);
            QString str;
            if(m_pOutChart)
            {
                str = QString("min:%1--max:%2--").arg(last_min,0,'g',4).arg(last_max,0,'g',4);    // --偏度:%4   .arg(sk,0,'g',3)

            }

            if(log(res) <= last_max && log(res) >= last_min)
            {
                res = m_lastStable;
                str += "IN";
            }
            else
                str += "OUT";

            m_pOutChart->appendString(str);
            //            qDebug()<<"stable2: "<<res;
            m_lastStable = res;
            return res;
        }
    }
    else
        return -1;
}


int HysteresisLogInterval::stablize(QQueue<int>& que)
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

int HysteresisLogInterval::filt(int stableLux, int newLux)
{
    return stableLux * 0.3 + newLux * 0.7;
}
