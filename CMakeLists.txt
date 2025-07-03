cmake_minimum_required(VERSION 3.10)
project("Snake")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/out)
set(EXECUTABLE_NAME ${PROJECT_NAME})

# set(CMAKE_SYSTEM_NAME Windows)

set(RESOURCES_ROOT "${CMAKE_SOURCE_DIR}/res")
set(OUTPUT_DIR "${CMAKE_SOURCE_DIR}/out/$<CONFIG>")

# MAIN APP
file(GLOB_RECURSE SOURCES "src/main.cpp")
add_executable(${EXECUTABLE_NAME} WIN32 ${SOURCES})

# DEPS
# if (LINUX)
#     SET(SDL_IMAGE_SHARED_PATH "${CMAKE_SOURCE_DIR}/SDL/SDL_image/libSDL3_image.so.0")
#     SET(SDL_TTF_SHARED_PATH "${CMAKE_SOURCE_DIR}/SDL/SDL_ttf/libSDL3_ttf.so.0")
#     SET(SDL_SHARED_PATH "${CMAKE_SOURCE_DIR}/SDL/SDL/libSDL3.so.0")
# else()
#     SET(SDL_IMAGE_SHARED_PATH "${CMAKE_SOURCE_DIR}/SDL/SDL_image/Release/SDL3_image.dll")
#     SET(SDL_TTF_SHARED_PATH "${CMAKE_SOURCE_DIR}/SDL/SDL_ttf/Release/SDL3_ttf.dll")
#     SET(SDL_SHARED_PATH "${CMAKE_SOURCE_DIR}/SDL/SDL/Release/SDL3.dll")
# endif()


# SDL config
add_subdirectory(lib/SDL EXCLUDE_FROM_ALL)

# SDL_ttf config
set(SDLTTF_VENDORED ON) # tell SDL_ttf to build its own dependencies
add_subdirectory(lib/SDL_ttf EXCLUDE_FROM_ALL)

# SDL_mixer config
set(SDLMIXER_VENDORED ON)   # tell SDL_mixer to build its own dependencies
add_subdirectory(lib/SDL_mixer EXCLUDE_FROM_ALL)

# # SDL_image config
# set(SDLIMAGE_VENDORED ON)  # tell SDL_image to build its own dependencies
# add_subdirectory(lib/SDL_image EXCLUDE_FROM_ALL)

target_link_libraries(${EXECUTABLE_NAME} PRIVATE
    SDL3_ttf::SDL3_ttf
    SDL3_mixer::SDL3_mixer
    SDL3_image::SDL3_image
    SDL3::SDL3
)

# SDL_Image bug: https://github.com/libsdl-org/SDL_image/issues/506
if (APPLE AND NOT BUILD_SHARED_LIBS)
    find_library(IO_LIB ImageIO REQUIRED)
    find_library(CS_LIB CoreServices REQUIRED)
    find_library(CT_LIB CoreText REQUIRED)
    find_library(CG_LIB CoreGraphics REQUIRED)
    find_library(CF_LIB CoreFoundation REQUIRED)
    target_link_libraries(${EXECUTABLE_NAME} PUBLIC ${CF_LIB} ${CT_LIB} ${IO_LIB} ${CS_LIB} ${CG_LIB})
endif()
# target_compile_definitions(${EXECUTABLE_NAME} PUBLIC SDL_MAIN_USE_CALLBACKS)

# Mostrar consola en Windows
# if (WIN32 AND MINGW)
#     target_link_options(${EXECUTABLE_NAME} "-mconsole")
# endif()


# Copiar carpeta res al directorio de salida (out/)
add_custom_target(copyRes
	COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different 
        "${RESOURCES_ROOT}"
        "${OUTPUT_DIR}/res"
    COMMENT "Copying assets"
)
add_dependencies(${EXECUTABLE_NAME} copyRes)
# add_custom_command(TARGET ${EXECUTABLE_NAME} POST_BUILD
#     COMMAND ${CMAKE_COMMAND} -E copy ${RESOURCES_ROOT} ${OUTPUT_DIR}
# )