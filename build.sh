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

if [[ ! "$target" ]]; then
    echo "The target wasn't specified, trying to build for the host system."
    target=$("${projectDir}"/.scripts/get-os.sh)
    # shellcheck disable=SC2181
    if [[ $? != 0 ]]; then
        echo "The OS \"$target\" isn't supported."
        exit 1
    fi
fi

case $target in
"darwin" | "Darwin")
    "${projectDir}"/.scripts/build-darwin.sh $cmakeAmalgamation
    ;;
"linux" | "Linux")
    "${projectDir}"/.scripts/build-linux.sh $cmakeAmalgamation
    ;;
"android" | "Android")
    "${projectDir}"/.scripts/build-android.sh $cmakeAmalgamation
    ;;
"ios" | "iOS")
    "${projectDir}"/.scripts/build-ios.sh $cmakeAmalgamation
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
