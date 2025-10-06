#ifndef SCHEDULER_H
#define SCHEDULER_H

#pragma once
#include <QtCore>
#include "SimClock.h"

class ElevatorCar;

class Scheduler {
public:
    virtual ~Scheduler() = default;
    virtual void onHallCall(const HallCall& call) = 0;
    virtual void assign(QVector<ElevatorCar*>& cars) = 0; // pending → car에 push
};

class NearestBiasScheduler : public Scheduler {
public:
    void onHallCall(const HallCall& call) override;
    void assign(QVector<ElevatorCar*>& cars) override;

    // 가중치 (필요 시 조절)
    double A_distance   = 1.0;
    double B_dirMismatch= 3.0;
    double C_load       = 2.0;
    double D_idleBonus  = 1.5;
private:
    QVector<HallCall> pending_;
};

#endif // SCHEDULER_H
