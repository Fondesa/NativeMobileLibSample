#!/bin/bash

scriptDir="$(cd "$(dirname "${BASH_SOURCE[0]}")" >/dev/null 2>&1 && pwd)"
projectDir=${scriptDir}
prebuiltLibDir=${PREBUILT_LIBS}/nativemobile

dirs=(
    ${projectDir}/build
    ${projectDir}/cmake-build-debug
    ${prebuiltLibDir}
)

for dir in "${dirs[@]}"; do
    rm -rf ${dir}
done
