#include "../IsaacUtils/Utils.h"
#include "UnitTestPP.h"
using Utils::Array;
using Utils::wString;
using Utils::String;
class Test_wString : public TestCase {
public:
	Test_wString() {
		Name = "Test_wString";
		Tests.push_back((TestCase::TestFunc)(&Test_wString::test_Split));
		Tests.push_back((TestCase::TestFunc)(&Test_wString::test_RSplit));
		Tests.push_back((TestCase::TestFunc)(&Test_wString::test_Join));
		Tests.push_back((TestCase::TestFunc)(&Test_wString::test_Replace_Ch_Ch));
		Tests.push_back((TestCase::TestFunc)(&Test_wString::test_Replace_Ch_Str));
		Tests.push_back((TestCase::TestFunc)(&Test_wString::test_Replace_Str_Ch));
		Tests.push_back((TestCase::TestFunc)(&Test_wString::test_Find_Ch));
		Tests.push_back((TestCase::TestFunc)(&Test_wString::test_Find_Str));
		Tests.push_back((TestCase::TestFunc)(&Test_wString::test_RFind_Ch));
		Tests.push_back((TestCase::TestFunc)(&Test_wString::test_RFind_Str));
	}
	void test_Split();
	void test_RSplit();
	void test_Join();
	void test_Replace_Ch_Ch();
	void test_Replace_Ch_Str();
	void test_Replace_Str_Ch();
	void test_Find_Ch();
	void test_Find_Str();
	void test_RFind_Ch();
	void test_RFind_Str();
};

class Test_String : public TestCase {
public:
	Test_String() {
		Name = "Test_String";
		Tests.push_back((TestCase::TestFunc)(&Test_String::test_Split));
		Tests.push_back((TestCase::TestFunc)(&Test_String::test_RSplit));
		Tests.push_back((TestCase::TestFunc)(&Test_String::test_Join));
		Tests.push_back((TestCase::TestFunc)(&Test_String::test_Replace_Ch_Ch));
		Tests.push_back((TestCase::TestFunc)(&Test_String::test_Replace_Ch_Str));
		Tests.push_back((TestCase::TestFunc)(&Test_String::test_Replace_Str_Ch));
		Tests.push_back((TestCase::TestFunc)(&Test_String::test_Find_Ch));
		Tests.push_back((TestCase::TestFunc)(&Test_String::test_Find_Str));
		Tests.push_back((TestCase::TestFunc)(&Test_String::test_RFind_Ch));
		Tests.push_back((TestCase::TestFunc)(&Test_String::test_RFind_Str));
	}
	void test_Split();
	void test_RSplit();
	void test_Join();
	void test_Replace_Ch_Ch();
	void test_Replace_Ch_Str();
	void test_Replace_Str_Ch();
	void test_Find_Ch();
	void test_Find_Str();
	void test_RFind_Ch();
	void test_RFind_Str();
};
