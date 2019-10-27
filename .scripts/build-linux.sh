#!/bin/bash

# The first argument can be ON or OFF to enable or disable the amalgamation.
cmakeAmalgamation=$1

scriptDir="$(cd "$(dirname "${BASH_SOURCE[0]}")" >/dev/null 2>&1 && pwd)"
projectDir=${scriptDir}/..

linuxBuildDir=${projectDir}/build/lib/linux

echo "Building shared lib for Linux..."

cmake ${projectDir} -B${linuxBuildDir} \
    -DCMAKE_SYSTEM_NAME=Linux \
    -DCMAKE_C_COMPILER=${CC} \
    -DCMAKE_CXX_COMPILER=${CXX} \
    -DENABLE_TESTS=OFF \
    -DAMALGAMATION=$cmakeAmalgamation
(cd ${linuxBuildDir} && make build-lib)
