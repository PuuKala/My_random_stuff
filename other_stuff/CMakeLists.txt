cmake_minimum_required(VERSION 3.10)

add_executable(LinuxInputPractice linux_input_practice.cpp)

add_executable(LinuxShmemTest linux_shmem_test.cpp)
target_include_directories(LinuxShmemTest PRIVATE json_include)
target_link_libraries(LinuxShmemTest rt) #Required for shm_open

add_executable(SystemControlTest control_testing.cpp)

add_executable(JsonTest json_test.cpp)
target_include_directories(JsonTest PRIVATE json_include)