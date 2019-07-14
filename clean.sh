#!/bin/bash

scriptDir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
projectDir=${scriptDir}
prebuiltLibDir=$PREBUILT_LIBS/nativemobile

dirs=(
    $projectDir/android_build 
    $projectDir/cmd_build 
    $projectDir/xcode_build
    $prebuiltLibDir/android
    $prebuiltLibDir/ios
)

for dir in "${dirs[@]}"
do
    rm -rf $dir
done