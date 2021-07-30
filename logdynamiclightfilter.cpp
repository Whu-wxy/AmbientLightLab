#include "logDynamicLightFilter.h"
#include <QDebug>
#include "lightchart.h"

logDynamicLightFilter::logDynamicLightFilter(int numTh, int stableMethod, int bLogFilt)
{
    m_numTh = numTh;
    m_lastStable = -1;
    m_methodName = "logDynamicLightFilter";
    m_alpha = 0.7;
    m_stableMethod = stableMethod;
    m_smallFactor = 0.05;
    m_largeFactor = 0.15;
    m_bLogFilt = bLogFilt;
}

logDynamicLightFilter::logDynamicLightFilter(LightChart* pChart, int numTh, int stableMethod, int bLogFilt)
{
    m_pOutChart = pChart;
    m_numTh = numTh;
    m_lastStable = -1;
    m_methodName = "logDynamicLightFilter";
    m_alpha = 0.7;
    m_stableMethod = stableMethod;
    m_smallFactor = 0.07;
    m_largeFactor = 0.15;
    m_bLogFilt = bLogFilt;
}


int logDynamicLightFilter::stableLux(int lux)
{
    m_luxQue.enqueue(log(lux+1));
    if(m_luxQue.size() >= m_numTh)
    {
        double stableLux = 0;
        if(m_stableMethod == 0)
            stableLux = stablize(m_luxQue);
        else if(m_stableMethod == 1)
            stableLux = WeightedMean(m_luxQue);
        else if(m_stableMethod == 2)
            stableLux = RMS(m_luxQue);
        else if(m_stableMethod == 3)
            stableLux = Median(m_luxQue);
        else if(m_stableMethod == 4)
            stableLux = medianMean(m_luxQue);

//        double sk = skewness(m_luxQue);
//        int stableLux = stablize(m_luxQue);
        // 离散系数
//        qDebug()<<"logDynamicLightFilter std: "<<std<<","<<"离散:"<<std / (meanVal+1);
//        if(m_pOutChart)
//        {
//            QString str = QString("std:%1--离散:%2").arg(std,0,'g',3).arg((std/(meanVal+1)),0,'g',3);    // --偏度:%4   .arg(sk,0,'g',3)
//            m_pOutChart->appendString(str);
//        }

        //        qDebug()<<"stable1: "<<stableLux;
        if(m_lastStable < 0)
        {
            m_lastStable = stableLux;
            return exp(m_lastStable)-1;
        }
        else
        {
            calNewAlpha(stableLux);
            if(m_pOutChart)
            {
                QString str = QString("alpha:%1").arg(m_alpha,0,'g',4);    // --偏度:%4   .arg(sk,0,'g',3)
                m_pOutChart->appendString(str);
            }
            double res = filt(m_lastStable, stableLux);
            //            qDebug()<<"stable2: "<<res;
            m_lastStable = res-1;
            return exp(res)-1;
        }
    }
    else
        return -1;
}


double logDynamicLightFilter::stablize(QQueue<double>& que)
{
    double mean = 0;
    int count = 0;
    while(!que.empty())
    {
        int lux = que.head();
        mean += lux;
        count++;
        que.dequeue();
    }

    mean /= count;
    qDebug()<<"logDynamicLightFilterstablize: "<<exp(mean);

    return mean;
}

double logDynamicLightFilter::WeightedMean(QQueue<double>& que)
{
    double mean = 0;
    int weight = 1;
    int all = 0;
    while(!que.empty())
    {
        int lux = que.head();
        mean = mean + lux*weight;
        all += weight;
        weight++;
        que.dequeue();
    }

    mean /= all;
    return mean;
}

double logDynamicLightFilter::RMS(QQueue<double>& que)
{
    double mean = 0;
    int count = 0;
    while(!que.empty())
    {
        int lux = que.head();
        mean = mean + lux*lux;
        count++;
        que.dequeue();
    }

    mean /= count;
    return sqrt(mean);
}

