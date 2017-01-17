#!/bin/python2
import subprocess
import os
import sys
class BaseContext(object):
    def __add__(self, other): return AddContext(self, other)
    def __mul__(self, other): return MulContext(self, other)
    def __enter__(self): pass
    def __exit__(self, exc_t, exc_v, tb): return False
class OpContext(BaseContext):
    def __init__(self, Cntxt0, Cntxt1):
        self.Cntxt0 = Cntxt0
        self.Cntxt1 = Cntxt1
    def __enter__(self):
        self.Cntxt0.__enter__()
        self.Cntxt1.__enter__()
    def __exit__(self, exc_t, exc_v, tb):
        Res0 = self.Cntxt0.__exit__(exc_t, exc_v, tb)
        Res1 = self.Cntxt1.__exit__(exc_t, exc_v, tb)
        return self.OpFunc(Res0, Res1)
    def OpFunc(self, Res0, Res1): return False
class AddContext(OpContext):
    def OpFunc(self, Res0, Res1): return Res0 or Res1
class MulContext(OpContext):
    def OpFunc(self, Res0, Res1): return Res0 and Res1
class VarContext(BaseContext):
    def __init__(self, Var, DctAttr):
        self.DctAttr = DctAttr
        self.Var = Var
    def __enter__(self):
        self.DctOrig = {k: getattr(self.Var, k) for k in self.DctAttr}
        for k in self.DctAttr: setattr(self.Var, k, self.DctAttr[k])
    def __exit__(self, exc_t, exc_v, tb):
        for k in self.DctOrig: setattr(self.Var, k, self.DctOrig[k])
class FuncContext(BaseContext):
    # e1 means pass exception data to f1
    def __init__(self, f0, f1, e1=False, args0=None, args1=None, kwargs0=None, kwargs1=None):
        self.f0 = f0
        self.f1 = f1
        self.e1 = e1
        self.args0 = list() if args0 is None else args0
        self.args1 = list() if args1 is None else args1
        self.kwargs0 = dict() if kwargs0 is None else kwargs0
        self.kwargs1 = dict() if kwargs1 is None else kwargs1
    def __enter__(self):
        self.f0(*self.args0, **self.kwargs0)
    def __exit__(self, exc_t, exc_v, tb):
        if self.e1:
            return self.f1(exc_t, exc_v, tb, *self.args1, **self.kwargs1)
        else:
            return self.f1(*self.args1, **self.kwargs1)
class GetSetContext(BaseContext):
    def __init__(self, Var, AttrGetter, AttrSetter, SetVal, *args):
        self.FuncAttrs = (AttrGetter, AttrSetter)
        self.Var = Var
        self.SetVal = SetVal
        self.args = args
    def __enter__(self):
        self.OrigVal = getattr(self.Var, self.FuncAttrs[0])(*self.args)
        getattr(self.Var, self.FuncAttrs[1])(*(list(self.args) + [self.SetVal]))
    def __exit__(self, exc_t, exc_v, tb):
        getattr(self.Var, self.FuncAttrs[1])(*(list(self.args) + [self.OrigVal]))
class GetSetItemContext(BaseContext):
    def __init__(self, Var, value, index):
        self.Var = Var
        self.SetVal = value
        self.index = index
    def __enter__(self):
        self.OrigVal = self.Var[self.index]
        self.Var[self.index] = self.SetVal
    def __exit__(self, exc_t, exc_v, tb):
        self.Var[self.index] = self.SetVal
# requires that Var be dict-like (not list-like)
class GetSetItemsContext(BaseContext):
    def __init__(self, Var, DctSetVals):
        self.Var = Var
        self.DctSetVals = DctSetVals
    def __enter__(self):
        self.OrigValues = {k: self.Var[k] for k in self.DctSetVals if k in self.Var}
        for k in self.DctSetVals: self.Var[k] = self.DctSetVals[k]
    def __exit__(self, exc_t, exc_v, tb):
        for k in self.DctSetVals:
            if k in self.OrigValues:
                self.Var[k] = self.OrigValues[k]
            else:
                del self.Var[k]
