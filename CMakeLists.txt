cmake_minimum_required(VERSION 3.10)
project("Snake game")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/out)


# Función para incluir dependencias
include(FetchContent)
function(include_dependency libName gitURL gitTag)
    FetchContent_Declare(${libName}
        GIT_REPOSITORY ${gitURL}
        GIT_TAG        ${gitTag}
        GIT_SHALLOW    TRUE
        GIT_PROGRESS   TRUE
    )
    FetchContent_MakeAvailable(${libName})
endfunction()


# # Limpiar el directorio de salida antes de compilar
# file(GLOB OLD_BINARIES "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/*")

# foreach(file ${OLD_BINARIES})
#     file(REMOVE_RECURSE ${file})
# endforeach()


# MAIN APP

# Recoger todos los .cpp de la raíz y crear el ejecutable
file(GLOB SOURCES CONFIGURE_DEPENDS src/*.cpp)
add_executable(snake ${SOURCES})

# Copiar carpeta res al directorio de salida (out/)
add_custom_command(TARGET snake POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/res
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/res
)

# DEPS

# Buscar o descargar SDL3 y linkear
find_package(SDL3 QUIET)
if (NOT SDL3_FOUND)
    message(STATUS "Getting SDL3 from Github")
    include_dependency(SDL3 https://github.com/libsdl-org/SDL.git release-3.2.0)
else()
    message(STATUS "Using local SDL3")
endif()

target_link_libraries(snake PRIVATE SDL3::SDL3-shared)

# Buscar o descargar SDL3_ttf y linkear
find_package(SDL3_ttf QUIET)
if (NOT SDL3_ttf_FOUND)
    message(STATUS "Getting SDL3_ttf from Github")
    include_dependency(SDL3_ttf https://github.com/libsdl-org/SDL_ttf.git release-3.2.0)
else()
    message(STATUS "Using local SDL3_ttf")
endif()

target_link_libraries(snake PRIVATE SDL3_ttf::SDL3_ttf)



# Mostrar consola en Windows
if (WIN32 AND MINGW)
    target_link_options(snake PRIVATE "-mconsole")
endif()