#include <iostream>
#include "UnitTestPP.h"

AssertException::AssertException(const std::string &TestNameIn, const std::string &MsgIn, size_t LineIn, const std::string &FnIdIn) {
	TestName = TestNameIn;
	Msg = MsgIn;
	Line = LineIn;
	FnId = FnIdIn;
}
std::ostream &operator<<(std::ostream &os, const AssertException &Exc) {
	return (
		os << "AssertException{TEST_NAME: " << Exc.TestName <<
		", MSG: " << Exc.Msg <<
		", LINE: " << Exc.Line <<
		", FUNCTION: " << Exc.FnId << "}");
}
TestCase::~TestCase() {}
void TestCase::setUp() {}
void TestCase::tearDown() {}
void TestCase::run_tests(std::ostream &out) {
	size_t c = 0;
	size_t Pass = 0;
	size_t Fail = 0;
	bool *Status = new bool[Tests.size()];
	out << "Begin Tests for " << Name << std::endl;
	for (TestFunc Test: Tests) {
		try{
			setUp();
			(this->*Test)();
			tearDown();
			Status[c] = true;
			++Pass;
		}
		catch (AssertException &Exc) {
			out << "  Test Failed: " << Exc << std::endl;
			Status[c] = false;
			++Fail;
		}
		catch (...) {
			out << "  Test Failed: UNKNOWN_ERROR{TEST_NAME: " << Name << ", TEST_NUM: " << c << "}" << std::endl;
			Status[c] = false;
			++Fail;
		}
		c += 1;
	}
	out << "  Begin Test Summary" << std::endl;
	for (c = 0; c < Tests.size(); ++c) {
		out << "  Test " << c << ": " << (Status[c] ? "PASSED" : "FAILED") << std::endl;
	}
	delete[] Status;
	out << "  End Test Summary" << std::endl;
	out << "  Test Results for " << Name << "\n    PASSED: " << Pass << "\n    FAILED: " << Fail << std::endl;
	out << "Finished Tests for " << Name << std::endl;
}

void main_assert(bool Expr, TestCase *Test, const std::string &Msg, size_t Line, const std::string &FnId) {
	if (!Expr) throw AssertException(Test->Name, Msg, Line, FnId);
}
void main_assertNot(bool Expr, TestCase *Test, const std::string &Msg, size_t Line, const std::string &FnId) {
	if (Expr) throw AssertException(Test->Name, Msg, Line, FnId);
}

struct TestEntry{
	TestCase *Obj;
	TestCaseDel Deleter;
};

std::vector<TestEntry> AllTestCases;
void DefTestCaseDel(TestCase *Obj) {
	delete Obj;
}
void AddTestCase(TestCase *Add, TestCaseDel Deleter) {
	AllTestCases.push_back({Add, Deleter});
}

int main() {
	InitTests();
	for (TestEntry &Cur : AllTestCases) {
		Cur.Obj->run_tests(std::cout);
	}
	for (TestEntry &Cur : AllTestCases) {
		Cur.Deleter(Cur.Obj);
	}
}
