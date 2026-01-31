cmake_minimum_required(VERSION 3.26)

project(raytracer LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)

add_executable(raytracer
        "src/camera.cpp"
        "src/interval.cpp"
        "src/main.cpp"
        "src/material.cpp"
        "src/ppmio.cpp"
        "src/aabb.cpp"
        "src/bvh.cpp"
        "src/sphere.cpp" )

target_include_directories(raytracer PRIVATE include)

