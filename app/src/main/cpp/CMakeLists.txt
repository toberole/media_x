cmake_minimum_required(VERSION 3.4.1)

include_directories(${CMAKE_SOURCE_DIR}/include)
aux_source_directory(${CMAKE_SOURCE_DIR}/src SRC_FILES)

set(third_libs "${CMAKE_SOURCE_DIR}/${CMAKE_ANDROID_ARCH_ABI}/lib")

add_library(libavcodec-56 SHARED IMPORTED)
set_target_properties(
        libavcodec-56
        PROPERTIES IMPORTED_LOCATION
        "${third_libs}/libavcodec-56.so")

add_library(libavdevice-56 SHARED IMPORTED)
set_target_properties(
        libavdevice-56
        PROPERTIES IMPORTED_LOCATION
        "${third_libs}/libavdevice-56.so")

add_library(libavfilter-5 SHARED IMPORTED)
set_target_properties(
        libavfilter-5
        PROPERTIES IMPORTED_LOCATION
        "${third_libs}/libavfilter-5.so")

add_library(libavformat-56 SHARED IMPORTED)
set_target_properties(
        libavformat-56
        PROPERTIES IMPORTED_LOCATION
        "${third_libs}/libavformat-56.so")

add_library(libavutil-54 SHARED IMPORTED)
set_target_properties(
        libavutil-54
        PROPERTIES IMPORTED_LOCATION
        "${third_libs}/libavutil-54.so")

add_library(libpostproc-53 SHARED IMPORTED)
set_target_properties(
        libpostproc-53
        PROPERTIES IMPORTED_LOCATION
        "${third_libs}/libpostproc-53.so")

add_library(libswresample-1 SHARED IMPORTED)
set_target_properties(
        libswresample-1
        PROPERTIES IMPORTED_LOCATION
        "${third_libs}/libswresample-1.so")

add_library(libswscale-3 SHARED IMPORTED)
set_target_properties(
        libswscale-3
        PROPERTIES IMPORTED_LOCATION
        "${third_libs}/libswscale-3.so")

add_library(native-lib SHARED ${SRC_FILES})

find_library(log-lib log)

target_link_libraries(
        native-lib
        libavcodec-56
        libavdevice-56
        libavfilter-5
        libavformat-56
        libavutil-54
        libpostproc-53
        libswresample-1
        libswscale-3
        ${log-lib}
        android
)