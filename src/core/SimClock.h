#ifndef SIMCLOCK_H
#define SIMCLOCK_H

#pragma once
#include <QtCore>
#include <optional>

enum class Dir : int { Down=-1, Idle=0, Up=1 };
inline int sgn(int x){ return (0<x)-(x<0); }

struct HallCall {
    int floor = 1;        // 호출층
    Dir dir = Dir::Up;    // 상/하행
    int dest = 0;         // 목적층(0=모름, >0이면 미리 지정된 승객)
    qint64 t_req = 0;     // 요청 시각(ms)
};

struct CarReq { int floor=1; qint64 t_req=0; };

struct CarState {
    int id=0;
    int floor=1;
    Dir dir=Dir::Idle;
    bool doorOpen=false;
    int load=0, capacity=12;
    QSet<int> upTargets, downTargets; // 내부 목적지(CollectiveControl)
};

// (옵션) 전역 타이머 감싸기 — 지금은 QTimer만 쓰지만 확장 대비해 둠
class SimClock : public QObject {
    Q_OBJECT
public:
    explicit SimClock(QObject* p=nullptr) : QObject(p) {}
};
#endif // SIMCLOCK_H
