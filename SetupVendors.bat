SET SOURCE_DIR=./vendor/assimp
SET GENERATOR=Visual Studio 17 2022

SET BINARIES_DIR="./vendor/assimp/build/x64"
cmake ./vendor/assimp -G "%GENERATOR%" -A x64 -S %SOURCE_DIR% -B %BINARIES_DIR%
cmake --build %BINARIES_DIR% --config debug
cmake --build %BINARIES_DIR% --config release

PAUSE