#!/bin/bash
set -e

CURR_DIR=`pwd`
TAG_FILE="$CURR_DIR/mahjong.tag"

if [ -f "$TAG_FILE" ]; then
    echo "Tag file found, root_dir=$CURR_DIR"
else
    echo "Tag file not found!"
    exit 1
fi

export MAHJONG_DIR="${CURR_DIR}"
export MAHJONG_SOURCE_DIR="${MAHJONG_DIR}"
export MAHJONG_BUILD_DIR="$MAHJONG_DIR/build"
