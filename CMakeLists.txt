cmake_minimum_required (VERSION 2.8)
project (final)
set(Boost_USE_STATIC_LIBS OFF) 
set(Boost_USE_MULTITHREADED ON)  
set(Boost_USE_STATIC_RUNTIME OFF) 
find_package(Boost) 

file(GLOB serverlib_SRC
    "server3/*.hpp"
    "server3/*.cpp"
)

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})     
    add_definitions(-std=c++11)
    add_executable(final final.cpp ${serverlib_SRC})    
    target_link_libraries(final boost_system boost_thread pthread)
endif()
