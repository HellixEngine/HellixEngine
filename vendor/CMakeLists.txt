set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

# Desativa documentação, testes e exemplos desnecessários do GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/GLFW/CMakeLists.txt")
    add_subdirectory(GLFW)
endif()

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/PYBIND11/CMakeLists.txt")
    add_subdirectory(PYBIND11)
endif()

# Glad / OpenGL Loader (procura arquivos .c em vendor/GLAD)
file(GLOB OPENGL_SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/GLAD/src/*.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/GLAD/*.c"
)

if(OPENGL_SOURCES)
    add_library(glad STATIC ${OPENGL_SOURCES})
    target_include_directories(glad PUBLIC
            "${CMAKE_CURRENT_SOURCE_DIR}/GLAD/include"
            "${CMAKE_CURRENT_SOURCE_DIR}/GLAD"
    )
endif()

# ImGui Docking + Backends para GLFW e OpenGL3
file(GLOB IMGUI_SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/IMGUIDOCKING/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/IMGUIDOCKING/backends/imgui_impl_glfw.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/IMGUIDOCKING/backends/imgui_impl_opengl3.cpp"
)

if(IMGUI_SOURCES)
    add_library(imgui STATIC ${IMGUI_SOURCES})
    target_include_directories(imgui PUBLIC
            "${CMAKE_CURRENT_SOURCE_DIR}/IMGUIDOCKING"
            "${CMAKE_CURRENT_SOURCE_DIR}/IMGUIDOCKING/backends"
    )
    if(TARGET glfw)
        target_link_libraries(imgui PUBLIC glfw)
    endif()
endif()


# Declara uma biblioteca de interface (header-only)
add_library(spdlog INTERFACE)

# Expõe os cabeçalhos para qualquer target que faça link com spdlog
target_include_directories(spdlog INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/spdlog/include
)

# ============================================================
# NLOHMANN JSON - Header Only
# ============================================================

add_library(NLOHMANN INTERFACE)

target_include_directories(NLOHMANN INTERFACE
        "${CMAKE_CURRENT_SOURCE_DIR}/NLOHMANN/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/NLOHMANN/single_include"
)


# ============================================================
# STB - Header Only
# ============================================================

add_library(STB INTERFACE)

target_include_directories(STB INTERFACE
        "${CMAKE_CURRENT_SOURCE_DIR}/STB"
)

# ============================================================
# MINIAUDIO - Header Only
# ============================================================

add_library(MINIAUDIO INTERFACE)

target_include_directories(MINIAUDIO INTERFACE
        "${CMAKE_CURRENT_SOURCE_DIR}/MINIAUDIO"
)