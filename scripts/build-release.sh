#!/bin/sh
set -e
. ./scripts/internal-check-env.sh

export CMAKE_BUILD_TYPE=Release
./scripts/internal-build.sh
