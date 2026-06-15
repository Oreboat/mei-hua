#include <stdlib.h>

#define NOB_IMPLEMENTATION
#include "nob.h"

// Some folder paths that we use throughout the build process.
#define BUILD_FOLDER "build/"
#define SRC_FOLDER   "src/"

#ifdef __APPLE__
#define SDL3_INCLUDE_DIR "dependecies/SDL3/macos/SDL3/"
#define SDL3_LINK_DIR "dependecies/SDL3/macos/SDL3.xcframework/macos-arm64_x86_64/"
#define SDL3_FRAMEWORK_BASENAME "SDL3.framework/"
#define SDL3_FRAMEWORK_DIR SDL3_LINK_DIR SDL3_FRAMEWORK_BASENAME

#define WGPU_INCLUDE_DIR "dependecies/WebGPU/macos/include/"
#define WGPU_LINK_DIR "dependecies/WebGPU/macos/lib/"
#define WGPU_LINK_DYLIB_BASENAME "libwgpu_native.dylib"
#define WGPU_LINK_DYLIB "dependecies/WebGPU/macos/lib/" WGPU_LINK_DYLIB_BASENAME
#endif

int include_libraies(Nob_Cmd* p_cmd) {
    nob_cmd_append(p_cmd, "-I", "dependecies");

    nob_cmd_append(p_cmd, "-I", SDL3_INCLUDE_DIR);
    nob_cmd_append(p_cmd, "-I", WGPU_INCLUDE_DIR);

    return EXIT_SUCCESS;
}

int link_libraries(Nob_Cmd* p_cmd) {
#ifdef __APPLE__
    nob_cmd_append(p_cmd, "-F" SDL3_LINK_DIR);
    nob_cmd_append(p_cmd, "-framework", "SDL3");

    //nob_cmd_append(p_cmd, "-Lpath");
    nob_cmd_append(p_cmd, WGPU_LINK_DYLIB);

    // Link sdl3webgpu glue
    //nob_cmd_append(p_cmd, "-x", "objective-c");
    nob_cmd_append(p_cmd, "-framework", "Cocoa", "-framework", "CoreVideo", "-framework", "IOKit", "-framework", "QuartzCore", "-framework", "Metal");

    //nob_cmd_append(p_cmd, "-x", "objective-c", "-framework", "Cocoa", "-framework", "CoreVideo", "-framework", "IOKit", "-framework", "QuartzCore");
    //-x objective-c and the link libraries -framework Cocoa, -framework CoreVideo, -framework IOKit, and -framework QuartzCore

    return EXIT_SUCCESS;
#endif

    //nob_cmd_append(p_cmd, "-Lpath");
    nob_cmd_append(p_cmd, "-lSDL3");

    return EXIT_SUCCESS;
}

int add_source_files(Nob_Cmd* p_cmd) {
    nob_cc_inputs(p_cmd, SRC_FOLDER "main.c");

#ifdef __APPLE__
    nob_cc_inputs(p_cmd, SRC_FOLDER "platform/macos/" "objc_dummy.m");
#endif

    return EXIT_SUCCESS;
}

int build_desktop() {
    // The working horse of nob is the Nob_Cmd structure. It's a Dynamic Array of strings which represent
    // command line that you want to execute.
    Nob_Cmd cmd = {0};

    nob_cc(&cmd);
    nob_cc_flags(&cmd);

#ifdef __APPLE__
    //nob_cmd_append(&cmd, "-x", "objective-c");
    //nob_cmd_append(&cmd, "-fobjc-arc");
    //nob_cmd_append(&cmd, "-ObjC");
#endif

    include_libraies(&cmd);

    nob_cc_output(&cmd, BUILD_FOLDER "main");

    add_source_files(&cmd);

    link_libraries(&cmd);

#ifdef __APPLE__
    // Necessary to use SDL3, at least on macOS
    nob_cmd_append(&cmd, "-Wl,-rpath,@executable_path");
#endif

    // Let's execute the command.
    if (!nob_cmd_run(&cmd)) return EXIT_FAILURE;
    // nob_cmd_run() automatically resets the cmd array, so you can nob_cmd_append() more strings
    // into it.

#ifdef __APPLE__
    if (!nob_file_exists(BUILD_FOLDER SDL3_FRAMEWORK_BASENAME)) {
        nob_copy_directory_recursively(SDL3_FRAMEWORK_DIR, BUILD_FOLDER SDL3_FRAMEWORK_BASENAME);
    }
    nob_copy_file(WGPU_LINK_DYLIB, BUILD_FOLDER WGPU_LINK_DYLIB_BASENAME);
#endif

    return EXIT_SUCCESS;
}

int main(int argc, char **argv)
{
    NOB_GO_REBUILD_URSELF(argc, argv);

    if (!nob_mkdir_if_not_exists(BUILD_FOLDER)) return EXIT_FAILURE;
    
    if (build_desktop() != EXIT_SUCCESS) {
        return EXIT_FAILURE;
    }

    printf("BUILD SUCCESS\n");

    return EXIT_SUCCESS;
}
