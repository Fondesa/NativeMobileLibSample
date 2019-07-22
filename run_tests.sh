#!/bin/bash

scriptDir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
projectDir=${scriptDir}
testsBuildDir=${projectDir}/build/tests

echo "Building all the tests and their dependencies..."
cmake -B${testsBuildDir} \
    -DBUILD_TESTS:BOOLEAN=true
(cd ${testsBuildDir} && make)
echo "Executing the lib tests..."
${testsBuildDir}/lib/test/lib_tests