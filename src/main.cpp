#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "Building.h"

int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);

    qmlRegisterType<Building>("ElevatorSim", 1, 0, "Building");

    QQmlApplicationEngine engine;
    engine.loadFromModule("ElevatorSim", "Main");  // Main.qml
    if (engine.rootObjects().isEmpty()) return -1;

    return app.exec();
}
