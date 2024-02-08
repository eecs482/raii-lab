cmake_minimum_required(VERSION 3.16)

project(EECS_482_Raii_Lab)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(THREADS_PREFER_PTHREAD_FLAG ON)

find_package(Threads REQUIRED)

add_library(libthread STATIC IMPORTED)
if (APPLE)
    add_compile_options(-D_XOPEN_SOURCE)
    set_target_properties(libthread PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_LIST_DIR}/libthread_macos.o)
else ()
    set_target_properties(libthread PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_LIST_DIR}/libthread.o)
endif ()

add_executable(file_size_counter file_size_counter.cpp)
target_link_libraries(file_size_counter libthread ${CMAKE_DL_LIBS} Threads::Threads)

add_executable(file_size_counter_solution solution/file_size_counter.cpp)
target_link_libraries(file_size_counter_solution libthread ${CMAKE_DL_LIBS} Threads::Threads)

add_executable(unique_pointers unique_pointers.cpp)

add_executable(unique_pointers_solution solution/unique_pointers.cpp)
