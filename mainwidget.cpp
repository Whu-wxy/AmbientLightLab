#include "mainwidget.h"
#include <androidsetup.h>

mainWidget::mainWidget(QWidget *parent) : QWidget(parent)
{
    m_chart = new LightChart(this);

    QHBoxLayout* mainLay = new QHBoxLayout(this);

    QVBoxLayout* vlay = new QVBoxLayout;
    m_dataEdit = new QLineEdit;
    m_dataEdit->setMaxLength(9999999);
    m_exportBtn = new QPushButton("导出");
    m_export2Btn = new QPushButton("到文件");
    m_clearBtn = new QPushButton("清除");
#ifndef ONLY_LOOK
    m_methodBtn = new QPushButton("滞后");
    m_method = 0;
    m_queThEdit = new QLineEdit(this);
    m_queThEdit->setPlaceholderText("queueNum");
    QFont font = m_queThEdit->font();
    font.setPixelSize(font.pixelSize()*0.7);
    m_queThEdit->setFont(font);
#endif

    vlay->addWidget(m_dataEdit);
    vlay->addWidget(m_exportBtn);
    vlay->addWidget(m_export2Btn);
    vlay->addWidget(m_clearBtn);
    vlay->addStretch();
#ifndef ONLY_LOOK
    vlay->addWidget(m_methodBtn);
    vlay->addWidget(m_queThEdit);
#endif
    mainLay->addWidget(m_chart);
    mainLay->addLayout(vlay);
    mainLay->setStretch(0, 20);
    mainLay->setStretch(1, 1);


    connect(m_exportBtn, SIGNAL(clicked()), this, SLOT(onExport()));
    connect(m_export2Btn, SIGNAL(clicked()), this, SLOT(onExportToFile()));
    connect(m_clearBtn, SIGNAL(clicked()), this, SLOT(onClear()));
#ifndef ONLY_LOOK
    connect(m_methodBtn, SIGNAL(clicked()), this, SLOT(onMethod()));
    connect(m_queThEdit, SIGNAL(returnPressed()), this, SLOT(onSetQue()));
#endif
}

void mainWidget::onExport()
{
    QString res = m_chart->exportData();
    m_dataEdit->setText(res);
}

void mainWidget::onExportToFile()
{
#ifdef Q_OS_ANDROID
    AndroidSetup setup;
    QString dataDir = setup.getAppDataDir();
    m_chart->exportToFile(dataDir + QDir::separator() + "record.txt");
#endif
}

void mainWidget::onClear()
{
    m_chart->clear();
}

void mainWidget::onSetQue()
{
    QString text = m_queThEdit->text();
    if(text.length() == 0) return;
    if(!text.data()->isNumber()) return;

    qDebug()<<"onSetQue:"<<text.toInt();
    m_chart->setQueueLimit(text.toInt());
}

void mainWidget::onMethod()
{
    m_method++;
    if(m_method > 4) m_method = 0;

    ILightFilter* filter = nullptr;
    if(m_method == 0)
    {
        if(m_queThEdit->text().length() == 0)
            filter = new lightfilter(m_chart->getQueueLimit());
        else
            filter = new lightfilter(m_queThEdit->text().toInt());
        m_methodBtn->setText("滞后");
        qDebug()<<"onMethod changeto:"<<"lightfilter";
    }
    else if(m_method == 1)
    {
        if(m_queThEdit->text().length() == 0)
            filter = new movefilter(m_chart->getQueueLimit());
        else
            filter = new movefilter(m_queThEdit->text().toInt());
        m_methodBtn->setText("运动");
        qDebug()<<"onMethod changeto:"<<"movefilter";
    }
    else if(m_method == 2)
    {
        if(m_queThEdit->text().length() == 0)
            filter = new HysteresisMinMax(m_chart->getQueueLimit());
        else
            filter = new HysteresisMinMax(m_queThEdit->text().toInt());
        m_methodBtn->setText("滞MinMax");
        qDebug()<<"onMethod changeto:"<<"HysteresisMinMax";

    }
    else if(m_method == 3)
    {
        if(m_queThEdit->text().length() == 0)
            filter = new HysteresisLogInterval(m_chart, m_chart->getQueueLimit());
        else
            filter = new HysteresisLogInterval(m_chart, m_queThEdit->text().toInt());
        m_methodBtn->setText("滞LogInterval");
        qDebug()<<"onMethod changeto:"<<"HysteresisLogInterval";
    }
    else if(m_method == 4)
    {
        if(m_queThEdit->text().length() == 0)
            filter = new dynamicLightFilter(m_chart, 5, 3);
        else
            filter = new dynamicLightFilter(m_chart, m_queThEdit->text().toInt(), 3);
        m_methodBtn->setText("滞后ALPHA");
        qDebug()<<"onMethod changeto:"<<"dynamicLightFilter--滞后ALPHA";
    }

    m_chart->setMethod(filter);
    onMethodMA();
}

#ifdef MA
void mainWidget::onMethodMA()
{
    ILightFilter* filter = nullptr;
    if(m_method == 0)
    {
        if(m_queThEdit->text().length() == 0)
            filter = new lightfilter(m_chart->getQueueLimit()*2);
        else
            filter = new lightfilter(m_queThEdit->text().toInt()*2);
    }
    else if(m_method == 1)
    {
        if(m_queThEdit->text().length() == 0)
            filter = new movefilter(m_chart->getQueueLimit()*2);
        else
            filter = new movefilter(m_queThEdit->text().toInt()*2);
    }
    else if(m_method == 2)
    {
        if(m_queThEdit->text().length() == 0)
            filter = new HysteresisMinMax(m_chart->getQueueLimit()*2);
        else
            filter = new HysteresisMinMax(m_queThEdit->text().toInt()*2);

//        if(m_queThEdit->text().length() == 0)
//            filter = new dynamicLightFilter(m_chart, m_chart->getQueueLimit()*2);
//        else
//            filter = new dynamicLightFilter(m_chart, m_queThEdit->text().toInt()*2);
    }
    else if(m_method == 3)
    {
//        if(m_queThEdit->text().length() == 0)
//            filter = new HysteresisLogInterval(m_chart, m_chart->getQueueLimit()*2);
//        else
//            filter = new HysteresisLogInterval(m_chart, m_queThEdit->text().toInt()*2);
    }
    else if(m_method == 4)
    {
//        if(m_queThEdit->text().length() == 0)
//            filter = new dynamicLightFilter(m_chart, m_chart->getQueueLimit()*2);
//        else
//            filter = new dynamicLightFilter(m_chart, m_queThEdit->text().toInt()*2);

//        if(m_queThEdit->text().length() == 0)
//            filter = new dynamicLightFilter(m_chart, 5, 3, false);
//        else
//            filter = new dynamicLightFilter(m_chart, m_queThEdit->text().toInt(), 3, false);
//        qDebug()<<"dynamicLightFilter compare to:"<<"dynamicLightFilter weight mean";

//        if(m_queThEdit->text().length() == 0)
//            filter = new lightfilter(m_chart->getQueueLimit());
//        else
//            filter = new lightfilter(m_queThEdit->text().toInt());
//        qDebug()<<"dynamicLightFilter compare to:"<<"lightfilter";
    }

    m_chart->setMethodMA(filter);
}
#endif
