cmake_minimum_required(VERSION 3.5)

project(wgt VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Widgets Core REQUIRED LinguistTools)
# installation libssh2 using 'vcpkg'
# cd /&&mkdir src&&cd src&&git clone https://github.com/microsoft/vcpkg&&.\vcpkg\bootstrap-vcpkg.bat&&.\vcpkg\vcpkg install libssh2:x64-windows
set(LIBSSH2_INCLUDE_DIRS "C:/src/vcpkg/installed/x64-windows/include")
set(LIBSSH2_LIBRARIES "C:/src/vcpkg/installed/x64-windows/lib/libssh2.lib")

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
    wgt_ru.ts
)

add_executable(wgt ${PROJECT_SOURCES})

target_include_directories(wgt PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${LIBSSH2_INCLUDE_DIRS}
)

target_link_libraries(wgt PRIVATE Qt5::Widgets Qt5::Core ${LIBSSH2_LIBRARIES} Crypt32.lib)

set_target_properties(wgt PROPERTIES
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS wgt
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

set(TS_FILES
  ${CMAKE_CURRENT_SOURCE_DIR}/wgt_ru.ts)
file(GLOB_RECURSE TS_SOURCES "*.cpp" "*.h" ".ui")
add_custom_target(translations)

foreach(_ts_file ${TS_FILES})
  get_filename_component(_ts_name ${_ts_file} NAME_WE)
  add_custom_command(
    OUTPUT "${_ts_file}"
    DEPENDS ${TS_SOURCES}

    COMMAND ${Qt5_LUPDATE_EXECUTABLE} -recursive
    ${CMAKE_CURRENT_SOURCE_DIR} -ts ${_ts_file}
    )
  add_custom_target(translate_${_ts_name} DEPENDS ${_ts_file})
  add_dependencies(translations translate_${_ts_name})
endforeach()

qt5_add_translation(QM_FILES ${TS_FILES})
