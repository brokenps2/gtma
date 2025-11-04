cmake_minimum_required(VERSION 3.15)
project(gtma C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_BUILD_TYPE Debug)

include(FetchContent)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

file(GLOB_RECURSE SRC_FILES ${CMAKE_SOURCE_DIR}/src/*.c)

include_directories(${CMAKE_SOURCE_DIR}/src)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_compile_options(-Wall -g -O2 -pthread)

add_executable(gtma ${SRC_FILES})

FetchContent_Declare(
  dr_wav
  GIT_REPOSITORY https://github.com/mackron/dr_libs.git
  GIT_TAG master
)
FetchContent_GetProperties(dr_wav)
if(NOT dr_wav_POPULATED)
  FetchContent_Populate(dr_wav)
  include_directories(${dr_wav_SOURCE_DIR})
endif()

FetchContent_Declare(
  cglm
  GIT_REPOSITORY https://github.com/recp/cglm.git
  GIT_TAG v0.9.4
)
FetchContent_MakeAvailable(cglm)

FetchContent_Declare(
  cgltf
  GIT_REPOSITORY https://github.com/jkuhlmann/cgltf.git
  GIT_TAG master
)
FetchContent_MakeAvailable(cgltf)

FetchContent_Declare(
  stb
  GIT_REPOSITORY https://github.com/nothings/stb.git
  GIT_TAG master
)
FetchContent_MakeAvailable(stb)

find_package(SDL2 REQUIRED)
find_package(GLEW REQUIRED)
find_package(cJSON REQUIRED)
find_package(OpenAL REQUIRED)

target_link_libraries(gtma
  m
  pthread
  GL
  cjson
  OpenAL::OpenAL
  SDL2::SDL2
  GLEW::GLEW
)

target_include_directories(gtma PRIVATE
  ${cglm_SOURCE_DIR}/include
  ${dr_wav_SOURCE_DIR}
  ${cgltf_SOURCE_DIR}
  ${stb_SOURCE_DIR}
)


set_target_properties(gtma PROPERTIES
  OBJECT_OUTPUTS ${CMAKE_SOURCE_DIR}/obj
)
