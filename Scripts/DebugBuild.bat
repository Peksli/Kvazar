@echo off

set BUILD_TYPE=Debug

echo [CMake] Configuring (%BUILD_TYPE%)...
cd ../build
cmake .. -DCMAKE_BUILD_TYPE=%BUILD_TYPE%

echo [CMake] Building...
cmake --build . --config %BUILD_TYPE%

pause