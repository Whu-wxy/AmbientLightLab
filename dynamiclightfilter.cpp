#include "dynamiclightfilter.h"
#include <QDebug>
#include "lightchart.h"

dynamicLightFilter::dynamicLightFilter(int numTh, int stableMethod, int bLogFilt)
{
    m_numTh = numTh;
    m_lastStable = -1;
    m_methodName = "dynamicLightFilter";
    m_alpha = 0.7;
    m_stableMethod = stableMethod;
    m_smallFactor = 0.1;
    m_largeFactor = 0.25;
    m_largelargeFactor = 0.3;
    m_bLogFilt = bLogFilt;
}

dynamicLightFilter::dynamicLightFilter(LightChart* pChart, int numTh, int stableMethod, int bLogFilt)
{
    m_pOutChart = pChart;
    m_numTh = numTh;
    m_lastStable = -1;
    m_methodName = "dynamicLightFilter";
    m_alpha = 0.7;
    m_stableMethod = stableMethod;
    m_smallFactor = 0.1;
    m_largeFactor = 0.25;
    m_largelargeFactor = 0.3;
    m_bLogFilt = bLogFilt;
}


int dynamicLightFilter::stableLux(int lux)
{
    m_luxQue.enqueue(lux);
    if(m_luxQue.size() >= m_numTh)
    {
//        double std = log_StandardDeviation(m_luxQue);
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
//        qDebug()<<"dynamicLightFilter std: "<<std<<","<<"离散:"<<std / (meanVal+1);
//        if(m_pOutChart)
//        {
//            QString str = QString("std:%1--离散:%2").arg(std,0,'g',3).arg((std/(meanVal+1)),0,'g',3);    // --偏度:%4   .arg(sk,0,'g',3)
//            m_pOutChart->appendString(str);
//        }

        //        qDebug()<<"stable1: "<<stableLux;
        if(m_lastStable == -1)
        {
            m_lastStable = stableLux;
            return m_lastStable;
        }
        else
        {
            calNewAlpha(stableLux);
            if(m_pOutChart)
            {
                QString str = QString("alpha:%1").arg(m_alpha,0,'g',4);    // --偏度:%4   .arg(sk,0,'g',3)
                m_pOutChart->appendString(str);
            }
            int res = filt(m_lastStable, stableLux);
            //            qDebug()<<"stable2: "<<res;
            m_lastStable = res;
            return res;
        }
    }
    else
        return -1;
}


double dynamicLightFilter::stablize(QQueue<int>& que)
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
    qDebug()<<"stablize: "<<mean;

    return mean;
}

double dynamicLightFilter::WeightedMean(QQueue<int>& que)
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

double dynamicLightFilter::RMS(QQueue<int>& que)
{
    int mean = 0;
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

double dynamicLightFilter::Median(QQueue<int>& que)
{
//    vector<int> vec = que.toVector().toStdVector();
//    sort(vec.begin(), vec.end());

    QVector<int> vec = que.toVector();
    sort(vec.begin(), vec.end());

    int mid = 0;
    if(vec.size() % 2 != 0)
    {
        mid = vec[vec.size() / 2];
    }
    else
    {
        mid = (vec[vec.size() / 2] + vec[vec.size() / 2 - 1]) / 2;
    }
    que.clear();
    return mid;
}

void dynamicLightFilter::quartile(QQueue<int>& que, double& up, double& down)
{
    QVector<int> vec = que.toVector();
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

double dynamicLightFilter::medianMean(QQueue<int>& que)
{
    QVector<int> vec = que.toVector();
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
    qDebug()<<"medianMean: "<<meanVal;
    return meanVal;
}

void dynamicLightFilter::getMinMax(int cur, int &minV, int &maxV, int &minV2, int &maxV2)
{
    double log_cur = log(cur+1);
    double last_max = std::max(log(5.0), log_cur*(1+m_smallFactor));
    double last_min = std::max(0.0, log_cur*(1-m_smallFactor));
    double last_larger_max = std::max(log(10.0), log_cur*(1+m_largeFactor));
    double last_lower_min = std::max(0.0, log_cur*(1-m_largeFactor));
    minV = std::exp(last_min);
    minV2 = std::exp(last_lower_min);
    maxV = std::exp(last_max);
    maxV2 = std::exp(last_larger_max);
}

int dynamicLightFilter::filt(int stableLux, int newLux)
{
    if(m_bLogFilt)
        return static_cast<int>(exp(log(stableLux+1) * m_alpha + log(newLux+1) * (1-m_alpha)));
    else
        return static_cast<int>(stableLux * m_alpha + newLux * (1-m_alpha));
}

double dynamicLightFilter::calLogistic(double deltaLux, double a2)
{
    return a2*exp(0.01*deltaLux) / (1+ 0.7*(exp(0.01*deltaLux)-1));
}

void dynamicLightFilter::calNewAlpha(double cur)
{
    double logCur = log(cur+1);
    double log_last = log(m_lastStable+1);
    double last_max = std::max(log(5.0), log_last*(1+m_smallFactor));
    double last_min = std::max(0.0, log_last*(1-m_smallFactor));
    double last_larger_max = std::max(log(10.0), log_last*(1+m_largeFactor));
    double last_lower_min = std::max(0.0, log_last*(1-m_largeFactor));
    double last_largerlarger_max = std::max(log(10.0), log_last*(1+m_largelargeFactor));
    double last_lowerlower_min = std::max(0.0, log_last*(1-m_largelargeFactor));
//    qDebug()<<std::exp(last_lower_min)<<"--"<<std::exp(last_min)<<"--"<<cur<<"--"<<std::exp(last_max)<<"--"<<std::exp(last_larger_max);

//    double a1 = 0.5, a2 = 0, a3 = 0.8;
    double a1 = 0, a2 = 0.7, a3 = 0.8;

//    if(logCur < last_lower_min || logCur > last_larger_max) m_alpha = a2; // 超过最大
    if(logCur < last_lower_min) m_alpha = calLogistic(exp(logCur-last_lower_min), a2);
    else if(logCur > last_larger_max ) m_alpha = calLogistic(exp(last_larger_max-logCur), a2);
    else if(logCur <= last_max && logCur >= last_min) m_alpha = 1;   // 小范围
    else if(logCur > last_max && logCur <= last_larger_max) m_alpha = (a1-a2)*(logCur-last_larger_max)/(last_max-last_larger_max)+a2;//上中范围
    else if(logCur < last_min && logCur >= last_lower_min) m_alpha = (a1-a2)*(logCur-last_lower_min)/(last_min-last_lower_min)+a2; //下中范围


//    if(logCur <= last_max && logCur >= last_min) m_alpha = 1;   // 小范围
//    else if(logCur > last_max && logCur <= last_larger_max) m_alpha = (a1-a2)*(logCur-last_larger_max)/(last_max-last_larger_max)+a2; //上中范围
//    else if(logCur < last_min && logCur >= last_lower_min) m_alpha = (a1-a2)*(logCur-last_lower_min)/(last_min-last_lower_min)+a2; //下中范围
//    if(logCur < last_lowerlower_min || logCur > last_largerlarger_max) m_alpha = a3; // 超过最大
//    else if(logCur > last_larger_max && logCur <= last_largerlarger_max) m_alpha = (a3-a2)*(logCur-last_max)/(last_largerlarger_max-last_larger_max)+a2;
//    else if(logCur < last_lower_min && logCur >= last_lowerlower_min) m_alpha = (a3-a2)*(logCur-last_lower_min)/(last_lowerlower_min-last_lower_min)+a2;

}
