#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "mainwidget.h"
class mainwindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit mainwindow(QWidget *parent = nullptr);

private:
    mainWidget* m_mainWidget;

signals:

public slots:
};

#endif // MAINWINDOW_H
