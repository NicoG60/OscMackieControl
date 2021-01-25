# thrid party
find_package(Qt5 COMPONENTS Core REQUIRED)
find_package(Qt5 COMPONENTS Gui REQUIRED)
find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt5 COMPONENTS Network REQUIRED)
find_package(Qt5 COMPONENTS Qml REQUIRED)
find_package(Qt5 COMPONENTS Quick REQUIRED)
find_package(Qt5 COMPONENTS QuickControls2 REQUIRED)
find_package(Qt5 COMPONENTS Xml REQUIRED)

include(ProcessorCount)
ProcessorCount(N_PROC)
if(N_PROC EQUAL 0) # fallback to 1 if failure
    set(N_PROC 1)
endif()

set(BUILD_SHARED_LIBS OFF CACHE BOOL "static lib for everyone" FORCE)

execute_process(COMMAND git submodule update --init -j ${N_PROC}
                WORKING_DIRECTORY ${OSCMACKIECONRTOL_SOURCE_DIR})

add_subdirectory(${OSCMACKIECONRTOL_SOURCE_DIR}/third_party/miniz-cpp
                 ${CMAKE_CURRENT_BINARY_DIR}/miniz-cpp
                 EXCLUDE_FROM_ALL)

add_subdirectory(${OSCMACKIECONRTOL_SOURCE_DIR}/third_party/qmdnsengine
                 ${CMAKE_CURRENT_BINARY_DIR}/qmdnsengine
                 EXCLUDE_FROM_ALL)

add_subdirectory(${OSCMACKIECONRTOL_SOURCE_DIR}/third_party/QOsc
                 ${CMAKE_CURRENT_BINARY_DIR}/QOsc
                 EXCLUDE_FROM_ALL)

add_subdirectory(${OSCMACKIECONRTOL_SOURCE_DIR}/third_party/QMidi
                 ${CMAKE_CURRENT_BINARY_DIR}/QMidi
                 EXCLUDE_FROM_ALL)

add_subdirectory(${OSCMACKIECONRTOL_SOURCE_DIR}/third_party/QtAwesome
                 ${CMAKE_CURRENT_BINARY_DIR}/QtAwesome
                 EXCLUDE_FROM_ALL)

# sources
file(GLOB headers *.h)
file(GLOB sources *.cpp)
qt5_add_resources(sources ${OSCMACKIECONRTOL_SOURCE_DIR}/gui/qml.qrc)



# target
add_executable(OscMackieControl ${sources} ${headers})

if(WIN32)
    target_sources(OscMackieControl PUBLIC ${OSCMACKIECONRTOL_SOURCE_DIR}/graphical/appicon.rc)
    set_target_properties(OscMackieControl PROPERTIES WIN32_EXECUTABLE TRUE)
elseif(APPLE)
    set(MACOSX_BUNDLE_ICON_FILE icon.icns)
    set(icon ${OSCMACKIECONRTOL_SOURCE_DIR}/graphical/icon.icns)
    set_source_files_properties(${icon} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")

    target_sources(OscMackieControl PUBLIC ${icon})
    set_target_properties(OscMackieControl PROPERTIES MACOSX_BUNDLE TRUE)
endif()

target_link_libraries(
    OscMackieControl
    PRIVATE
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        Qt5::Network
        Qt5::Qml
        Qt5::Quick
        Qt5::QuickControls2
        Qt5::Xml
        miniz-cpp
        qmdnsengine
        QOsc
        QMidi
        QtAwesome
)
target_compile_definitions(OscMackieControl PRIVATE SOFT_VERSION=${PROJECT_VERSION})
target_compile_definitions(OscMackieControl PRIVATE GIT_VERSION=${GIT_VERSION})

set_target_properties(OscMackieControl PROPERTIES RUNTIME_OUTPUT_DIRECTORY bin)
