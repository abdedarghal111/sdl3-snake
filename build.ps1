param (
    [switch]$Release
)
# colocar -Release para compilar sin debug

# Guarda el path original
Push-Location

# Define ruta de build
$buildDir = "build"
if (-not (Test-Path $buildDir)) {
    New-Item -ItemType Directory -Path $buildDir | Out-Null
}

# Entra en build temporalmente
Set-Location $buildDir

# Detectar tipo de build
$buildType = if ($Release) { "Release" } else { "Debug" }

# Ejecutar cmake solo si no está configurado
cmake .. -DCMAKE_TOOLCHAIN_FILE="../mingw-toolchain.cmake" -DCMAKE_BUILD_TYPE=$buildType -Wno-dev

# Compilar
cmake --build . --config $buildType

# Volver al directorio original
Pop-Location
