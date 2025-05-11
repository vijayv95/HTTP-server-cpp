cmake_minimum_required(VERSION 3.28)
project(http-server)

find_package(Crow REQUIRED)

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/server.cpp
    src/handlers/users.cpp
    src/handlers/base.cpp
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        Crow::Crow
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)