file(GLOB_RECURSE EDITOR_SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp"
)

add_executable(HellixEditor ${EDITOR_SOURCES})

target_link_libraries(HellixEditor PRIVATE HellixCore)