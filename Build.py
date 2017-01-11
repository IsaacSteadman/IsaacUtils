#!/bin/python2
import subprocess
import os
import sys
LstSources = map(lambda x: "./IsaacUtils/" + x, [
    "CryptoUtils.cpp", "FsUtils.cpp", "IsaacUtils.cpp", "NixImp.cpp", "SockUtils.cpp", "String.cpp", "Utils.cpp"])
GCCArgs = ["gcc", "-fdiagnostics-color=always", "-lstdc++", "-ldl", "-L.", "-pthread", "-Wall"]
IsaacUtilsArgs = ["-shared", "-fPIC", "-o", "libIsaacUtils.so"]
LstTestSources = ["./TestIsaacUtils/TestIsaacUtils.cpp"]
TestIsaacUtilsArgs = ["-L.", "-lIsaacUtils", "-fpie", "-o", "IsaacUtilsTest"]
LstTest1Sources = map("./Test1IsaacUtils/".__add__, [
    "Test1IsaacUtils.cpp", "UnitTestPP.cpp", "Test_String.cpp", "Test_wString.cpp", "Test_BaseBigLong.cpp", "Test_BigLong.cpp"])
Test1IsaacUtilsArgs = ["-L.", "-lIsaacUtils", "-fpie", "-o", "IsaacUtilsTest1"]
print "BUILDING: IsaacUtils"
DoRun = [True, True, True]
for arg in sys.argv[1:]:
    if arg.startswith("-x"):
        for Ch in arg[2:]:
            if Ch.isdigit():
                DoRun[int(Ch)] = False
    elif arg.startswith("-skip"):
        for Ch in arg[5:]:
            if Ch.isdigit():
                DoRun[int(Ch)] = False
if DoRun[0]:
    Rtn = subprocess.call(GCCArgs + IsaacUtilsArgs + LstSources)
    if Rtn != 0:
        print "ERROR: build of IsaacUtils failed"
        raise SystemExit(Rtn)
else: print "  Skipped"
print "BUILDING: Test1IsaacUtils"
if DoRun[1]:
    Rtn = subprocess.call(GCCArgs + Test1IsaacUtilsArgs + LstTest1Sources)
    if Rtn != 0:
        print "ERROR: build of Test1IsaacUtils failed"
        raise SystemExit(Rtn)
else: print "  Skipped"
print "TESTING: with Test1IsaacUtils"
if DoRun[2]:
    WasIn = "LD_LIBRARY_PATH" in os.environ
    Orig = os.environ["LD_LIBRARY_PATH"] if WasIn else None
    os.environ["LD_LIBRARY_PATH"] = "."
    Rtn = subprocess.call(["./IsaacUtilsTest1"])
    if WasIn: os.environ["LD_LIBRARY_PATH"] = Orig
    else: del os.environ["LD_LIBRARY_PATH"]
    if Rtn != 0:
        print "ERROR: Test runner failed to run"
else: print "  Skipped"

