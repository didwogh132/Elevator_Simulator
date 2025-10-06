#ifndef PASSENGERGEN_H
#define PASSENGERGEN_H

#pragma once
#include <QtCore>
#include "SimClock.h"

class PassengerGen {
public:
    explicit PassengerGen(int floors=10) : m_floors(floors) {}

    void setFloors(int f){ m_floors = std::max(2,f); }
    void setSpawnProb(double p){ m_prob = std::clamp(p, 0.0, 1.0); }

    // 틱마다 호출 → 생성되면 HallCall 리턴
    std::optional<HallCall> maybeSpawn() {
        if (QRandomGenerator::global()->generateDouble() >= m_prob) return std::nullopt;
        int a = QRandomGenerator::global()->bounded(1, m_floors+1);
        int b = QRandomGenerator::global()->bounded(1, m_floors+1);
        if (a==b) b = (b==m_floors? b-1 : b+1);
        HallCall hc;
        hc.floor = a; hc.dest = b;
        hc.dir = (b>a? Dir::Up : Dir::Down);
        hc.t_req = QDateTime::currentMSecsSinceEpoch();
        return hc;
    }
private:
    int m_floors=10;
    double m_prob=0.08; // 틱당 생성확률
};

#endif // PASSENGERGEN_H
