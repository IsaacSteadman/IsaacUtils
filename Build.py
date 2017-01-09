#!/bin/python2
import subprocess
import os
LstSources = map(lambda x: "./IsaacUtils/" + x, [
    "CryptoUtils.cpp", "FsUtils.cpp", "IsaacUtils.cpp", "NixImp.cpp", "SockUtils.cpp", "String.cpp", "Utils.cpp"])
GCCArgs = ["gcc", "-fdiagnostics-color=always", "-lstdc++", "-ldl", "-L.", "-pthread", "-Wall"]
IsaacUtilsArgs = ["-shared", "-fPIC", "-o", "libIsaacUtils.so"]
LstTestSources = ["./TestIsaacUtils/TestIsaacUtils.cpp"]
TestIsaacUtilsArgs = ["-L.", "-lIsaacUtils", "-fpie", "-o", "IsaacUtilsTest"]
LstTest1Sources = map("./Test1IsaacUtils/".__add__, [
    "Test1IsaacUtils.cpp", "UnitTestPP.cpp", "Test_String.cpp", "Test_wString.cpp"])
Test1IsaacUtilsArgs = ["-L.", "-lIsaacUtils", "-fpie", "-o", "IsaacUtilsTest1"]
print "BUILDING: IsaacUtils"
Rtn = subprocess.call(GCCArgs + IsaacUtilsArgs + LstSources)
if Rtn != 0:
    print "ERROR: build of IsaacUtils failed"
    raise SystemExit(Rtn)
print "BUILDING: Test1IsaacUtils"
Rtn = subprocess.call(GCCArgs + Test1IsaacUtilsArgs + LstTest1Sources)
if Rtn != 0:
    print "ERROR: build of Test1IsaacUtils failed"
    raise SystemExit(Rtn)
print "TESTING: with Test1IsaacUtils"
WasIn = "LD_LIBRARY_PATH" in os.environ
Orig = os.environ["LD_LIBRARY_PATH"] if WasIn else None
os.environ["LD_LIBRARY_PATH"] = "."
Rtn = subprocess.call(["./IsaacUtilsTest1"])
if WasIn: os.environ["LD_LIBRARY_PATH"] = Orig
else: del os.environ["LD_LIBRARY_PATH"]
if Rtn != 0:
    print "ERROR: Test runner failed to run"

