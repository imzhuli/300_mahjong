#!/bin/sh
set -e
. ./scripts/internal-check-env.sh

cd "${MAHJONG_BUILD_DIR}"
make -j --
