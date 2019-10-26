#!/bin/bash

scriptDir="$(cd "$(dirname "${BASH_SOURCE[0]}")" >/dev/null 2>&1 && pwd)"
projectDir=${scriptDir}

if [[ ! -x "$(command -v pcpp)" ]]; then
    cat <<EOF
Install pcpp to merge the header files.
https://pypi.org/project/pcpp/
EOF
    exit 1
fi

includeDir=$projectDir/include
headers=$(ls $includeDir)
(cd $includeDir && pcpp $headers \
    --passthru-unfound-includes \
    --passthru-unknown-exprs \
    --line-directive \
    -o $projectDir/single_include/notes.hpp)
