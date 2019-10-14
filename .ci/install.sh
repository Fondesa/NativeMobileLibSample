#!/bin/bash

formulae=()

if [[ "$TRAVIS_OS_NAME" == "osx" ]]; then
    # Install GCC if needed.
    if [[ "$CC" == "$CC_GCC" ]]; then
        formulae+=(gcc@9)
    fi

    # Install LCOV only when the tests are run with coverage.
    # This is optimal since we don't want to install it in jobs which don't use it.
    if [[ "$RUN_TESTS_WITH_COVERAGE" == "true" ]]; then
        formulae+=(lcov)
    fi
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