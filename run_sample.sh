#!/bin/bash

scriptDir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
projectDir=${scriptDir}
sampleBuildDir=$projectDir/build/sample

echo "Building the sample and its dependencies..."
cmake -B$sampleBuildDir \
    -DBUILD_TESTS:BOOLEAN=false
(cd $sampleBuildDir && make)
echo "Executing the sample..."
$sampleBuildDir/sample/sample