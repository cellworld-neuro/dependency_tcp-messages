cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 20)

project(tcp_messages
        VERSION 2019.1.0
        DESCRIPTION "create json services easily"
        LANGUAGES CXX)

find_package(Dependencies)

string(APPEND CMAKE_CXX_FLAGS " -fno-strict-aliasing -pthread ")

set (tcp_messages_files
        src/message.cpp
        src/message_client.cpp
        src/message_router.cpp
        src/message_server.cpp
        src/message_event.cpp
        src/message_list.cpp)

install_git_dependency(Json-cpp
        https://github.com/germanespinosa/json-cpp
        PRIVATE
        CMAKE_PROJECT
        IMPORT_TARGETS
        json-cpp
        CONNECTED)

add_library(tcp_messages ${tcp_messages_files})

install_git_dependency(Easy-tcp https://github.com/germanespinosa/easy-tcp
        CMAKE_PROJECT
        CONNECTED
        TARGET tcp_messages)

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

include_directories(include)
include_directories(${dependencies_folder}/easy-tcp/include)

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
#
#test_library(tcp_messages catchtests/message.cpp)
#
#test_library(tcp_messages catchtests/long_message.cpp)
#

###
### LIBRARY INSTALLATION
###

include(GNUInstallDirs)

install(TARGETS tcp_messages EXPORT Tcp_messagesConfig
        ARCHIVE  DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY  DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME  DESTINATION ${CMAKE_INSTALL_BINDIR})

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(EXPORT Tcp_messagesConfig
        DESTINATION ${CMAKE_INSTALL_DATADIR}/cmake/Tcp_messages
        EXPORT_LINK_INTERFACE_LIBRARIES)

export(TARGETS tcp_messages FILE Tcp_messagesConfig.cmake)
