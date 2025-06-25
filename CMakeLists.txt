cmake_minimum_required(VERSION 3.10)
project("Snake game")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/out)

include(FetchContent)

# Función para incluir dependencias
function(include_dependency libName gitURL gitTag)
    FetchContent_Declare(${libName}
        GIT_REPOSITORY ${gitURL}
        GIT_TAG        ${gitTag}
        GIT_SHALLOW    TRUE
        GIT_PROGRESS   TRUE
    )
    FetchContent_MakeAvailable(${libName})
endfunction()

# Buscar o descargar SDL3
find_package(SDL3 QUIET)
if (NOT SDL3_FOUND)
    message(STATUS "Getting SDL3 from Github")
    include_dependency(SDL3 https://github.com/libsdl-org/SDL.git release-3.2.0)
else()
    message(STATUS "Using local SDL3")
endif()

# Recoger todos los .cpp de la raíz
file(GLOB SOURCES CONFIGURE_DEPENDS src/*.cpp)

# Crear el ejecutable
add_executable(snake ${SOURCES})

# Enlazar SDL3
target_link_libraries(snake PRIVATE SDL3::SDL3-shared)

# Mostrar consola en Windows
if (WIN32 AND MINGW)
    target_link_options(snake PRIVATE "-mconsole")
endif()
