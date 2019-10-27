#!/bin/bash

scriptDir="$(cd "$(dirname "${BASH_SOURCE[0]}")" >/dev/null 2>&1 && pwd)"
projectDir=${scriptDir}

cmakeAmalgamation=OFF

while [ $# -gt 0 ]; do
    case "$1" in
    --target=*)
        target="${1#*=}"
        ;;
    --amalgamation)
        cmakeAmalgamation=ON
        ;;
    *)
        cat <<EOF
The argument "$1" can't be recognized.
Supported args:
--target:
    - darwin -> builds the library for Darwin
    - linux -> builds the library for Linux
    - android -> builds the library for Android
    - ios -> builds the library for iOS

--amalgamation: links the library to a single header generated from the original ones
EOF
        exit 1
        ;;
    esac
    shift
done

if [[ -z "$target" ]]; then
    echo "A target should be specified."
    exit 1
fi

case $target in
"darwin")
    ${projectDir}/.scripts/build-darwin.sh $cmakeAmalgamation
    ;;
"linux")
    ${projectDir}/.scripts/build-linux.sh $cmakeAmalgamation
    ;;
"android")
    ${projectDir}/.scripts/build-android.sh $cmakeAmalgamation
    ;;
"ios")
    ${projectDir}/.scripts/build-ios.sh $cmakeAmalgamation
    ;;
*)
    cat <<EOF
The value of "target" can't be recognized:
Supported values:
    - darwin -> builds the library for Darwin
    - linux -> builds the library for Linux
    - android -> builds the library for Android
    - ios -> builds the library for iOS
EOF
    exit 1
    ;;
esac
