cmake_minimum_required(VERSION 3.26)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/run)
set(ASSETS_DIR ${CMAKE_SOURCE_DIR}/assets)

project(sm-workbench)

add_subdirectory(lib/sm-engine)

add_executable(sm-workbench WIN32 src/App.cpp
                                  src/MainWin.cpp
                                  )

target_include_directories(sm-workbench PUBLIC src
                                        PUBLIC lib/sm-engine/src)

target_link_libraries(sm-workbench PUBLIC sm-engine)

set_target_properties(sm-workbench PROPERTIES 
                                   VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:sm-workbench>"
                                   VS_STARTUP_PROJECT sm-workbench
                                   )
                                    

# copy assets to final exe directory
add_custom_command(TARGET sm-workbench POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                   ${ASSETS_DIR} $<TARGET_FILE_DIR:sm-workbench>)
