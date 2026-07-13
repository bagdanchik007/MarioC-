cmake_minimum_required(VERSION 3.16)
project(MarioClone LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- SFML wird direkt ins Projekt geholt, kein manuelles vcpkg/Download noetig ---
include(FetchContent)
FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG        2.6.1
    GIT_SHALLOW    ON
)

# Auf Windows/MSVC statisch linken ist meist am unkompliziertesten fuer Visual Studio.
set(SFML_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_DOC OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(SFML)

file(GLOB SOURCES ${CMAKE_SOURCE_DIR}/src/*.cpp)

add_executable(MarioClone ${SOURCES})

target_include_directories(MarioClone PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(MarioClone PRIVATE sfml-graphics sfml-window sfml-system)

target_compile_options(MarioClone PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra>
)

# Windows: SFML-DLLs (bei dynamischem Build) neben die .exe kopieren,
# damit "Debug starten" in Visual Studio ohne PATH-Gefrickel funktioniert.
if (WIN32)
    add_custom_command(TARGET MarioClone POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_RUNTIME_DLLS:MarioClone>
            $<TARGET_FILE_DIR:MarioClone>
        COMMAND_EXPAND_LISTS
    )
endif()
