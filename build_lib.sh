#!/bin/bash

scriptDir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
projectDir=${scriptDir}
libName="nativemobile"
libTarget=${projectDir}/lib
libBuildDir=${projectDir}/build/lib
unixBuildDir=${libBuildDir}/unix
androidBuildDir=${libBuildDir}/android
iosBuildDir=${libBuildDir}/ios
iosFrameworkDir=${iosBuildDir}/framework
iosFrameworkFileName=${libName}.framework
iosUniversalFrameworkDir=${iosFrameworkDir}/universal

function notify_uncorrect_usage() {
    printf "Supported args:\n--unix\n--android\n--ios\n--all\n"
    exit 1
}

function unix() {
    echo "Building Unix lib and sample..."
    cmake --target ${libTarget} -B${unixBuildDir} \
        -DPLATFORM=Unix \
        -DBUILD_TESTS=OFF
    (cd ${unixBuildDir} && make)

    symlink_prebuilt_lib ${unixBuildDir}/lib${libName}.dylib unix
}

function android() {
    local abis=(armeabi-v7a arm64-v8a x86 x86_64)
    for abi in "${abis[@]}"
    do
        build_android_abi ${abi}
    done
}

function build_android_abi() {
    local abi=$1
    local abiBuildDir=${androidBuildDir}/${abi}
    echo "Building Android shared lib for ABI $abi..."
    cmake --target ${libTarget} -B${abiBuildDir} \
        -DPLATFORM=Android \
        -DBUILD_TESTS=OFF \
        -DCMAKE_TOOLCHAIN_FILE=${ANDROID_NDK}/build/cmake/android.toolchain.cmake \
        -DANDROID_PLATFORM=android-16 \
        -DANDROID_ABI=${abi}

    (cd ${abiBuildDir} && make)

    symlink_prebuilt_lib ${abiBuildDir}/lib${libName}.so android/${abi}
}

function ios() {
    echo "Building iOS shared lib..."
    cmake --target ${libTarget} -B${iosBuildDir} -GXcode \
        -DPLATFORM=iOS \
        -DBUILD_TESTS=OFF \
        -DCMAKE_SYSTEM_NAME=iOS \
        -DCMAKE_OSX_DEPLOYMENT_TARGET=9.3

    build_universal_framework
    symlink_prebuilt_lib ${iosUniversalFrameworkDir}/${iosFrameworkFileName} ios
}

function build_universal_framework() {
    local universalFramework=${iosUniversalFrameworkDir}/${iosFrameworkFileName}

    build_framework_for_sdk iphoneos
    build_framework_for_sdk iphonesimulator

    local phoneFramework=${iosFrameworkDir}/iphoneos/${iosFrameworkFileName}
    local simulatorFramework=${iosFrameworkDir}/iphonesimulator/${iosFrameworkFileName}

    rm -rf ${iosUniversalFrameworkDir}
    mkdir -p ${iosUniversalFrameworkDir}

    # Copy the files from the iphoneos framework to the universal one.
    cp -r ${phoneFramework}/. ${universalFramework}

    # Create the universal framework using the simulator and the phone framework.
    lipo ${simulatorFramework}/${libName} ${phoneFramework}/${libName} -create -output ${universalFramework}/${libName} | echo

    # Change the install name of the framework to avoid absolute paths.
    install_name_tool -id "@rpath/${iosFrameworkFileName}/$libName" ${universalFramework}/${libName}
}

function build_framework_for_sdk() {
    local sdk=$1

    (cd ${iosBuildDir} && xcodebuild -target lib-framework \
        -configuration "Release" \
        -UseModernBuildSystem=NO \
        -sdk ${sdk} \
        CONFIGURATION_BUILD_DIR=${iosFrameworkDir}/${sdk} \
        clean \
        build)
}

function symlink_prebuilt_lib() {
    local libFilePath=$1
    local linkRelativePath=$2
    local linkDir=${PREBUILT_LIBS}/${libName}/${linkRelativePath}
    mkdir -p "$linkDir"
    ln -sf ${libFilePath} ${linkDir}
}

function symlink_include_dir() {
    local headersSymLink=${PREBUILT_LIBS}/${libName}
    mkdir -p ${headersSymLink}
    ln -sf ${projectDir}/lib/include ${headersSymLink}
}

[[ $# -eq 0 ]] && notify_uncorrect_usage

case $1 in
    "--unix")
        unix
        symlink_include_dir
        ;;
    "--android")
        android
        symlink_include_dir
        ;;
    "--ios")
        ios
        symlink_include_dir
        ;;
    "--all")
        unix
        android
        ios
        symlink_include_dir
        ;;
    *)
        notify_uncorrect_usage
esac