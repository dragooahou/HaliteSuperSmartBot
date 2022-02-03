cmake_minimum_required(VERSION 2.8)

project(HaliteSuperSmartBot)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/out")

if(MINGW)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O2 -Wall -Wno-unused-function -pedantic")
endif()

include_directories(${CMAKE_SOURCE_DIR}/hlt)

get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)

foreach(dir ${dirs})
    file(GLOB_RECURSE SOURCE ${dir}/*.[ch]*)
    set(SOURCE_FILES "${SOURCE_FILES}" ${SOURCE})
endforeach()

include_directories(${CMAKE_SOURCE_DIR})

set(SOURCE_FILES "${SOURCE_FILES}"
        src/MyBot.cpp

        # Here the sources

        src/BehaviourTree.cpp src/BehaviourTree.h
        src/MainBehaviour.cpp src/MainBehaviour.h
        src/InputNode.cpp src/InputNode.h
        src/ActionNodes.cpp src/ActionNodes.h
        src/PointOfInterest.cpp src/PointOfInterest.h src/AiNodes/ExploitPoi.cpp src/AiNodes/ExploitPoi.h)

add_executable(HaliteSuperSmartBot ${SOURCE_FILES})

if(MINGW)
    target_link_libraries(HaliteSuperSmartBot -static)
endif()
