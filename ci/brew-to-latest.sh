#!/bin/bash

packageName=$1

brew update

if brew ls --versions ${packageName} > /dev/null; then
  # The package is installed.
  # If the package is outdated, it will be upgraded.
  brew outdated | grep -q ${packageName} && brew upgrade ${packageName}
else
  # The package is not installed so it will be installed.
  brew install ${packageName}
fi