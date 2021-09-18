#include "lightchart.h"

LightChart::LightChart(QWidget *parent)
    : QChartView(parent)
{
#ifdef ONLY_LOOK
    m_filter = nullptr;
#ifdef MA
    m_filter2 = nullptr;
#endif
#else
    m_filter = new lightfilter(4);
#ifdef MA
    m_filter2 = new lightfilter(8);
#endif
#endif

    m_lastDelta = 0;
    m_lastStable -1;
    moveNum = 0;
    moveNum2 = 0;
    line_max = 100;

    setupChart();

#ifdef Q_OS_ANDROID
    m_lightSensor = new QLightSensor(this);
    connect(m_lightSensor, SIGNAL(readingChanged()), this, SLOT(onDataReach()));
    m_lightSensor->setAlwaysOn(true);
    m_lightSensor->setActive(true);

    m_lightSensor->start();
#else
    m_fileReader = new FileReader("C:\\codes\\AmbientLightLab\\recordWin.txt");
    m_readTimer = new QTimer(this);
    connect(m_readTimer, SIGNAL(timeout()), this, SLOT(onDataReach()));
    if(m_fileReader->hasData())
        m_readTimer->start(200);
    else
        qDebug()<<"read file failed!";
#endif
}

LightChart::~LightChart()
{
    if(m_filter)
        delete m_filter;
#ifdef MA
    if(m_filter2)
        delete m_filter2;
#endif
}

void LightChart::setupChart()
{
#ifdef Q_OS_ANDROID
    int width = 5;
#else
    int width = 2;
#endif

    line = new QSplineSeries(this);
    line->setName("lux");//设置曲线名称
    QPen linePen(QColor(255, 0, 0));
    linePen.setWidth(width);
    line->setPen(linePen);//设置曲线颜色
    line->setUseOpenGL(true);//openGl 加速
    m_xySeries = new QScatterSeries();
    m_xySeries->setMarkerShape(QScatterSeries::MarkerShapeCircle);
    m_xySeries->setMarkerSize(3);
    QPen xyPen(QColor(0, 0, 255));
    xyPen.setWidth(width);
    m_xySeries->setBrush(QBrush(QColor(0,0,255)));
    m_xySeries->setPen(xyPen);

    // stable
    stableline = new QSplineSeries(this);
    stableline->setName("stable lux");//设置曲线名称
    QPen linePen2(QColor(180, 250, 180));
    linePen2.setWidth(width);
    stableline->setPen(linePen2);//设置曲线颜色
    stableline->setUseOpenGL(true);//openGl 加速
    m_stableXYSeries = new QScatterSeries();
    m_stableXYSeries->setMarkerShape(QScatterSeries::MarkerShapeCircle);
    m_stableXYSeries->setMarkerSize(3);
    QPen xyPen2(QColor(50, 200, 30));
    xyPen2.setWidth(width);
    m_stableXYSeries->setBrush(QBrush(QColor(50, 200, 30)));
    m_stableXYSeries->setPen(xyPen2);

#ifdef MINMAX
    minline = new QSplineSeries(this);
    minline->setPen(QPen(QColor(150,150,150)));//设置曲线颜色
    minline->setUseOpenGL(true);
    maxline = new QSplineSeries(this);
    maxline->setPen(QPen(QColor(150,150,150)));//设置曲线颜色
    maxline->setUseOpenGL(true);
    minline2 = new QSplineSeries(this);
    minline2->setPen(QPen(QColor(40,40,40)));//设置曲线颜色
    minline2->setUseOpenGL(true);
    maxline2 = new QSplineSeries(this);
    maxline2->setPen(QPen(QColor(40,40,40)));//设置曲线颜色
    maxline2->setUseOpenGL(true);
#endif

#ifdef MA
    stableline2 = new QSplineSeries(this);
    stableline2->setName("MA");//设置曲线名称
    QPen linePen3(QColor(250, 150, 50));
    linePen3.setWidth(width);
    stableline2->setPen(linePen3);//设置曲线颜色
    stableline2->setUseOpenGL(true);//openGl 加速
    m_stableXYSeries2 = new QScatterSeries();
    m_stableXYSeries2->setMarkerShape(QScatterSeries::MarkerShapeCircle);
    m_stableXYSeries2->setMarkerSize(3);
    QPen xyPen3(QColor(250, 150, 150));
    xyPen3.setWidth(width);
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
    axisY->setRange(-20,1000);

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
#ifdef MINMAX
    chart->addSeries(minline);
    chart->addSeries(maxline);
    chart->setAxisX(axisX,minline);
    chart->setAxisY(axisY,minline);
    chart->setAxisX(axisX,maxline);
    chart->setAxisY(axisY,maxline);
    chart->addSeries(minline2);
    chart->addSeries(maxline2);
    chart->setAxisX(axisX,minline2);
    chart->setAxisY(axisY,minline2);
    chart->setAxisX(axisX,maxline2);
    chart->setAxisY(axisY,maxline2);
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
#ifdef Q_OS_ANDROID
    QLightReading* reading = m_lightSensor->reading();
    int lux = reading->lux();
#else
    if(m_fileReader->finished())
    {
        m_fileReader->reset();
        line->clear();
        m_xySeries->clear();
        stableline->clear();
        m_stableXYSeries->clear();
        stableline2->clear();
        m_stableXYSeries2->clear();
        m_allLux.clear();
        if(m_filter)
            m_filter->reset();
        if(m_filter2)
            m_filter2->reset();
#ifdef MINMAX
        minline->clear();
        maxline->clear();
        minline2->clear();
        maxline2->clear();
#endif
    }
    int lux = m_fileReader->getLux();
#endif

    m_allLux.enqueue(lux);
    while(m_allLux.count() > 100000) m_allLux.dequeue();


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

    int res = -1;
    if(m_filter)
        res = stablize(lux);
#ifdef MA
    if(m_filter2)
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
    if(!m_filter) return -1;

#ifdef MINMAX
    if(m_filter->m_methodName == "HysteresisMinMax")
    {
        HysteresisMinMax* filter = dynamic_cast<HysteresisMinMax*> (m_filter);
        if(filter)
        {
            int minV = 0, maxV = 0;
            bool hasPrevious = filter->getPrevisousMinMax(lux, minV, maxV);
            qDebug()<<"[LightChart] hasPrevious:"<<hasPrevious<<"--minV:"<<minV<<"--maxV:"<<maxV;

            if(hasPrevious)
            {
                QVector<QPointF> listMinMax;
                listMinMax.append(QPointF(0, minV));
                listMinMax.append(QPointF(line_max, minV));
                minline->replace(listMinMax);
                listMinMax.clear();
                listMinMax.append(QPointF(0, maxV));
                listMinMax.append(QPointF(line_max, maxV));
                maxline->replace(listMinMax);
            }
        }
        else
            qDebug()<<"dynamic_cast to HysteresisMinMax failed!";
    }
#endif

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
        bool bReduce = false;
        if(moveNum > getQueueLimit())
        {
            moveNum = 0;
            bReduce = true;
        }

        //错位移动
        if(!bReduce)
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
        //        qDebug()<<"Point:"<<QPointF(line->pointsVector().size(),lux);
    }

