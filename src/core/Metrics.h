#ifndef METRICS_H
#define METRICS_H

#pragma once
#include <QtCore>

class Metrics {
public:
    void reset(){ waitSum=0; waitMax=0; cnt=0; }
    void addWait(qint64 ms){ waitSum += ms; waitMax = std::max(waitMax, ms); ++cnt; }
    qint64 avgWait() const { return cnt? waitSum/cnt : 0; }
    qint64 maxWait() const { return waitMax; }
private:
    qint64 waitSum=0, waitMax=0;
    qint64 cnt=0;
};

#endif // METRICS_H
