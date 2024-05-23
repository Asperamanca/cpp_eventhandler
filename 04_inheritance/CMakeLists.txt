cmake_minimum_required(VERSION 3.5)

project(04_inheritance VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Core Gui Quick)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Gui Quick)

set(PROJECT_SOURCES
        main.cpp
)

qt_add_executable(04_inheritance
    ${PROJECT_SOURCES}
)

qt_add_qml_module(04_inheritance
    URI mehQml
    VERSION 1.0
    QML_FILES
        Main.qml
        SOURCES windoweventfilter.h windoweventfilter.cpp
        QML_FILES SelectionFrame.qml
        SOURCES ../common/itemdragoperation.cpp
                ../common/itemdragoperation.h
                ../common/lassoaccess.cpp
                ../common/lassoaccess.h
                ../common/lassoselectionoperation.cpp
                ../common/lassoselectionoperation.h
                ../common/selectionaccess.cpp
                ../common/selectionaccess.h
                ../common/commandlineparser.cpp
                ../common/commandlineparser.h
        SOURCES lassoinputrule.h lassoinputrule.cpp
        SOURCES dragmoveinputrule.h dragmoveinputrule.cpp
        SOURCES selectioninputrule.h selectioninputrule.cpp
        SOURCES inputrule.h
        SOURCES inputinfo.h
        SOURCES inputrulemanager.h inputrulemanager.cpp
        SOURCES abstractinputrule.h abstractinputrule.cpp
        SOURCES inputrulemanagerfactory.h inputrulemanagerfactory.cpp
)

target_link_libraries(04_inheritance PRIVATE Qt${QT_VERSION_MAJOR}::Core)
target_link_libraries(04_inheritance PRIVATE Qt${QT_VERSION_MAJOR}::Gui)
target_link_libraries(04_inheritance PRIVATE Qt${QT_VERSION_MAJOR}::Quick)

set_target_properties(04_inheritance PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)
target_include_directories(04_inheritance PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../common)

include(GNUInstallDirs)
install(TARGETS 04_inheritance
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

