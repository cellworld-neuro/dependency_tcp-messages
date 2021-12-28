cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 20)

project(tcp_messages
        VERSION 2019.1.0
        DESCRIPTION "create json services easily"
        LANGUAGES CXX)

find_package(Dependencies)

install_dependency(https://github.com/germanespinosa/json-cpp Json-cpp)
install_dependency(https://github.com/germanespinosa/easy-tcp Easy-tcp)

string(APPEND CMAKE_CXX_FLAGS " -fno-strict-aliasing -pthread ")

set (tcp_messages_files
        src/message.cpp
        src/message_client.cpp
        src/message_router.cpp
        src/message_server.cpp)

add_library(tcp_messages ${tcp_messages_files})

set_target_properties(tcp_messages
        PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED On
        CXX_EXTENSIONS Off
        VERSION ${PROJECT_VERSION})

if(MSVC)
    add_compile_options(/W4)
else(MSVC)
    add_compile_options(-Wall -Wextra -pedantic)
endif(MSVC)

dependency_include(include)
dependency_include(${dependencies_folder}/easy-tcp/include)

target_include_directories(tcp_messages
        SYSTEM INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>)

target_include_directories(tcp_messages
        PRIVATE
        include )

target_link_libraries(tcp_messages
        PUBLIC
        json-cpp
        easy-tcp)

####
#### TESTS
####

#install_dependency(https://github.com/germanespinosa/catch CatchTests)
#test_library(tcp_messages)