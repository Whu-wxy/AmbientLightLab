#include "mainwidget.h"

mainWidget::mainWidget(QWidget *parent) : QWidget(parent)
{
    m_chart = new LightChart(this);

    QHBoxLayout* mainLay = new QHBoxLayout(this);

    QVBoxLayout* vlay = new QVBoxLayout;
    m_dataEdit = new QLineEdit;
    m_dataEdit->setMaxLength(9999999);
    m_exportBtn = new QPushButton("导出");
    m_clearBtn = new QPushButton("清除");
    m_methodBtn = new QPushButton("滞后");
    m_method = 0;
    m_queThEdit = new QLineEdit(this);
    m_queThEdit->setPlaceholderText("queueNum");
    QFont font = m_queThEdit->font();
    font.setPixelSize(font.pixelSize()*0.7);
    m_queThEdit->setFont(font);

    vlay->addWidget(m_dataEdit);
    vlay->addWidget(m_exportBtn);
    vlay->addWidget(m_clearBtn);
    vlay->addStretch();
    vlay->addWidget(m_methodBtn);
    vlay->addWidget(m_queThEdit);

    mainLay->addWidget(m_chart);
    mainLay->addLayout(vlay);
    mainLay->setStretch(0, 20);
    mainLay->setStretch(1, 1);


    connect(m_exportBtn, SIGNAL(clicked()), this, SLOT(onExport()));
    connect(m_clearBtn, SIGNAL(clicked()), this, SLOT(onClear()));
    connect(m_methodBtn, SIGNAL(clicked()), this, SLOT(onMethod()));
#ifdef MA
    connect(m_methodBtn, SIGNAL(clicked()), this, SLOT(onMethodMA()));
#endif

    connect(m_queThEdit, SIGNAL(returnPressed()), this, SLOT(onSetQue()));

}

void mainWidget::onExport()
{
    QString res = m_chart->exportData();
    m_dataEdit->setText(res);
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
    if(m_method > 2) m_method = 0;

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
        m_methodBtn->setText("滞后MinMax");
        qDebug()<<"onMethod changeto:"<<"HysteresisMinMax";
    }

    m_chart->setMethod(filter);
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
//        if(m_queThEdit->text().length() == 0)
//            filter = new HysteresisMinMax(m_chart->getQueueLimit()*2);
//        else
//            filter = new HysteresisMinMax(m_queThEdit->text().toInt()*2);
    }

    m_chart->setMethodMA(filter);
}
#endif
