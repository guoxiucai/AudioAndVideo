#!/usr/bin/env python

import sys
import os
import errno
import shutil
import multiprocessing

def mkdir_p(path):
    try:
        os.makedirs(path)
    except OSError as exc:  # Python >2.5
        if exc.errno == errno.EEXIST and os.path.isdir(path):
            pass
        else:
            raise
            
def GetExeSuffix():
    """Returns '' or '.exe' depending on how executables work on this platform."""
    if sys.platform.startswith(('cygwin', 'win')):
        return '.exe'
    return ''            

args = {"folder": "build"}
supported_sys = ["osx", "win2017", "android", "ios", "win2015", "hisilicon"]
supported_build_system = ["make"]
for parameter in sys.argv:
    if (parameter in supported_sys):
        args["system"] = parameter
    elif(parameter in supported_build_system):
        args["build_system"] = parameter        
    elif("build.py" in parameter):
        pass
    else:
        args["folder"] = parameter

if (not "system" in args):
    print("you must specifiy one system [%s] in paramters." % (",".join(supported_sys)))
    exit(-1)

print("Preparing build script for system: %s" % args["system"])

currentDir = os.getcwd()
path = os.path.join(currentDir, args["folder"], args["system"])
mkdir_p(path)
os.chdir(path)

work_counts = str(int(multiprocessing.cpu_count() / 2))
print("work process: ", work_counts)

os.system("cmake --build . -- -j%s" % work_counts)
