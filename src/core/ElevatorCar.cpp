#include "ElevatorCar.h"
#include <algorithm>

ElevatorCar::ElevatorCar(int id, int floors, QObject* parent)
    : QObject(parent), m_id(id), m_floors(floors) {
    rebuildCache();
}

void ElevatorCar::rebuildCache(){
    m_stateCache = { m_id, m_floor, m_dir, m_doorOpen, m_load, 12, m_upTargets, m_downTargets };
    emit changed();
}

void ElevatorCar::pushHallPickup(const HallCall& hc){
    // 픽업층을 내부 큐에 추가(진행 방향 기준)
    if (hc.dir==Dir::Up) m_upTargets.insert(hc.floor);
    else                 m_downTargets.insert(hc.floor);

    if (hc.dest>0) {
        (hc.dest > hc.floor ? m_upTargets : m_downTargets).insert(hc.dest);
    }

    if (m_dir==Dir::Idle) { m_dir = (hc.floor>m_floor?Dir::Up:Dir::Down); m_target = hc.floor; }
    rebuildCache();
}

void ElevatorCar::pushInside(int floor){
    if (floor==m_floor) return;
    (floor>m_floor ? m_upTargets : m_downTargets).insert(floor);
    rebuildCache();
}

void ElevatorCar::stepDoorCycle(){
    if (--m_phaseTicks>0) return;
    if (m_phase==OpenP){ m_phase=Dwell; m_phaseTicks=m_dwellTicks; }
    else if (m_phase==Dwell){ m_phase=CloseP; m_phaseTicks=m_closeTicks; m_doorOpen=false; }
    else if (m_phase==CloseP){ m_phase=Move; }
    rebuildCache();
}

void ElevatorCar::boardTo(int dest, int count){
    if (count<=0 || dest==m_floor) return;
    (dest>m_floor ? m_upTargets : m_downTargets).insert(dest);
    m_dropMap[dest] += count;
    m_load = std::min(m_capacity, m_load + count);
    rebuildCache();
}

int ElevatorCar::dropHere(){
    int n = m_dropMap.value(m_floor, 0);
    if (n>0){
        m_dropMap.remove(m_floor);
        m_load = std::max(0, m_load - n);
        rebuildCache();
    }
    return n;
}

std::optional<int> ElevatorCar::nextTarget(){
    if (m_dir==Dir::Up){
        QList<int> ups = m_upTargets.values();
        std::sort(ups.begin(), ups.end());
        for (int f : ups) if (f>=m_floor) return f;
        if (!m_downTargets.isEmpty()){ m_dir=Dir::Down; return nextTarget(); }
        return std::nullopt;
    } else if (m_dir==Dir::Down){
        QList<int> downs = m_downTargets.values();
        std::sort(downs.begin(), downs.end(), std::greater<int>());
        for (int f : downs) if (f<=m_floor) return f;
        if (!m_upTargets.isEmpty()){ m_dir=Dir::Up; return nextTarget(); }
        return std::nullopt;
    } else {
        if (!m_upTargets.isEmpty()){ m_dir=Dir::Up; return nextTarget(); }
        if (!m_downTargets.isEmpty()){ m_dir=Dir::Down; return nextTarget(); }
        return std::nullopt;
    }
}

void ElevatorCar::tick(){
    if (m_phase!=Move){ stepDoorCycle(); return; }

    auto tgt = nextTarget();
    if (!tgt.has_value()){ m_dir=Dir::Idle; rebuildCache(); return; }
    m_target = *tgt;

    int before=m_floor;
    if (m_floor < m_target) { m_floor++; m_dir=Dir::Up; }
    else if (m_floor > m_target) { m_floor--; m_dir=Dir::Down; }

    if (m_floor!=before){ rebuildCache(); return; }

    // 도착
    if (m_dir==Dir::Up)   m_upTargets.remove(m_floor);
    if (m_dir==Dir::Down) m_downTargets.remove(m_floor);
    m_doorOpen=true; m_phase=OpenP; m_phaseTicks=m_openTicks;
    rebuildCache();
}
