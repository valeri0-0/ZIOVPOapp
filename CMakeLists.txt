cmake_minimum_required(VERSION 3.10)

project(ZloVPO)

set(CMAKE_CXX_STANDARD 17)

add_executable(ZloVPO WIN32
    main.cpp
    tray.cpp
)

target_compile_definitions(ZloVPO PRIVATE UNICODE _UNICODE)

target_link_libraries(ZloVPO
    user32
    shell32
)