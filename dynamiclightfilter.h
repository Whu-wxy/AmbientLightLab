#ifndef dynamicLightFilter_H
#define dynamicLightFilter_H

#include <QQueue>
#include "ilightfilter.h"
#include "lightchart.h"

using namespace std;

template <typename T>
double stats_skewness(T data[], int n)
{
    if (n <= 0)
    {
        std::cerr << "wrong parameter!" << std::endl;
        return -1;
    }

    double mean = 0.0;
    for (int i = 0; i < n; i++)
        mean += data[i];
    mean /= n;

    double variance = 0.0;
    for (int i = 0; i < n; i++)
        variance += pow(data[i] - mean, 2);
    variance /= n;
    double sigma = sqrt(variance);

    double skewness = 0.0;
    for (int i = 0; i < n; i++)
        skewness += pow((data[i] - mean) / sigma, 3);
    skewness /= n;

    return skewness;
}


// 一阶滞后滤波
class dynamicLightFilter: public ILightFilter
{
public:
    dynamicLightFilter(int numTh=4);
    dynamicLightFilter(LightChart* pChart, int numTh=4) {m_pOutChart = pChart; }

    int stableLux(int);
    double StandardDeviation(QQueue<int> list ){
        if(list.count() == 0) return 0;

        int n = list.count();
        double sigma = 0, pSigma = 0;
        for (int i = 0; i < n; ++i) {
            int v = list.at(i);
            sigma += v;        // sum
            pSigma += v*v;     // 平方和
        }
        sigma /= list.count ();          // 获得平均值
        return qSqrt((pSigma/n - sigma*sigma)) ;
    }

    double log_StandardDeviation(QQueue<int> list ){
        if(list.count() == 0) return 0;

        int n = list.count();
        double sigma = 0, pSigma = 0;
        for (int i = 0; i < n; ++i) {
            int v = list.at(i);
            double logv = log(v+1);   // log为e为底，   log10（）
            sigma += logv;        // sum
            pSigma += logv*logv;     // 平方和
        }
        sigma /= list.count ();          // 获得平均值
        return qSqrt((pSigma/n - sigma*sigma)) ;
    }


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

    double log_mean(QQueue<int> list ){
        if(list.count() == 0) return 0;

        int n = list.count();
        double sigma = 0;
        for (int i = 0; i < n; ++i) {
            int v = list.at(i);
            sigma += log(v);        // sum
        }
        sigma /= list.count ();          // 获得平均值
        return sigma;
    }

    double skewness(QQueue<int> data)
    {
        int n = data.count();
        if (n <= 0)
        {
            std::cerr << "skewness wrong parameter!" << std::endl;
            return 0;
        }

        double mean = 0.0;
        for (int i = 0; i < n; i++)
            mean += data[i];
        mean /= n;

        double variance = 0.0;
        for (int i = 0; i < n; i++)
            variance += pow(data[i] - mean, 2);
        variance /= n;
        double sigma = sqrt(variance);

        double skewness = 0.0;
        for (int i = 0; i < n; i++)
            skewness += pow((data[i] - mean) / sigma, 3);
        skewness /= n;

        return skewness;
    }

private:
    QQueue<int> m_luxQue;
    int m_lastStable = -1;
    LightChart* m_pOutChart = nullptr;


    int stablize(QQueue<int>& que);
    int filt(int, int);

};

#endif // dynamicLightFilter_H
