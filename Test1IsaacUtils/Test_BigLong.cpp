#include "Test1IsaacUtils.h"
#include <string>
using Utils::BigLong;
using Utils::Array;
using Utils::ByteArray;
using Utils::String;
using Utils::Byte;
Test_BigLong::Test_BigLong() {
	Name = "Test_BigLong";
	blA.FromStr("0");
	blB.FromStr("0");
	blE.FromStr("0");
	blF.FromStr("0");
	Tests.push_back((TestCase::TestFunc)(&Test_BigLong::test_Add));
	Tests.push_back((TestCase::TestFunc)(&Test_BigLong::test_Sub));
	Tests.push_back((TestCase::TestFunc)(&Test_BigLong::test_Mul));
	Tests.push_back((TestCase::TestFunc)(&Test_BigLong::test_Div));
	Tests.push_back((TestCase::TestFunc)(&Test_BigLong::test_Mod));
	Tests.push_back((TestCase::TestFunc)(&Test_BigLong::test_And));
	Tests.push_back((TestCase::TestFunc)(&Test_BigLong::test_Or));
	Tests.push_back((TestCase::TestFunc)(&Test_BigLong::test_Xor));
	Tests.push_back((TestCase::TestFunc)(&Test_BigLong::test_RShift));
	Tests.push_back((TestCase::TestFunc)(&Test_BigLong::test_LShift));
	Tests.push_back((TestCase::TestFunc)(&Test_BigLong::test_IAdd));
	Tests.push_back((TestCase::TestFunc)(&Test_BigLong::test_ISub));
	Tests.push_back((TestCase::TestFunc)(&Test_BigLong::test_IMul));
	Tests.push_back((TestCase::TestFunc)(&Test_BigLong::test_IDiv));
	Tests.push_back((TestCase::TestFunc)(&Test_BigLong::test_IMod));
	Tests.push_back((TestCase::TestFunc)(&Test_BigLong::test_IAnd));
	Tests.push_back((TestCase::TestFunc)(&Test_BigLong::test_IOr));
	Tests.push_back((TestCase::TestFunc)(&Test_BigLong::test_IXor));
	Tests.push_back((TestCase::TestFunc)(&Test_BigLong::test_IRShift));
	Tests.push_back((TestCase::TestFunc)(&Test_BigLong::test_ILShift));
	Tests.push_back((TestCase::TestFunc)(&Test_BigLong::test_IsEq));
	Tests.push_back((TestCase::TestFunc)(&Test_BigLong::test_IsGe));
	Tests.push_back((TestCase::TestFunc)(&Test_BigLong::test_IsGt));
	Tests.push_back((TestCase::TestFunc)(&Test_BigLong::test_IsLe));
	Tests.push_back((TestCase::TestFunc)(&Test_BigLong::test_IsLt));
	Tests.push_back((TestCase::TestFunc)(&Test_BigLong::test_IsNe));
}
void Test_BigLong::setUp() {
	blA.FromStr("4423591923692010144000");
	blB.FromStr("48297727666299140");
	blE.FromStr("48297727666299140");
	blF.FromStr("4423591923692010144000");
}
void Test_BigLong::tearDown() {
	blA.FromStr("0");
	blB.FromStr("0");
	blE.FromStr("0");
	blF.FromStr("0");
}
void Test_BigLong::test_Add() {
	{
		BigLong blC = blA + blB;
		String Test;
		blA.ToStr(Test);
		assertEqual(Test, "4423591923692010144000", "No Modify blA Failed");
		blB.ToStr(Test);
		assertEqual(Test, "48297727666299140", "No Modify blB Failed");
		blC.ToStr(Test);
		assertEqual(Test, "4423640221419676443140", "Add Failed Result");
	}
	{
		BigLong blC = blE + blF;
		String Test;
		blE.ToStr(Test);
		assertEqual(Test, "48297727666299140", "No Modify blE Failed");
		blF.ToStr(Test);
		assertEqual(Test, "4423591923692010144000", "No Modify blF Failed");
		blC.ToStr(Test);
		assertEqual(Test, "4423640221419676443140", "Add Failed Result");
	}
	{
		BigLong blC = blA + blF;
		String Test;
		blA.ToStr(Test);
		assertEqual(Test, "4423591923692010144000", "No Modify blA Failed");
		blF.ToStr(Test);
		assertEqual(Test, "4423591923692010144000", "No Modify blF Failed");
		blC.ToStr(Test);
		assertEqual(Test, "8847183847384020288000", "Add Failed Result");
	}
	{
		BigLong blC = blE + blB;
		String Test;
		blE.ToStr(Test);
		assertEqual(Test, "48297727666299140", "No Modify blE Failed");
		blB.ToStr(Test);
		assertEqual(Test, "48297727666299140", "No Modify blB Failed");
		blC.ToStr(Test);
		assertEqual(Test, "96595455332598280", "Add Failed Result");
	}
}
void Test_BigLong::test_Sub() {
	{
		BigLong blC = blA - blB;
		String Test;
		blA.ToStr(Test);
		assertEqual(Test, "4423591923692010144000", "No Modify blA Failed");
		blB.ToStr(Test);
		assertEqual(Test, "48297727666299140", "No Modify blB Failed");
		blC.ToStr(Test);
		assertEqual(Test, "4423543625964343844860", "Sub Failed Result");
	}
	{
		BigLong blC = blE - blF;
		String Test;
		blE.ToStr(Test);
		assertEqual(Test, "48297727666299140", "No Modify blE Failed");
		blF.ToStr(Test);
		assertEqual(Test, "4423591923692010144000", "No Modify blF Failed");
		blC.ToStr(Test);
		assertEqual(Test, "-4423543625964343844860", "Sub Failed Result");
	}
	{
		BigLong blC = blA - blF;
		String Test;
		blA.ToStr(Test);
		assertEqual(Test, "4423591923692010144000", "No Modify blA Failed");
		blF.ToStr(Test);
		assertEqual(Test, "4423591923692010144000", "No Modify blF Failed");
		blC.ToStr(Test);
		assertEqual(Test, "0", "Sub Failed Result");
	}
	{
		BigLong blC = blE - blB;
		String Test;
		blE.ToStr(Test);
		assertEqual(Test, "48297727666299140", "No Modify blE Failed");
		blB.ToStr(Test);
		assertEqual(Test, "48297727666299140", "No Modify blB Failed");
		blC.ToStr(Test);
		assertEqual(Test, "0", "Sub Failed Result");
	}
}
void Test_BigLong::test_Mul() {
	{
		BigLong blC = blA * blB;
		String Test;
		blA.ToStr(Test);
		assertEqual(Test, "4423591923692010144000", "No Modify blA Failed");
		blB.ToStr(Test);
		assertEqual(Test, "48297727666299140", "No Modify blB Failed");
		blC.ToStr(Test);
		assertEqual(Test, "213649438037317032483074671818476160000", "Mul Failed Result");
	}
	{
		BigLong blC = blE * blF;
		String Test;
		blE.ToStr(Test);
		assertEqual(Test, "48297727666299140", "No Modify blE Failed");
		blF.ToStr(Test);
		assertEqual(Test, "4423591923692010144000", "No Modify blF Failed");
		blC.ToStr(Test);
		assertEqual(Test, "213649438037317032483074671818476160000", "Mul Failed Result");
	}
	{
		BigLong blC = blA * blF;
		String Test;
		blA.ToStr(Test);
		assertEqual(Test, "4423591923692010144000", "No Modify blA Failed");
		blF.ToStr(Test);
		assertEqual(Test, "4423591923692010144000", "No Modify blF Failed");
		blC.ToStr(Test);
		assertEqual(Test, "19568165507353178896743811863398900736000000", "Mul Failed Result");
	}
	{
		BigLong blC = blE * blB;
		String Test;
		blE.ToStr(Test);
		assertEqual(Test, "48297727666299140", "No Modify blE Failed");
		blB.ToStr(Test);
		assertEqual(Test, "48297727666299140", "No Modify blB Failed");
		blC.ToStr(Test);
		assertEqual(Test, "2332670497727997372064103964739600", "Mul Failed Result");
	}
}
void Test_BigLong::test_Div() {
	{
		BigLong blC = blA / blB;
		String Test;
		blA.ToStr(Test);
		assertEqual(Test, "4423591923692010144000", "No Modify blA Failed");
		blB.ToStr(Test);
		assertEqual(Test, "48297727666299140", "No Modify blB Failed");
		blC.ToStr(Test);
		assertEqual(Test, "91590", "Div Failed Result");
	}
	{
		BigLong blC = blE / blF;
		String Test;
		blE.ToStr(Test);
		assertEqual(Test, "48297727666299140", "No Modify blE Failed");
		blF.ToStr(Test);
		assertEqual(Test, "4423591923692010144000", "No Modify blF Failed");
		blC.ToStr(Test);
		assertEqual(Test, "0", "Div Failed Result");
	}
	{
		BigLong blC = blA / blF;
		String Test;
		blA.ToStr(Test);
		assertEqual(Test, "4423591923692010144000", "No Modify blA Failed");
		blF.ToStr(Test);
		assertEqual(Test, "4423591923692010144000", "No Modify blF Failed");
		blC.ToStr(Test);
		assertEqual(Test, "1", "Div Failed Result");
	}
	{
		BigLong blC = blE / blB;
		String Test;
		blE.ToStr(Test);
		assertEqual(Test, "48297727666299140", "No Modify blE Failed");
		blB.ToStr(Test);
		assertEqual(Test, "48297727666299140", "No Modify blB Failed");
		blC.ToStr(Test);
		assertEqual(Test, "1", "Div Failed Result");
	}
}
void Test_BigLong::test_Mod() {
	{
		BigLong blC = blA % blB;
		String Test;
		blA.ToStr(Test);
		assertEqual(Test, "4423591923692010144000", "No Modify blA Failed");
		blB.ToStr(Test);
		assertEqual(Test, "48297727666299140", "No Modify blB Failed");
		blC.ToStr(Test);
		assertEqual(Test, "3046735671911400", "Mod Failed Result");
	}
	{
		BigLong blC = blE % blF;
		String Test;
		blE.ToStr(Test);
		assertEqual(Test, "48297727666299140", "No Modify blE Failed");
		blF.ToStr(Test);
		assertEqual(Test, "4423591923692010144000", "No Modify blF Failed");
		blC.ToStr(Test);
		assertEqual(Test, "48297727666299140", "Mod Failed Result");
	}
	{
		BigLong blC = blA % blF;
		String Test;
		blA.ToStr(Test);
		assertEqual(Test, "4423591923692010144000", "No Modify blA Failed");
		blF.ToStr(Test);
		assertEqual(Test, "4423591923692010144000", "No Modify blF Failed");
		blC.ToStr(Test);
		assertEqual(Test, "0", "Mod Failed Result");
	}
	{
		BigLong blC = blE % blB;
		String Test;
		blE.ToStr(Test);
		assertEqual(Test, "48297727666299140", "No Modify blE Failed");
		blB.ToStr(Test);
		assertEqual(Test, "48297727666299140", "No Modify blB Failed");
		blC.ToStr(Test);
		assertEqual(Test, "0", "Mod Failed Result");
	}
}
void Test_BigLong::test_And() {
	{
		BigLong blC = blA & blB;
		String Test;
		blA.ToStr(Test);
		assertEqual(Test, "4423591923692010144000", "No Modify blA Failed");
		blB.ToStr(Test);
		assertEqual(Test, "48297727666299140", "No Modify blB Failed");
		blC.ToStr(Test);
		assertEqual(Test, "48272988654665984", "And Failed Result");
	}
	{
		BigLong blC = blE & blF;
		String Test;
		blE.ToStr(Test);
		assertEqual(Test, "48297727666299140", "No Modify blE Failed");
		blF.ToStr(Test);
		assertEqual(Test, "4423591923692010144000", "No Modify blF Failed");
		blC.ToStr(Test);
		assertEqual(Test, "48272988654665984", "And Failed Result");
	}
	{
		BigLong blC = blA & blF;
		String Test;
		blA.ToStr(Test);
		assertEqual(Test, "4423591923692010144000", "No Modify blA Failed");
		blF.ToStr(Test);
		assertEqual(Test, "4423591923692010144000", "No Modify blF Failed");
		blC.ToStr(Test);
		assertEqual(Test, "4423591923692010144000", "And Failed Result");
	}
	{
		BigLong blC = blE & blB;
		String Test;
		blE.ToStr(Test);
		assertEqual(Test, "48297727666299140", "No Modify blE Failed");
		blB.ToStr(Test);
		assertEqual(Test, "48297727666299140", "No Modify blB Failed");
		blC.ToStr(Test);
		assertEqual(Test, "48297727666299140", "And Failed Result");
	}
}
void Test_BigLong::test_Or() {
	{
		BigLong blC = blA | blB;
		String Test;
		blA.ToStr(Test);
		assertEqual(Test, "4423591923692010144000", "No Modify blA Failed");
		blB.ToStr(Test);
		assertEqual(Test, "48297727666299140", "No Modify blB Failed");
		blC.ToStr(Test);
		assertEqual(Test, "4423591948431021777156", "Or Failed Result");
	}
	{
		BigLong blC = blE | blF;
		String Test;
		blE.ToStr(Test);
		assertEqual(Test, "48297727666299140", "No Modify blE Failed");
		blF.ToStr(Test);
		assertEqual(Test, "4423591923692010144000", "No Modify blF Failed");
		blC.ToStr(Test);
		assertEqual(Test, "4423591948431021777156", "Or Failed Result");
	}
	{
		BigLong blC = blA | blF;
		String Test;
		blA.ToStr(Test);
		assertEqual(Test, "4423591923692010144000", "No Modify blA Failed");
		blF.ToStr(Test);
		assertEqual(Test, "4423591923692010144000", "No Modify blF Failed");
		blC.ToStr(Test);
		assertEqual(Test, "4423591923692010144000", "Or Failed Result");
	}
	{
		BigLong blC = blE | blB;
		String Test;
		blE.ToStr(Test);
		assertEqual(Test, "48297727666299140", "No Modify blE Failed");
		blB.ToStr(Test);
		assertEqual(Test, "48297727666299140", "No Modify blB Failed");
		blC.ToStr(Test);
		assertEqual(Test, "48297727666299140", "Or Failed Result");
	}
}
void Test_BigLong::test_Xor() {
	{
		BigLong blC = blA ^ blB;
		String Test;
		blA.ToStr(Test);
		assertEqual(Test, "4423591923692010144000", "No Modify blA Failed");
		blB.ToStr(Test);
		assertEqual(Test, "48297727666299140", "No Modify blB Failed");
		blC.ToStr(Test);
		assertEqual(Test, "4423543675442367111172", "Xor Failed Result");
	}
	{
		BigLong blC = blE ^ blF;
		String Test;
		blE.ToStr(Test);
		assertEqual(Test, "48297727666299140", "No Modify blE Failed");
		blF.ToStr(Test);
		assertEqual(Test, "4423591923692010144000", "No Modify blF Failed");
		blC.ToStr(Test);
		assertEqual(Test, "4423543675442367111172", "Xor Failed Result");
	}
	{
		BigLong blC = blA ^ blF;
		String Test;
		blA.ToStr(Test);
		assertEqual(Test, "4423591923692010144000", "No Modify blA Failed");
		blF.ToStr(Test);
		assertEqual(Test, "4423591923692010144000", "No Modify blF Failed");
		blC.ToStr(Test);
		assertEqual(Test, "0", "Xor Failed Result");
	}
	{
		BigLong blC = blE ^ blB;
		String Test;
		blE.ToStr(Test);
		assertEqual(Test, "48297727666299140", "No Modify blE Failed");
		blB.ToStr(Test);
		assertEqual(Test, "48297727666299140", "No Modify blB Failed");
		blC.ToStr(Test);
		assertEqual(Test, "0", "Xor Failed Result");
	}
}
void Test_BigLong::test_RShift() {
	// operator<<
	{
		BigLong blC = blA << 1;
		String Test;
		blA.ToStr(Test);
		assertEqual(Test, "4423591923692010144000", "No Modify blA Failed");
		blC.ToStr(Test);
		assertEqual(Test, "8847183847384020288000", "RShift Failed Result");
	}
	{
		BigLong blC = blB << 23;
		String Test;
		blB.ToStr(Test);
		assertEqual(Test, "48297727666299140", "No Modify blB Failed");
		blC.ToStr(Test);
		assertEqual(Test, "405150704683338296197120", "RShift Failed Result");
	}
	{
		BigLong blC = blF << 12;
		String Test;
		blF.ToStr(Test);
		assertEqual(Test, "4423591923692010144000", "No Modify blF Failed");
		blC.ToStr(Test);
		assertEqual(Test, "18119032519442473549824000", "RShift Failed Result");
	}
	{
		BigLong blC = blE << 32;
		String Test;
		blE.ToStr(Test);
		assertEqual(Test, "48297727666299140", "No Modify blE Failed");
		blC.ToStr(Test);
		assertEqual(Test, "207437160797869207652925440", "RShift Failed Result");
	}
	{
		BigLong blC = blA << 31;
		String Test;
		blA.ToStr(Test);
		assertEqual(Test, "4423591923692010144000", "No Modify blA Failed");
		blC.ToStr(Test);
		assertEqual(Test, "9499591321553455572490125312000", "RShift Failed Result");
	}
	{
		BigLong blC = blB << 15;
		String Test;
		blB.ToStr(Test);
		assertEqual(Test, "48297727666299140", "No Modify blB Failed");
		blC.ToStr(Test);
		assertEqual(Test, "1582619940169290219520", "RShift Failed Result");
	}
	{
		BigLong blC = blF << 33;
		String Test;
		blF.ToStr(Test);
		assertEqual(Test, "4423591923692010144000", "No Modify blF Failed");
		blC.ToStr(Test);
		assertEqual(Test, "37998365286213822289960501248000", "RShift Failed Result");
	}
	{
		BigLong blC = blE << 17;
		String Test;
		blE.ToStr(Test);
		assertEqual(Test, "48297727666299140", "No Modify blE Failed");
		blC.ToStr(Test);
		assertEqual(Test, "6330479760677160878080", "RShift Failed Result");
	}
	{
		BigLong blC = blA << 0;
		String Test;
		blA.ToStr(Test);
		assertEqual(Test, "4423591923692010144000", "No Modify blA Failed");
		blC.ToStr(Test);
		assertEqual(Test, "4423591923692010144000", "RShift Failed Result");
	}
	{
		BigLong blC = blB << 0;
		String Test;
		blB.ToStr(Test);
		assertEqual(Test, "48297727666299140", "No Modify blB Failed");
		blC.ToStr(Test);
		assertEqual(Test, "48297727666299140", "RShift Failed Result");
	}
	{
		BigLong blC = blF << 0;
		String Test;
		blF.ToStr(Test);
		assertEqual(Test, "4423591923692010144000", "No Modify blF Failed");
		blC.ToStr(Test);
		assertEqual(Test, "4423591923692010144000", "RShift Failed Result");
	}
	{
		BigLong blC = blE << 0;
		String Test;
		blE.ToStr(Test);
		assertEqual(Test, "48297727666299140", "No Modify blE Failed");
		blC.ToStr(Test);
		assertEqual(Test, "48297727666299140", "RShift Failed Result");
	}
}
void Test_BigLong::test_LShift() {
	// operator>>
	{
		BigLong blC = blA >> 1;
		String Test;
		blA.ToStr(Test);
		assertEqual(Test, "4423591923692010144000", "No Modify blA Failed");
		blC.ToStr(Test);
		assertEqual(Test, "2211795961846005072000", "LShift Failed Result");
	}
	{
		BigLong blC = blB >> 23;
		String Test;
		blB.ToStr(Test);
		assertEqual(Test, "48297727666299140", "No Modify blB Failed");
		blC.ToStr(Test);
		assertEqual(Test, "5757537802", "LShift Failed Result");
	}
	{
		BigLong blC = blF >> 12;
		String Test;
		blF.ToStr(Test);
		assertEqual(Test, "4423591923692010144000", "No Modify blF Failed");
		blC.ToStr(Test);
		assertEqual(Test, "1079978496995119664", "LShift Failed Result");
	}
	{
		BigLong blC = blE >> 32;
		String Test;
		blE.ToStr(Test);
		assertEqual(Test, "48297727666299140", "No Modify blE Failed");
		blC.ToStr(Test);
		assertEqual(Test, "11245191", "LShift Failed Result");
	}
	{
		BigLong blC = blA >> 31;
		String Test;
		blA.ToStr(Test);
		assertEqual(Test, "4423591923692010144000", "No Modify blA Failed");
		blC.ToStr(Test);
		assertEqual(Test, "2059895509710", "LShift Failed Result");
	}
	{
		BigLong blC = blB >> 15;
		String Test;
		blB.ToStr(Test);
		assertEqual(Test, "48297727666299140", "No Modify blB Failed");
		blC.ToStr(Test);
		assertEqual(Test, "1473929677316", "LShift Failed Result");
	}
	{
		BigLong blC = blF >> 33;
		String Test;
		blF.ToStr(Test);
		assertEqual(Test, "4423591923692010144000", "No Modify blF Failed");
		blC.ToStr(Test);
		assertEqual(Test, "514973877427", "LShift Failed Result");
	}
	{
		BigLong blC = blE >> 17;
		String Test;
		blE.ToStr(Test);
		assertEqual(Test, "48297727666299140", "No Modify blE Failed");
		blC.ToStr(Test);
		assertEqual(Test, "368482419329", "LShift Failed Result");
	}
	{
		BigLong blC = blA >> 0;
		String Test;
		blA.ToStr(Test);
		assertEqual(Test, "4423591923692010144000", "No Modify blA Failed");
		blC.ToStr(Test);
		assertEqual(Test, "4423591923692010144000", "LShift Failed Result");
	}
	{
		BigLong blC = blB >> 0;
		String Test;
		blB.ToStr(Test);
		assertEqual(Test, "48297727666299140", "No Modify blB Failed");
		blC.ToStr(Test);
		assertEqual(Test, "48297727666299140", "LShift Failed Result");
	}
	{
		BigLong blC = blF >> 0;
		String Test;
		blF.ToStr(Test);
		assertEqual(Test, "4423591923692010144000", "No Modify blF Failed");
		blC.ToStr(Test);
		assertEqual(Test, "4423591923692010144000", "LShift Failed Result");
	}
	{
		BigLong blC = blE >> 0;
		String Test;
		blE.ToStr(Test);
		assertEqual(Test, "48297727666299140", "No Modify blE Failed");
		blC.ToStr(Test);
		assertEqual(Test, "48297727666299140", "LShift Failed Result");
	}
}
void Test_BigLong::test_IAdd() {
	{
		BigLong blC = blA;
		blC += blB;
		String Test;
		blA.ToStr(Test);
		assertEqual(Test, "4423591923692010144000", "No Modify blA Failed");
		blB.ToStr(Test);
		assertEqual(Test, "48297727666299140", "No Modify blB Failed");
		blC.ToStr(Test);
		assertEqual(Test, "4423640221419676443140", "IAdd Failed Result");
	}
	{
		BigLong blC = blE;
		blC += blF;
		String Test;
		blE.ToStr(Test);
		assertEqual(Test, "48297727666299140", "No Modify blE Failed");
		blF.ToStr(Test);
		assertEqual(Test, "4423591923692010144000", "No Modify blF Failed");
		blC.ToStr(Test);
		assertEqual(Test, "4423640221419676443140", "IAdd Failed Result");
	}
	{
		BigLong blC = blA;
		blC += blF;
		String Test;
		blA.ToStr(Test);
		assertEqual(Test, "4423591923692010144000", "No Modify blA Failed");
		blF.ToStr(Test);
		assertEqual(Test, "4423591923692010144000", "No Modify blF Failed");
		blC.ToStr(Test);
		assertEqual(Test, "8847183847384020288000", "IAdd Failed Result");
	}
	{
		BigLong blC = blE;
		blC += blB;
		String Test;
		blE.ToStr(Test);
		assertEqual(Test, "48297727666299140", "No Modify blE Failed");
		blB.ToStr(Test);
		assertEqual(Test, "48297727666299140", "No Modify blB Failed");
		blC.ToStr(Test);
		assertEqual(Test, "96595455332598280", "IAdd Failed Result");
	}
}
void Test_BigLong::test_ISub() {
	{
		BigLong blC = blA;
		blC -= blB;
		String Test;
		blA.ToStr(Test);
		assertEqual(Test, "4423591923692010144000", "No Modify blA Failed");
		blB.ToStr(Test);
		assertEqual(Test, "48297727666299140", "No Modify blB Failed");
		blC.ToStr(Test);
		assertEqual(Test, "4423543625964343844860", "ISub Failed Result");
	}
	{
		BigLong blC = blE;
		blC -= blF;
		String Test;
		blE.ToStr(Test);
		assertEqual(Test, "48297727666299140", "No Modify blE Failed");
		blF.ToStr(Test);
		assertEqual(Test, "4423591923692010144000", "No Modify blF Failed");
		blC.ToStr(Test);
		assertEqual(Test, "-4423543625964343844860", "ISub Failed Result");
	}
	{
		BigLong blC = blA;
		blC -= blF;
		String Test;
		blA.ToStr(Test);
		assertEqual(Test, "4423591923692010144000", "No Modify blA Failed");
		blF.ToStr(Test);
		assertEqual(Test, "4423591923692010144000", "No Modify blF Failed");
		blC.ToStr(Test);
		assertEqual(Test, "0", "ISub Failed Result");
	}
	{
		BigLong blC = blE;
		blC -= blB;
		String Test;
		blE.ToStr(Test);
		assertEqual(Test, "48297727666299140", "No Modify blE Failed");
		blB.ToStr(Test);
		assertEqual(Test, "48297727666299140", "No Modify blB Failed");
		blC.ToStr(Test);
		assertEqual(Test, "0", "ISub Failed Result");
	}
}
void Test_BigLong::test_IMul() {
	{
		BigLong blC = blA;
		blC *= blB;
		String Test;
		blA.ToStr(Test);
		assertEqual(Test, "4423591923692010144000", "No Modify blA Failed");
		blB.ToStr(Test);
		assertEqual(Test, "48297727666299140", "No Modify blB Failed");
		blC.ToStr(Test);
		assertEqual(Test, "213649438037317032483074671818476160000", "IMul Failed Result");
	}
	{
		BigLong blC = blE;
		blC *= blF;
		String Test;
		blE.ToStr(Test);
		assertEqual(Test, "48297727666299140", "No Modify blE Failed");
		blF.ToStr(Test);
		assertEqual(Test, "4423591923692010144000", "No Modify blF Failed");
		blC.ToStr(Test);
		assertEqual(Test, "213649438037317032483074671818476160000", "IMul Failed Result");
	}
	{
		BigLong blC = blA;
		blC *= blF;
		String Test;
		blA.ToStr(Test);
		assertEqual(Test, "4423591923692010144000", "No Modify blA Failed");
		blF.ToStr(Test);
		assertEqual(Test, "4423591923692010144000", "No Modify blF Failed");
		blC.ToStr(Test);
		assertEqual(Test, "19568165507353178896743811863398900736000000", "IMul Failed Result");
	}
	{
		BigLong blC = blE;
		blC *= blB;
		String Test;
		blE.ToStr(Test);
		assertEqual(Test, "48297727666299140", "No Modify blE Failed");
		blB.ToStr(Test);
		assertEqual(Test, "48297727666299140", "No Modify blB Failed");
		blC.ToStr(Test);
		assertEqual(Test, "2332670497727997372064103964739600", "IMul Failed Result");
	}
}
void Test_BigLong::test_IDiv() {
	{
		BigLong blC = blA;
		blC /= blB;
		String Test;
		blA.ToStr(Test);
		assertEqual(Test, "4423591923692010144000", "No Modify blA Failed");
		blB.ToStr(Test);
		assertEqual(Test, "48297727666299140", "No Modify blB Failed");
		blC.ToStr(Test);
		assertEqual(Test, "91590", "IDiv Failed Result");
	}
	{
		BigLong blC = blE;
		blC /= blF;
		String Test;
		blE.ToStr(Test);
		assertEqual(Test, "48297727666299140", "No Modify blE Failed");
		blF.ToStr(Test);
		assertEqual(Test, "4423591923692010144000", "No Modify blF Failed");
		blC.ToStr(Test);
		assertEqual(Test, "0", "IDiv Failed Result");
	}
	{
		BigLong blC = blA;
		blC /= blF;
		String Test;
		blA.ToStr(Test);
		assertEqual(Test, "4423591923692010144000", "No Modify blA Failed");
		blF.ToStr(Test);
		assertEqual(Test, "4423591923692010144000", "No Modify blF Failed");
		blC.ToStr(Test);
		assertEqual(Test, "1", "IDiv Failed Result");
	}
	{
		BigLong blC = blE;
		blC /= blB;
		String Test;
		blE.ToStr(Test);
		assertEqual(Test, "48297727666299140", "No Modify blE Failed");
		blB.ToStr(Test);
		assertEqual(Test, "48297727666299140", "No Modify blB Failed");
		blC.ToStr(Test);
		assertEqual(Test, "1", "IDiv Failed Result");
	}
}
void Test_BigLong::test_IMod() {
	{
		BigLong blC = blA;
		blC %= blB;
		String Test;
		blA.ToStr(Test);
		assertEqual(Test, "4423591923692010144000", "No Modify blA Failed");
		blB.ToStr(Test);
		assertEqual(Test, "48297727666299140", "No Modify blB Failed");
		blC.ToStr(Test);
		assertEqual(Test, "3046735671911400", "IMod Failed Result");
	}
	{
		BigLong blC = blE;
		blC %= blF;
		String Test;
		blE.ToStr(Test);
		assertEqual(Test, "48297727666299140", "No Modify blE Failed");
		blF.ToStr(Test);
		assertEqual(Test, "4423591923692010144000", "No Modify blF Failed");
		blC.ToStr(Test);
		assertEqual(Test, "48297727666299140", "IMod Failed Result");
	}
	{
		BigLong blC = blA;
		blC %= blF;
		String Test;
		blA.ToStr(Test);
		assertEqual(Test, "4423591923692010144000", "No Modify blA Failed");
		blF.ToStr(Test);
		assertEqual(Test, "4423591923692010144000", "No Modify blF Failed");
		blC.ToStr(Test);
		assertEqual(Test, "0", "IMod Failed Result");
	}
	{
		BigLong blC = blE;
		blC %= blB;
		String Test;
		blE.ToStr(Test);
		assertEqual(Test, "48297727666299140", "No Modify blE Failed");
		blB.ToStr(Test);
		assertEqual(Test, "48297727666299140", "No Modify blB Failed");
		blC.ToStr(Test);
		assertEqual(Test, "0", "IMod Failed Result");
	}
}
void Test_BigLong::test_IAnd() {
	{
		BigLong blC = blA;
		blC &= blB;
		String Test;
		blA.ToStr(Test);
		assertEqual(Test, "4423591923692010144000", "No Modify blA Failed");
		blB.ToStr(Test);
		assertEqual(Test, "48297727666299140", "No Modify blB Failed");
		blC.ToStr(Test);
		assertEqual(Test, "48272988654665984", "IAnd Failed Result");
	}
	{
		BigLong blC = blE;
		blC &= blF;
		String Test;
		blE.ToStr(Test);
		assertEqual(Test, "48297727666299140", "No Modify blE Failed");
		blF.ToStr(Test);
		assertEqual(Test, "4423591923692010144000", "No Modify blF Failed");
		blC.ToStr(Test);
		assertEqual(Test, "48272988654665984", "IAnd Failed Result");
	}
	{
		BigLong blC = blA;
		blC &= blF;
		String Test;
		blA.ToStr(Test);
		assertEqual(Test, "4423591923692010144000", "No Modify blA Failed");
		blF.ToStr(Test);
		assertEqual(Test, "4423591923692010144000", "No Modify blF Failed");
		blC.ToStr(Test);
		assertEqual(Test, "4423591923692010144000", "IAnd Failed Result");
	}
	{
		BigLong blC = blE;
		blC &= blB;
		String Test;
		blE.ToStr(Test);
		assertEqual(Test, "48297727666299140", "No Modify blE Failed");
		blB.ToStr(Test);
		assertEqual(Test, "48297727666299140", "No Modify blB Failed");
		blC.ToStr(Test);
		assertEqual(Test, "48297727666299140", "IAnd Failed Result");
	}
}
void Test_BigLong::test_IOr() {
	{
		BigLong blC = blA;
		blC |= blB;
		String Test;
		blA.ToStr(Test);
		assertEqual(Test, "4423591923692010144000", "No Modify blA Failed");
		blB.ToStr(Test);
		assertEqual(Test, "48297727666299140", "No Modify blB Failed");
		blC.ToStr(Test);
		assertEqual(Test, "4423591948431021777156", "IOr Failed Result");
	}
	{
		BigLong blC = blE;
		blC |= blF;
		String Test;
		blE.ToStr(Test);
		assertEqual(Test, "48297727666299140", "No Modify blE Failed");
		blF.ToStr(Test);
		assertEqual(Test, "4423591923692010144000", "No Modify blF Failed");
		blC.ToStr(Test);
		assertEqual(Test, "4423591948431021777156", "IOr Failed Result");
	}
	{
		BigLong blC = blA;
		blC |= blF;
		String Test;
		blA.ToStr(Test);
		assertEqual(Test, "4423591923692010144000", "No Modify blA Failed");
		blF.ToStr(Test);
		assertEqual(Test, "4423591923692010144000", "No Modify blF Failed");
		blC.ToStr(Test);
		assertEqual(Test, "4423591923692010144000", "IOr Failed Result");
	}
	{
		BigLong blC = blE;
		blC |= blB;
		String Test;
		blE.ToStr(Test);
		assertEqual(Test, "48297727666299140", "No Modify blE Failed");
		blB.ToStr(Test);
		assertEqual(Test, "48297727666299140", "No Modify blB Failed");
		blC.ToStr(Test);
		assertEqual(Test, "48297727666299140", "IOr Failed Result");
	}
}
void Test_BigLong::test_IXor() {
	{
		BigLong blC = blA;
		blC ^= blB;
		String Test;
		blA.ToStr(Test);
		assertEqual(Test, "4423591923692010144000", "No Modify blA Failed");
		blB.ToStr(Test);
		assertEqual(Test, "48297727666299140", "No Modify blB Failed");
		blC.ToStr(Test);
		assertEqual(Test, "4423543675442367111172", "IXor Failed Result");
	}
	{
		BigLong blC = blE;
		blC ^= blF;
		String Test;
		blE.ToStr(Test);
		assertEqual(Test, "48297727666299140", "No Modify blE Failed");
		blF.ToStr(Test);
		assertEqual(Test, "4423591923692010144000", "No Modify blF Failed");
		blC.ToStr(Test);
		assertEqual(Test, "4423543675442367111172", "IXor Failed Result");
	}
	{
		BigLong blC = blA;
		blC ^= blF;
		String Test;
		blA.ToStr(Test);
		assertEqual(Test, "4423591923692010144000", "No Modify blA Failed");
		blF.ToStr(Test);
		assertEqual(Test, "4423591923692010144000", "No Modify blF Failed");
		blC.ToStr(Test);
		assertEqual(Test, "0", "IXor Failed Result");
	}
	{
		BigLong blC = blE;
		blC ^= blB;
		String Test;
		blE.ToStr(Test);
		assertEqual(Test, "48297727666299140", "No Modify blE Failed");
		blB.ToStr(Test);
		assertEqual(Test, "48297727666299140", "No Modify blB Failed");
		blC.ToStr(Test);
		assertEqual(Test, "0", "IXor Failed Result");
	}
}
void Test_BigLong::test_IRShift() {
	// operator<<
	{
		BigLong blC = blA;
		blC <<= 1;
		String Test;
		blA.ToStr(Test);
		assertEqual(Test, "4423591923692010144000", "No Modify blA Failed");
		blC.ToStr(Test);
		assertEqual(Test, "8847183847384020288000", "IRShift Failed Result");
	}
	{
		BigLong blC = blB;
		blC <<= 23;
		String Test;
		blB.ToStr(Test);
		assertEqual(Test, "48297727666299140", "No Modify blB Failed");
		blC.ToStr(Test);
		assertEqual(Test, "405150704683338296197120", "IRShift Failed Result");
	}
	{
		BigLong blC = blF;
		blC <<= 12;
		String Test;
		blF.ToStr(Test);
		assertEqual(Test, "4423591923692010144000", "No Modify blF Failed");
		blC.ToStr(Test);
		assertEqual(Test, "18119032519442473549824000", "IRShift Failed Result");
	}
	{
		BigLong blC = blE;
		blC <<= 32;
		String Test;
		blE.ToStr(Test);
		assertEqual(Test, "48297727666299140", "No Modify blE Failed");
		blC.ToStr(Test);
		assertEqual(Test, "207437160797869207652925440", "IRShift Failed Result");
	}
	{
		BigLong blC = blA;
		blC <<= 31;
		String Test;
		blA.ToStr(Test);
		assertEqual(Test, "4423591923692010144000", "No Modify blA Failed");
		blC.ToStr(Test);
		assertEqual(Test, "9499591321553455572490125312000", "IRShift Failed Result");
	}
	{
		BigLong blC = blB;
		blC <<= 15;
		String Test;
		blB.ToStr(Test);
		assertEqual(Test, "48297727666299140", "No Modify blB Failed");
		blC.ToStr(Test);
		assertEqual(Test, "1582619940169290219520", "IRShift Failed Result");
	}
	{
		BigLong blC = blF;
		blC <<= 33;
		String Test;
		blF.ToStr(Test);
		assertEqual(Test, "4423591923692010144000", "No Modify blF Failed");
		blC.ToStr(Test);
		assertEqual(Test, "37998365286213822289960501248000", "IRShift Failed Result");
	}
	{
		BigLong blC = blE;
		blC <<= 17;
		String Test;
		blE.ToStr(Test);
		assertEqual(Test, "48297727666299140", "No Modify blE Failed");
		blC.ToStr(Test);
		assertEqual(Test, "6330479760677160878080", "IRShift Failed Result");
	}
	{
		BigLong blC = blA;
		blC <<= 0;
		String Test;
		blA.ToStr(Test);
		assertEqual(Test, "4423591923692010144000", "No Modify blA Failed");
		blC.ToStr(Test);
		assertEqual(Test, "4423591923692010144000", "IRShift Failed Result");
	}
	{
		BigLong blC = blB;
		blC <<= 0;
		String Test;
		blB.ToStr(Test);
		assertEqual(Test, "48297727666299140", "No Modify blB Failed");
		blC.ToStr(Test);
		assertEqual(Test, "48297727666299140", "IRShift Failed Result");
	}
	{
		BigLong blC = blF;
		blC <<= 0;
		String Test;
		blF.ToStr(Test);
		assertEqual(Test, "4423591923692010144000", "No Modify blF Failed");
		blC.ToStr(Test);
		assertEqual(Test, "4423591923692010144000", "IRShift Failed Result");
	}
	{
		BigLong blC = blE;
		blC <<= 0;
		String Test;
		blE.ToStr(Test);
		assertEqual(Test, "48297727666299140", "No Modify blE Failed");
		blC.ToStr(Test);
		assertEqual(Test, "48297727666299140", "IRShift Failed Result");
	}
}
void Test_BigLong::test_ILShift() {
	// operator>>
	{
		BigLong blC = blA;
		blC >>= 1;
		String Test;
		blA.ToStr(Test);
		assertEqual(Test, "4423591923692010144000", "No Modify blA Failed");
		blC.ToStr(Test);
		assertEqual(Test, "2211795961846005072000", "ILShift Failed Result");
	}
	{
		BigLong blC = blB;
		blC >>= 23;
		String Test;
		blB.ToStr(Test);
		assertEqual(Test, "48297727666299140", "No Modify blB Failed");
		blC.ToStr(Test);
		assertEqual(Test, "5757537802", "ILShift Failed Result");
	}
	{
		BigLong blC = blF;
		blC >>= 12;
		String Test;
		blF.ToStr(Test);
		assertEqual(Test, "4423591923692010144000", "No Modify blF Failed");
		blC.ToStr(Test);
		assertEqual(Test, "1079978496995119664", "ILShift Failed Result");
	}
	{
		BigLong blC = blE;
		blC >>= 32;
		String Test;
		blE.ToStr(Test);
		assertEqual(Test, "48297727666299140", "No Modify blE Failed");
		blC.ToStr(Test);
		assertEqual(Test, "11245191", "ILShift Failed Result");
	}
	{
		BigLong blC = blA;
		blC >>= 31;
		String Test;
		blA.ToStr(Test);
		assertEqual(Test, "4423591923692010144000", "No Modify blA Failed");
		blC.ToStr(Test);
		assertEqual(Test, "2059895509710", "ILShift Failed Result");
	}
	{
		BigLong blC = blB;
		blC >>= 15;
		String Test;
		blB.ToStr(Test);
		assertEqual(Test, "48297727666299140", "No Modify blB Failed");
		blC.ToStr(Test);
		assertEqual(Test, "1473929677316", "ILShift Failed Result");
	}
	{
		BigLong blC = blF;
		blC >>= 33;
		String Test;
		blF.ToStr(Test);
		assertEqual(Test, "4423591923692010144000", "No Modify blF Failed");
		blC.ToStr(Test);
		assertEqual(Test, "514973877427", "ILShift Failed Result");
	}
	{
		BigLong blC = blE;
		blC >>= 17;
		String Test;
		blE.ToStr(Test);
		assertEqual(Test, "48297727666299140", "No Modify blE Failed");
		blC.ToStr(Test);
		assertEqual(Test, "368482419329", "ILShift Failed Result");
	}
	{
		BigLong blC = blA;
		blC >>= 0;
		String Test;
		blA.ToStr(Test);
		assertEqual(Test, "4423591923692010144000", "No Modify blA Failed");
		blC.ToStr(Test);
		assertEqual(Test, "4423591923692010144000", "ILShift Failed Result");
	}
	{
		BigLong blC = blB;
		blC >>= 0;
		String Test;
		blB.ToStr(Test);
		assertEqual(Test, "48297727666299140", "No Modify blB Failed");
		blC.ToStr(Test);
		assertEqual(Test, "48297727666299140", "ILShift Failed Result");
	}
	{
		BigLong blC = blF;
		blC >>= 0;
		String Test;
		blF.ToStr(Test);
		assertEqual(Test, "4423591923692010144000", "No Modify blF Failed");
		blC.ToStr(Test);
		assertEqual(Test, "4423591923692010144000", "ILShift Failed Result");
	}
	{
		BigLong blC = blE;
		blC >>= 0;
		String Test;
		blE.ToStr(Test);
		assertEqual(Test, "48297727666299140", "No Modify blE Failed");
		blC.ToStr(Test);
		assertEqual(Test, "48297727666299140", "ILShift Failed Result");
	}
}
void Test_BigLong::test_IsEq() {
	assertTrue(blA == blF, "Comparison result test failed");
	assertTrue(blB == blE, "Comparison result test failed");
	assertFalse(blA == blE, "Comparison result test failed");
	assertFalse(blB == blF, "Comparison result test failed");
	assertTrue(blF == blA, "Comparison result test failed");
	assertTrue(blE == blB, "Comparison result test failed");
	assertFalse(blE == blA, "Comparison result test failed");
	assertFalse(blF == blB, "Comparison result test failed");
	assertFalse(blA == blB, "Comparison result test failed");
	assertFalse(blB == blA, "Comparison result test failed");
	assertFalse(blF == blE, "Comparison result test failed");
	assertFalse(blE == blF, "Comparison result test failed");
}
void Test_BigLong::test_IsGe() {
	assertTrue(blA >= blB, "Comparison result test failed");
	assertFalse(blB >= blA, "Comparison result test failed");
	assertFalse(blE >= blF, "Comparison result test failed");
	assertTrue(blF >= blE, "Comparison result test failed");
	assertTrue(blA >= blF, "Comparison result test failed");
	assertTrue(blF >= blA, "Comparison result test failed");
	assertTrue(blB >= blE, "Comparison result test failed");
	assertTrue(blE >= blB, "Comparison result test failed");
}
void Test_BigLong::test_IsGt() {
	assertTrue(blA > blB, "Comparison result test failed");
	assertFalse(blB > blA, "Comparison result test failed");
	assertFalse(blE > blF, "Comparison result test failed");
	assertTrue(blF > blE, "Comparison result test failed");
	assertFalse(blA > blF, "Comparison result test failed");
	assertFalse(blF > blA, "Comparison result test failed");
	assertFalse(blB > blE, "Comparison result test failed");
	assertFalse(blE > blB, "Comparison result test failed");
}
void Test_BigLong::test_IsLe() {
	assertFalse(blA <= blB, "Comparison result test failed");
	assertTrue(blB <= blA, "Comparison result test failed");
	assertTrue(blE <= blF, "Comparison result test failed");
	assertFalse(blF <= blE, "Comparison result test failed");
	assertTrue(blA <= blF, "Comparison result test failed");
	assertTrue(blF <= blA, "Comparison result test failed");
	assertTrue(blB <= blE, "Comparison result test failed");
	assertTrue(blE <= blB, "Comparison result test failed");
}
void Test_BigLong::test_IsLt() {
	assertFalse(blA < blB, "Comparison result test failed");
	assertTrue(blB < blA, "Comparison result test failed");
	assertTrue(blE < blF, "Comparison result test failed");
	assertFalse(blF < blE, "Comparison result test failed");
	assertFalse(blA < blF, "Comparison result test failed");
	assertFalse(blF < blA, "Comparison result test failed");
	assertFalse(blB < blE, "Comparison result test failed");
	assertFalse(blE < blB, "Comparison result test failed");
}
void Test_BigLong::test_IsNe() {
	assertFalse(blA != blF, "Comparison result test failed");
	assertFalse(blB != blE, "Comparison result test failed");
	assertTrue(blA != blE, "Comparison result test failed");
	assertTrue(blB != blF, "Comparison result test failed");
	assertFalse(blF != blA, "Comparison result test failed");
	assertFalse(blE != blB, "Comparison result test failed");
	assertTrue(blE != blA, "Comparison result test failed");
	assertTrue(blF != blB, "Comparison result test failed");
	assertTrue(blA != blB, "Comparison result test failed");
	assertTrue(blB != blA, "Comparison result test failed");
	assertTrue(blF != blE, "Comparison result test failed");
	assertTrue(blE != blF, "Comparison result test failed");
}
