#!/bin/sh
set -e

if [ -z "$P300_INSTALL_DIR" ]; then
    echo "P300_INSTALL_DIR not found, will do further searching"
else
    export X_ROOT="$P300_INSTALL_DIR/$CMAKE_BUILD_TYPE"
    export X_INCLUDE_DIRS="$X_ROOT/include"
    export X_LIBRARY_DIRS="$X_ROOT/lib"
    export X_LIBRARIES="xel"
fi

if [ -z "$X_INCLUDE_DIRS" ]; then
    echo "X_INCLUDE_DIRS undefined"
    exit 1
fi

if [ -z "$X_LIBRARY_DIRS" ]; then
    echo "X_LIBRARY_DIRS undefined"
    exit 1
fi

if [ -z "$X_LIBRARIES" ]; then
    echo "X_LIBRARIES undefined"
    exit 1
fi

CMAKE_FLAGS="--no-warn-unused-cli -Wno-dev -DX_OPTION_STATIC=true -DX_INCLUDE_DIRS='$X_INCLUDE_DIRS' -DX_LIBRARY_DIRS='$X_LIBRARY_DIRS' -DX_LIBRARIES='$X_LIBRARIES' "

rm -rf "$MAHJONG_BUILD_DIR"
mkdir -p "${MAHJONG_BUILD_DIR}"
cd "${MAHJONG_BUILD_DIR}"

if [ -z "$CMAKE_INSTALL_PREFIX" ]; then
    cmake ${CMAKE_FLAGS} "$MAHJONG_SOURCE_DIR/source"
else
    cmake -DCMAKE_INSTALL_PREFIX="$CMAKE_INSTALL_PREFIX" ${CMAKE_FLAGS} "$MAHJONG_SOURCE_DIR/source"
fi
make -j --

# new installation
make install

