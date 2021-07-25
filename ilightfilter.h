#ifndef ILIGHTFILTER_H
#define ILIGHTFILTER_H


class ILightFilter
{
public:
    ILightFilter() = default;
    virtual ~ILightFilter() = default;


    virtual int stableLux(int) = 0;
    void setQueueLimit(int num) {m_numTh = num; }
    int getQueueLimit() {return m_numTh; }

    int m_numTh = 4;


};

#endif // ILIGHTFILTER_H
