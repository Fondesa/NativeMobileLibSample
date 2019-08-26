#!/bin/bash

scriptDir="$(cd "$(dirname "${BASH_SOURCE[0]}")" >/dev/null 2>&1 && pwd)"
projectDir=${scriptDir}
testsBuildDir=${projectDir}/build/tests

if [ -z "$1" ]; then
    enableCoverage=OFF
    targetName=run-tests
elif [ $1 = "--coverage" ]; then
    enableCoverage=ON
    targetName=coverage-report
else
    cat <<EOF
The argument \"$1\" can't be recognized.
Don't specify any argument or use \"--coverage\" to run the tests with coverage.
EOF
    exit 1
fi

cmake -B${testsBuildDir} \
    -DENABLE_TESTS=ON \
    -DENABLE_TESTS_COVERAGE=${enableCoverage}
(cd ${testsBuildDir} && make $targetName)
