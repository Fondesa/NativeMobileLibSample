#!/bin/bash

scriptDir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
projectDir=${scriptDir}
testsBuildDir=${projectDir}/build/tests

cmake -B${testsBuildDir} \
    -DBUILD_TESTS=ON
(cd ${testsBuildDir} && make run-tests)