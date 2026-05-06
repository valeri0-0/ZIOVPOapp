cmake_minimum_required(VERSION 3.10)
project(ZloVPO)

set(CMAKE_CXX_STANDARD 17)

add_executable(ZloVPO
    main.cpp
    tray.cpp
)

target_link_libraries(ZloVPO
    user32
    shell32
)