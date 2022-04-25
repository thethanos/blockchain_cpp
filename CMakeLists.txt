cmake_minimum_required(VERSION 3.0.0)

project(blockchain)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(blockchain main.cpp)

include_directories(include)

aux_source_directory(source SOURCES)

target_sources(blockchain PRIVATE ${SOURCES})

find_package(Boost 1.70.0 COMPONENTS serialization program_options  REQUIRED)
include_directories(Boost_INCLUDE_DIRS)
target_link_libraries(blockchain PRIVATE ${Boost_LIBRARIES})

find_package(OpenSSL REQUIRED)
include_directories(OpenSSL_INCLUDE_DIR)
target_link_libraries(blockchain PRIVATE ${OPENSSL_LIBRARIES})

add_subdirectory(libs/leveldb)

include_directories(LEVELDB_PUBLIC_INCLUDE_DIR)
target_link_libraries(blockchain PRIVATE leveldb)