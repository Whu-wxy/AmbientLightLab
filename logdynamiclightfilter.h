#ifndef logdynamicLightFilter_H
#define logdynamicLightFilter_H

#include <QQueue>
#include <QtMath>
#include "ilightfilter.h"
#include "math.h"
#include "vector"
#include "algorithm"

using namespace std;
class LightChart;



// 一阶滞后滤波
class logDynamicLightFilter: public ILightFilter
{
public:
    logDynamicLightFilter(int numTh=4, int stableMethod=0, int bLogFilt=false);
    logDynamicLightFilter(LightChart* pChart, int numTh=4, int stableMethod=0, int bLogFilt=false);
    virtual ~logDynamicLightFilter(){}

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
            sigma += list.at(i);        // sum
        }
        sigma /= n;          // 获得平均值
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

    virtual void reset() {m_alpha = 0.7;
                         m_luxQue.clear();
                         m_lastStable = -1;}
    double WeightedMean(QQueue<double>& que);  //时序加权平均
    double RMS(QQueue<double>& que);   //均方根
    double Median(QQueue<double>& que);   //中位数
    void quartile(QQueue<double>& que, double& up, double& down);   //四分位数
    double medianMean(QQueue<double>& que);

    void getMinMax(int cur, int &minV, int &maxV, int &minV2, int &maxV2);

private:
    QQueue<double> m_luxQue;
    double m_lastStable = -1;
    double m_alpha;
    int m_stableMethod;
    LightChart* m_pOutChart = nullptr;
    double m_smallFactor;
    double m_largeFactor;
    bool m_bLogFilt;

    double stablize(QQueue<double>& que);
    double filt(double, double);
    void calNewAlpha(double cur);

};

#endif // dynamicLightFilter_H
