project(AggregatorService CXX)

add_executable(aggregator 
    src/main.cpp 
)

target_include_directories(aggregator PRIVATE src)
