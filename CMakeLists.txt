cmake_minimum_required(VERSION 3.26)

project(raytracer)

add_executable(raytracer main.cpp "utils/ppm_io.cpp" "vec3.h" "ray.h")
