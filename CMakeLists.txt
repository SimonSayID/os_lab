cmake_minimum_required(VERSION 3.8)
project(os_lab)

set(CMAKE_C_STANDARD 11)

include_directories(
        "../os_lab"
)

file(GLOB SOURCE_FILES
        main.c main.h
        communication/*.h
        communication/*.c
        mmap/*.h
        mmap/*.c
        multi/*.h
        multi/*.c
        synchronization/*.h
        synchronization/*.c)

link_libraries(
        pthread
        rt
)

add_executable(os_lab ${SOURCE_FILES} communication/posix_mq.c communication/posix_shm.c)