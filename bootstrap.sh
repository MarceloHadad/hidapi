@echo off

if not exist "%cd%\build" (
    mkdir build && cd build
    cmake -DCMAKE_TOOLCHAIN_FILE=submodules/vcpkg/scripts/buildsystems/vcpkg.cmake -DVCPKG_TARGET_TRIPLET=x64-linux ..
)
else (
    echo "build folder already exists!"
)