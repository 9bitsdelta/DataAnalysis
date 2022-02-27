cmake_minimum_required(VERSION 3.17)

include(FetchContent)
FetchContent_Declare(mahi-gui GIT_REPOSITORY https://github.com/mahilab/mahi-gui.git)
FetchContent_MakeAvailable(mahi-gui)

file(GLOB SRC
    "src/*.hpp"
    "src/*.cpp"
    )

project(plot-pp)
add_executable(${PROJECT_NAME} ${SRC})
target_link_libraries(${PROJECT_NAME} mahi::gui)
