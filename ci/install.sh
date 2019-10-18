#!/bin/bash

function install_osx() {
    local formulae=()
    if [[ "$CC" == "$CC_GCC_OSX" ]]; then
        # Even if GCC is provided by default in macOS, we want to upgrade it if possible.
        # TODO: find a way to use gcc@9
        formulae+=(gcc)
    fi

    if [[ "$RUN_TESTS_WITH_COVERAGE" == "true" ]]; then
        # Install LCOV only when the tests are run with coverage because we don't want to install it in jobs which
        # don't use it.
        formulae+=(lcov)
    fi

    for formula in "${formulae[@]}"; do
        for retry in $(seq 1 3); do
            echo "Upgrading ${formula}"
            brew outdated | grep -q "$formula"
            STATUS="$?"
            if [ "${STATUS}" -ne 0 ]; then
                echo "Already up to date or not installed."
                break
            fi

            brew upgrade "$formula"
            STATUS="$?"
            if [ "${STATUS}" -eq 0 ]; then
                break
            fi
            echo "Attempt ${retry} failed."
            if [ "${retry}" -eq 3 ]; then
                echo "Too many retries. Aborting."
                exit 1
            else
                echo "Retrying..."
            fi
        done
    done

    for formula in "${formulae[@]}"; do
        for retry in $(seq 1 3); do
            echo "Installing ${formula}"
            brew list | grep -q "$formula" || brew install "$formula"
            STATUS="$?"
            if [ "${STATUS}" -eq 0 ]; then
                break
            fi
            echo "Attempt ${retry} failed."
            if [ "${retry}" -eq 3 ]; then
                echo "Too many retries. Aborting."
                exit 1
            else
                echo "Retrying..."
            fi
        done
    done
}

if [[ "$TRAVIS_OS_NAME" == "osx" ]]; then
    install_osx
fi
