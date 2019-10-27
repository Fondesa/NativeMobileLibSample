#!/bin/bash

# The first argument can be ON or OFF to enable or disable the amalgamation.
cmakeAmalgamation=$1

scriptDir="$(cd "$(dirname "${BASH_SOURCE[0]}")" >/dev/null 2>&1 && pwd)"
projectDir=${scriptDir}/..

if [[ -z "${ANDROID_NDK}" ]]; then
    echo "The environment variable ANDROID_NDK must be set before building the library for Android."
    exit 1
fi

androidBuildDir=${projectDir}/build/lib/android

function build_android_abi() {
    local abi=$1
    local abiBuildDir=${androidBuildDir}/${abi}

    echo "Building Android shared lib for ABI $abi..."

    cmake ${projectDir} -B${abiBuildDir} \
        -DENABLE_TESTS=OFF \
        -DAMALGAMATION=$cmakeAmalgamation \
        -DCMAKE_TOOLCHAIN_FILE=${ANDROID_NDK}/build/cmake/android.toolchain.cmake \
        -DCMAKE_SYSTEM_NAME=Android \
        -DANDROID_PLATFORM=android-16 \
        -DANDROID_TOOLCHAIN=${CC} \
        -DANDROID_ABI=${abi}

    (cd ${abiBuildDir} && make build-lib)
}

abis=(armeabi-v7a arm64-v8a x86 x86_64)
for abi in "${abis[@]}"; do
    build_android_abi ${abi}
done
