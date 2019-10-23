# Notes - native library

[![Build Status](https://travis-ci.org/Fondesa/notes-native.svg?branch=master)](https://travis-ci.org/Fondesa/notes-native)
[![Coverage Status](https://coveralls.io/repos/github/Fondesa/notes-native/badge.svg?branch=master)](https://coveralls.io/github/Fondesa/notes-native?branch=master)

This library contains all the cross-platform native code, written in C/C++, currently used by the _Notes_ mobile apps:
- [Android](https://github.com/Fondesa/notes-android)
- [iOS](https://github.com/Fondesa/notes-ios)

## Prerequisites
- One of the supported compilers listed in the section below
- CMake 3.10 or above
- Having Xcode installed if the library should be built for iOS
- Having the Android NDK R20 installed if the library should be built for Android
- The following environment variables:
  - `CC` &rarr; specifies the C compiler
  - `CXX` &rarr; specifies the C++ compiler
  - `ANDROID_NDK` (needed only to build the library for Android) &rarr; indicates the absolute path of the directory in which the Android NDK are contained (e.g. in MacOS `/Users/myuser/Library/Android/sdk/ndk/20.0.5594570`)

## Build

The library can be built through `./build.sh` which supports the following args:
- `--system` &rarr; builds the library for the host system
- `--android` &rarr; builds the library for Android generating the multiple `.so` libraries for the architectures _armeabi-v7a_, _arm64-v8a_, _x86_, _x86_64_
- `--ios` &rarr; builds the library for iOS generating a single `.framework` for all the architectures used by _iphoneos_ and _iphonesimulator_

The build files can be cleaned with `./clean.sh`

## Tests
All the tests can be run with `./run_tests.sh`.
It supports two additional args:
- `--coverage=raw|html` &rarr; runs the tests with coverage generating a Gcov raw report or an HTML report
- `--gcov-tool=path/to/gcov` &rarr; specifies the Gcov tool which should be used to generate the coverage report, otherwise it will be found with CMake's `find_program()`

## Supported compilers:
- GCC 6.5 - 9.2 (and possibly later)
- AppleClang 8.1 - 11.0 (and possibly later)
- Clang 3.9 - 9.0 (and possibly later)
