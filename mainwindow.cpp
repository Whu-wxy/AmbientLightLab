#include "mainwindow.h"

mainwindow::mainwindow(QWidget *parent) : QMainWindow(parent)
{
#ifdef Q_OS_ANDROID
    setWindowFlags(Qt::FramelessWindowHint); //setting windows tool bar icon invisiable
#endif
    m_mainWidget = new mainWidget(this);

    setCentralWidget(m_mainWidget);

}
