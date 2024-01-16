cmake_minimum_required(VERSION 3.5)

project(wgt VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
include_directories(C:/Qt_Projects/WGTimeout)
link_directories(C:/Qt_Projects/WGTimeout)

# Add the paths to the static libraries
set(LIB_DIR "C:/Qt_Projects/WGTimeout")
set(LIBS
    ${LIB_DIR}/libcrypto.lib
    ${LIB_DIR}/libssh2.lib
    ${LIB_DIR}/zlib.lib
)

set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    settings.cpp
    settings.h
    settings.ui
    connect.cpp
    connect.h
    users.h
    users.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(wgt
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    if(ANDROID)
        add_library(wgt SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(wgt
            ${PROJECT_SOURCES}
            main.cpp
            mainwindow.cpp
            mainwindow.h
            mainwindow.ui
            settings.cpp
            settings.h
            settings.ui
            connect.cpp
            connect.h
            users.h
            users.cpp
        )
    endif()
endif()

# Link against the static libraries
target_link_libraries(wgt PRIVATE Qt${QT_VERSION_MAJOR}::Widgets ${LIBS} Crypt32.lib)


# Set target properties
set_target_properties(wgt PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS wgt
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(wgt)
endif()
