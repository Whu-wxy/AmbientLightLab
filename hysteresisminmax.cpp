#include "hysteresisminmax.h"

HysteresisMinMax::HysteresisMinMax(int numTh)
{
    m_numTh = numTh;
    m_lastStable = -1;
    m_previousNum = 8;
    m_methodName = "HysteresisMinMax";
}

#include <QDebug>
int HysteresisMinMax::stableLux(int lux)
{
    m_luxQue.enqueue(lux);
    if(m_luxQue.size() > m_numTh)
    {
        int minV = 0, maxV = 0;
        bool hasPrevious = calPrevisousMinMax(minV, maxV);
        int meanLux = stablize(m_luxQue);
//        qDebug()<<"hasPrevious:"<<hasPrevious<<"--minV:"<<minV<<"--meanLux:"<<meanLux<<"--maxV:"<<maxV;

        //        qDebug()<<"stable1: "<<stableLux;
        if(m_lastStable == -1)
        {
            m_lastStable = meanLux;
            return m_lastStable;
        }
        else
        {
            int res = filt(m_lastStable, meanLux);
            //            qDebug()<<"stable2: "<<res;

            if(hasPrevious && meanLux <= maxV && meanLux >= minV && m_lastStable != -1)
            {
                return m_lastStable;
            }

            m_lastStable = res;
            return res;
        }
    }
    else
        return -1;
}

bool HysteresisMinMax::calPrevisousMinMax(int &minV, int &maxV)
{
    if(m_previousQue.count() != m_previousNum) return false;

    int minVal = INT_MAX;
    int maxVal = -1;
    for(int i=0; i<m_previousNum; i++)
    {
        int lux = m_previousQue.at(i);
        minVal = min(lux, minVal);
        maxVal = max(lux, maxVal);
    }
    minV = minVal;
    maxV = maxVal;

    return true;
}

bool HysteresisMinMax::getPrevisousMinMax(int lux, int &minV, int &maxV)
{
    if(m_previousQue.count() < m_previousNum-1) return false;

    int minVal = INT_MAX;
    int maxVal = -1;
    int start = 0;
    if(m_previousQue.count() == m_previousNum)
        start = 1;
    for(int i=start; i<m_previousQue.count(); i++)
    {
        int lux = m_previousQue.at(i);
        minVal = min(lux, minVal);
        maxVal = max(lux, maxVal);
    }
    minVal = min(lux, minVal);
    maxVal = max(lux, maxVal);
    minV = minVal;
    maxV = maxVal;

    return true;
}


int HysteresisMinMax::stablize(QQueue<int>& que)
{
    int curMean = 0;
    int count = 0;
    while(!que.empty())
    {
        int lux = que.head();
        curMean += lux;
        count++;
        que.dequeue();

        m_previousQue.enqueue(lux);
        if(m_previousQue.count() > m_previousNum) m_previousQue.dequeue();
    }

    curMean /= count;
    return curMean;
}

int HysteresisMinMax::filt(int stableLux, int newLux)
{
    return stableLux * 0.2 + newLux * 0.8;
}