class MajorStep(object):
    def __init__(self, Name):
        self.Name = Name
class BadReturnCode(ValueError):
    def __init__(self, Code, Msg):
        super(BadReturnCode, self).__init__(Code, Msg)
        self.Code = Code
DefCheckRtn = lambda Rtn: Rtn == 0
class SubProcStep(MajorStep):
    def __init__(self, Name, ProcArgs, ChgEnv=None, DctSubProcArgs=None, FailAll=True, CheckRtnFn=None):
        super(SubProcStep, self).__init__(Name)
        self.FailAll = FailAll
        self.CheckRtnFn = DefCheckRtn if CheckRtnFn is None else CheckRtnFn
        self.DctSubProcArgs = {} if DctSubProcArgs is None else DctSubProcArgs
        self.ProcArgs = ProcArgs
        self.ChgEnv = {} if ChgEnv is None else ChgEnv
    def Run(self):
        Rtn = None
        with GetSetItemsContext(os.environ, self.ChgEnv):
            Rtn = subprocess.call(self.ProcArgs, **self.DctSubProcArgs)
        if not self.CheckRtnFn(Rtn):
            raise BadReturnCode(Rtn, "ERROR: '%s' failed" % self.Name)
def RunSteps(DoRun, Steps):
    for c in xrange(len(Steps)):
        Cur = Steps[c]
        print "BEGIN: %s" % Cur.Name
        IsDoRun = True
        try:
            IsDoRun = DoRun[c]
        except StandardError:
            pass
        if IsDoRun:
            try:
                Cur.Run()
            except BadReturnCode as Exc:
                print Exc
                if Cur.FailAll:
                    raise SystemExit(Exc.Code)
        else:
            print "  Skipped"
        print "END: %s" % Cur.Name
DoRun = {}
for arg in sys.argv[1:]:
    if arg.startswith("-x"):
        for Ch in arg[2:]:
            if Ch.isdigit():
                DoRun[int(Ch)] = False
    elif arg.startswith("-skip"):
        for Ch in arg[5:]:
            if Ch.isdigit():
                DoRun[int(Ch)] = False
LstSources = map(lambda x: "./IsaacUtils/" + x, [
    "CryptoUtils.cpp", "FsUtils.cpp", "IsaacUtils.cpp", "NixImp.cpp", "SockUtils.cpp", "String.cpp", "Utils.cpp"])
GCCArgs = ["gcc", "-fdiagnostics-color=always", "-lstdc++", "-ldl", "-L.", "-pthread", "-Wall"]
IsaacUtilsArgs = ["-shared", "-fPIC", "-o", "libIsaacUtils.so"]
LstTestSources = ["./TestIsaacUtils/TestIsaacUtils.cpp"]
TestIsaacUtilsArgs = ["-L.", "-lIsaacUtils", "-fpie", "-o", "IsaacUtilsTest"]
LstTest1Sources = map("./Test1IsaacUtils/".__add__, [
    "Test1IsaacUtils.cpp", "UnitTestPP.cpp", "Test_String.cpp", "Test_wString.cpp", "Test_BaseBigLong.cpp", "Test_BigLong.cpp"])
Test1IsaacUtilsArgs = ["-L.", "-lIsaacUtils", "-fpie", "-o", "IsaacUtilsTest1"]
Steps = [
    SubProcStep("build of IsaacUtils", GCCArgs + IsaacUtilsArgs + LstSources),
    SubProcStep("build of Test1IsaacUtils", GCCArgs + Test1IsaacUtilsArgs + LstTest1Sources),
    SubProcStep("test of IsaacUtils with Test1IsaacUtils", ["./IsaacUtilsTest1"], {"LD_LIBRARY_PATH": "."})]
RunSteps(DoRun, Steps)
