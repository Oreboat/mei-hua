# mei-hua

A toy game engine written in C based on SDL3 and WebGPU.

# Setup

Clone the repo and initialize submodules:
```
git submodule update --init --recursive
```

Link the dependencies:
- SDL3
- WebGPU

Only macOS is supported at the moment.

# Usage

Build with nob.h:
```
cc -o nob nob.c && ./nob
```

Run:
```
./build/main
```

# References

https://eliemichel.github.io/LearnWebGPU

https://github.com/niranjanbalachandra/sdl3_wgpu/blob/main/main.cpp
