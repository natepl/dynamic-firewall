cmake_minimum_required(VERSION 3.14)
project(DynamicFirewall CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add subdirectories for each component
add_subdirectory(enforcer)
add_subdirectory(aggregator)
add_subdirectory(analysis)
