cmake_minimum_required(VERSION 3.30)

set(SDL_DEPS_SHARED FALSE CACHE BOOL "Build SDL dependencies as shared libraries")
set(BIN_DIR "${CMAKE_SOURCE_DIR}/bin")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${BIN_DIR}")


include(FetchContent)

set(PROJECT_NAME mei-hua)

project(${PROJECT_NAME} LANGUAGES C CXX)

# Make sure all the dependencies are satisfied
find_program(GIT git REQUIRED)


message(STATUS "Compiler ID: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "Frontend: ${CMAKE_CXX_COMPILER_FRONTEND_VARIANT}")
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "Linker: ${CMAKE_LINKER}")

if(WIN32)
    if(CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "MSVC")
        set(WinCompiler "msvc")
        set(WinExt "lib")
        set(WinFron "")
    else()
        set(WinCompiler "gnu")
        set(WinExt "a")
        set(WinFron "lib")
    endif()
    set(WGPU_URL "https://github.com/gfx-rs/wgpu-native/releases/download/v29.0.0.0/wgpu-windows-x86_64-${WinCompiler}-release.zip")
elseif(APPLE)
  set(WGPU_URL "https://github.com/gfx-rs/wgpu-native/releases/download/v29.0.0.0/wgpu-macos-aarch64-release.zip")
else()
  set(WGPU_URL "https://github.com/gfx-rs/wgpu-native/releases/download/v29.0.0.0/wgpu-linux-x86_64-release.zip")
endif()

FetchContent_Declare(wgpu_native URL ${WGPU_URL})
FetchContent_Declare(
    sdl3
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG release-3.4.10
)
FetchContent_Declare(
  sdl3webgpu
  GIT_REPOSITORY https://github.com/eliemichel/sdl3webgpu.git
  GIT_TAG v1.0.0
)

FetchContent_Declare(
    flecs
    GIT_REPOSITORY https://github.com/SanderMertens/flecs.git
    GIT_TAG v4.1.5
)

FetchContent_MakeAvailable(flecs)
FetchContent_MakeAvailable(wgpu_native)
add_library(wgpu_native STATIC IMPORTED)
add_library(webgpu ALIAS wgpu_native)
FetchContent_MakeAvailable(sdl3)
FetchContent_MakeAvailable(sdl3webgpu)


if(WIN32)
    set(WGPU_LIB "${wgpu_native_SOURCE_DIR}/lib/${WinFron}wgpu_native.${WinExt}")
elseif(APPLE)
    set(WGPU_LIB "${wgpu_native_SOURCE_DIR}/lib/libwgpu_native.a")
else()
    set(WGPU_LIB "${wgpu_native_SOURCE_DIR}/lib/libwgpu_native.a")
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
  set(CMAKE_C_STANDARD_INCLUDE_DIRECTORIES
      ${CMAKE_C_IMPLICIT_INCLUDE_DIRECTORIES})
endif()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${BUILD_PATH}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${BUILD_PATH}")

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c*")
file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h*")

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_link_libraries(${PROJECT_NAME} PRIVATE webgpu)

# add_subdirectory(dependencies/SDL SYSTEM)
# add_subdirectory(dependencies/sdl3webgpu SYSTEM)

target_include_directories(sdl3webgpu PUBLIC
   ${wgpu_native_SOURCE_DIR}/include
)

target_link_libraries(sdl3webgpu PUBLIC webgpu)

# set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${SDL_SOURCE_DIR}/cmake")
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${flecs_SOURCE_DIR}/cmake")
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${sdl3webgpu_SOURCE_DIR}/cmake")

if(UNIX AND NOT APPLE)
    target_link_libraries(${PROJECT_NAME} PRIVATE m)
endif()
target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3)
target_link_libraries(${PROJECT_NAME} PRIVATE flecs)
target_link_libraries(${PROJECT_NAME} PRIVATE sdl3webgpu)

if (MSVC)
    target_link_libraries(${PROJECT_NAME} PRIVATE
        ntdll
        userenv
        bcrypt
        ws2_32
    )
endif()

target_include_directories(${PROJECT_NAME}
    PUBLIC
    ${wgpu_native_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/dependencies
)

