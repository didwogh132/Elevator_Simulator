#include "Scheduler.h"
#include "ElevatorCar.h"

void NearestBiasScheduler::onHallCall(const HallCall& call){
    pending_.push_back(call);
}

static double score(const CarState& cs, const HallCall& hc,
                    double A,double B,double C,double D){
    const int dist = std::abs(cs.floor - hc.floor);
    const bool mismatch = (cs.dir!=Dir::Idle && (int)cs.dir != sgn(hc.floor - cs.floor));
    const double loadRatio = double(cs.load) / std::max(1, cs.capacity);
    const double idleBonus = (cs.dir==Dir::Idle) ? D : 0.0;
    return A*dist + B*(mismatch?1.0:0.0) + C*loadRatio - idleBonus;
}

void NearestBiasScheduler::assign(QVector<ElevatorCar*>& cars){
    if (pending_.isEmpty() || cars.isEmpty()) return;

    QVector<int> eraseIdx;
    for (int i=0;i<pending_.size();++i){
        const auto& hc = pending_[i];
        int best=-1; double bestScore=1e9;
        for (auto* car : cars){
            const auto& cs = car->state();
            const double s = score(cs, hc, A_distance, B_dirMismatch, C_load, D_idleBonus);
            if (s < bestScore){ bestScore=s; best=cs.id; }
        }
        if (best>=0){
            for (auto* car : cars) if (car->id()==best){ car->pushHallPickup(hc); break; }
            eraseIdx.push_back(i);
        }
    }
    std::sort(eraseIdx.begin(), eraseIdx.end(), std::greater<int>());
    for (int k: eraseIdx) pending_.removeAt(k);
}
