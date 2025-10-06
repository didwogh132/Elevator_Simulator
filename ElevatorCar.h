#ifndef ELEVATORCAR_H
#define ELEVATORCAR_H

#pragma once
#include <QtCore>
#include "SimClock.h"

class ElevatorCar : public QObject {
    Q_OBJECT
    Q_PROPERTY(int id READ id CONSTANT)
    Q_PROPERTY(int floor READ floor NOTIFY changed)
    Q_PROPERTY(bool doorOpen READ doorOpen NOTIFY changed)
    Q_PROPERTY(int load READ load NOTIFY changed)
    Q_PROPERTY(int capacity READ capacity CONSTANT)
    Q_PROPERTY(bool full READ full NOTIFY changed)

public:
    explicit ElevatorCar(int id, int floors, QObject* parent=nullptr);

    int id() const { return m_id; }
    int floor() const { return m_floor; }
    bool doorOpen() const { return m_doorOpen; }
    int load() const { return m_load; }
    int capacity() const { return m_capacity; }
    bool full() const { return m_load >= m_capacity; }
    int freeSeats() const { return std::max(0, m_capacity - m_load); }
    void boardTo(int dest, int count=1);   // 탑승: 목적층 등록 + load 증가
    int dropHere();                        // 현재층 하차 인원 수만큼 처리
    Dir dir() const { return m_dir; }
    const CarState& state() const { return m_stateCache; }

    // 배정기에서 호출
    void pushHallPickup(const HallCall& hc); // 픽업층 등록(Collective)
    void pushInside(int floor);              // 내부 목적층 등록

    // 틱마다 상태 업데이트
    void tick();

signals:
    void changed();

private:
    void rebuildCache();
    void stepDoorCycle(); // Open → Dwell → Close
    std::optional<int> nextTarget(); // CollectiveControl 규칙으로 다음 목표

    int m_id=0, m_floors=10;
    int m_floor=1, m_target=1;

    Dir m_dir=Dir::Idle;

    QSet<int> m_upTargets, m_downTargets;

    enum Phase { Move, OpenP, Dwell, CloseP } m_phase=Move;
    int m_openTicks=3, m_dwellTicks=6, m_closeTicks=3, m_phaseTicks=0;
    bool m_doorOpen=false;

    int m_load=0; // 단순 근사(승객수)
    int m_capacity=10;
    QHash<int,int> m_dropMap;
    CarState m_stateCache;
};
#endif // ELEVATORCAR_H
