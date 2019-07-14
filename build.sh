#!/bin/bash

scriptDir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
projectDir=${scriptDir}
libName="nativemobile"
iosFrameworkDir=$projectDir/ios_build/framework
iosFrameworkFileName=$libName.framework
iosUniversalFrameworkDir=$iosFrameworkDir/universal

function notify_uncorrect_usage() {
    printf "Supported args:\n--cmd\n--android\n--ios\n--all\n"
    exit 1
}

function cmd() {
    echo "Building CMD lib and sample..."
    cmake -B$projectDir/cmd_build
    (cd $projectDir/cmd_build && make)
}

function android() {
    local abis=(armeabi-v7a arm64-v8a x86 x86_64)
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
    cmake --target $projectDir/lib -B$abiBuildDir \
        -DCMAKE_TOOLCHAIN_FILE=$ANDROID_NDK/build/cmake/android.toolchain.cmake \
        -DANDROID_PLATFORM=android-28 \
        -DANDROID_ABI=$abi

    (cd $abiBuildDir && make)

    symlink_prebuilt_lib $abiBuildDir "lib$libName.so" android/$abi
}

function add_gradle_properties_prebuilt_libs_entry() {
    local gradlePropertiesDir="$HOME/.gradle/gradle.properties"
    local propName="prebuilt_libs"
    # Delete the line starting with "prebuilt_libs" if found.
    sed -i '' "/^$propName/d" $gradlePropertiesDir
    # Write a new line containing the property.
    echo "$propName=$PREBUILT_LIBS" >> $gradlePropertiesDir
}

function ios() {
    local buildDir=$projectDir/ios_build
    echo "Building iOS shared lib..."
    cmake --target $projectDir/lib -B$buildDir -GXcode \
        -DBUILD_FRAMEWORK:BOOLEAN=true \
        -DCMAKE_SYSTEM_NAME=iOS \
        -DCMAKE_OSX_ARCHITECTURES="armv7;armv7s;arm64;i386;x86_64" \
        -DCMAKE_OSX_DEPLOYMENT_TARGET=9.3 \
        -DCMAKE_XCODE_ATTRIBUTE_ONLY_ACTIVE_ARCH=NO \
        -DCMAKE_IOS_INSTALL_COMBINED=YES

    build_universal_framework
    symlink_prebuilt_lib $iosUniversalFrameworkDir $iosFrameworkFileName ios
}

function build_universal_framework() {
    local universalFramework=$iosUniversalFrameworkDir/$iosFrameworkFileName

    build_framework_for_sdk iphoneos
    build_framework_for_sdk iphonesimulator

    local phoneFramework=$iosFrameworkDir/iphoneos/$iosFrameworkFileName
    local simulatorFramework=$iosFrameworkDir/iphonesimulator/$iosFrameworkFileName

    rm -rf $iosUniversalFrameworkDir
    mkdir -p $iosUniversalFrameworkDir

    # Copy the files from the iphoneos framework to the universal one.
    cp -r $phoneFramework/. $universalFramework

    # Create the universal framework using the simulator and the phone framework.
    lipo $simulatorFramework/$libName $phoneFramework/$libName -create -output $universalFramework/$libName | echo

    # Change the install name of the framework to avoid absolute paths.
    install_name_tool -id "@rpath/${iosFrameworkFileName}/$libName" $universalFramework/$libName
}

function build_framework_for_sdk() {
    local sdk=$1

    (cd $projectDir/ios_build && xcodebuild -target lib-framework \
        -configuration "Release" \
        -UseModernBuildSystem=NO \
        -sdk $sdk \
        CONFIGURATION_BUILD_DIR=$iosFrameworkDir/$sdk \
        clean \
        build)
}

function symlink_prebuilt_lib() {
    local buildDir=$1
    local libFileName=$2
    local linkRelativePath=$3
    local linkDir=$PREBUILT_LIBS/$libName/$linkRelativePath
    mkdir -p "$linkDir"
    ln -sf $buildDir/$libFileName $linkDir/$libFileName
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
    "--ios")
        ios
        symlink_include_dir
        ;;
    "--all")
        cmd
        android
        ios
        symlink_include_dir
        ;;
    *)
        notify_uncorrect_usage
esac