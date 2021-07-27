#ifndef HYSTERESISMINMAX_H
#define HYSTERESISMINMAX_H

#include <QQueue>
#include "ilightfilter.h"
using namespace std;

class HysteresisMinMax : public ILightFilter
{
public:
    HysteresisMinMax(int numTh=4);
    int stableLux(int);

    bool getPrevisousMinMax(int lux, int &minV, int &maxV);

private:
    QQueue<int> m_luxQue;
    QQueue<int> m_previousQue;
    int m_previousNum;
    int m_lastStable = -1;

    int stablize(QQueue<int>& que);
    int filt(int, int);
    bool calPrevisousMinMax(int &minV, int &maxV);

};

#endif // HYSTERESISMINMAX_H
