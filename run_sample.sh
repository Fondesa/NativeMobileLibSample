#!/bin/bash

scriptDir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
projectDir=${scriptDir}

# Build the sample.
$projectDir/build.sh --cmd
# Run it.
$projectDir/cmd_build/sample/sample