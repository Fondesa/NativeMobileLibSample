#!/bin/bash

case "$OSTYPE" in
darwin*)
    echo "Darwin"
    ;;
linux*)
    echo "Linux"
    ;;
msys*)
    echo "Windows"
    # Unsupported ATM.
    exit 1
    ;;
*)
    echo "${OSTYPE}"
    exit 1
    ;;
esac
