cmake_minimum_required(VERSION 3.16)

project(untitled VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Qt6 REQUIRED COMPONENTS Quick)

qt_standard_project_setup(REQUIRES 6.5)

qt_add_executable(appuntitled

)

qt_add_qml_module(appuntitled
    URI ElevatorSim
    VERSION 1.0
    QML_FILES
        Main.qml
        SOURCES
        SOURCES Building.h
        SOURCES ElevatorCar.h
        SOURCES Scheduler.h
        SOURCES PassengerGen.h
        SOURCES Metrics.h
        SOURCES SimClock.h
        SOURCES Building.cpp
        SOURCES ElevatorCar.cpp
        SOURCES Scheduler.cpp
        SOURCES PassengerGen.cpp
        SOURCES Metrics.cpp
        SOURCES SimClock.cpp
        QML_FILES BuildingView.qml
        QML_FILES ControlsPane.qml
        SOURCES main.cpp
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appuntitled PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appuntitled
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appuntitled
    PRIVATE Qt6::Quick
)

include(GNUInstallDirs)
install(TARGETS appuntitled
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
