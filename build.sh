#!/bin/bash

scriptDir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
projectDir=${scriptDir}
libName="nativemobile"
libFileName="lib$libName.so"

function notify_uncorrect_usage() {
    printf "Supported args:\n--cmd\n--android\n--xcode\n"
    exit 1
}

function cmd() {
    echo "Building CMD lib and sample..."
    cmake -B$projectDir/cmd_build
    (cd $projectDir/cmd_build && make)
}

function android() {
    declare -a abis=(armeabi-v7a arm64-v8a x86 x86_64)
    #declare -a abis=(x86)
    for abi in "${abis[@]}"
    do
        build_android_abi $abi
    done

    # This is needed on macOS because Android Studio can't retrieve always
    # environment variables.
    add_gradle_properties_prebuilt_libs_entry
}

function build_android_abi() {
    local abi=$1
    local abiBuildDir=$projectDir/android_build/$abi
    echo "Building Android shared lib for ABI $abi..."
    cmake -B$abiBuildDir \
        -DCMAKE_SYSTEM_NAME=Android \
        -DCMAKE_SYSTEM_VERSION=28 \
        -DCMAKE_ANDROID_ARCH_ABI=$abi \
        -DCMAKE_ANDROID_NDK=$ANDROID_NDK \
        -DCMAKE_TRY_COMPILE_TARGET_TYPE="STATIC_LIBRARY"  # Needed only for NDK r20.

    (cd $abiBuildDir && make)

    symlink_prebuilt_lib $abiBuildDir/lib android/$abi
}

function symlink_prebuilt_lib() {
    local buildDir=$1
    local linkRelativePath=$2
    local linkDir=$PREBUILT_LIBS/$libName/$linkRelativePath
    mkdir -p "$linkDir"
    ln -sf $buildDir/$libFileName $linkDir/$libFileName
}

function add_gradle_properties_prebuilt_libs_entry() {
    local gradlePropertiesDir="$HOME/.gradle/gradle.properties"
    local propName="prebuilt_libs"
    # Delete the line starting with "prebuilt_libs" if found.
    sed -i '' "/^$propName/d" $gradlePropertiesDir
    # Write a new line containing the property.
    echo "$propName=$PREBUILT_LIBS" >> $gradlePropertiesDir
}

function xcode() {
    echo "Building XCode project..."
    cmake -GXcode -B$projectDir/xcode_build
}

function symlink_include_dir() {
    local headersSymLink=$PREBUILT_LIBS/$libName
    mkdir -p $headersSymLink
    ln -sf $projectDir/lib/include $headersSymLink
}

[[ $# -eq 0 ]] && notify_uncorrect_usage

case $1 in
    "--cmd")
        cmd
        symlink_include_dir
        ;;
    "--android")
        android
        symlink_include_dir
        ;;
    "--xcode")
        xcode
        symlink_include_dir
        ;;
    *)
        notify_uncorrect_usage
esac