#!/bin/bash

function install_osx() {
    local formulae=()
    # Install GCC if needed.
    if [[ "$CC" == "$CC_GCC_OSX" ]]; then
        formulae+=(gcc)
    fi

    # Install LCOV only when the tests are run with coverage.
    # This is optimal since we don't want to install it in jobs which don't use it.
    if [[ "$RUN_TESTS_WITH_COVERAGE" == "true" ]]; then
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

function install_linux() {
    sudo add-apt-repository ppa:ubuntu-toolchain-r/test -y
    sudo add-apt-repository ppa:jonathonf/llvm -y
    sudo apt-get update

    # Install GCC if needed.
    if [[ "$CC" == "$CC_GCC_LINUX" ]]; then
        sudo apt-get install g++-9
        which gcc-9
        which g++-9
        gcc-9 --version
        dpkg -L gcc-9
        g++-9 --version
    elif [[ "$CC" == "$CC_CLANG" ]]; then
        wget https://apt.llvm.org/llvm.sh
        chmod +x llvm.sh
        sudo ./llvm.sh 9
        sudo apt-get install llvm-9-dev
    fi

    # Install LCOV only when the tests are run with coverage.
    # This is optimal since we don't want to install it in jobs which don't use it.
    if [[ "$RUN_TESTS_WITH_COVERAGE" == "true" ]]; then
        sudo apt-get install lcov
    fi
}

if [[ "$TRAVIS_OS_NAME" == "osx" ]]; then
    install_osx
elif [[ "$TRAVIS_OS_NAME" == "linux" ]]; then
    install_linux
fi
