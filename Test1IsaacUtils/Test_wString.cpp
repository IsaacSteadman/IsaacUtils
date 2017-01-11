#include "Test1IsaacUtils.h"
using Utils::Array;
using Utils::wString;
Test_wString::Test_wString() {
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
void Test_wString::test_Split() {
	wString a("hello there spaces");
	{
		Array<wString> Cmp("", 3);
		Cmp[0] = "hello";
		Cmp[1] = "there";
		Cmp[2] = "spaces";
		assertEqual(a.Split(" "), Cmp, "1st Split Failed");
	}
	{
		Array<wString> Cmp("", 2);
		Cmp[0] = "hello";
		Cmp[1] = "there spaces";
		assertEqual(a.Split(" ", 1), Cmp, "2nd Split Failed");
	}
	{
		Array<wString> Cmp("", 3);
		Cmp[0] = "hello";
		Cmp[1] = "there";
		Cmp[2] = "spaces";
		assertEqual(a.Split(" ", 2), Cmp, "3rd Split Failed");
	}
}

void Test_wString::test_RSplit() {
	wString a("hello there spaces");
	{
		Array<wString> Cmp("", 3);
		Cmp[0] = "hello";
		Cmp[1] = "there";
		Cmp[2] = "spaces";
		assertEqual(a.RSplit(" "), Cmp, "1st RSplit Failed");
	}
	{
		Array<wString> Cmp("", 2);
		Cmp[0] = "hello there";
		Cmp[1] = "spaces";
		assertEqual(a.RSplit(" ", 1), Cmp, "2nd RSplit Failed");
	}
	{
		Array<wString> Cmp("", 3);
		Cmp[0] = "hello";
		Cmp[1] = "there";
		Cmp[2] = "spaces";
		assertEqual(a.RSplit(" ", 2), Cmp, "3rd RSplit Failed");
	}
}

void Test_wString::test_Join() {
	Array<wString> a("", 5);
	a[0] = "hello";
	a[1] = "1234";
	a[2] = "there";
	a[3] = "5678";
	a[4] = "spaces";
	assertEqual(
		wString(",").Join(a),
		wString("hello,1234,there,5678,spaces"),
		"1st Join Failed");
	assertEqual(
		wString(", ").Join(a),
		wString("hello, 1234, there, 5678, spaces"),
		"2nd Join Failed");
	assertEqual(
		wString("| |").Join(a),
		wString("hello| |1234| |there| |5678| |spaces"),
		"3rd Join Failed");
}

void Test_wString::test_Replace_Ch_Ch() {
	{
		wString a = "hello there is good";
		wString b = "hell0 there is g00d";
		SizeL Rtn = a.Replace('o', '0');
		assertEqual(a, b, "1st ReplaceChCh !!Failed!!");
		assertTrue(Rtn == 3, "1st ReplaceChCh Failed Return");
	}
	{
		wString a = "hello there is good";
		wString b = "h3llo th3r3 is good";
		SizeL Rtn = a.Replace('e', '3');
		assertEqual(a, b, "2nd ReplaceChCh !!Failed!!");
		assertTrue(Rtn == 3, "2nd ReplaceChCh Failed Return");
	}
	{
		wString a = "hello there is good";
		wString b = "he11o there is good";
		SizeL Rtn = a.Replace('l', '1');
		assertEqual(a, b, "3rd ReplaceChCh !!Failed!!");
		assertTrue(Rtn == 2, "3rd ReplaceChCh Failed Return");
	}
}

void Test_wString::test_Replace_Ch_Str() {
	{
		wString a = "hello there is good";
		wString b = "hell0 there is g00d";
		SizeL Rtn = a.Replace('o', "0");
		assertEqual(a, b, "1st ReplaceChStr !!Failed!!");
		assertTrue(Rtn == 3, "1st ReplaceChStr Failed Return");
	}
	{
		wString a = "hello there is good";
		wString b = "h3@llo th3@r3@ is good";
		SizeL Rtn = a.Replace('e', "3@");
		assertEqual(a, b, "2nd ReplaceChStr !!Failed!!");
		assertTrue(Rtn == 3, "2nd ReplaceChStr Failed Return");
	}
	{
		wString a = "hello there is good";
		wString b = "he$12$12o there is good";
		SizeL Rtn = a.Replace('l', "$12");
		assertEqual(a, b, "3rd ReplaceChStr !!Failed!!");
		assertTrue(Rtn == 2, "3rd ReplaceChStr Failed Return");
	}
}

void Test_wString::test_Replace_Str_Ch() {
	{
		wString a = "hell0 there is g00d";
		wString b = "hello there is good";
		SizeL Rtn = a.Replace("0", 'o');
		assertEqual(a, b, "1st ReplaceStrCh !!Failed!!");
		assertTrue(Rtn == 3, "1st ReplaceStrCh Failed Return");
	}
	{
		wString a = "h3@llo th3@r3@ is good";
		wString b = "hello there is good";
		SizeL Rtn = a.Replace("3@", 'e');
		assertEqual(a, b, "2nd ReplaceStrCh !!Failed!!");
		assertTrue(Rtn == 3, "2nd ReplaceStrCh Failed Return");
	}
	{
		wString a = "he$12$12o there is good";
		wString b = "hello there is good";
		SizeL Rtn = a.Replace("$12", 'l');
		assertEqual(a, b, "3rd ReplaceStrCh !!Failed!!");
		assertTrue(Rtn == 2, "3rd ReplaceStrCh Failed Return");
	}
}

void Test_wString::test_Find_Ch() {
	wString a = "__Hello there Hellish";
	{
		SizeL Pos = 0;
		bool Rtn = a.Find(Pos, 'H');
		assertTrue(Pos == 2, "1st Find_Ch !!Failed!!");
		assertTrue(Rtn, "1st Find_Ch Failed Return");
	}
	{
		SizeL Pos = 12;
		bool Rtn = a.Find(Pos, 'l');
		assertTrue(Pos == 4, "2nd Find_Ch !!Failed!!");
		assertTrue(Rtn, "2nd Find_Ch Failed Return");
	}
	{
		SizeL Pos = 4;
		bool Rtn = a.Find(Pos, 'l', true);
		assertTrue(Pos == 4, "3rd Find_Ch !!Failed!!");
		assertTrue(Rtn, "3rd Find_Ch Failed Return");
	}
	{
		SizeL Pos = 4;
		bool Rtn = a.Find(Pos, 'e', true);
		assertTrue(Pos == 10, "4th Find_Ch !!Failed!!");
		assertTrue(Rtn, "4th Find_Ch Failed Return");
	}
	{
		SizeL Pos = 2;
		bool Rtn = a.Find(Pos, 'z');
		assertFalse(Rtn, "5th Find_Ch Failed Return");
	}
	{
		SizeL Pos = 4;
		bool Rtn = a.Find(Pos, 'z', true);
		assertFalse(Rtn, "6th Find_Ch Failed Return");
	}
	{
		SizeL Pos = 3;
		bool Rtn = a.Find(Pos, 'H', true);
		assertTrue(Pos == 14, "7th Find_Ch !!Failed!!");
		assertTrue(Rtn, "7th Find_Ch Failed Return");
	}
}

void Test_wString::test_Find_Str() {
	wString a = "__Hello there Hellish";
	{
		SizeL Pos = 0;
		bool Rtn = a.Find(Pos, "He");
		assertTrue(Pos == 2, "1st Find_Str !!Failed!!");
		assertTrue(Rtn, "1st Find_Str Failed Return");
	}
	{
		SizeL Pos = 12;
		bool Rtn = a.Find(Pos, "_He");
		assertTrue(Pos == 1, "2nd Find_Str !!Failed!!");
		assertTrue(Rtn, "2nd Find_Str Failed Return");
	}
	{
		SizeL Pos = 4;
		bool Rtn = a.Find(Pos, "llo", true);
		assertTrue(Pos == 4, "3rd Find_Str !!Failed!!");
		assertTrue(Rtn, "3rd Find_Str Failed Return");
	}
	{
		SizeL Pos = 4;
		bool Rtn = a.Find(Pos, "he", true);
		assertTrue(Pos == 9, "4th Find_Str !!Failed!!");
		assertTrue(Rtn, "4th Find_Str Failed Return");
	}
	{
		SizeL Pos = 2;
		bool Rtn = a.Find(Pos, " tH");
		assertFalse(Rtn, "5th Find_Str Failed Return");
	}
	{
		SizeL Pos = 4;
		bool Rtn = a.Find(Pos, "_H", true);
		assertFalse(Rtn, "6th Find_Str Failed Return");
	}
	{
		SizeL Pos = 3;
		bool Rtn = a.Find(Pos, "Hell", true);
		assertTrue(Pos == 14, "7th Find_Str !!Failed!!");
		assertTrue(Rtn, "7th Find_Str Failed Return");
	}
	{
		a = "elhelhelf";
		SizeL Pos = 0;
		bool Rtn = a.Find(Pos, "elhelf");
		assertTrue(Pos == 3, "8th Find_Str !!Failed!!");
		assertTrue(Rtn, "8th Find_Str Failed Return");
	}
}

void Test_wString::test_RFind_Ch() {
	wString a = "__Hello there Hellish";
	{
		SizeL Pos = 0;
		bool Rtn = a.RFind(Pos, 'H');
		assertTrue(Pos == 14, "1st RFind_Ch !!Failed!!");
		assertTrue(Rtn, "1st RFind_Ch Failed Return");
	}
	{
		SizeL Pos = 12;
		bool Rtn = a.RFind(Pos, 'l');
		assertTrue(Pos == 17, "2nd RFind_Ch !!Failed!!");
		assertTrue(Rtn, "2nd RFind_Ch Failed Return");
	}
	{
		SizeL Pos = 4;
		bool Rtn = a.RFind(Pos, 'l', true);
		assertTrue(Pos == 4, "3rd RFind_Ch !!Failed!!");
		assertTrue(Rtn, "3rd RFind_Ch Failed Return");
	}
	{
		SizeL Pos = 11;
		bool Rtn = a.RFind(Pos, 'e', true);
		assertTrue(Pos == 10, "4th RFind_Ch !!Failed!!");
		assertTrue(Rtn, "4th RFind_Ch Failed Return");
	}
	{
		SizeL Pos = 2;
		bool Rtn = a.RFind(Pos, 'z');
		assertFalse(Rtn, "5th RFind_Ch Failed Return");
	}
	{
		SizeL Pos = 4;
		bool Rtn = a.RFind(Pos, 'o', true);
		assertFalse(Rtn, "6th RFind_Ch Failed Return");
	}
	{
		SizeL Pos = 5;
		bool Rtn = a.RFind(Pos, 'H', true);
		assertTrue(Pos == 2, "7th RFind_Ch !!Failed!!");
		assertTrue(Rtn, "7th RFind_Ch Failed Return");
	}
}

void Test_wString::test_RFind_Str() {
	wString a = "__Hello there Hellish";
	{
		SizeL Pos = 0;
		bool Rtn = a.RFind(Pos, "He");
		assertTrue(Pos == 14, "1st RFind_Str !!Failed!!");
		assertTrue(Rtn, "1st RFind_Str Failed Return");
	}
	{
		SizeL Pos = 12;
		bool Rtn = a.RFind(Pos, "_He");
		assertTrue(Pos == 1, "2nd RFind_Str !!Failed!!");
		assertTrue(Rtn, "2nd RFind_Str Failed Return");
	}
	{
		SizeL Pos = 4;
		bool Rtn = a.RFind(Pos, "llo", true);
		assertFalse(Rtn, "3rd RFind_Str Failed Return");
	}
	{
		SizeL Pos = 12;
		bool Rtn = a.RFind(Pos, "he", true);
		assertTrue(Pos == 9, "4th RFind_Str !!Failed!!");
		assertTrue(Rtn, "4th RFind_Str Failed Return");
	}
	{
		SizeL Pos = 2;
		bool Rtn = a.RFind(Pos, " tH");
		assertFalse(Rtn, "5th RFind_Str Failed Return");
	}
	{
		SizeL Pos = 4;
		bool Rtn = a.RFind(Pos, "there", true);
		assertFalse(Rtn, "6th RFind_Str Failed Return");
	}
	{
		SizeL Pos = 15;
		bool Rtn = a.RFind(Pos, "Hell", true);
		assertTrue(Pos == 2, "7th RFind_Str !!Failed!!");
		assertTrue(Rtn, "7th RFind_Str Failed Return");
	}
	{
		a = "flehlehle";
		SizeL Pos = 2;
		bool Rtn = a.RFind(Pos, "flehle");
		assertTrue(Pos == 0, "8th RFind_Str !!Failed!!");
		assertTrue(Rtn, "8th RFind_Str Failed Return");
	}
}
