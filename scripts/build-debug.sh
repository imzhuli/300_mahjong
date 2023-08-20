#!/bin/sh
set -e
. ./scripts/internal-check-env.sh

export CMAKE_BUILD_TYPE=Debug
./scripts/internal-build.sh
