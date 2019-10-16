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

function install_linux() {
    sudo add-apt-repository ppa:ubuntu-toolchain-r/test -y
    sudo add-apt-repository ppa:jonathonf/llvm -y
    wget -O - https://apt.llvm.org/llvm-snapshot.gpg.key | sudo apt-key add -
    sudo add-apt-repository 'deb http://apt.llvm.org/xenial/ llvm-toolchain-xenial-9 main' -y
    sudo apt-get update

    if [[ "$CC" == "$CC_GCC_LINUX" ]]; then
        # Install GNU 9.x.
        sudo apt-get install g++-9
    elif [[ "$CC" == "$CC_CLANG" ]]; then
        #        wget https://apt.llvm.org/llvm.sh
        #        chmod +x llvm.sh
        #        sudo ./llvm.sh 9
        #        sudo apt-get install llvm-9-dev
        #        sudo ./llvm.sh 9
        sudo apt-get install clang-9
        which clang-9
        clang-9 --version
        dpkg -L clang-9
    fi

    if [[ "$RUN_TESTS_WITH_COVERAGE" == "true" ]]; then
        # Install LCOV only when the tests are run with coverage because we don't want to install it in jobs which
        # don't use it.
        sudo apt-get install lcov
    fi
}

if [[ "$TRAVIS_OS_NAME" == "osx" ]]; then
    install_osx
elif [[ "$TRAVIS_OS_NAME" == "linux" ]]; then
    install_linux
fi
