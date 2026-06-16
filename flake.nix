{
  description = "mei hua game engine dev environment";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = import nixpkgs { inherit system; };
      in
      {
        packages.default = pkgs.godot-mono;

        apps.default = {
          type = "app";
          program = "${pkgs.godot-mono}/bin/godot-mono";
        };

        devShells.default = pkgs.mkShell {
          packages = with pkgs; [
                llvmPackages.clang
                llvmPackages.clang-tools
                vscodium
                cmake


                # X11
                libX11
                libXext
                libXrandr
                libXcursor
                libXi
                libXfixes
                libxscrnsaver
                libxtst
                libxcb

                # Wayland
                wayland
                wayland-protocols
                libxkbcommon
                mesa
          ];
        };
      }
    );
}
