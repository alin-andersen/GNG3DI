cmake_minimum_required(VERSION 3.7)
project("gng3d-interactive")

set(GNG3DI_VERSION_MAJOR 1)
set(GNG3DI_VERSION_MINOR 0)
set(GNG3DI_VERSION_BUILD 0)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(SOURCES
    # Core
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/GNG3DVec.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/GNG3DResult.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/GNG3DConfig.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/GNG3DInputMesh.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/GNG3DSimplifiedMesh.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/GNG3DNode.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/GNG3DEdge.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/GNG3DHandler.cpp
    # Windows
    ${CMAKE_CURRENT_SOURCE_DIR}/Windows/GNG3DIWindow.cpp
    # Widgets
    ${CMAKE_CURRENT_SOURCE_DIR}/Widgets/GNG3DIMainWidget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Widgets/GNG3DIOpenGLWidget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Widgets/GNG3DIEmptyWidget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Widgets/GNG3DILoadingWidget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Widgets/GNG3DISettingsWidget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Widgets/GNG3DIHelpWidget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Widgets/GNG3DIAboutWidget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Widgets/GNG3DIParameterWidget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Widgets/GNG3DIParameterInfoWidget.cpp
)

set(MOC_HEADERS
    # Windows
    ${CMAKE_CURRENT_SOURCE_DIR}/Windows/GNG3DIWindow.h
    # Widgets
    ${CMAKE_CURRENT_SOURCE_DIR}/Widgets/GNG3DIMainWidget.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Widgets/GNG3DIEmptyWidget.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Widgets/GNG3DILoadingWidget.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Widgets/GNG3DISettingsWidget.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Widgets/GNG3DIHelpWidget.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Widgets/GNG3DIAboutWidget.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Widgets/GNG3DIParameterWidget.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Widgets/GNG3DIParameterInfoWidget.h
)

set(UIS
    # Windows
    ${CMAKE_CURRENT_SOURCE_DIR}/Windows/GNG3DIWindow.ui
    # Widgets
    ${CMAKE_CURRENT_SOURCE_DIR}/Widgets/GNG3DIMainWidget.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/Widgets/GNG3DIEmptyWidget.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/Widgets/GNG3DILoadingWidget.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/Widgets/GNG3DISettingsWidget.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/Widgets/GNG3DIHelpWidget.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/Widgets/GNG3DIAboutWidget.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/Widgets/GNG3DIParameterWidget.ui
)

set(QT5_LIBRARIES Qt5::Widgets)

find_package(Qt5Widgets REQUIRED)
include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${Qt5Widgets_INCLUDE_DIRS})

qt5_wrap_ui(UI_HEADERS ${UIS})

add_executable(gng3di ${SOURCES} ${UI_HEADERS})
target_link_libraries(gng3di ${QT5_LIBRARIES} assimp)
target_compile_options(gng3di PRIVATE -fPIC -fpermissive)

# copy shaders
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/node.vsh DESTINATION .)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/node.fsh DESTINATION .)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/edge.vsh DESTINATION .)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/edge.fsh DESTINATION .)

# copy assets
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/Assets DESTINATION .)
