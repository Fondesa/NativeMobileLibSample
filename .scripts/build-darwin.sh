#!/bin/bash

# The first argument can be ON or OFF to enable or disable the amalgamation.
cmakeAmalgamation=$1

scriptDir="$(cd "$(dirname "${BASH_SOURCE[0]}")" >/dev/null 2>&1 && pwd)"
projectDir=${scriptDir}/..

if [[ $("${projectDir}"/.scripts/get-os.sh) != "Darwin" ]]; then
    echo "The library for Darwin can be built only on a Darwin system."
    exit 1
fi

darwinBuildDir=${projectDir}/build/lib/darwin

echo "Building shared lib for Darwin..."

cmake "${projectDir}" -B"${darwinBuildDir}" \
    -DCMAKE_SYSTEM_NAME=Darwin \
    -DCMAKE_C_COMPILER="${CC}" \
    -DCMAKE_CXX_COMPILER="${CXX}" \
    -DENABLE_TESTS=OFF \
    -DAMALGAMATION="$cmakeAmalgamation"
(cd "${darwinBuildDir}" && make build-lib)
