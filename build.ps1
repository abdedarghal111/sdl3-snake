param (
    [switch]$Release,
    [switch]$NoRefresh
)
# colocar -Release para compilar sin debug
# colocar -NoRefresh para no ejecutar cmake de configuración

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

# Ejecutar cmake solo si NoRefresh no está presente
if (-not $NoRefresh) {
    cmake .. -DCMAKE_TOOLCHAIN_FILE="../mingw-toolchain.cmake" -DCMAKE_BUILD_TYPE=$buildType -Wno-dev
}

# Compilar
cmake --build . --config $buildType

# Volver al directorio original
Pop-Location
