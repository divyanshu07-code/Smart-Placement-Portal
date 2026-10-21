cmake_minimum_required(VERSION 3.16)
project(SmartPlacementPortal VERSION 1.0.0 LANGUAGES CXX)

# Require C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Source files
set(SOURCES
    main.cpp
    portal.cpp
    Database.cpp
    login.cpp
)

set(HEADERS
    Portal.h
    Database.h
    login.h
)

add_executable(SmartPlacementPortal ${SOURCES} ${HEADERS})

# Compiler warnings
if (MSVC)
    target_compile_options(SmartPlacementPortal PRIVATE /W4)
else()
    target_compile_options(SmartPlacementPortal PRIVATE -Wall -Wextra -pedantic)
endif()
