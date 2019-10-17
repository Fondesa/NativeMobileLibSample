#!/bin/bash

scriptDir="$(cd "$(dirname "${BASH_SOURCE[0]}")" >/dev/null 2>&1 && pwd)"
projectDir=${scriptDir}
testsBuildDir=${projectDir}/build/tests

if [ -z "$1" ]; then
    enableCoverage=OFF
    targetName=run-tests
    generateHtmlReport=OFF
elif [ $1 = "--coverage-html" ]; then
    enableCoverage=ON
    targetName=coverage-report
    generateHtmlReport=ON
elif [ $1 = "--coverage-raw" ]; then
    enableCoverage=ON
    targetName=coverage-report
    generateHtmlReport=OFF
else
    cat <<EOF
The argument "$1" can't be recognized.
Don't specify any argument or use:
--coverage-raw -> run the tests with coverage generating a Gcov raw report
--coverage-html -> run the tests with coverage generating an HTML report
EOF
    exit 1
fi

cmake ${projectDir} -B${testsBuildDir} \
    -DCMAKE_C_COMPILER=${CC} \
    -DCMAKE_CXX_COMPILER=${CXX} \
    -DENABLE_TESTS=ON \
    -DENABLE_TESTS_COVERAGE=${enableCoverage} \
    -DGENERATE_HTML_REPORT=${generateHtmlReport}
(cd ${testsBuildDir} && make $targetName)
