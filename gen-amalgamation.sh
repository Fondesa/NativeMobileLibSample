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
amalgamationFile=$projectDir/amalgamation/notes.hpp

# Remove the amalgamation file before generating it again.
rm -f "$amalgamationFile"

headers=$(ls "$includeDir")

echo "LYRA: $headers"

# shellcheck disable=SC2086
(cd "$includeDir" && pcpp $headers \
    --passthru-unfound-includes \
    --passthru-unknown-exprs \
    --line-directive \
    -o "$amalgamationFile")

# Prepend "#pragma once" at the start of the file since pcpp doesn't prepend it automatically.
echo -e "#pragma once\n\n$(cat "$amalgamationFile")" >"$amalgamationFile"
