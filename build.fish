#!/bin/fish

# Guardar el directorio actual
set original_dir (pwd)

# Opcional: comprobar si se pasa "--release"
set build_type "Debug"
for arg in $argv
    if test "$arg" = "--release"
        set build_type "Release"
    end
end

# Crear y entrar en build/
mkdir -p build
cd build

# Ejecutar cmake
cmake .. -DCMAKE_TOOLCHAIN_FILE=../mingw-toolchain.cmake -DCMAKE_BUILD_TYPE=$build_type

# Compilar
make

# Volver al directorio original
cd $original_dir
