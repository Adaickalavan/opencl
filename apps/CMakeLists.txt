find_package(OpenCL REQUIRED) 
# find_package(PNG REQUIRED)

# # Ch1.1 (matvec)
# add_executable(matvec ./Ch1/matvec.c)
# target_link_libraries(matvec PRIVATE OpenCL::OpenCL)
# target_link_libraries(matvec PRIVATE util)
# target_include_directories(matvec PRIVATE ../libs/util/)

# # Ch2.1 (platform_ext_test)
# add_executable(platform_ext_test ./Ch2/platform_ext_test.c)
# target_link_libraries(platform_ext_test PRIVATE OpenCL::OpenCL)

# # Ch2.2 (device_ext_test)
# add_executable(device_ext_test ./Ch2/device_ext_test.c)
# target_link_libraries(device_ext_test PRIVATE OpenCL::OpenCL)

# # Ch2.3 (context_count)
# add_executable(context_count ./Ch2/context_count.c)
# target_link_libraries(context_count PRIVATE OpenCL::OpenCL)

# # Ch3.1 (buffer_check) -- have issues
# add_executable(buffer_check ./Ch3/buffer_check.c)
# target_link_libraries(buffer_check PRIVATE OpenCL::OpenCL)

# # Ch3.2 (buffer_test)
# add_executable(buffer_test ./Ch3/buffer_test.c)
# target_link_libraries(buffer_test PRIVATE OpenCL::OpenCL)

# # Ch3.3 (map_copy)
# add_executable(map_copy ./Ch3/map_copy.c)
# target_link_libraries(map_copy PRIVATE OpenCL::OpenCL)

# # Ch4.1 (hello_kernel)
# add_executable(hello_kernel ./Ch4/hello_kernel.c)
# target_link_libraries(hello_kernel PRIVATE OpenCL::OpenCL)

# # Ch4.2 (double_test)
# add_executable(double_test ./Ch4/double_test.c)
# target_link_libraries(double_test PRIVATE OpenCL::OpenCL)

# # Ch4.3 (float_config)
# add_executable(float_config ./Ch4/float_config.c)
# target_link_libraries(float_config PRIVATE OpenCL::OpenCL)

# # Ch4.4 (vector_widths)
# add_executable(vector_widths ./Ch4/vector_widths.c)
# target_link_libraries(vector_widths PRIVATE OpenCL::OpenCL)

# # Ch4.5 (vector_bytes)
# add_executable(vector_bytes ./Ch4/vector_bytes.c)
# target_link_libraries(vector_bytes PRIVATE OpenCL::OpenCL)

# # Ch5.1 (op_test)
# add_executable(op_test ./Ch5/op_test.c)
# target_link_libraries(op_test PRIVATE OpenCL::OpenCL)

# # Ch5.2 (id_check)
# add_executable(id_check ./Ch5/id_check.c)
# target_link_libraries(id_check PRIVATE OpenCL::OpenCL)

# # Ch5.3 (mod_round)
# add_executable(mod_round ./Ch5/mod_round.c)
# target_link_libraries(mod_round PRIVATE OpenCL::OpenCL)

# # Ch5.4 (polar_rect)
# add_executable(polar_rect ./Ch5/polar_rect.c)
# target_link_libraries(polar_rect PRIVATE OpenCL::OpenCL)

# # Ch5.5 (mad_test)
# add_executable(mad_test ./Ch5/mad_test.c)
# target_link_libraries(mad_test PRIVATE OpenCL::OpenCL)

# # Ch5.6 (shuffle_test)
# add_executable(shuffle_test ./Ch5/shuffle_test.c)
# target_link_libraries(shuffle_test PRIVATE OpenCL::OpenCL)

# # Ch5.7 (select_test)
# add_executable(select_test ./Ch5/select_test.c)
# target_link_libraries(select_test PRIVATE OpenCL::OpenCL)

# # Ch6.1 (simple_image) -- have issues
# add_executable(simple_image ./Ch6/simple_image.c)
# target_link_libraries(simple_image PRIVATE OpenCL::OpenCL)
# target_link_libraries(simple_image PRIVATE PNG::PNG)
# target_link_libraries(simple_image PRIVATE util)
# target_include_directories(simple_image PRIVATE ../libs/util/)

