{ pkgs ? import <nixpkgs> {} }:

pkgs.mkShell {
  buildInputs = with pkgs; [
    gcc
    cmake
    qt6.qtbase
    qt6.qtdeclarative
    yaml-cpp
    ninja
    pkg-config
  ];

  shellHook = ''
    echo "Entering Drink Menu development environment"
    # Явно указываем CMake пути к пакетам
    export CMAKE_PREFIX_PATH="${pkgs.yaml-cpp}:${pkgs.qt6.qtbase}:${pkgs.qt6.qtdeclarative}:$CMAKE_PREFIX_PATH"
  '';
}