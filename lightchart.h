#ifndef LightChart_H
#define LightChart_H

#include <iostream>
#include <QDebug>
#include <algorithm>
#include <QList>
#include <QQueue>

#include "ilightfilter.h"
#include "lightfilter.h"
#include "hysteresisminmax.h"
#include "dynamiclightfilter.h"
#include "logdynamiclightfilter.h"

#include <QDialog>
#include <QTime>

#ifdef Q_OS_ANDROID
#include <QLightSensor>
#include <QLightReading>
#else
#include "filereader.h"
#include "QTimer"
#endif

//QChart
#include <QtCharts>
using namespace QtCharts;
using namespace std;

#define MA
#define MINMAX
//#define ONLY_LOOK

class LightChart : public QChartView
{
    Q_OBJECT

public:
    LightChart(QWidget *parent = nullptr);
    ~LightChart();

    QString exportData();
    void exportToFile(QString path);

    void clear();
    void setQueueLimit(int num) {m_filter->setQueueLimit(num);
                                    #ifdef MA
                                    m_filter2->setQueueLimit(num*2);
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
#ifdef Q_OS_ANDROID
    QLightSensor* m_lightSensor;
#else
    QTimer* m_readTimer;
    FileReader* m_fileReader;
#endif

    QTime m_curTime;
    QTime m_lastTime;

    //曲线
    QSplineSeries* line;
    QScatterSeries*  m_xySeries;
    //曲线点的最大数量
    int line_max;

    //绘图变量和坐标
    QChart* chart;
    QString m_appendTitle = "";
    QValueAxis *axisX;
    QValueAxis *axisY;
    int moveNum;

    void setupChart();

    // stable
    QQueue<int>  m_allLux;
    int m_lastStable = -1;
    int m_lastDelta = 0;
    QSplineSeries* stableline;
    QScatterSeries*  m_stableXYSeries;
    int stablize(int);

#ifdef MINMAX
    QSplineSeries* minline;
    QSplineSeries* maxline;
    QSplineSeries* minline2;
    QSplineSeries* maxline2;
#endif

#ifdef MA
    ILightFilter* m_filter2;
    int m_lastStable2 = -1;
    int moveNum2;
    QSplineSeries* stableline2;
    QScatterSeries*  m_stableXYSeries2;
    int stablize2(int);
#endif

};

#endif // LightChart_H
