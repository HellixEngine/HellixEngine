file(GLOB_RECURSE ENGINE_HEADERS
        "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp"
)

file(GLOB_RECURSE ENGINE_SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

add_library(HellixCore STATIC ${ENGINE_HEADERS} ${ENGINE_SOURCES})

# Headers públicos acessíveis por sandbox e editor
target_include_directories(HellixCore PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# Headers internos e dependências puras (header-only) privadas da engine
target_include_directories(HellixCore PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/vendor/STB
        ${CMAKE_SOURCE_DIR}/vendor/MINIAUDIO
        ${CMAKE_SOURCE_DIR}/vendor/NLOHMANN/include
        ${CMAKE_SOURCE_DIR}/vendor/NLOHMANN/single_include
)

find_package(OpenGL REQUIRED)

target_link_libraries(HellixCore PUBLIC
        OpenGL::GL
)

if(TARGET glad)
    target_link_libraries(HellixCore PUBLIC glad)
endif()

if(TARGET glfw)
    target_link_libraries(HellixCore PUBLIC glfw)
endif()

if(TARGET imgui)
    target_link_libraries(HellixCore PUBLIC imgui)
endif()

if(TARGET spdlog)
    target_link_libraries(HellixCore PUBLIC spdlog)
endif()