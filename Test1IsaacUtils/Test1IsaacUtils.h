#include "../IsaacUtils/Utils.h"
#include "UnitTestPP.h"
class Test_wString : public TestCase {
public:
	Test_wString();
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
	Test_String();
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

class Test_BaseBigLong : public TestCase {
public:
	Test_BaseBigLong();
	void test_FromStr();
	void test_ToStr();
	void test_BytesNomMul();
	void test_BytesKarMul();
	void test_BytesMul();
	void test_StrNomMul();
	void test_StrKarMul();
	void test_StrMul();
};

class Test_BigLong : public TestCase {
private:
	Utils::BigLong blA, blB, blE, blF;
public:
	Test_BigLong();
	virtual void setUp();
	virtual void tearDown();
	void test_Add();
	void test_Sub();
	void test_Mul();
	void test_Div();
	void test_Mod();
	void test_And();
	void test_Or();
	void test_Xor();
	void test_RShift();
	void test_LShift();
	void test_IAdd();
	void test_ISub();
	void test_IMul();
	void test_IDiv();
	void test_IMod();
	void test_IAnd();
	void test_IOr();
	void test_IXor();
	void test_IRShift();
	void test_ILShift();
	void test_IsEq();
	void test_IsGe();
	void test_IsGt();
	void test_IsLe();
	void test_IsLt();
	void test_IsNe();
};
