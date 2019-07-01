# DO NOT EDIT
# This makefile makes sure all linkable targets are
# up-to-date with anything they link to
default:
	echo "Do not invoke directly"

# Rules to remove targets that are older than anything to which they
# link.  This forces Xcode to relink the targets from scratch.  It
# does not seem to check these dependencies itself.
PostBuild.sample.Debug:
PostBuild.lib.Debug: /Users/antoniolig/Developer/c++/native_mobile_lib/xcode_build/sample/Debug/sample
/Users/antoniolig/Developer/c++/native_mobile_lib/xcode_build/sample/Debug/sample:\
	/Users/antoniolig/Developer/c++/native_mobile_lib/xcode_build/lib/Debug/liblib.a
	/bin/rm -f /Users/antoniolig/Developer/c++/native_mobile_lib/xcode_build/sample/Debug/sample


PostBuild.sample.Release:
PostBuild.lib.Release: /Users/antoniolig/Developer/c++/native_mobile_lib/xcode_build/sample/Release/sample
/Users/antoniolig/Developer/c++/native_mobile_lib/xcode_build/sample/Release/sample:\
	/Users/antoniolig/Developer/c++/native_mobile_lib/xcode_build/lib/Release/liblib.a
	/bin/rm -f /Users/antoniolig/Developer/c++/native_mobile_lib/xcode_build/sample/Release/sample


PostBuild.sample.MinSizeRel:
PostBuild.lib.MinSizeRel: /Users/antoniolig/Developer/c++/native_mobile_lib/xcode_build/sample/MinSizeRel/sample
/Users/antoniolig/Developer/c++/native_mobile_lib/xcode_build/sample/MinSizeRel/sample:\
	/Users/antoniolig/Developer/c++/native_mobile_lib/xcode_build/lib/MinSizeRel/liblib.a
	/bin/rm -f /Users/antoniolig/Developer/c++/native_mobile_lib/xcode_build/sample/MinSizeRel/sample


PostBuild.sample.RelWithDebInfo:
PostBuild.lib.RelWithDebInfo: /Users/antoniolig/Developer/c++/native_mobile_lib/xcode_build/sample/RelWithDebInfo/sample
/Users/antoniolig/Developer/c++/native_mobile_lib/xcode_build/sample/RelWithDebInfo/sample:\
	/Users/antoniolig/Developer/c++/native_mobile_lib/xcode_build/lib/RelWithDebInfo/liblib.a
	/bin/rm -f /Users/antoniolig/Developer/c++/native_mobile_lib/xcode_build/sample/RelWithDebInfo/sample




# For each target create a dummy ruleso the target does not have to exist
/Users/antoniolig/Developer/c++/native_mobile_lib/xcode_build/lib/Debug/liblib.a:
/Users/antoniolig/Developer/c++/native_mobile_lib/xcode_build/lib/MinSizeRel/liblib.a:
/Users/antoniolig/Developer/c++/native_mobile_lib/xcode_build/lib/RelWithDebInfo/liblib.a:
/Users/antoniolig/Developer/c++/native_mobile_lib/xcode_build/lib/Release/liblib.a:
