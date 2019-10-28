#!/bin/bash

# The first argument can be ON or OFF to enable or disable the amalgamation.
cmakeAmalgamation=$1

scriptDir="$(cd "$(dirname "${BASH_SOURCE[0]}")" >/dev/null 2>&1 && pwd)"
projectDir=${scriptDir}/..

if [[ $("${projectDir}"/.scripts/get-os.sh) != "Linux" ]]; then
    echo "The library for Linux can be built only on a Linux system."
    exit 1
fi

linuxBuildDir=${projectDir}/build/lib/linux

echo "Building shared lib for Linux..."

cmake "${projectDir}" -B"${linuxBuildDir}" \
    -DCMAKE_SYSTEM_NAME=Linux \
    -DCMAKE_C_COMPILER="${CC}" \
    -DCMAKE_CXX_COMPILER="${CXX}" \
    -DENABLE_TESTS=OFF \
    -DAMALGAMATION="$cmakeAmalgamation"
(cd "${linuxBuildDir}" && make build-lib)
