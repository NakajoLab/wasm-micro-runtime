{
  stdenv,
  emscripten,
  gmp,
}:

stdenv.mkDerivation {
  pname = "gmp-wasm";
  inherit (gmp) version src;

  nativeBuildInputs = [ emscripten ];

  env.EM_CACHE = "/tmp/emcache";

  configurePhase = ''
    runHook preConfigure
    emconfigure env HOST_CC=gcc ./configure --disable-assembly --host none --prefix=$out
    runHook postConfigure
  '';

  buildPhase = ''
    runHook preBuild
    emmake make -j$NIX_BUILD_CORES
    runHook postBuild
  '';

  postInstall = ''
    mv $out/lib/pkgconfig/gmp{,-wasm}.pc
  '';
}
