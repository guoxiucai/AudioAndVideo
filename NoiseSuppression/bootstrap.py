#!/usr/bin/env python

import sys
import os
import errno

def mkdir_p(path):
    try:
        os.makedirs(path)
    except OSError as exc:  # Python >2.5
        if exc.errno == errno.EEXIST and os.path.isdir(path):
            pass
        else:
            raise

args = {"folder": "build"}
supported_sys = ["osx", "win2017", "android", "ios", "win2015", "hisilicon"]
supported_archs = ["armv7", "arm64", "x86_64"] #for cocoa
supported_build_types = ["Debug", "Release"] #for cocoa
supported_build_system = ["ninja"]

for parameter in sys.argv:
    if (parameter in supported_sys):
        args["system"] = parameter
    elif(parameter in supported_archs):
        args["arch"] = parameter
    elif(parameter in supported_build_types):
        args["build_type"] = parameter
    elif(parameter in supported_build_system):
        args["build_system"] = parameter
    elif("bootstrap.py" in parameter):
        pass
    else:
        args["folder"] = parameter

if (not "system" in args):
    print("you must specifiy one system [%s] in paramters." % (",".join(supported_sys)))
    exit(-1)

#cocoa
arch = "arm64"
build_type = "Debug"
if(args["system"] == "osx"):
    arch = "x86_64"
if ("arch" in args):
    arch = args["arch"]
if ("build_type" in args):
    build_type = args["build_type"]
#cocoa

print("Preparing build script for system: %s" % args["system"])

try:
    os.remove("./build/%s/CMakeCache.txt" % args["system"])
except OSError as exc:
    if exc.errno == errno.ENOENT:
        pass
    else:
        raise

paramMake = ""
buildSystem = args["build_system"] if "build_system" in args else ""
if (args["system"].find('win') >= 0):
    vesion_year = args["system"].replace('win', '')
    if (buildSystem == 'ninja'):
        toolchainFile = os.path.join(".", "cmake/msvc/msvc.toolchain.cmake")
        paramMake = '-G "Ninja" -DMSVC_VERSION_YEAR=%s -DCMAKE_BUILD_TYPE=%s -DCMAKE_TOOLCHAIN_FILE=%s' % (vesion_year, build_type, toolchainFile)
    else:
        if (vesion_year == '2017'):
            paramMake = '-G "Visual Studio 15 2017"'
        elif (vesion_year == '2015'):
            paramMake = '-G "Visual Studio 14 2015"'       
elif (args["system"] == "osx"):
    paramMake = '-G "Xcode" -DCMAKE_OSX_SYSROOT=macosx -DCMAKE_OSX_ARCHITECTURES=%s -DCMAKE_BUILD_TYPE=%s -DMACOS=ON' % (arch, build_type)
elif (args["system"] == "ios"):
    toolchainFile = os.path.join(".", "cmake/ios/ios.toolchain.cmake")
    build_system = "Ninja" if (buildSystem == 'ninja') else "Xcode"
    sysroot = "iphoneos"
    if(arch == "x86_64"):
        sysroot = "iphonesimulator"
    paramMake = '-G "%s" -DCMAKE_OSX_SYSROOT=%s -DCMAKE_OSX_ARCHITECTURES=%s -DCMAKE_BUILD_TYPE=%s -DIOS=ON -DCMAKE_TOOLCHAIN_FILE=%s' % (build_system, sysroot, arch, build_type, toolchainFile)
elif (args["system"] == "android" or args["system"] == "hisilicon"):
    toolchainFile = os.environ["ANDROID_NDK_HOME"] + "/build/cmake/android.toolchain.cmake"
    build_system = "Ninja" if (buildSystem == 'ninja') else "Unix Makefiles"
    paramMake = '-G "%s" -DANDROID_TOOLCHAIN=gcc -DANDROID_STL=gnustl_static -DCMAKE_BUILD_TYPE=%s -DANDROID_ABI=armeabi-v7a -DANDROID_NATIVE_API_LEVEL=19 -DCMAKE_TOOLCHAIN_FILE=%s -DUSE_ANDROID=ON' % (build_system, build_type, toolchainFile)
else:
    pass
          

currentDir = os.getcwd()
path = os.path.join(currentDir, args["folder"], args["system"])
mkdir_p(path)
os.chdir(path)
    
cmdRun = "cmake ../.. %s -DCMAKE_INSTALL_PREFIX='./install'" % (paramMake)
print(cmdRun)
os.system(cmdRun)
