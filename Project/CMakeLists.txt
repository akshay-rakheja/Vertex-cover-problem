cmake_minimum_required(VERSION 2.8)
project(prjece650)

if (EXISTS "${CMAKE_SOURCE_DIR}/minisat")
  add_subdirectory(minisat)
else()
  message (FATAL_ERROR "MiniSat not found. Use `git clone https://github.com/agurfinkel/minisat` to check it out.")
endif()


## ECE650 Specific configuration start here
## Project specific configurations go here

# enable c++11 support
set (CMAKE_CXX_FLAGS "-std=c++11 -pthread -Wall ${CMAKE_CXX_FLAGS}")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# expose minisat header files
include_directories(${CMAKE_SOURCE_DIR}/minisat)

# create the main executable
## add additional .cpp files if needed
# add_executable(a4ece650 cnf_sat_vc.cpp)
add_executable(prjece650 project_satoshi.cpp cnf_sat_refactor.cpp)

# link MiniSAT libraries
target_link_libraries(prjece650 minisat-lib-static)

#install(TARGETS prjece650 RUNTIME DESTINATION bin) ## gotta check if this is the correct way to implement this

