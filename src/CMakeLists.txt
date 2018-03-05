find_package(OpenCV)
include_directories(${OpenCV_INCLUDE_DIRS})

#aux_source_directory(. DIR_SRCS)

add_executable(video_synopsis main.cpp synopsis.cpp synopsis.h)

target_link_libraries(video_synopsis ${OpenCV_LIBS})