cmake_minimum_required(VERSION 3.8.2)

project(GNG3DI)

# Find includes in the build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Turn on automatic invocation of the MOC, UIC & RCC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Add a compiler flags.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

find_package(Qt5 REQUIRED COMPONENTS Widgets)

if(WIN32)
  
  set(CMAKE_WIN32_EXECUTABLE ON)



endif()

if(UNIX)

  include_directories(${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/assimp/linux/headers/)

  find_library(
    AssimpLibrary 
    assimp
    HINTS "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/assimp/linux/libs")

endif(UNIX)

file(GLOB_RECURSE GNG3DI_SourceFiles ${CMAKE_CURRENT_SOURCE_DIR}/srcs/**/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/srcs/**/*.ui)
add_executable(gng3di ${GNG3DI_SourceFiles})

target_include_directories(gng3di PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/srcs/)

# Add the Qt5 Widgets for linking
target_link_libraries(gng3di Qt5::Widgets ${AssimpLibrary})

if(UNIX)

  add_custom_target(
    CopyLibraries
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/assimp/linux/libs/libassimp.so ${CMAKE_BINARY_DIR}
    )

endif(UNIX)


