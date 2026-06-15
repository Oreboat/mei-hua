#include <stdlib.h>

#define NOB_IMPLEMENTATION
#include "nob.h"

// Some folder paths that we use throughout the build process.
#define BUILD_FOLDER "build/"
#define SRC_FOLDER   "src/"



int include_libraies(Nob_Cmd* p_cmd) {
#ifdef __APPLE__
    nob_cmd_append(p_cmd, "-I", "/Users/human/Documents/dev/GitHub/mei-hua/dependecies/SDL3/macos/SDL3/");
#endif

    return EXIT_SUCCESS;
}

int link_libraries(Nob_Cmd* p_cmd) {
#ifdef __APPLE__
    nob_cmd_append(p_cmd, "-F", "dependecies/SDL3/macos/SDL3.xcframework/");
    nob_cmd_append(p_cmd, "-framework", "SDL3");
    return EXIT_SUCCESS;
#endif

    //nob_cmd_append(p_cmd, "-Lpath");
    nob_cmd_append(p_cmd, "-lSDL3");

    return EXIT_SUCCESS;
}

int build_desktop() {
    // The working horse of nob is the Nob_Cmd structure. It's a Dynamic Array of strings which represent
    // command line that you want to execute.
    Nob_Cmd cmd = {0};

    nob_cc(&cmd);
    nob_cc_flags(&cmd);

    include_libraies(&cmd);

    //nob_cmd_append(&cmd, "-I.", "-I" BUILD_FOLDER, "-I" SRC_BUILD_FOLDER); // -I is usually the same across all compilers
    nob_cc_output(&cmd, BUILD_FOLDER "main");
    nob_cc_inputs(&cmd, SRC_FOLDER "main.c");

    link_libraries(&cmd);

#ifdef __APPLE__
    // Necessary to use SDL3, at least on macOS
    nob_cmd_append(&cmd, "-Wl,-rpath,@executable_path");
#endif

    // Let's execute the command.
    if (!nob_cmd_run(&cmd)) return EXIT_FAILURE;
    // nob_cmd_run() automatically resets the cmd array, so you can nob_cmd_append() more strings
    // into it.

    return EXIT_SUCCESS;
}

int main(int argc, char **argv)
{
    NOB_GO_REBUILD_URSELF(argc, argv);

    if (!nob_mkdir_if_not_exists(BUILD_FOLDER)) return EXIT_FAILURE;
    
    if (!build_desktop()) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
