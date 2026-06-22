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

MacOS, Windows, and Linux are all currently supported, however there are known issues when running on NixOS
# Usage

Build with CMakeLists:
```
cmake -B build
cmake --build ./build
```

Run:
```
Linux and MacOS
    bin/mei-hua
Windows
    bin/{debug or release}/mei-hua.exe
```

Build with build.sh
```
bash build.sh

//to run

bash build.sh run
```

# References

https://eliemichel.github.io/LearnWebGPU

https://github.com/niranjanbalachandra/sdl3_wgpu/blob/main/main.cpp

https://github.com/gfx-rs/wgpu-native/blob/trunk/examples/triangle/main.c

https://stackoverflow.com/questions/75997043/whats-the-difference-between-a-gpuadapter-and-gpudevice-in-the-webgpu-api
