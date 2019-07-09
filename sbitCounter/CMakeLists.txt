cmake_minimum_required (VERSION 2.8)
project(sbitCounter)


set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_MODULE_PATH}/cmake-files ${CMAKE_CURRENT_SOURCE_DIR}/cmake-files)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)

set(INSTALL_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

find_package(SystemC REQUIRED)
find_package(SystemC_AMS REQUIRED)

include_directories(${SYSTEMC_INCLUDE_DIR} ${SYSTEMC_AMS_INCLUDE_DIR})


FILE(GLOB SRCFILES src/*.cpp)

add_executable(run
src/main.cpp  
${SRCFILES}
)


target_link_libraries( run ${SYSTEMC_LIBRARIES} ${SYSTEMC_AMS_LIBRARIES})

#add_subdirectory(src)
