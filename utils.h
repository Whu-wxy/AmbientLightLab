#ifndef MUTILS_H
#define MUTILS_H

#include <QList>
#include <QQueue>
#include <QtMath>


//double StandardDeviation(QList<int> list ){
//    if(list.count() == 0) return 0;

//    int n = list.count();
//    double sigma = 0, pSigma = 0;
//    for (int i = 0; i < n; ++i) {
//        int v = list.at (i);
//        sigma += v;        // sum
//        pSigma += v*v;     // 平方和
//    }
//    sigma /= list.count ();          // 获得平均值
//    return qSqrt((pSigma/n - sigma*sigma)) ;
//}

//double StandardDeviation(QList<double> list ){
//    if(list.count() == 0) return 0;

//    int n = list.count();
//    double sigma = 0, pSigma = 0;
//    for (int i = 0; i < n; ++i) {
//        double v = list.at (i);
//        sigma += v;        // sum
//        pSigma += v*v;     // 平方和
//    }
//    sigma /= list.count ();          // 获得平均值
//    return qSqrt((pSigma/n - sigma*sigma)) ;
//}

//double StandardDeviation(QQueue<int> list ){
//    if(list.count() == 0) return 0;

//    int n = list.count();
//    double sigma = 0, pSigma = 0;
//    for (int i = 0; i < n; ++i) {
//        int v = list.at(i);
//        sigma += v;        // sum
//        pSigma += v*v;     // 平方和
//    }
//    sigma /= list.count ();          // 获得平均值
//    return qSqrt((pSigma/n - sigma*sigma)) ;
//}


//double StandardDeviation(QQueue<double> list ){
//    if(list.count() == 0) return 0;

//    int n = list.count();
//    double sigma = 0, pSigma = 0;
//    for (int i = 0; i < n; ++i) {
//        double v = list.at(i);
//        sigma += v;        // sum
//        pSigma += v*v;     // 平方和
//    }
//    sigma /= list.count ();          // 获得平均值
//    return qSqrt((pSigma/n - sigma*sigma)) ;
//}

#endif // MUTILS_H
