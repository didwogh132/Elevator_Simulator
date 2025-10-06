#include "Building.h"

Building::Building(QObject* parent) : QObject(parent){
    connect(&m_loop, &QTimer::timeout, this, &Building::tick);
    m_loop.setInterval(120);
    setup(10, 3); // 기본: 10층/카 3대
}

void Building::setup(int floors, int carCount){
    stop();
    qDeleteAll(m_cars); m_cars.clear();
    m_floors = std::max(2, floors);
    m_gen.setFloors(m_floors);
    for (int i=0;i<carCount;i++)
        m_cars.push_back(new ElevatorCar(i, m_floors, this));
    m_waitUp.assign(m_floors+1, 0);
    m_waitDown.assign(m_floors+1, 0);
    m_lastUpDest.assign(m_floors+1, 0);
    m_lastDownDest.assign(m_floors+1, 0);
    emit changed();
}

void Building::configure(int floors, int carCount){
    setup(floors, carCount);
}

void Building::spawnOnce(int floor, int dest){
    if (floor<1 || floor>m_floors) return;
    if (dest==floor) dest = (dest==m_floors ? dest-1 : dest+1);
    Dir d = (dest>floor?Dir::Up:Dir::Down);
    m_sched->onHallCall(HallCall{floor,d,dest,QDateTime::currentMSecsSinceEpoch()});
    (d==Dir::Up? m_waitUp[floor] : m_waitDown[floor])++;
    emit changed();
}

void Building::start(){ if(!m_loop.isActive()){ m_loop.start(); emit changed(); } }
void Building::stop(){  if(m_loop.isActive()){  m_loop.stop();  emit changed(); } }

void Building::callUp(int floor){
    if (floor<1 || floor>m_floors) return;
    HallCall hc; hc.floor=floor; hc.dir=Dir::Up; hc.t_req=QDateTime::currentMSecsSinceEpoch();
    m_sched->onHallCall(hc);
    if (m_waitUp.size() < m_floors+1) m_waitUp.assign(m_floors+1, 0);
    m_waitUp[floor]++;
    emit changed();
}
void Building::callDown(int floor){
    if (floor<1 || floor>m_floors) return;
    HallCall hc; hc.floor=floor; hc.dir=Dir::Down; hc.t_req=QDateTime::currentMSecsSinceEpoch();
    m_sched->onHallCall(hc);
    if (m_waitDown.size() < m_floors+1) m_waitDown.assign(m_floors+1, 0);
    m_waitDown[floor]++;
    emit changed();
}
void Building::pushInside(int carId, int floor){
    for (auto* c : m_cars) if (c->id()==carId) { c->pushInside(floor); break; }
}

QVariantList Building::carsQml() const{
    QVariantList out; out.reserve(m_cars.size());
    for (auto* c : m_cars) out.push_back(QVariant::fromValue(c));
    return out;
}

QVariantList Building::waitUpQml() const {
    QVariantList v; v.reserve(m_floors);
    for(int f=1; f<=m_floors; ++f) v << m_waitUp[f];
    return v;
}
QVariantList Building::waitDownQml() const {
    QVariantList v; v.reserve(m_floors);
    for(int f=1; f<=m_floors; ++f) v << m_waitDown[f];
    return v;
}

QVariantList Building::waitDestLabelQml() const {
    QVariantList v; v.reserve(m_floors);
    for (int f=1; f<=m_floors; ++f){
        QString s;
        if (m_waitUp[f] > 0 && m_lastUpDest[f] > 0)      s = QStringLiteral("↗ %1층").arg(m_lastUpDest[f]);
        else if (m_waitDown[f] > 0 && m_lastDownDest[f]) s = QStringLiteral("↘ %1층").arg(m_lastDownDest[f]);
        v << s;
    }
    return v;
}

void Building::tick(){
    // 1) 랜덤 승객 생성 → 전역 큐에 추가
    if (m_autoSpawn) {
        if (auto hc = m_gen.maybeSpawn()) {
            if (hc->dir==Dir::Up)   { m_waitUp[hc->floor]++;   m_lastUpDest[hc->floor] = hc->dest; }
            else                    { m_waitDown[hc->floor]++; m_lastDownDest[hc->floor] = hc->dest; }
            m_sched->onHallCall(*hc);
        }
    }

    // 2) 호출 배정
    m_sched->assign(m_cars);

    // 3) 카 업데이트
    static QHash<int,bool> prevDoor;  // carId -> 지난 틱 doorOpen
    for (auto* c : m_cars){
        bool wasOpen = prevDoor.value(c->id(), false);
        c->tick();

        // 이번 틱에서 "닫힘 → 열림" 으로 바뀐 경우에만 처리
        if (!wasOpen && c->doorOpen()){
            int f = c->floor();

            // (a) 하차
            c->dropHere();

            // (b) 탑승: 진행 방향 우선(Idle이면 Up부터)
            Dir d = (c->dir()==Dir::Idle ? Dir::Up : c->dir());
            QVector<int>& q = (d==Dir::Up ? m_waitUp : m_waitDown);
            int can = c->freeSeats();
            int take = std::min(can, q[f]);
            for (int i=0;i<take;i++){
                int dest = (d==Dir::Up)
                ? QRandomGenerator::global()->bounded(f+1, m_floors+1)
                : QRandomGenerator::global()->bounded(1, f);
                c->boardTo(dest, 1);
                q[f]--;
            }
        }
        prevDoor[c->id()] = c->doorOpen();
    }
    emit changed();
}
