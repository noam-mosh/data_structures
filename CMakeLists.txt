cmake_minimum_required(VERSION 3.0.0)
set(CMAKE_CXX_STANDARD 11)

project(<PROJNAME> VERSION 0.1.0 LANGUAGES CXX)
set(DS_FLAGS_DEBUG "-std=c++11 --pedantic-errrs -wall -werror")
set(DS "${DS_FLAGS_DEBUG} -DNDEBUG")

set(CMAKE_CPP_FLAGS ${DS_FLAGS_DEBUG})
set(CPP_FILES)
add_executable(project main2.cpp PlayerManager.cpp library2.cpp)