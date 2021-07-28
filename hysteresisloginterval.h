#ifndef HYSTERESISLOGINTERVAL_H
#define HYSTERESISLOGINTERVAL_H

#include <QQueue>
#include "lightchart.h"
#include <math.h>
#include "ilightfilter.h"
using namespace std;

class HysteresisLogInterval : public ILightFilter
{
public:
    HysteresisLogInterval(int numTh=4);
    HysteresisLogInterval(LightChart* pChart, int numTh=4);
    virtual ~HysteresisLogInterval(){}

    int stableLux(int);

    double mean(QQueue<int> list ){
        if(list.count() == 0) return 0;

        int n = list.count();
        double sigma = 0;
        for (int i = 0; i < n; ++i) {
            int v = list.at(i);
            sigma += v;        // sum
        }
        sigma /= list.count ();          // 获得平均值
        return sigma;
    }

private:
    QQueue<int> m_luxQue;
    int m_lastStable = -1;
    LightChart* m_pOutChart = nullptr;
    int lastMean = -1;

    int stablize(QQueue<int>& que);
    int filt(int, int);
};

#endif // HYSTERESISLOGINTERVAL_H
