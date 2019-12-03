find_package(OpenCL REQUIRED) 
find_package(PNG REQUIRED)

# Ch1 (matvec)
add_executable(matvec ./Ch1/matvec.c)
target_link_libraries(matvec PRIVATE OpenCL::OpenCL)
target_link_libraries(matvec PRIVATE util)
target_include_directories(matvec PRIVATE ../libs/util/)

# Ch2 (platform_ext_test)
add_executable(platform_ext_test ./Ch2/platform_ext_test.c)
target_link_libraries(platform_ext_test PRIVATE OpenCL::OpenCL)

# Ch2 (device_ext_test)
add_executable(device_ext_test ./Ch2/device_ext_test.c)
target_link_libraries(device_ext_test PRIVATE OpenCL::OpenCL)

# Ch2 (context_count)
add_executable(context_count ./Ch2/context_count.c)
target_link_libraries(context_count PRIVATE OpenCL::OpenCL)

# Ch3 (buffer_check)
add_executable(buffer_check ./Ch3/buffer_check.c)
target_link_libraries(buffer_check PRIVATE OpenCL::OpenCL)

# Ch3 (buffer_test)
add_executable(buffer_test ./Ch3/buffer_test.c)
target_link_libraries(buffer_test PRIVATE OpenCL::OpenCL)

# Ch3 (map_copy)
add_executable(map_copy ./Ch3/map_copy.c)
target_link_libraries(map_copy PRIVATE OpenCL::OpenCL)

# Ch4 (hello_kernel)
add_executable(hello_kernel ./Ch4/hello_kernel.c)
target_link_libraries(hello_kernel PRIVATE OpenCL::OpenCL)

# Ch4 (double_test)
add_executable(double_test ./Ch4/double_test.c)
target_link_libraries(double_test PRIVATE OpenCL::OpenCL)

# Ch4 (float_config)
add_executable(float_config ./Ch4/float_config.c)
target_link_libraries(float_config PRIVATE OpenCL::OpenCL)

# Ch4 (vector_widths)
add_executable(vector_widths ./Ch4/vector_widths.c)
target_link_libraries(vector_widths PRIVATE OpenCL::OpenCL)

# Ch4 (vector_bytes)
add_executable(vector_bytes ./Ch4/vector_bytes.c)
target_link_libraries(vector_bytes PRIVATE OpenCL::OpenCL)

# Ch5 (op_test)
add_executable(op_test ./Ch5/op_test.c)
target_link_libraries(op_test PRIVATE OpenCL::OpenCL)

# Ch5 (id_check)
add_executable(id_check ./Ch5/id_check.c)
target_link_libraries(id_check PRIVATE OpenCL::OpenCL)

# Ch5 (mod_round)
add_executable(mod_round ./Ch5/mod_round.c)
target_link_libraries(mod_round PRIVATE OpenCL::OpenCL)

# Ch5 (polar_rect)
add_executable(polar_rect ./Ch5/polar_rect.c)
target_link_libraries(polar_rect PRIVATE OpenCL::OpenCL)

# Ch5 (mad_test)
add_executable(mad_test ./Ch5/mad_test.c)
target_link_libraries(mad_test PRIVATE OpenCL::OpenCL)

# Ch5 (shuffle_test)
add_executable(shuffle_test ./Ch5/shuffle_test.c)
target_link_libraries(shuffle_test PRIVATE OpenCL::OpenCL)

# Ch5 (select_test)
add_executable(select_test ./Ch5/select_test.c)
target_link_libraries(select_test PRIVATE OpenCL::OpenCL)

# Ch6 (simple_image)
add_executable(simple_image ./Ch6/simple_image.c)
target_link_libraries(simple_image PRIVATE OpenCL::OpenCL)
target_link_libraries(simple_image PRIVATE PNG::PNG)
target_link_libraries(simple_image PRIVATE util)
target_include_directories(simple_image PRIVATE ../libs/util/)

# # Ch6 (interp)
# add_executable(interp ./Ch6/interp.c)
# target_link_libraries(interp PRIVATE OpenCL::OpenCL)

