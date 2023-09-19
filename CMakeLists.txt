cmake_minimum_required(VERSION 3.25)

project(lua_beyond)

add_subdirectory(${CMAKE_SOURCE_DIR}/libs/lua)
add_subdirectory(${CMAKE_SOURCE_DIR}/libs/sfml)
add_executable(${PROJECT_NAME}
    ${CMAKE_SOURCE_DIR}/src/main.cpp
    ${CMAKE_SOURCE_DIR}/src/Engine.cpp
    ${CMAKE_SOURCE_DIR}/src/Scene.cpp
    ${CMAKE_SOURCE_DIR}/src/Resources.cpp
    ${CMAKE_SOURCE_DIR}/src/Input.cpp
    )

target_link_libraries(${PROJECT_NAME}
    PUBLIC sfml-system
    PUBLIC sfml-window
    PUBLIC sfml-graphics
    PUBLIC lua
)

target_include_directories(${PROJECT_NAME}
    PUBLIC ${CMAKE_SOURCE_DIR}/include
    PUBLIC ${CMAKE_SOURCE_DIR}/libs/lua/src
    PUBLIC ${CMAKE_SOURCE_DIR}/libs/entt
)
