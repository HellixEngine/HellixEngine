cmake_minimum_required(VERSION 3.20)
project(HellixEngine VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

# Centraliza executáveis em /bin e bibliotecas estáticas em /lib
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

if(MINGW)
    add_link_options(-static-libgcc -static-libstdc++)
endif()

# 1. Dependências externas
add_subdirectory(vendor)

# 2. Núcleo da Engine
add_subdirectory(engine)

# 3. Executáveis clientes
add_subdirectory(sandbox)
add_subdirectory(editor)