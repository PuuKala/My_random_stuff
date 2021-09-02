cmake_minimum_required(VERSION 3.10)
set(CMAKE_BUILD_TYPE Debug)

add_executable(LinuxInputPractice linux_input_practice.cpp)

add_executable(LinuxShmemTest linux_shmem_test.cpp)
target_include_directories(LinuxShmemTest PRIVATE json_include)
target_link_libraries(LinuxShmemTest rt) #Required for shm_open

add_executable(SystemControlTest control_testing.cpp)
find_package(Boost 1.30.0 COMPONENTS filesystem system iostreams REQUIRED)
target_link_libraries(SystemControlTest PRIVATE ${Boost_LIBRARIES})
target_compile_options(SystemControlTest PUBLIC -std=c++17)

add_executable(JsonTest json_test.cpp)
target_include_directories(JsonTest PRIVATE json_include)

find_package(Curses REQUIRED)
find_library(MENU_LIBRARY menu)
include_directories(${CURSES_INCLUDE_DIR})

add_executable(CursesTest curses_test.cpp)
target_link_libraries(CursesTest ${MENU_LIBRARY} ${CURSES_LIBRARIES})

#add_executable(cJSONTest cjson_test.cpp cJSON/cJSON.c)
#target_include_directories(cJSONTest PRIVATE cJSON)

add_executable(ncurses_JSON_editor ncurses_form.cpp)
target_include_directories(ncurses_JSON_editor PRIVATE json_include)
target_link_libraries(ncurses_JSON_editor PRIVATE ncurses form)
