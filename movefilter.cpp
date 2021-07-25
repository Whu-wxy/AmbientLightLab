#include "movefilter.h"
#include <QDebug>

movefilter::movefilter(int numTh)
{
    m_numTh = numTh;
    m_lastStable = -1;

    m_frictionFactor = 50;
}

int movefilter::stableLux(int lux)
{
    m_luxQue.push(lux);
    if(m_luxQue.size() >= m_numTh)
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


int movefilter::stablize(queue<int>& que)
{
    int mean = 0;
    int count = 0;
    while(!que.empty())
    {
        int lux = que.front();
        mean += lux;
        count++;
        que.pop();
    }

    mean /= count;
    return mean;
}

int movefilter::filt(int stableLux, int newLux)
{
    int F = newLux - stableLux;
    if(F >= 0)
        F = max(0, F - m_frictionFactor);
    else
        F = min(0, F + m_frictionFactor);

    // v = v0 + at
    qDebug()<<"F: "<<F<<" , "<<"movefilter: "<<stableLux + F;
    return stableLux + F;
}
