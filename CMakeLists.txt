cmake_minimum_required(VERSION 3.5)

project(wgt VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    # Static linking with the C++ Runtime library for release
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
    # Static linking with the C++ Runtime library for debug
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")

    # Exclude flags set by default in CMake
    string(REPLACE "/MDd" "" CMAKE_CXX_FLAGS_DEBUG ${CMAKE_CXX_FLAGS_DEBUG})
    string(REPLACE "/MD" "" CMAKE_CXX_FLAGS_RELEASE ${CMAKE_CXX_FLAGS_RELEASE})
endif()

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets Core REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets Core REQUIRED)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/WGTimeout)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/WGTimeout)

set(LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/)
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
        )
    endif()
endif()

target_link_libraries(wgt PRIVATE Qt5::Widgets Qt5::Core ${LIBS} Crypt32.lib)

target_include_directories(wgt PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/WGTimeout
)

set_target_properties(wgt PROPERTIES
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
