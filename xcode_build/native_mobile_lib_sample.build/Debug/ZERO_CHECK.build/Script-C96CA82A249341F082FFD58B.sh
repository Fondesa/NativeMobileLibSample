#!/bin/sh
make -C /Users/antoniolig/Developer/c++/native_mobile_lib/xcode_build -f /Users/antoniolig/Developer/c++/native_mobile_lib/xcode_build/CMakeScripts/ZERO_CHECK_cmakeRulesBuildPhase.make$CONFIGURATION OBJDIR=$(basename "$OBJECT_FILE_DIR_normal") all
