#!/bin/bash

scriptDir="$(cd "$(dirname "${BASH_SOURCE[0]}")" >/dev/null 2>&1 && pwd)"
projectDir=${scriptDir}

dirs=(
    ${projectDir}/build
    ${projectDir}/cmake-build-debug
)

if [[ ! -z "${PREBUILT_LIBS}" ]]; then
    # Clean the symlinks in the PREBUILT_LIBS directory.
    dirs+=(${PREBUILT_LIBS}/notesnative)
fi

for dir in "${dirs[@]}"; do
    rm -rf ${dir}
done
