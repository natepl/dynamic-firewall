project(EnforcerDaemon CXX)

add_executable(enforcer 
    src/main.cpp 
    src/pf_manager.cpp
)

target_include_directories(enforcer PRIVATE src)
