{
  description = "A basic shell";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs?ref=nixos-unstable";
    systems.url = "github:nix-systems/default";
    treefmt-nix = {
      url = "github:numtide/treefmt-nix";
      inputs.nixpkgs.follows = "nixpkgs";
    };
  };

  outputs =
    {
      self,
      nixpkgs,
      systems,
      treefmt-nix,
    }:
    let
      eachSystem = nixpkgs.lib.genAttrs (import systems);
      pkgsFor =
        system:
        import nixpkgs {
          inherit system;
          overlays = [ self.overlays.default ];
        };
    in
    {
      overlays.default = final: prev: {
        gmp-wasm = final.callPackage ./gmp-wasm.nix { };
      };

      devShells = eachSystem (
        system:
        let
          pkgs = pkgsFor system;
        in
        {
          default = pkgs.mkShell {
            packages = [
              pkgs.emscripten
              pkgs.gmp
              pkgs.gmp-wasm
              pkgs.pkg-config
              pkgs.ninja
            ];
          };
        }
      );

      formatter = eachSystem (
        system:
        (treefmt-nix.lib.evalModule (pkgsFor system) {
          projectRootFile = "flake.nix";
          programs.nixfmt.enable = true;
        }).config.build.wrapper
      );

      legacyPackages = eachSystem pkgsFor;
    };
}
