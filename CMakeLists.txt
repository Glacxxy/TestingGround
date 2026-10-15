cmake_minimum_required(VERSION 3.22)
project(BaseScout LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

include(FetchContent)
FetchContent_Declare(
  preloader_android
  GIT_REPOSITORY https://github.com/LiteLDev/preloader-android.git
  GIT_TAG 0.2.2
  SOURCE_SUBDIR cmake/preloader-sdk
)
FetchContent_Declare(fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt.git
  GIT_TAG 11.2.0
)
set(FMT_DOC OFF CACHE BOOL "" FORCE)
set(FMT_TEST OFF CACHE BOOL "" FORCE)
set(FMT_INSTALL OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(preloader_android fmt)
FetchContent_GetProperties(preloader_android)
set(LEVI_PRELOADER_INCLUDE_DIR "${preloader_android_SOURCE_DIR}/include")

if(NOT ANDROID)
  message(STATUS "Configure this project with the Android NDK to build the .levipack.")
  return()
endif()

if(NOT CMAKE_ANDROID_ARCH_ABI STREQUAL "arm64-v8a")
  message(FATAL_ERROR "BaseScout currently targets arm64-v8a.")
endif()

if(NOT TARGET preloader)
  add_subdirectory("${preloader_android_SOURCE_DIR}" "${CMAKE_BINARY_DIR}/_preloader_android")
endif()

add_library(basescout SHARED src/BaseScoutMod.cpp)
target_include_directories(basescout PRIVATE "${LEVI_PRELOADER_INCLUDE_DIR}")
target_compile_options(basescout PRIVATE
  $<$<COMPILE_LANG_AND_ID:CXX,Clang,GNU>:-fvisibility=hidden -fvisibility-inlines-hidden>
)
target_link_options(basescout PRIVATE "-Wl,-z,max-page-size=16384")
find_library(log-lib log REQUIRED)
find_library(dl-lib dl REQUIRED)
target_link_libraries(basescout PRIVATE ${log-lib} ${dl-lib} fmt::fmt preloader)

if(CMAKE_STRIP)
  add_custom_command(TARGET basescout POST_BUILD
    COMMAND "${CMAKE_STRIP}" --strip-unneeded "$<TARGET_FILE:basescout>"
    VERBATIM
  )
endif()

set(PACKAGE_ROOT "${CMAKE_BINARY_DIR}/package")
set(PACKAGE_MOD_DIR "${PACKAGE_ROOT}/basescout")
file(MAKE_DIRECTORY "${PACKAGE_MOD_DIR}")
configure_file("${CMAKE_SOURCE_DIR}/manifest.json" "${PACKAGE_MOD_DIR}/manifest.json" COPYONLY)
add_custom_command(TARGET basescout POST_BUILD
  COMMAND "${CMAKE_COMMAND}" -E copy "$<TARGET_FILE:basescout>" "${PACKAGE_MOD_DIR}/libbasescout.so"
  COMMAND "${CMAKE_COMMAND}" -E tar cfv "${CMAKE_BINARY_DIR}/BaseScout.levipack" --format=zip -- basescout
  WORKING_DIRECTORY "${PACKAGE_ROOT}"
  COMMENT "Packaging BaseScout.levipack"
  VERBATIM
)
