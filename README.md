# Sobre este repositorio

Un simple videojuego de la serpiente para aprender a usar SDL3 y C++.

Al final he aprendido mas a compilar (incompatibilidades y dolores de cabeza) que el propio usar SDL pero por la parte de escribir código ha estado entretenido.

# Creditos

- `res/sounds/eat.mp3`: https://pixabay.com/sound-effects/game-eat-sound-83240/
- `res/sounds/gameOver.mp3`: https://pixabay.com/sound-effects/game-over-arcade-6435/
- Roboto Mono font
- Simple DirectMedia Layer (SDL)
- Emscriptem (Versión web)

# dependencias

- emscripten (versión wasm)
- vcpkg
- SDL_mixer clonado
- Los compiladores que hacen falta (cmake y todo eso)...

### Versión windows
```
git submodule update --init --recursive
mkdir build
cd build
cmake ..
cmake --build .
```

### Versión web
git submodule update --init --recursive
mkdir build
cd build
emcmake cmake ..
emmake make