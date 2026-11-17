cmake_minimum_required(VERSION 3.16)
project(MyDatabase VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

# Flags
if(NOT MSVC)
    add_compile_options(-Wall -Wextra)
endif()

# Source files
set(SOURCES
    src/main.cpp
    src/core/database.cpp
    src/core/transaction.cpp
    src/storage/store.cpp
    src/storage/logger.cpp
    src/storage/index.cpp
    src/utils/command.cpp
    src/utils/method.cpp
    src/interactive/shell.cpp
)

# Executable target
add_executable(database ${SOURCES})

# Include directories
target_include_directories(database PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)