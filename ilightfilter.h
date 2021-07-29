#ifndef ILIGHTFILTER_H
#define ILIGHTFILTER_H

#include "QString"

class ILightFilter
{
public:
    ILightFilter() = default;
    virtual ~ILightFilter() = default;


    virtual int stableLux(int) = 0;
    void setQueueLimit(int num) {m_numTh = num; }
    int getQueueLimit() {return m_numTh; }
    virtual void reset() = 0;

    int m_numTh = 4;
    QString m_methodName = "ILightFilter";

};

#endif // ILIGHTFILTER_H
