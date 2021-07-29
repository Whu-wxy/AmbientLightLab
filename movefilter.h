#ifndef movefilter_H
#define movefilter_H

#include <QQueue>
#include "ilightfilter.h"

using namespace std;

// 运动学
class movefilter: public ILightFilter
{
public:
    movefilter(int numTh=4);

    int stableLux(int);
    virtual void reset() {
                         m_luxQue.clear();
                         m_lastStable = -1;}
private:
    QQueue<int> m_luxQue;
    int m_lastStable = -1;
    int m_frictionFactor;


    int stablize(QQueue<int>& que);
    int filt(int, int);

};

#endif // movefilter_H
