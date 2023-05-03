cmake_minimum_required(VERSION 3.26)

project(sm-workbench)

add_subdirectory(lib/sm-engine)

add_executable(sm-workbench WIN32 src/App.cpp
                                  src/MainWin.cpp
                                  )

target_include_directories(sm-workbench PUBLIC src
                                        PUBLIC lib/sm-engine/src)
target_link_libraries(sm-workbench lib/sm-engine/Debug/sm-engine)
