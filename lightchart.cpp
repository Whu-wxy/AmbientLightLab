#include "lightchart.h"

LightChart::LightChart(QWidget *parent)
    : QChartView(parent)
{
    m_filter = new lightfilter(4);
#ifdef MA
    m_filter2 = new lightfilter(8);
#endif

    m_lastDelta = 0;
    m_lastStable -1;

    m_lightSensor = new QLightSensor(this);
    connect(m_lightSensor, SIGNAL(readingChanged()), this, SLOT(onDataReach()));
    m_lightSensor->setAlwaysOn(true);
    m_lightSensor->setActive(true);

    setupChart();


    m_lightSensor->start();

}

LightChart::~LightChart()
{
    delete m_filter;
}

void LightChart::setupChart()
{
    line = new QSplineSeries(this);
    line->setName("lux");//设置曲线名称
    QPen linePen(QColor(255, 0, 0));
    linePen.setWidth(5);
    line->setPen(linePen);//设置曲线颜色
    line->setUseOpenGL(true);//openGl 加速
    m_xySeries = new QScatterSeries();
    m_xySeries->setMarkerShape(QScatterSeries::MarkerShapeCircle);
    m_xySeries->setMarkerSize(3);
    QPen xyPen(QColor(0, 0, 255));
    xyPen.setWidth(5);
    m_xySeries->setBrush(QBrush(QColor(0,0,255)));
    m_xySeries->setPen(xyPen);

    // stable
    stableline = new QSplineSeries(this);
    stableline->setName("stable lux");//设置曲线名称
    QPen linePen2(QColor(180, 250, 180));
    linePen2.setWidth(5);
    stableline->setPen(linePen2);//设置曲线颜色
    stableline->setUseOpenGL(true);//openGl 加速
    m_stableXYSeries = new QScatterSeries();
    m_stableXYSeries->setMarkerShape(QScatterSeries::MarkerShapeCircle);
    m_stableXYSeries->setMarkerSize(3);
    QPen xyPen2(QColor(50, 200, 30));
    xyPen2.setWidth(5);
    m_stableXYSeries->setBrush(QBrush(QColor(50, 200, 30)));
    m_stableXYSeries->setPen(xyPen2);
#ifdef MA
    stableline2 = new QSplineSeries(this);
    stableline2->setName("MA");//设置曲线名称
    QPen linePen3(QColor(250, 150, 50));
    linePen3.setWidth(5);
    stableline2->setPen(linePen3);//设置曲线颜色
    stableline2->setUseOpenGL(true);//openGl 加速
    m_stableXYSeries2 = new QScatterSeries();
    m_stableXYSeries2->setMarkerShape(QScatterSeries::MarkerShapeCircle);
    m_stableXYSeries2->setMarkerSize(3);
    QPen xyPen3(QColor(250, 150, 150));
    xyPen3.setWidth(5);
    m_stableXYSeries2->setBrush(QBrush(QColor(250, 150, 150)));
    m_stableXYSeries2->setPen(xyPen3);
#endif

    chart = new QChart();
    chart->addSeries(line);
    chart->addSeries(m_xySeries);
    chart->addSeries(stableline);
    chart->addSeries(m_stableXYSeries);

    axisX = new QValueAxis(this);
    axisX->setRange(0,line_max);//范围
    axisY = new QValueAxis(this);
    axisY->setRange(0,1000);


    chart->setTitle("light Data");//设置图标标题
    chart->createDefaultAxes();//设置坐标轴

    chart->setAxisX(axisX,line);
    chart->setAxisY(axisY,line);
    chart->setAxisX(axisX,m_xySeries);
    chart->setAxisY(axisY,m_xySeries);
    chart->setAxisX(axisX,stableline);
    chart->setAxisY(axisY,stableline);
    chart->setAxisX(axisX,m_stableXYSeries);
    chart->setAxisY(axisY,m_stableXYSeries);
#ifdef MA
    chart->addSeries(stableline2);
    chart->addSeries(m_stableXYSeries2);
    chart->setAxisX(axisX,stableline2);
    chart->setAxisY(axisY,stableline2);
    chart->setAxisX(axisX,m_stableXYSeries2);
    chart->setAxisY(axisY,m_stableXYSeries2);
#endif

    axisX->setRange(0,line_max);//范围   5个*20秒
    axisX->setTitleText("times");//标题
    axisX->setTickCount(10);//分隔个数
    axisY->setTitleText("lux");
    axisY->setTickCount(10);

    setChart(chart);
    setRenderHint(QPainter::Antialiasing);
}

bool mycheck(QPointF i,QPointF j) { return i.y()<j.y(); }