double logDynamicLightFilter::Median(QQueue<double>& que)
{
//    vector<int> vec = que.toVector().toStdVector();
//    sort(vec.begin(), vec.end());

    QVector<double> vec = que.toVector();
    sort(vec.begin(), vec.end());

    double mid = 0;
    if(vec.size() % 2 != 0)
    {
        mid = vec[vec.size() / 2];
    }
    else
    {
        mid = (vec[vec.size() / 2] + vec[vec.size() / 2 - 1]) / 2;
    }
    que.clear();
    qDebug()<<"logDynamicLightFilterstablize: "<<exp(mid);

    return mid;
}

void logDynamicLightFilter::quartile(QQueue<double>& que, double& up, double& down)
{
    QVector<double> vec = que.toVector();
    sort(vec.begin(), vec.end());

    // 中位数和相应的索引
    int mid, mid1, mid3;
    int size = vec.count();

    mid = size / 2;
    mid1 = size % 2 == 0 ? (mid-1)/2 : mid/2;
    mid3 = mid+mid1+1;
    if (size%2 != 0 )
    { // 整数集是奇数的情况
        down = mid % 2 == 0 ? (vec[mid1] + vec[mid1-1]) / 2.0
                : vec[mid1];
        up = mid % 2 == 0 ? (vec[mid3] + vec[mid3-1]) / 2.0
                : vec[mid3];
    }
    else
    { // 整数集偶数的情况
        down = (mid-1) % 2 == 0 ? (vec[mid1] + vec[mid1-1]) / 2.0
                : vec[mid1];
        up = (mid-1) % 2 == 0 ? (vec[mid3] + vec[mid3-1]) / 2.0
                : vec[mid3];
    }
}

double logDynamicLightFilter::medianMean(QQueue<double>& que)
{
    QVector<double> vec = que.toVector();
    que.clear();
    sort(vec.begin(), vec.end());

    int start = 0, end = vec.count();
    if(vec.count() > 2)
    {
        start = 1;
        end = vec.count()-1;
    }

    double meanVal = 0.0;
    for(int i=start; i<end; i++)
    {
        meanVal += vec.at(i);
    }
    meanVal = meanVal / (end-start);
    qDebug()<<"logDynamicLightFilter medianMean: "<<exp(meanVal);
    return meanVal;
}

void logDynamicLightFilter::getMinMax(int cur, int &minV, int &maxV, int &minV2, int &maxV2)
{
    double log_cur = cur;
    double last_max = std::max(log(5.0), log_cur*(1+m_smallFactor));
    double last_min = std::max(0.0, log_cur*(1-m_smallFactor));
    double last_larger_max = std::max(log(10.0), log_cur*(1+m_largeFactor));
    double last_lower_min = std::max(0.0, log_cur*(1-m_largeFactor));
    minV = std::exp(last_min);
    minV2 = std::exp(last_lower_min);
    maxV = std::exp(last_max);
    maxV2 = std::exp(last_larger_max);
}

double logDynamicLightFilter::filt(double stableLux, double newLux)
{
    return stableLux * m_alpha + newLux * (1-m_alpha);
}

void logDynamicLightFilter::calNewAlpha(double cur)
{
    double logCur = cur;
    double log_last = m_lastStable;
    double last_max = std::max(log(5.0), log_last*(1+m_smallFactor));
    double last_min = std::max(0.0, log_last*(1-m_smallFactor));
    double last_larger_max = std::max(log(10.0), log_last*(1+m_largeFactor));
    double last_lower_min = std::max(0.0, log_last*(1-m_largeFactor));
//    qDebug()<<std::exp(last_lower_min)<<"--"<<std::exp(last_min)<<"--"<<cur<<"--"<<std::exp(last_max)<<"--"<<std::exp(last_larger_max);

    if(logCur < last_lower_min || logCur > last_larger_max) m_alpha = 0.7;
    else if(logCur <= last_max && logCur >= last_min) m_alpha = 1;
//    else if(logCur > last_max && logCur <= last_larger_max) m_alpha = (0.6-0.3)*(logCur-last_larger_max)/(last_max-last_larger_max)+0.3;
//    else if(logCur < last_min && logCur >= last_lower_min) m_alpha = (0.6-0.3)*(logCur-last_lower_min)/(last_min-last_lower_min)+0.3;
}