# # Ch6.1_1 (simple_image) -- have issues
# add_executable(simple_image1 ./Ch61/simple_image.c)
# target_link_libraries(simple_image1 PRIVATE OpenCL::OpenCL)
# target_link_libraries(simple_image1 PRIVATE PNG::PNG)
# target_link_libraries(simple_image1 PRIVATE util)
# target_include_directories(simple_image1 PRIVATE ../libs/util/)

# # Ch6.2 (interp) -- have issues
# add_executable(interp ./Ch6/interp.c)
# target_link_libraries(interp PRIVATE OpenCL::OpenCL)
# target_link_libraries(interp PRIVATE PNG::PNG)
# target_link_libraries(interp PRIVATE util)
# target_include_directories(interp PRIVATE ../libs/util/)

# # Ch7.2 (callback)
# add_executable(callback ./Ch7/callback.c)
# target_link_libraries(callback PRIVATE OpenCL::OpenCL)
# target_link_libraries(callback PRIVATE util)
# target_include_directories(callback PRIVATE ../libs/util/)

# # Ch7.3 (user_event)
# add_executable(user_event ./Ch7/user_event.c)
# target_link_libraries(user_event PRIVATE OpenCL::OpenCL)
# target_link_libraries(user_event PRIVATE util)
# target_include_directories(user_event PRIVATE ../libs/util/)

# # Ch7.6 (profile_read)
# add_executable(profile_read ./Ch7/profile_read.c)
# target_link_libraries(profile_read PRIVATE OpenCL::OpenCL)
# target_link_libraries(profile_read PRIVATE util)
# target_include_directories(profile_read PRIVATE ../libs/util/)

# # Ch7.7 (profile_items)
# add_executable(profile_items ./Ch7/profile_items.c)
# target_link_libraries(profile_items PRIVATE OpenCL::OpenCL)
# target_link_libraries(profile_items PRIVATE util)
# target_include_directories(profile_items PRIVATE ../libs/util/)

# # Ch7.8 (atomic)
# add_executable(atomic ./Ch7/atomic.c)
# target_link_libraries(atomic PRIVATE OpenCL::OpenCL)
# target_link_libraries(atomic PRIVATE util)
# target_include_directories(atomic PRIVATE ../libs/util/)

# # Ch7.9 (mutex)
# add_executable(mutex ./Ch7/mutex.c)
# target_link_libraries(mutex PRIVATE OpenCL::OpenCL)
# target_link_libraries(mutex PRIVATE util)
# target_include_directories(mutex PRIVATE ../libs/util/)

# Ch8.1 (full_context_8_1)
add_executable(full_context_8_1 ./Ch8/full_context.cpp)
target_link_libraries(full_context_8_1 PRIVATE OpenCL::OpenCL)

# Ch8.2 (create_kernels_8_2)
add_executable(create_kernels_8_2 ./Ch8/create_kernels.cpp)
target_link_libraries(create_kernels_8_2 PRIVATE OpenCL::OpenCL)

# # Ch8.3 (sub_buffer_8_3) -- have issues
# add_executable(sub_buffer_8_3 ./Ch8/sub_buffer.cpp)
# target_link_libraries(sub_buffer_8_3 PRIVATE OpenCL::OpenCL)

# Ch8.4 (buffer_test_8_4)
add_executable(buffer_test_8_4 ./Ch8/buffer_test.cpp)
target_link_libraries(buffer_test_8_4 PRIVATE OpenCL::OpenCL)

# Ch8.5 (map_copy_8_5)
add_executable(map_copy_8_5 ./Ch8/map_copy.cpp)
target_link_libraries(map_copy_8_5 PRIVATE OpenCL::OpenCL)

# Ch8.6 (callback_8_6)
add_executable(callback_8_6 ./Ch8/callback.cpp)
target_link_libraries(callback_8_6 PRIVATE OpenCL::OpenCL)

# Ch8.7 (user_event_8_7)
add_executable(user_event_8_7 ./Ch8/user_event.cpp)
target_link_libraries(user_event_8_7 PRIVATE OpenCL::OpenCL)

# Ch8.8 (profile_8_8)
add_executable(profile_8_8 ./Ch8/profile.cpp)
target_link_libraries(profile_8_8 PRIVATE OpenCL::OpenCL)
