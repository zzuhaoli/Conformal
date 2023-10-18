cmake_minimum_required(VERSION 3.0)
project(conformal)

set(_acts_eigen3_version 3.3.7)
set(_acts_root_version 6.20)
find_package(Eigen3 ${_acts_boost_version} REQUIRED COMPONENTS Core)
find_package(ROOT ${_acts_root_version} REQUIRED COMPONENTS Core Geom)
#include_directories(${ROOT_INCLUDE_DIRS})

#aux_source_directory(${CMAKE_SOURCE_DIR} SRC_LIST)

set(SRC_LIST  DriftChamber.cpp DriftWire.cc )
include_directories(${CMAKE_SOURCE_DIR})


add_library(drift SHARED ${SRC_LIST})
target_link_libraries(drift PRIVATE ROOT::Geom)
target_link_libraries(drift PRIVATE Eigen3::Eigen)

set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR})
install(FILES DriftWire.hh DriftChamber.hh Algebra.hh f.hh DESTINATION /home/lihao/root/include)
install(TARGETS drift LIBRARY DESTINATION /home/lihao/root/lib)


