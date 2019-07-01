# DO NOT EDIT
# This makefile makes sure all linkable targets are
# up-to-date with anything they link to
default:
	echo "Do not invoke directly"

# Rules to remove targets that are older than anything to which they
# link.  This forces Xcode to relink the targets from scratch.  It
# does not seem to check these dependencies itself.
PostBuild.lib.Debug:
/Users/antoniolig/Developer/c++/native_mobile_lib/xcode_build/lib/Debug/liblib.a:
	/bin/rm -f /Users/antoniolig/Developer/c++/native_mobile_lib/xcode_build/lib/Debug/liblib.a


PostBuild.lib.Release:
/Users/antoniolig/Developer/c++/native_mobile_lib/xcode_build/lib/Release/liblib.a:
	/bin/rm -f /Users/antoniolig/Developer/c++/native_mobile_lib/xcode_build/lib/Release/liblib.a


PostBuild.lib.MinSizeRel:
/Users/antoniolig/Developer/c++/native_mobile_lib/xcode_build/lib/MinSizeRel/liblib.a:
	/bin/rm -f /Users/antoniolig/Developer/c++/native_mobile_lib/xcode_build/lib/MinSizeRel/liblib.a


PostBuild.lib.RelWithDebInfo:
/Users/antoniolig/Developer/c++/native_mobile_lib/xcode_build/lib/RelWithDebInfo/liblib.a:
	/bin/rm -f /Users/antoniolig/Developer/c++/native_mobile_lib/xcode_build/lib/RelWithDebInfo/liblib.a




# For each target create a dummy ruleso the target does not have to exist
