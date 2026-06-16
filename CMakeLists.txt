cmake_minimum_required(VERSION 3.30)
find_program(CMAKE_C_COMPILER NAMES $ENV{CC} gcc PATHS ENV PATH NO_DEFAULT_PATH)
find_program(CMAKE_CXX_COMPILER NAMES $ENV{CXX} g++ PATHS ENV PATH NO_DEFAULT_PATH)

include(FetchContent)

set(PROJECT_NAME mei-hua)

project(${PROJECT_NAME} LANGUAGES C CXX)

# Make sure all the dependencies are satisfied
find_program(GIT git REQUIRED)


if(NOT EXISTS "${CMAKE_CURRENT_LIST_DIR}/dependencies/SDL/src")
    message(NOTICE "sdl3 source not found")
    message(NOTICE "initializing/updating the sdl submodule...")

    execute_process(
        COMMAND git submodule update --init dependencies/SDL
        WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
        COMMAND_ERROR_IS_FATAL ANY
    )
endif()

# Ensure flecs submodule has been updated
if(NOT EXISTS "${CMAKE_CURRENT_LIST_DIR}/dependencies/flecs/src")
    message(NOTICE "flecs source not found")
    message(NOTICE "initializing/updating the flecs submodule...")

    execute_process(
        COMMAND git submodule update --init dependencies/flecs
        WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
        COMMAND_ERROR_IS_FATAL ANY
    )
endif()

# Ensure sdl3 webgpu submodule has been updated
if(NOT EXISTS "${CMAKE_CURRENT_LIST_DIR}/dependencies/sdl3webgpu/src")
    message(NOTICE "sdl3webgpu source not found")
    message(NOTICE "initializing/updating the sdl3 webgpu submodule...")

    execute_process(
        COMMAND git submodule update --init dependencies/sdl3webgpu
        WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
        COMMAND_ERROR_IS_FATAL ANY
    )
endif()

if (WIN32)
  set(WGPU_URL "https://github.com/gfx-rs/wgpu-native/releases/download/v29.0.0.0/wgpu-native-windows-x86_64-release.zip")
elseif(APPLE)
  set(WGPU_URL "https://github.com/gfx-rs/wgpu-native/releases/download/v29.0.0.0/wgpu-native-macos-x86_64-release.zip")
else()
  set(WGPU_URL "https://github.com/gfx-rs/wgpu-native/releases/download/v29.0.0.0/wgpu-linux-x86_64-release.zip")
endif()

FetchContent_Declare(wgpu_native URL ${WGPU_URL})

FetchContent_MakeAvailable(wgpu_native)

add_library(wgpu_native SHARED IMPORTED)

if(WIN32)
    set(WGPU_LIB "${wgpu_native_SOURCE_DIR}/lib/wgpu_native.dll")
    set(WGPU_IMPLIB "${wgpu_native_SOURCE_DIR}/bin/wgpu_native.lib")
elseif(APPLE)
    set(WGPU_LIB "${wgpu_native_SOURCE_DIR}/lib/libwgpu_native.dylib")
else()
    set(WGPU_LIB "${wgpu_native_SOURCE_DIR}/lib/libwgpu_native.so")
endif()

set_target_properties(wgpu_native PROPERTIES
    IMPORTED_LOCATION "${WGPU_LIB}"
)

if(WIN32)
    set_target_properties(wgpu_native PROPERTIES
        IMPORTED_IMPLIB "${WGPU_IMPLIB}"
    )
endif()





set(CMAKE_C_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")
if(CMAKE_EXPORT_COMPILE_COMMANDS)
  set(CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES 
      ${CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES})
endif()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${BUILD_PATH}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${BUILD_PATH}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${BUILD_PATH}")

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c*")
file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h*")

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_link_libraries(${PROJECT_NAME} PRIVATE wgpu_native)

add_subdirectory(dependencies/SDL SYSTEM)
add_subdirectory(dependencies/flecs SYSTEM)
add_subdirectory(dependencies/sdl3webgpu SYSTEM)

target_include_directories(sdl3webgpu PUBLIC
    ${wgpu_native_SOURCE_DIR}/include
)

target_link_libraries(sdl3webgpu PUBLIC wgpu_native)

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${SDL_SOURCE_DIR}/cmake")
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${flecs_SOURCE_DIR}/cmake")
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${sdl3webgpu_SOURCE_DIR}/cmake")

target_link_libraries(${PROJECT_NAME} PRIVATE SDL)
target_link_libraries(${PROJECT_NAME} PRIVATE flecs)
target_link_libraries(${PROJECT_NAME} PRIVATE sdl3webgpu)


target_include_directories(${PROJECT_NAME}
    PUBLIC
    
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/dependencies
)