#include <string>
#include <vector>
#include <ostream>

class AssertException {
public:
	std::string TestName;
	std::string Msg;
	size_t Line;
	std::string FnId;
	AssertException(const std::string &TestNameIn, const std::string &MsgIn, size_t LineIn, const std::string &FnIdIn);
};
std::ostream &operator<<(std::ostream &os, const AssertException &Exc);
class TestCase {
public:
	typedef void(TestCase::*TestFunc)();
	std::string Name;
	std::vector<TestFunc> Tests;
	virtual void setUp();
	virtual void tearDown();
	void run_tests(std::ostream &out);
	virtual ~TestCase();
};
typedef void(*TestCaseDel)(TestCase *);
void DefTestCaseDel(TestCase *Obj);
void AddTestCase(TestCase *Add, TestCaseDel Deleter=DefTestCaseDel);
void main_assert(bool Expr, TestCase *Test, const std::string &Msg, size_t Line, const std::string &FnId);
void main_assertNot(bool Expr, TestCase *Test, const std::string &Msg, size_t Line, const std::string &FnId);
#define assertTrue(a, Msg) main_assert(a, this, Msg, __LINE__, __PRETTY_FUNCTION__)
#define assertFalse(a, Msg) main_assertNot(a, this, Msg, __LINE__, __PRETTY_FUNCTION__)
#define assertEqual(a, b, Msg) main_assert(a == b, this, Msg, __LINE__, __PRETTY_FUNCTION__)
#define assertNotEqual(a, b, Msg) main_assertNot(a == b, this, Msg, __LINE__, __PRETTY_FUNCTION__)
//You Define the stuff below
void InitTests();
