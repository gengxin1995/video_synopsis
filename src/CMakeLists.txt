add_executable(video_synopsis main.cpp synopsis.cpp synopsis.h)

target_link_libraries(video_synopsis
        -lopencv_core
        -lopencv_highgui
        -lopencv_imgproc
        -lopencv_features2d
        )