#!/bin/bash

brew update

# The Android NDK toolchain file is needed to build the library for Android so, before building, Travis will download
# the NDK R20, unzip them and export their path to use it inside the build script.
if [[ "$BUILD_ANDROID" == "true" ]]; then
    curl https://dl.google.com/android/repository/android-ndk-r20-darwin-x86_64.zip -o ndk-r20.zip
    unzip -q ndk-r20.zip
    export ANDROID_NDK=android-ndk-r20
fi
