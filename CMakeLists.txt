cmake_minimum_required(VERSION 3.10)
project("Snake")

include(/vcpkg/scripts/buildsystems/vcpkg.cmake)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/out)


set(EXECUTABLE_NAME ${PROJECT_NAME})
set(RESOURCES_ROOT "${CMAKE_SOURCE_DIR}/res")
set(OUTPUT_DIR "${CMAKE_SOURCE_DIR}/out/$<CONFIG>")

if(EMSCRIPTEN)
    set(BUILD_SHARED_LIBS OFF CACHE INTERNAL "")    # Disable shared builds on platforms where it does not make sense to use them
    set(SDL_SHARED OFF)
	set(CMAKE_EXECUTABLE_SUFFIX ".html" CACHE INTERNAL "")

    # Aquí añadimos la línea para pre-cargar los archivos
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s USE_SDL=3 -s USE_SDL_IMAGE=3 -s USE_SDL_TTF=3 -s USE_FREETYPE=1 -s USE_SDL_MIXER=3 -s USE_OGG=1 --preload-file ${CMAKE_SOURCE_DIR}/res@/res")

    # file(GLOB_RECURSE ASSET_FILES "${CMAKE_SOURCE_DIR}/res/*")  # Cambia "src/*" si tienes otros directorios

    # foreach(FILE ${ASSET_FILES})
    #     list(APPEND PRELOAD_FILES "--preload-file \"${FILE}@/\"")
    # endforeach()
endif()

# MAIN APP
file(GLOB_RECURSE SOURCES "src/main.cpp")
add_executable(${EXECUTABLE_NAME} ${SOURCES})

find_package(SDL3 CONFIG REQUIRED)
# include_directories(${SDL3_INCLUDE_DIRS})

set(SDLIMAGE_VENDORED ON)
find_package(SDL3_image CONFIG REQUIRED)

set(SDLTTF_VENDORED ON)
find_package(SDL3_ttf CONFIG REQUIRED)

# set(SDLMIXER_VENDORED ON)   # tell SDL_mixer to build its own dependencies
set(SDLMIXER_MIDI_NATIVE OFF)
set(SDLMIXER_GME OFF)
set(SDLMIXER_WAVPACK OFF)     
set(SDLMIXER_MOD OFF)
set(SDLMIXER_OPUS OFF)
set(SDLMIXER_OGG OFF)
add_subdirectory(SDL_mixer EXCLUDE_FROM_ALL)

target_link_libraries(${EXECUTABLE_NAME} PRIVATE
    SDL3_ttf::SDL3_ttf
    SDL3_mixer::SDL3_mixer
    $<IF:$<TARGET_EXISTS:SDL3_image::SDL3_image-shared>,SDL3_image::SDL3_image-shared,SDL3_image::SDL3_image-static>
    SDL3::SDL3
)

# target_compile_definitions(${EXECUTABLE_NAME} PUBLIC SDL_MAIN_USE_CALLBACKS)

if(!EMSCRIPTEN)
    # Copiar carpeta res al directorio de salida (out/)
    add_custom_target(copyRes
        COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different 
            "${RESOURCES_ROOT}"
            "${OUTPUT_DIR}/res"
        COMMENT "Copying assets"
    )

    add_dependencies(${EXECUTABLE_NAME} copyRes)
endif()