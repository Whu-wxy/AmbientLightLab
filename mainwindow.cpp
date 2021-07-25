#include "mainwindow.h"

mainwindow::mainwindow(QWidget *parent) : QMainWindow(parent)
{
    setWindowFlags(Qt::FramelessWindowHint); //setting windows tool bar icon invisiable

    m_mainWidget = new mainWidget(this);




    setCentralWidget(m_mainWidget);

}
