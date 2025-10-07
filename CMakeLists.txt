cmake_minimum_required(VERSION 3.16)

project(untitled VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Qt6 REQUIRED COMPONENTS Quick)

qt_standard_project_setup(REQUIRES 6.5)

qt_add_executable(Elevator_Simulator
    src/main.cpp
    src/core/Building.cpp
    src/core/ElevatorCar.cpp
    src/core/Scheduler.cpp
    src/core/PassengerGen.cpp
    src/core/Metrics.cpp
    src/core/SimClock.cpp
)

target_include_directories(Elevator_Simulator PRIVATE src/core)

qt_add_qml_module(Elevator_Simulator
    URI ElevatorSim
    VERSION 1.0
    QML_FILES
        qml/Main.qml
        qml/BuildingView.qml
        qml/ControlsPane.qml
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(Elevator_Simulator PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.Elevator_Simulator
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(Elevator_Simulator
    PRIVATE Qt6::Quick
)

include(GNUInstallDirs)
install(TARGETS Elevator_Simulator
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
