project(AnalysisEngine CXX)

add_executable(analysis 
    src/main.cpp 
)

target_include_directories(analysis PRIVATE src)
target_link_libraries(analysis PRIVATE pcap)
