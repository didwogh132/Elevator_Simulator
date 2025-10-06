#ifndef BUILDING_H
#define BUILDING_H

#pragma once
#include <QtCore>
#include "SimClock.h"
#include "ElevatorCar.h"
#include "Scheduler.h"
#include "PassengerGen.h"
#include "Metrics.h"

class Building : public QObject {
    Q_OBJECT
    Q_PROPERTY(int floors READ floors CONSTANT)
    Q_PROPERTY(QVariantList cars READ carsQml NOTIFY changed)
    Q_PROPERTY(bool running READ running NOTIFY changed)
    Q_PROPERTY(int tickMs READ tickMs WRITE setTickMs NOTIFY changed)
    Q_PROPERTY(QVariantList waitUp READ waitUpQml NOTIFY changed)
    Q_PROPERTY(QVariantList waitDown READ waitDownQml NOTIFY changed)
    Q_PROPERTY(QVariantList waitDestLabel READ waitDestLabelQml NOTIFY changed)
    Q_PROPERTY(bool autoSpawn READ autoSpawn WRITE setAutoSpawn NOTIFY changed)

public:
    explicit Building(QObject* parent=nullptr);

    Q_INVOKABLE void setup(int floors, int carCount);
    Q_INVOKABLE void start();
    Q_INVOKABLE void stop();
    Q_INVOKABLE void callUp(int floor);
    Q_INVOKABLE void callDown(int floor);
    Q_INVOKABLE void pushInside(int carId, int floor);
    Q_INVOKABLE void configure(int floors, int carCount);
    Q_INVOKABLE void spawnOnce(int floor, int dest);

    int floors() const { return m_floors; }
    bool running() const { return m_loop.isActive(); }
    int  tickMs()  const { return m_loop.interval(); }
    void setTickMs(int ms){ m_loop.setInterval(ms); emit changed(); }
    bool autoSpawn() const { return m_autoSpawn; }
    void setAutoSpawn(bool v){ if(m_autoSpawn==v) return; m_autoSpawn=v; emit changed(); }

    QVariantList carsQml() const;
    QVariantList waitUpQml() const;
    QVariantList waitDownQml() const;
    QVariantList waitDestLabelQml() const;

signals:
    void changed();

private slots:
    void tick();

private:
    int m_floors=10;
    bool m_autoSpawn = true;
    QVector<ElevatorCar*> m_cars;
    QScopedPointer<Scheduler> m_sched { new NearestBiasScheduler() };
    QTimer m_loop;
    PassengerGen m_gen { m_floors };
    Metrics m_metrics;

    QVector<int> m_waitUp, m_waitDown;
    QVector<int> m_lastUpDest, m_lastDownDest;
};

#endif // BUILDING_H
