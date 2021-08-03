#include "mainwindow.h"
#include <QApplication>

//#include "algorithm"
//#include <vector>
//#include <iostream>
#include <QDebug>
//using namespace std;

#include "QtAndroid"

bool checkPermission() {
    QtAndroid::PermissionResult r = QtAndroid::checkPermission("android.permission.WRITE_EXTERNAL_STORAGE");
    if(r == QtAndroid::PermissionResult::Denied) {
        QtAndroid::requestPermissionsSync( QStringList() << "android.permission.WRITE_EXTERNAL_STORAGE" );
        r = QtAndroid::checkPermission("android.permission.WRITE_EXTERNAL_STORAGE");
        if(r == QtAndroid::PermissionResult::Denied) {
             return false;
        }
   }
   return true;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

//    vector<int> vec{1,4,99,10};
//    double up=0, down=0;
//    sort(vec.begin(), vec.end());

//    // 中位数和相应的索引
//    int mid, mid1, mid3;
//    int size = vec.size();

//    mid = size / 2;
//    mid1 = (size % 2) == 0 ? (mid-1)/2 : mid/2;
//    mid3 = mid+mid1+1;
//    if (size%2 != 0 )
//    { // 整数集是奇数的情况
//        down = mid % 2 == 0 ? (vec[mid1] + vec[mid1-1]) / 2.0
//                : vec[mid1];
//        up = mid % 2 == 0 ? (vec[mid3] + vec[mid3-1]) / 2.0
//                : vec[mid3];
//    }
//    else
//    { // 整数集偶数的情况
//        down = (mid-1) % 2 == 0 ? (vec[mid1] + vec[mid1-1]) / 2.0
//                : vec[mid1];
//        up = (mid-1) % 2 == 0 ? (vec[mid3] + vec[mid3-1]) / 2.0
//                : vec[mid3];
//    }

//    qDebug()<<up<<","<<down;

    bool res = checkPermission();
    qDebug()<<"checkPermission: "<<res;

    mainwindow w;
    w.setWindowState(Qt::WindowMaximized);
    w.show();

    return a.exec();
}
