#!/bin/bash

if [[ "$TRAVIS_OS_NAME" == "osx" ]]; then
    brew update
fi

# The Android NDK toolchain file is needed to build the library for Android so, before building, Travis will download
# the NDK R20, unzip them and export their path to use it inside the build script.
if [[ "$BUILD_ANDROID" == "true" ]]; then
    if [[ "$TRAVIS_OS_NAME" == "osx" ]]; then
        curl https://dl.google.com/android/repository/android-ndk-r20-darwin-x86_64.zip -o ndk-r20.zip
        unzip -q ndk-r20.zip
    elif [[ "$TRAVIS_OS_NAME" == "linux" ]]; then
        wget -P ndk-r20.zip https://dl.google.com/android/repository/android-ndk-r20-linux-x86_64.zip
        unzip -q ndk-r20.zip
    else
        echo "The OS $TRAVIS_OS_NAME isn't supported."
        exit 1
    fi
    export ANDROID_NDK=android-ndk-r20
fi
