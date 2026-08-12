cmake_minimum_required(VERSION 3.16)
project(r29_consumer LANGUAGES CXX)

find_package(PkgConfig REQUIRED)
pkg_check_modules(PCRE REQUIRED IMPORTED_TARGET libpcrecpp)
pkg_check_modules(RLOTTIE REQUIRED IMPORTED_TARGET rlottie)
find_package(flatbuffers CONFIG REQUIRED)

get_target_property(FLATBUFFERS_LOCATION flatbuffers::flatbuffers_shared IMPORTED_LOCATION_RELEASE)
get_target_property(FLATBUFFERS_INCLUDES flatbuffers::flatbuffers_shared INTERFACE_INCLUDE_DIRECTORIES)
message(STATUS "R29_PCRE_INCLUDE_DIRS=${PCRE_INCLUDE_DIRS}")
message(STATUS "R29_PCRE_LIBRARY_DIRS=${PCRE_LIBRARY_DIRS}")
message(STATUS "R29_RLOTTIE_INCLUDE_DIRS=${RLOTTIE_INCLUDE_DIRS}")
message(STATUS "R29_RLOTTIE_LIBRARY_DIRS=${RLOTTIE_LIBRARY_DIRS}")
message(STATUS "R29_FLATBUFFERS_LOCATION=${FLATBUFFERS_LOCATION}")
message(STATUS "R29_FLATBUFFERS_INCLUDES=${FLATBUFFERS_INCLUDES}")

add_executable(r29-pcre pcre.cpp)
target_link_libraries(r29-pcre PRIVATE PkgConfig::PCRE)

add_executable(r29-lottie lottie.cpp)
target_link_libraries(r29-lottie PRIVATE PkgConfig::RLOTTIE)

add_executable(r29-flatbuffers flatbuffers.cpp)
target_link_libraries(r29-flatbuffers PRIVATE flatbuffers::flatbuffers_shared)

install(TARGETS r29-pcre r29-lottie r29-flatbuffers RUNTIME DESTINATION bin)
