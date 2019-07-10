#!/bin/bash

scriptDir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
projectDir=${scriptDir}

dirs=(android_build cmd_build xcode_build)
for dir in "${dirs[@]}"
do
    rm -rf $projectDir/$dir
done