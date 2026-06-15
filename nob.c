#include <stdlib.h>

#define NOB_IMPLEMENTATION
#include "nob.h"

// Some folder paths that we use throughout the build process
#define BUILD_FOLDER "build/"
#define SRC_FOLDER   "src/"


#define SDL3_DIR ""
#define WGPU_DIR "dependecies/WebGPU/linux/"

// Set to the header dirs
#define SDL3_INCLUDE_DIR ""
#define WGPU_INCLUDE_DIR WGPU_DIR "include/"

// Set to the directory containing the library
#define SDL3_LINK_DIR ""
#define WGPU_LINK_DIR WGPU_DIR "lib/"

#ifdef __APPLE__
#include "build_system/macos_build_util.h"
#endif


void include_libraies(Nob_Cmd* p_cmd) {
    nob_cmd_append(p_cmd, "-I", "dependecies");
    nob_cmd_append(p_cmd, "-I", SDL3_INCLUDE_DIR);
    nob_cmd_append(p_cmd, "-I", WGPU_INCLUDE_DIR);
}

void link_libraries(Nob_Cmd* p_cmd) {
#ifdef __APPLE__
    {
        macos_link_libraries(p_cmd);
        return;
    }
#endif

    {
        nob_cmd_append(p_cmd, "-L" SDL3_LINK_DIR);
        nob_cmd_append(p_cmd, "-lSDL3");

        nob_cmd_append(p_cmd, "-L" WGPU_LINK_DIR);
        nob_cmd_append(p_cmd, WGPU_LINK_DIR);
    }
}

void add_source_files(Nob_Cmd* p_cmd) {
    nob_cc_inputs(p_cmd, SRC_FOLDER "main.c");

#ifdef __APPLE__
    macos_add_source_files(p_cmd);
#endif
}

void install_app() {
#ifdef __APPLE__
    macos_install_app();
#endif
}

int build_desktop() {
    // The working horse of nob is the Nob_Cmd structure. It's a Dynamic Array of strings which represent
    // command line that you want to execute.
    Nob_Cmd cmd = {0};

    nob_cc(&cmd);
    nob_cc_flags(&cmd);

    include_libraies(&cmd);

    nob_cc_output(&cmd, BUILD_FOLDER "main");

    add_source_files(&cmd);

    link_libraries(&cmd);

    // Let's execute the command.
    if (!nob_cmd_run(&cmd)) return EXIT_FAILURE;
    // nob_cmd_run() automatically resets the cmd array, so you can nob_cmd_append() more strings
    // into it.

    install_app();

    return EXIT_SUCCESS;
}

int main(int argc, char **argv)
{
    NOB_GO_REBUILD_URSELF(argc, argv);

    if (!nob_mkdir_if_not_exists(BUILD_FOLDER)) return EXIT_FAILURE;
    
    if (build_desktop() != EXIT_SUCCESS) {
        return EXIT_FAILURE;
    }

    printf("BUILD SUCCEEDED!\n");

    return EXIT_SUCCESS;
}
