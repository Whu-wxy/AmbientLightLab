#ifndef LIGHTFILTER_H
#define LIGHTFILTER_H

#include <QQueue>
#include "ilightfilter.h"

using namespace std;

// 一阶滞后滤波
class lightfilter: public ILightFilter
{
public:
    lightfilter(int numTh=4);

    int stableLux(int);
    virtual void reset() {
                         m_luxQue.clear();
                         m_lastStable = -1; }
private:
    QQueue<int> m_luxQue;
    int m_lastStable = -1;

    int stablize(QQueue<int>& que);
    int filt(int, int);

};

#endif // LIGHTFILTER_H
