#!/usr/bin/env sh

if [ "$OS" = "Windows_NT" ]; then
    CMAKE_GENERATOR='-G "MinGW Makefiles"'
else
    CMAKE_GENERATOR=''
fi

CMAKE_FLAGS="$CMAKE_GENERATOR \
-DCMAKE_C_COMPILER=clang \
-DCMAKE_CXX_COMPILER=clang++ \
-DCMAKE_EXPORT_COMPILE_COMMANDS=ON"

case "${1:-build}" in
    build)
        eval cmake -B build $CMAKE_FLAGS
        cmake --build build
        ;;
    run)
        eval cmake -B build $CMAKE_FLAGS
        cmake --build build
        ./.build/my_program
        ;;
    config)
        eval cmake -B build $CMAKE_FLAGS
        ;;
    *)
        echo "Usage: $0 {build|run|config}"
        exit 1
        ;;
esac