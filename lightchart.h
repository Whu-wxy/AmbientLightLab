#ifndef LightChart_H
#define LightChart_H

#include <iostream>
#include <QDebug>
#include <algorithm>
#include <QList>
#include <QQueue>

#include "ilightfilter.h"
#include "lightfilter.h"

#include <QDialog>
#include <QTime>
#include <QLightSensor>
#include <QLightReading>

//QChart
#include <QtCharts>
using namespace QtCharts;
using namespace std;

#define MA

class LightChart : public QChartView
{
    Q_OBJECT

public:
    LightChart(QWidget *parent = nullptr);
    ~LightChart();

    QString exportData();
    void clear();
    void setQueueLimit(int num) {m_filter->setQueueLimit(num);
                                    #ifdef MA
                                    m_filter->setQueueLimit(num*2);
                                    #endif
                                    }
    int getQueueLimit() {return m_filter->getQueueLimit();}
    void setMethod(ILightFilter* filter);
    void appendString(QString);
#ifdef MA
    void setMethodMA(ILightFilter* filter);
#endif

protected slots:
    void onDataReach();

private:
    ILightFilter* m_filter;
    //绘图定时器
    QLightSensor* m_lightSensor;
    QTime m_curTime;
    QTime m_lastTime;

    //曲线
    QSplineSeries* line;
    QScatterSeries*  m_xySeries;
    //曲线点的最大数量
    int line_max = 100;

    //绘图变量和坐标
    QChart* chart;
    QString m_appendTitle = "";
    QValueAxis *axisX;
    QValueAxis *axisY;


    void setupChart();

    // stable
    QQueue<int>  m_allLux;
    int m_lastStable = -1;
    int m_lastDelta = 0;
    QSplineSeries* stableline;
    QScatterSeries*  m_stableXYSeries;
    int stablize(int);

#ifdef MA
    ILightFilter* m_filter2;
    int m_lastStable2 = -1;
    QSplineSeries* stableline2;
    QScatterSeries*  m_stableXYSeries2;
    int stablize2(int);
#endif

};

#endif // LightChart_H
