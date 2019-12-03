add_library (util SHARED
    ./util.c)    
find_package(OpenCL REQUIRED) 
target_link_libraries(util PRIVATE OpenCL::OpenCL)    