#ifdef MINMAX
    if(m_filter->m_methodName == "dynamicLightFilter")
    {
        dynamicLightFilter* filter = dynamic_cast<dynamicLightFilter*> (m_filter);
        if(filter && lux != -1)
        {
            int minV = 0, maxV = 0, minV2 = 0, maxV2 = 0;
            filter->getMinMax(lux, minV, maxV, minV2, maxV2);
//            qDebug()<<"[LightChart] hasPrevious:"<<hasPrevious<<"--minV:"<<minV<<"--maxV:"<<maxV;

            QVector<QPointF> listMinMax;
            listMinMax.append(QPointF(0, minV));
            listMinMax.append(QPointF(line_max, minV));
            minline->replace(listMinMax);
            listMinMax.clear();
            listMinMax.append(QPointF(0, maxV));
            listMinMax.append(QPointF(line_max, maxV));
            maxline->replace(listMinMax);
            listMinMax.clear();
            listMinMax.append(QPointF(0, minV2));
            listMinMax.append(QPointF(line_max, minV2));
            minline2->replace(listMinMax);
            listMinMax.clear();
            listMinMax.append(QPointF(0, maxV2));
            listMinMax.append(QPointF(line_max, maxV2));
            maxline2->replace(listMinMax);
        }
    }
#endif

    stableline->replace(newlist);//替换更新
    m_stableXYSeries->replace(newlist);
    return lux;
}

#ifdef MA
int LightChart::stablize2(int lux)
{
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
        int iReduce = 0;
        if(moveNum2 > getQueueLimit()*2)
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
//        qDebug()<<"Point:"<<QPointF(line->pointsVector().size(),lux);
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
    for(int j = 0; j<300; j++)
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

void LightChart::exportToFile(QString path)
{
    if(m_allLux.empty()) return;

    qDebug()<<"exportToFile: "<<path;

    QString res;
    for(int i=0; i<m_allLux.count(); i++)
    {
        int lux = m_allLux.at(i);
        res += QString::number(lux);
        res += ",";
    }
    res.remove(res.count()-1, 1);

    QFile file(path);
    if(file.open(QIODevice::ReadWrite))
    {
        file.write(res.toLatin1());
        qDebug()<<"exportToFile: write";
    }
    else
        qDebug()<<"exportToFile: file.open failed";
}

void LightChart::clear()
{
    m_allLux.clear();
}

void LightChart::setMethod(ILightFilter* filter)
{
    if(m_filter)
    {
        delete m_filter;
        m_filter = nullptr;
    }

    m_filter = filter;

    m_appendTitle = "";
    moveNum2 = moveNum = 0;
    m_lastDelta = 0;
    m_lastStable -1;
    line->clear();
    m_xySeries->clear();
    stableline->clear();
    m_stableXYSeries->clear();
    m_allLux.clear();

#ifdef MINMAX
    minline->clear();
    maxline->clear();
    minline2->clear();
    maxline2->clear();
#endif

#ifdef WIN32
    m_fileReader->reset();
#endif
}

#ifdef MA
void LightChart::setMethodMA(ILightFilter* filter)
{
    if(m_filter2)
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
