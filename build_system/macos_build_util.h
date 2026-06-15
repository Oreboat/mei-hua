//#include "../nob.h"

#define SDL3_INCLUDE_DIR "dependecies/SDL3/macos/SDL3/"
#define SDL3_LINK_DIR "dependecies/SDL3/macos/SDL3.xcframework/macos-arm64_x86_64/"
#define SDL3_FRAMEWORK_BASENAME "SDL3.framework/"
#define SDL3_FRAMEWORK_DIR SDL3_LINK_DIR SDL3_FRAMEWORK_BASENAME

#define WGPU_INCLUDE_DIR "dependecies/WebGPU/macos/include/"
#define WGPU_LINK_DIR "dependecies/WebGPU/macos/lib/"
#define WGPU_LINK_DYLIB_BASENAME "libwgpu_native.dylib"
#define WGPU_LINK_DYLIB "dependecies/WebGPU/macos/lib/" WGPU_LINK_DYLIB_BASENAME


//nob_cmd_append(&cmd, "-x", "objective-c");
//nob_cmd_append(&cmd, "-fobjc-arc");
//nob_cmd_append(&cmd, "-ObjC");

void macos_link_libraries(Nob_Cmd* p_cmd) {
    nob_cmd_append(p_cmd, "-F" SDL3_LINK_DIR);
    nob_cmd_append(p_cmd, "-framework", "SDL3");

    //nob_cmd_append(p_cmd, "-Lpath");
    nob_cmd_append(p_cmd, WGPU_LINK_DYLIB);

    // Link sdl3webgpu glue
    //nob_cmd_append(p_cmd, "-x", "objective-c");
    nob_cmd_append(p_cmd, "-framework", "Cocoa", "-framework", "CoreVideo", "-framework", "IOKit", "-framework", "QuartzCore", "-framework", "Metal");

    //nob_cmd_append(p_cmd, "-x", "objective-c", "-framework", "Cocoa", "-framework", "CoreVideo", "-framework", "IOKit", "-framework", "QuartzCore");
    //-x objective-c and the link libraries -framework Cocoa, -framework CoreVideo, -framework IOKit, and -framework QuartzCore

    // Necessary to use SDL3, at least on macOS
    nob_cmd_append(&cmd, "-Wl,-rpath,@executable_path");
}

void macos_add_source_files(Nob_Cmd* p_cmd) {
    nob_cc_inputs(p_cmd, SRC_FOLDER "platform/macos/" "objc_dummy.m");
}

void macos_install_app() {
    if (!nob_file_exists(BUILD_FOLDER SDL3_FRAMEWORK_BASENAME)) {
        nob_copy_directory_recursively(SDL3_FRAMEWORK_DIR, BUILD_FOLDER SDL3_FRAMEWORK_BASENAME);
    }
    nob_copy_file(WGPU_LINK_DYLIB, BUILD_FOLDER WGPU_LINK_DYLIB_BASENAME);
}