# Sobre este repositorio

Un simple videojuego de la serpiente para aprender a usar SDL3 y C++.
Puedes probarlo en [Itch.io](https://abdedarghal111.itch.io/sdl3-snake).

Aviso: el proyecto está repleto de malas prácticas de C++.

Al final he aprendido mas a compilar (incompatibilidades y dolores de cabeza) que el propio usar SDL pero por la parte de escribir código ha estado entretenido.

# Creditos

- `res/sounds/eat.mp3`: https://pixabay.com/sound-effects/game-eat-sound-83240/
- `res/sounds/gameOver.mp3`: https://pixabay.com/sound-effects/game-over-arcade-6435/
- Roboto Mono font
- Simple DirectMedia Layer (SDL)
- Emscriptem (Para la versión web)

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
```
git submodule update --init --recursive
mkdir build
cd build
emcmake cmake ..
emmake make
# en mi caso "emmake make" no me ha funcionado por alguna razón, he usado "make" directamente.
```