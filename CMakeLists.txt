cmake_minimum_required(VERSION 3.8)
project(conv)

set(CMAKE_CXX_STANDARD 11)

list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} $ENV{ROOTSYS}/etc/cmake)
find_package(ROOT REQUIRED COMPONENTS RIO Net)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${ROOT_INCLUDE_DIRS})

link_directories($ENV{ROOTSYS}/lib)

set(SOURCE_FILES conv2.cpp
        ./src/TreeManager.cpp ./src/Visualization.cpp ./src/BothReadDetector.cpp
        ./src/CosmicMeasSystem.cpp ./src/TriggerManager.cpp ./src/CsIManager.cpp
        ./src/CosmicRayCounter.cpp ./src/CsI.cpp)
add_executable(conv ${SOURCE_FILES})

target_link_libraries(conv ${ROOT_LIBRARIES})
