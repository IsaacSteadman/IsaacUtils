#include "Test1IsaacUtils.h"
#include <iostream>
using Utils::BigLong;
using Utils::Array;
using Utils::ByteArray;
using Utils::String;
using Utils::Byte;
Test_BaseBigLong::Test_BaseBigLong() {
	Name = "Test_BaseBigLong";
	Tests.push_back((TestCase::TestFunc)(&Test_BaseBigLong::test_StrNomMul));
	Tests.push_back((TestCase::TestFunc)(&Test_BaseBigLong::test_StrKarMul));
	Tests.push_back((TestCase::TestFunc)(&Test_BaseBigLong::test_StrMul));
	Tests.push_back((TestCase::TestFunc)(&Test_BaseBigLong::test_BytesNomMul));
	Tests.push_back((TestCase::TestFunc)(&Test_BaseBigLong::test_BytesKarMul));
	Tests.push_back((TestCase::TestFunc)(&Test_BaseBigLong::test_BytesMul));
	Tests.push_back((TestCase::TestFunc)(&Test_BaseBigLong::test_FromStr));
	Tests.push_back((TestCase::TestFunc)(&Test_BaseBigLong::test_ToStr));
}
void Test_BaseBigLong::test_BytesNomMul() {
	BigLong blA(reinterpret_cast<const Byte *>("\x02\x34\x19\xaf\xb7\x85\x6e"), 7); // 31109271401346050
	BigLong blB(reinterpret_cast<const Byte *>("\xb7\xe2\x16\xcd\x43\x09\x5a\x8f\x2c\x3d"), 10); // 288886341773173714248375
	BigLong blC = BigLong::NomMul(blA, blB); // 8987043610363673776242525362039025168750
	assertEqual(blC.Zero(), 2, "Failed Comparison with Zero Test");
	Array<UInt32> &Tmp = blC.GetLongs();
	const UInt32 a[] = {2098852206, 2469308472, 3071609969, 1763288027, 26};
	Array<UInt32> Cmp(a, sizeof(a)/sizeof(UInt32));
	assertEqual(Tmp.Length(), Cmp.Length(), "Failed Length Comparison Test");
	assertEqual(Tmp, Cmp, "Failed Bytes Comparison Test");
}
void Test_BaseBigLong::test_BytesKarMul() {
	BigLong blA(reinterpret_cast<const Byte *>("\x02\x34\x19\xaf\xb7\x85\x6e"), 7); // 31109271401346050
	BigLong blB(reinterpret_cast<const Byte *>("\xb7\xe2\x16\xcd\x43\x09\x5a\x8f\x2c\x3d"), 10); // 288886341773173714248375
	BigLong blC = BigLong::KarMul(blA, blB); // 8987043610363673776242525362039025168750
	assertEqual(blC.Zero(), 2, "Failed Comparison with Zero Test");
	Array<UInt32> &Tmp = blC.GetLongs();
	const UInt32 a[] = {2098852206, 2469308472, 3071609969, 1763288027, 26};
	Array<UInt32> Cmp(a, sizeof(a)/sizeof(UInt32));
	assertEqual(Tmp.Length(), Cmp.Length(), "Failed Length Comparison Test");
	assertEqual(Tmp, Cmp, "Failed Bytes Comparison Test");
}
void Test_BaseBigLong::test_BytesMul() {
	BigLong blA(reinterpret_cast<const Byte *>("\x02\x34\x19\xaf\xb7\x85\x6e"), 7); // 31109271401346050
	BigLong blB(reinterpret_cast<const Byte *>("\xb7\xe2\x16\xcd\x43\x09\x5a\x8f\x2c\x3d"), 10); // 288886341773173714248375
	assertEqual(blA.GetLongs()[0], 2937664514, "Failed Init Assert");
	assertEqual(blA.GetLongs()[1], 7243191, "Failed Init Assert");
	assertEqual(blB.GetLongs()[0], 3440829111, "Failed Init Assert");
	assertEqual(blB.GetLongs()[1], 2405042499, "Failed Init Assert");
	assertEqual(blB.GetLongs()[2], 15660, "Failed Init Assert");
	BigLong blC = blA * blB; // 8987043610363673776242525362039025168750
	assertEqual(blA.GetLongs()[0], 2937664514, "Failed Post Assert");
	assertEqual(blA.GetLongs()[1], 7243191, "Failed Post Assert");
	assertEqual(blB.GetLongs()[0], 3440829111, "Failed Post Assert");
	assertEqual(blB.GetLongs()[1], 2405042499, "Failed Post Assert");
	assertEqual(blB.GetLongs()[2], 15660, "Failed Post Assert");
	assertEqual(blC.Zero(), 2, "Failed Comparison with Zero Test");
	Array<UInt32> &Tmp = blC.GetLongs();
	const UInt32 a[] = {2098852206, 2469308472, 3071609969, 1763288027, 26};
	Array<UInt32> Cmp(a, sizeof(a)/sizeof(UInt32));
	assertEqual(Tmp.Length(), Cmp.Length(), "Failed Length Comparison Test");
	assertEqual(Tmp, Cmp, "Failed Bytes Comparison Test");
}
void Test_BaseBigLong::test_StrNomMul() {
	BigLong blA, blB;
	blA.FromStr("31109271401346050", 10);
	blB.FromStr("288886341773173714248375", 10);
	BigLong blC = BigLong::NomMul(blA, blB); // 8987043610363673776242525362039025168750
	assertEqual(blC.Zero(), 2, "Failed Comparison with Zero Test");
	String Str;
	blC.ToStr(Str, 10);
	assertEqual(Str, "8987043610363673776242525362039025168750", "Failed base 10 comparison test");
}
void Test_BaseBigLong::test_StrKarMul() {
	BigLong blA, blB;
	blA.FromStr("31109271401346050", 10);
	blB.FromStr("288886341773173714248375", 10);
	BigLong blC = BigLong::KarMul(blA, blB); // 8987043610363673776242525362039025168750
	assertEqual(blC.Zero(), 2, "Failed Comparison with Zero Test");
	String Str;
	blC.ToStr(Str, 10);
	assertEqual(Str, "8987043610363673776242525362039025168750", "Failed base 10 comparison test");
}
void Test_BaseBigLong::test_StrMul() {
	BigLong blA, blB;
	blA.FromStr("31109271401346050", 10);
	blB.FromStr("288886341773173714248375", 10);
	assertEqual(blA.GetLongs()[0], 2937664514, "Failed Init Assert");
	assertEqual(blA.GetLongs()[1], 7243191, "Failed Init Assert");
	assertEqual(blB.GetLongs()[0], 3440829111, "Failed Init Assert");
	assertEqual(blB.GetLongs()[1], 2405042499, "Failed Init Assert");
	assertEqual(blB.GetLongs()[2], 15660, "Failed Init Assert");
	BigLong blC = blA * blB; // 8987043610363673776242525362039025168750
	assertEqual(blA.GetLongs()[0], 2937664514, "Failed Post Assert");
	assertEqual(blA.GetLongs()[1], 7243191, "Failed Post Assert");
	assertEqual(blB.GetLongs()[0], 3440829111, "Failed Post Assert");
	assertEqual(blB.GetLongs()[1], 2405042499, "Failed Post Assert");
	assertEqual(blB.GetLongs()[2], 15660, "Failed Post Assert");
	assertEqual(blC.Zero(), 2, "Failed Comparison with Zero Test");
	String Str;
	blC.ToStr(Str, 10);
	assertEqual(Str, "8987043610363673776242525362039025168750", "Failed base 10 comparison test");
}
void Test_BaseBigLong::test_FromStr() {
	{
		BigLong Test;
		bool Rtn = Test.FromStr("26", 10);
		assertTrue(Rtn, "BigLong 26 bad Result");
		assertEqual(Test.Zero(), 2, "BigLong 26 bad Sign");
		assertEqual(Test.GetLongs().Length(), 1, "BigLong 26 bad length");
		assertEqual(Test.GetLongs()[0], 26, "BigLong 26 bad long");
	}
	{
		BigLong Test;
		bool Rtn = Test.FromStr("26", 16);
		assertTrue(Rtn, "BigLong 0x26 bad Result");
		assertEqual(Test.Zero(), 2, "BigLong 0x26 bad Sign");
		assertEqual(Test.GetLongs().Length(), 1, "BigLong 0x26 bad length");
		assertEqual(Test.GetLongs()[0], 0x26, "BigLong 0x26 bad long");
	}
	{
		BigLong Test;
		bool Rtn = Test.FromStr("12326", 10);
		assertTrue(Rtn, "BigLong 12326 bad Result");
		assertEqual(Test.Zero(), 2, "BigLong 12326 bad Sign");
		assertEqual(Test.GetLongs().Length(), 1, "BigLong 12326 bad length");
		assertEqual(Test.GetLongs()[0], 12326, "BigLong 12326 bad long");
	}
	{
		BigLong Test;
		bool Rtn = Test.FromStr("12326888888", 16);
		assertTrue(Rtn, "Biglong 0x12326888888 bad Result");
		assertEqual(Test.Zero(), 2, "BigLong 0x12326888888 bad Sign");
		assertEqual(Test.GetLongs().Length(), 2, "BigLong 0x12326888888 bad length");
		assertEqual(Test.GetLongs()[0], 0x26888888, "BigLong 0x26888888 bad long0");
		assertEqual(Test.GetLongs()[1], 0x123, "BigLong 0x123 bad long1");
	}
	{
		BigLong Test;
		bool Rtn = Test.FromStr("12356231436124143524342", 10);
		assertTrue(Rtn, "Biglong 3LONG_NUM bad Result");
		assertEqual(Test.Zero(), 2, "BigLong 3LONG_NUM bad Sign");
		assertEqual(Test.GetLongs().Length(), 3, "BigLong 3LONG_NUM bad length");
		assertEqual(Test.GetLongs()[0], 0xf65499f6, "BigLong 0xf65499f6 bad long0");
		assertEqual(Test.GetLongs()[1], 0xd5287108, "BigLong 0xd5287108 bad long1");
		assertEqual(Test.GetLongs()[2], 0x29d, "BigLong 0x29d bad long2");
	}
	{
		BigLong Test;
		bool Rtn = Test.FromStr("-12356231436124143524342", 10);
		assertTrue(Rtn, "Biglong 3LONG_NUM_SIGN bad Result");
		assertEqual(Test.Zero(), 0, "BigLong 3LONG_NUM_SIGN bad Sign");
		assertEqual(Test.GetLongs().Length(), 3, "BigLong 3LONG_NUM_SIGN bad length");
		assertEqual(Test.GetLongs()[0], 0xf65499f6, "BigLong 0xf65499f6 bad long0");
		assertEqual(Test.GetLongs()[1], 0xd5287108, "BigLong 0xd5287108 bad long1");
		assertEqual(Test.GetLongs()[2], 0x29d, "BigLong 0x29d bad long2");
	}
}
void Test_BaseBigLong::test_ToStr() {
	{
		BigLong Test;
		String Test1;
		assertEqual(Test.Zero(), 1, "BAD ZERO");
		Test.ToStr(Test1, 10);
		assertEqual(Test1, "0", "BigLong ZERO bad Result");
	}
	{
		BigLong Test;
		Test.Minus();
		String Test1;
		assertEqual(Test.Zero(), 1, "BAD ZERO");
		Test.ToStr(Test1, 10);
		assertEqual(Test1, "0", "BigLong ZERO_SIGN bad Result");
	}
	{
		BigLong Test;
		String Test1;
		Test.GetLongs().SetLength(1);
		Test.GetLongs()[0] = 26;
		Test.ToStr(Test1, 10);
		assertEqual(Test1, "26", "BigLong 26 bad Result");
	}
	{
		BigLong Test;
		String Test1;
		Test.GetLongs().SetLength(1);
		Test.GetLongs()[0] = 0x26;
		Test.ToStr(Test1, 16);
		assertEqual(Test1, "26", "BigLong 0x26 bad Result");
	}
	{
		BigLong Test;
		String Test1;
		Test.GetLongs().SetLength(1);
		Test.GetLongs()[0] = 12326;
		Test.ToStr(Test1, 10);
		assertEqual(Test1, "12326", "BigLong 12326 bad Result");
	}
	{
		BigLong Test;
		String Test1;
		Test.GetLongs().SetLength(2);
		Test.GetLongs()[0] = 0x26888888;
		Test.GetLongs()[1] = 0x123;
		Test.ToStr(Test1, 16);
		assertEqual(Test1, "12326888888", "BigLong 0x12326888888 bad Result");
	}
	{
		BigLong Test;
		String Test1;
		Test.GetLongs().SetLength(3);
		Test.GetLongs()[0] = 0xf65499f6;
		Test.GetLongs()[1] = 0xd5287108;
		Test.GetLongs()[2] = 0x29d;
		Test.ToStr(Test1, 10);
		assertEqual(Test1, "12356231436124143524342", "BigLong 3LONG_NUM bad Result");
	}
	{
		BigLong Test;
		String Test1;
		Test.GetLongs().SetLength(3);
		Test.GetLongs()[0] = 0xf65499f6;
		Test.GetLongs()[1] = 0xd5287108;
		Test.GetLongs()[2] = 0x29d;
		Test.Minus();
		assertEqual(Test.Zero(), 0, "BAD ZERO");
		Test.ToStr(Test1, 10);
		assertEqual(Test1, "-12356231436124143524342", "BigLong 3LONG_NUM_SIGN bad Result");
	}
}
