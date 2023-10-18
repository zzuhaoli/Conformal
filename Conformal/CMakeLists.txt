cmake_minimum_required(VERSION 3.0)
project(conformal)

#when debugging own code, alsywas avoid install anything into global system folder
#set defualt CMAKE_INATALL_PREFIX to local INSTALL 
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR}/INSTALL CACHE PATH "Default prefix of install directories" FORCE)
endif()

set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_STANDARD 17)

#set(_acts_eigen3_version 3.3.7)
#set(_acts_root_version 6.20)

find_package(Eigen3 REQUIRED NO_MODULE)
find_package(ROOT REQUIRED COMPONENTS Geom)


include_directories({CMAKE_SOURCE_DIR})

set(SRC_LIST  DriftChamber.cpp DriftWire.cc circle.cpp)
add_library(drift SHARED ${SRC_LIST})
target_link_libraries(drift PRIVATE ROOT::Geom Eigen3::Eigen)


install(TARGETS drift LIBRARY DESTINATION ${PROJECT_SOURCE_DIR})

