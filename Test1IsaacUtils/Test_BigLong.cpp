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
