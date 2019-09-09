# Notes - native library

This library contains all the cross-platform native code, written in C/C++, currently used by the _Notes_ mobile apps:
- [Android](https://github.com/Fondesa/notes-android)
- [iOS](https://github.com/Fondesa/notes-ios)

## Setup

- Add the environment variable `PREBUILT_LIBS` indicating the absolute path of the directory in which this library will be published.
- If you are going to build this library for Android, add the environment variable `ANDROID_NDK` indicating the absolute path of the directory in which the Android NDK are contained (e.g. in MacOS `/Users/myuser/Library/Android/sdk/ndk/20.0.5594570`.

<!--
The build script can generate the dynamic library for the following operative systems:
- Darwin -> generates
- Android since 
-->
