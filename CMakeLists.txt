cmake_minimum_required(VERSION 3.26)

project(raytracer LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

include(FetchContent)
FetchContent_Declare(
        tinygraphics
        GIT_REPOSITORY https://github.com/anil100391/tinygraphics.git
        GIT_TAG main)
FetchContent_MakeAvailable(tinygraphics)

set(SOURCES
        "src/camera.cpp"
        "src/interval.cpp"
        "src/main.cpp"
        "src/material.cpp"
        "src/ppmio.cpp"
        "src/aabb.cpp"
        "src/bvh.cpp"
        "src/sphere.cpp"
        "src/texture.cpp"
        "src/rtSTBImage.cpp"
        "src/perlin.cpp"
        "src/constantMedium.cpp"
        "src/hittable.cpp"
        "src/quad.cpp")

set(HEADERS
        "include/aabb.h"
        "include/bvh.h"
        "include/camera.h"
        "include/color.h"
        "include/constantMedium.h"
        "include/hittable.h"
        "include/hittableList.h"
        "include/interval.h"
        "include/material.h"
        "include/perlin.h"
        "include/ppmio.h"
        "include/quad.h"
        "include/ray.h"
        "include/rtDefs.h"
        "include/rtSTBImage.h"
        "include/sphere.h"
        "include/texture.h"
        "include/vec3.h")

add_executable(raytracer ${SOURCES} ${HEADERS})
target_include_directories(raytracer PRIVATE include third_party)
target_link_libraries(raytracer PRIVATE tinygraphics)

#set_target_properties(raytracer PROPERTIES FOLDER "unity/core/modules/freedom/representations")
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${HEADERS})

# enable multiprocessor compilation
if (MSVC)
    target_compile_options(raytracer PRIVATE /MP)
endif()

# copy images directory to output directory after build
add_custom_command(TARGET raytracer POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/images
    $<TARGET_FILE_DIR:raytracer>/images
)

install(TARGETS raytracer RUNTIME DESTINATION .)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/images DESTINATION .)

# create package
set(CPACK_PACKAGE_NAME "raytracer")
set(CPACK_PACKAGE_VERSION "0.1.0")
set(CPACK_GENERATOR "ZIP")

include(CPack)