void LightChart::onDataReach()
{
    QLightReading* reading = m_lightSensor->reading();
    int lux = reading->lux();
    m_allLux.enqueue(lux);
    while(m_allLux.count() > 10000) m_allLux.dequeue();



    //    if(!m_curTime.isValid())
    //        m_curTime = QTime::currentTime();
    //    else
    //    {
    //        m_lastTime = m_curTime;
    //        m_curTime = QTime::currentTime();
    //        int msecs = m_lastTime.msecsTo(m_curTime);
    //        cout<<"msecs: "<<msecs<<endl;
    //        qDebug()<<"msecs: "<<msecs<<endl;
    //    }

    QVector<QPointF> list;
    QVector<QPointF> newlist;
    list = line->pointsVector();//获取现在图中列表
    if (list.size() < line_max)
    {
        //保持原来
        newlist = list;
    }
    else
    {
        //错位移动
        for(int i = 1; i< list.size();i++)
        {
            newlist.append(QPointF(i-1, list.at(i).y()));
        }
    }
    newlist.append(QPointF(newlist.size(),lux));//最后补上新的数据

    if(newlist.size() % 10 == 0 || newlist.size() == 1)
    {
        QPointF maxP = *std::max_element(newlist.begin(), newlist.end(), mycheck);
        int maxLux = maxP.y();
        axisY->setMax(ceil(maxLux)*1.3);
    }

    line->replace(newlist);//替换更新
    m_xySeries->replace(newlist);

    int res = stablize(lux);
#ifdef MA
    stablize2(lux);
#endif

    int delta  = 0;
    if(res != -1)
    {
        if(m_lastStable == -1) m_lastStable = res;
        else
        {
            delta = res - m_lastStable;
            m_lastDelta = delta;
            m_lastStable = res;
        }

        QString title = "current:" + QString::number(lux) + "lux        --" + "stable:"
                + QString::number(res) + "        --stable delta:"+QString::number(delta);
        chart->setTitle(title);//设置图标标题
    }
    else
    {
        QString title = "current:" + QString::number(lux) + "lux--" + "stable:"
                + QString::number(m_lastStable) + "--stable delta:"+QString::number(m_lastDelta);
        chart->setTitle(title);//设置图标标题
    }
    chart->setTitle(chart->title() + "--" + m_appendTitle);
}

int LightChart::stablize(int lux)
{
    static int moveNum = 0;

    // stablize
    lux = m_filter->stableLux(lux);

    QVector<QPointF> list;
    QVector<QPointF> newlist;
    list = stableline->pointsVector();//获取现在图中列表
    if (line->pointsVector().size() < line_max)
    {
        //保持原来
        newlist = list;
    }
    else
    {
        moveNum++;
        int iReduce = 0;
        if(moveNum > getQueueLimit())
        {
            moveNum = 0;
            iReduce = 1;
        }

        //错位移动
        if(iReduce == 0)
        {
            for(int i = 0; i< list.size();i++)
            {
                newlist.append(QPointF(list.at(i).x()-1,list.at(i).y()));
            }
        }
        else
        {
            for(int i = 1; i< list.size();i++)
            {
                newlist.append(QPointF(list.at(i).x()-1,list.at(i).y()));
            }
        }

    }
//    if(lux == -1) return -1;
    if(lux != -1)
    {
        newlist.append(QPointF(line->pointsVector().size(),lux));//最后补上新的数据
        qDebug()<<"Point:"<<QPointF(line->pointsVector().size(),lux);
    }

    stableline->replace(newlist);//替换更新
    m_stableXYSeries->replace(newlist);
    return lux;
}

#ifdef MA
int LightChart::stablize2(int lux)
{
    static int moveNum2 = 0;

    // stablize
    lux = m_filter2->stableLux(lux);

    QVector<QPointF> list;
    QVector<QPointF> newlist;
    list = stableline2->pointsVector();//获取现在图中列表
    if (line->pointsVector().size() < line_max)
    {
        //保持原来
        newlist = list;
    }
    else
    {
        moveNum2++;
        int iReduce = 0;
        if(moveNum2 > getQueueLimit())
        {
            moveNum2 = 0;
            iReduce = 1;
        }

        //错位移动
        if(iReduce == 0)
        {
            for(int i = 0; i< list.size();i++)
            {
                newlist.append(QPointF(list.at(i).x()-1,list.at(i).y()));
            }
        }
        else
        {
            for(int i = 1; i< list.size();i++)
            {
                newlist.append(QPointF(list.at(i).x()-1,list.at(i).y()));
            }
        }

    }
//    if(lux == -1) return -1;
    if(lux != -1)
    {
        newlist.append(QPointF(line->pointsVector().size(),lux));//最后补上新的数据
        qDebug()<<"Point:"<<QPointF(line->pointsVector().size(),lux);
    }

    stableline2->replace(newlist);//替换更新
    m_stableXYSeries2->replace(newlist);
    return lux;
}
#endif

// 每次输出最前面400个数
QString LightChart::exportData()
{
    if(m_allLux.empty()) return "";

    QString res;
    for(int j = 0; j<400; j++)
    {
        if(m_allLux.count() == 0) break;

        int i = m_allLux.head();
        res += QString::number(i);
        res += ",";
        m_allLux.dequeue();
    }
    res.remove(res.count()-1, 1);

    return res;
}

void LightChart::clear()
{
    m_allLux.clear();
}

void LightChart::setMethod(ILightFilter* filter)
{
    if(m_filter && filter)
    {
        delete m_filter;
        m_filter = nullptr;
    }

    m_filter = filter;

    m_appendTitle = "";
    line->clear();
    m_xySeries->clear();
    stableline->clear();
    m_stableXYSeries->clear();
    m_allLux.clear();
}

#ifdef MA
void LightChart::setMethodMA(ILightFilter* filter)
{
    if(m_filter2 && filter)
    {
        delete m_filter2;
        m_filter2 = nullptr;
    }

    m_filter2 = filter;

    stableline2->clear();
    m_stableXYSeries2->clear();
}
#endif

void LightChart::appendString(QString text)
{
    m_appendTitle = text;
    chart->setTitle(chart->title() + "--" + m_appendTitle);
}
