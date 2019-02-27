cmake_minimum_required(VERSION 3.8)
project(conv)

set(CMAKE_CXX_STANDARD 11)

list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} $ENV{ROOTSYS}/etc/cmake)
find_package(ROOT REQUIRED COMPONENTS RIO Net)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${ROOT_INCLUDE_DIRS})

link_directories($ENV{ROOTSYS}/lib)

set(SOURCE_FILES conv.cpp
        src/Visualizer.cpp src/BothReadDetector.cpp
        src/CosmicRay.cpp src/Converter.cpp src/COSMICManager.cpp
        src/COSMIC.cpp src/CSIManager.cpp src/CSI.cpp)
add_executable(conv ${SOURCE_FILES})

target_link_libraries(conv ${ROOT_LIBRARIES})
