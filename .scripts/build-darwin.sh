#!/bin/bash

# The first argument can be ON or OFF to enable or disable the amalgamation.
cmakeAmalgamation=$1

scriptDir="$(cd "$(dirname "${BASH_SOURCE[0]}")" >/dev/null 2>&1 && pwd)"
projectDir=${scriptDir}/..

darwinBuildDir=${projectDir}/build/lib/darwin

echo "Building shared lib for Darwin..."

cmake ${projectDir} -B${darwinBuildDir} \
    -DCMAKE_SYSTEM_NAME=Darwin \
    -DCMAKE_C_COMPILER=${CC} \
    -DCMAKE_CXX_COMPILER=${CXX} \
    -DENABLE_TESTS=OFF \
    -DAMALGAMATION=$cmakeAmalgamation
(cd ${darwinBuildDir} && make build-lib)
