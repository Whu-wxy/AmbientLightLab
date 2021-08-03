#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLineEdit>

#include "lightchart.h"
#include "ilightfilter.h"
#include "lightfilter.h"
#include "movefilter.h"
#include "dynamiclightfilter.h"
#include "hysteresisminmax.h"
#include "hysteresisloginterval.h"
#include "logdynamiclightfilter.h"

class mainWidget : public QWidget
{
    Q_OBJECT
public:
    explicit mainWidget(QWidget *parent = nullptr);

private:
    LightChart* m_chart;
    QLineEdit* m_dataEdit;
    QPushButton* m_exportBtn;
    QPushButton* m_export2Btn;
    QPushButton* m_clearBtn;
    QLineEdit* m_queThEdit;
    QPushButton* m_methodBtn;
    int m_method;
#ifdef MA
    void onMethodMA();
#endif
signals:

public slots:
    void onExport();
    void onExportToFile();
    void onClear();
    void onSetQue();
    void onMethod();

};

#endif // MAINWIDGET_H
