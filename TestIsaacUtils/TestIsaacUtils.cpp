// TestIsaacUtils.cpp : Defines the entry point for the console application.
//

#include "../IsaacUtils/Utils.h"
#include <iostream>
#include <string>

using namespace Utils;
using std::wcout;
using std::endl;
using std::flush;

class TestExc {
private:
	wString Msg;
public:
	TestExc(const wString &Txt) {
		Msg = Txt;
	}
	explicit operator std::wstring() const{
		return std::wstring(Msg.GetData(), Msg.Length());
	}
};

std::wostream &operator<<(std::wostream &os, const TestExc &Exc) {
	return os << (std::wstring)Exc;
}
class TestInst;
typedef void(*TestFunc)(TestInst&);

struct TestData {
	wString Name;
	TestFunc Func;
};
class TestInst {
private:
	wString Name;
	TestFunc Func;
public:
	wString GetName() {
		return Name;
	}
	wString FormatMsg(const wString &Txt) {
		wString Rtn = wString("Error in ") + Name + ": \n  " + Txt;
		return Rtn;
	}
	void assertTrue(bool Expr, const wString &Msg) {
		if (!Expr) throw TestExc(FormatMsg(Msg));
	}
	void assertFalse(bool Expr, const wString &Msg) {
		if (Expr) throw TestExc(FormatMsg(Msg));
	}
	template<typename T>
	void assertEqual(const T &a, const T &b, const wString &Msg) {
		if (!(a == b)) throw TestExc(FormatMsg(Msg));
	}
	template<typename T>
	void assertNotEqual(const T &a, const T &b, const wString &Msg) {
		if (!(a != b)) throw TestExc(FormatMsg(Msg));
	}
	TestInst(const wString &TheName, TestFunc TheFunc) {
		Func = TheFunc;
		Name = TheName;
	}
	TestInst(const TestData &Data) {
		Func = Data.Func;
		Name = Data.Name;
	}
	bool Run(std::wostream &os) {
		try {
			Func(*this);
			return true;
		} catch (TestExc &Exc) {
			os << Exc << endl;
		} catch (...) {
			os << "Unknown Error in " << std::wstring(Name.GetData(), Name.Length()) << endl;
		}
		return false;
	}
};
void Test_wString_Split(TestInst &self) {
	wString a("hello there spaces");
	{
		Array<wString> Cmp("", 3);
		Cmp[0] = "hello";
		Cmp[1] = "there";
		Cmp[2] = "spaces";
		self.assertEqual(a.Split(" "), Cmp, "1st Split Failed");
	}
	{
		Array<wString> Cmp("", 2);
		Cmp[0] = "hello";
		Cmp[1] = "there spaces";
		self.assertEqual(a.Split(" ", 1), Cmp, "2nd Split Failed");
	}
	{
		Array<wString> Cmp("", 3);
		Cmp[0] = "hello";
		Cmp[1] = "there";
		Cmp[2] = "spaces";
		self.assertEqual(a.Split(" ", 2), Cmp, "3rd Split Failed");
	}
}

void Test_wString_RSplit(TestInst &self) {
	wString a("hello there spaces");
	{
		Array<wString> Cmp("", 3);
		Cmp[0] = "hello";
		Cmp[1] = "there";
		Cmp[2] = "spaces";
		self.assertEqual(a.RSplit(" "), Cmp, "1st RSplit Failed");
	}
	{
		Array<wString> Cmp("", 2);
		Cmp[0] = "hello there";
		Cmp[1] = "spaces";
		self.assertEqual(a.RSplit(" ", 1), Cmp, "2nd RSplit Failed");
	}
	{
		Array<wString> Cmp("", 3);
		Cmp[0] = "hello";
		Cmp[1] = "there";
		Cmp[2] = "spaces";
		self.assertEqual(a.RSplit(" ", 2), Cmp, "3rd RSplit Failed");
	}
}

void Test_wString_Join(TestInst &self) {
	Array<wString> a("", 5);
	a[0] = "hello";
	a[1] = "1234";
	a[2] = "there";
	a[3] = "5678";
	a[4] = "spaces";
	self.assertEqual(
		wString(",").Join(a),
		wString("hello,1234,there,5678,spaces"),
		"1st Join Failed");
	self.assertEqual(
		wString(", ").Join(a),
		wString("hello, 1234, there, 5678, spaces"),
		"2nd Join Failed");
	self.assertEqual(
		wString("| |").Join(a),
		wString("hello| |1234| |there| |5678| |spaces"),
		"3rd Join Failed");
}

void Test_wString_Replace_Ch_Ch(TestInst &self) {
	{
		wString a = "hello there is good";
		wString b = "hell0 there is g00d";
		SizeL Rtn = a.Replace('o', '0');
		self.assertEqual(a, b, "1st ReplaceChCh !!Failed!!");
		self.assertTrue(Rtn == 3, "1st ReplaceChCh Failed Return");
	}
	{
		wString a = "hello there is good";
		wString b = "h3llo th3r3 is good";
		SizeL Rtn = a.Replace('e', '3');
		self.assertEqual(a, b, "2nd ReplaceChCh !!Failed!!");
		self.assertTrue(Rtn == 3, "2nd ReplaceChCh Failed Return");
	}
	{
		wString a = "hello there is good";
		wString b = "he11o there is good";
		SizeL Rtn = a.Replace('l', '1');
		self.assertEqual(a, b, "3rd ReplaceChCh !!Failed!!");
		self.assertTrue(Rtn == 2, "3rd ReplaceChCh Failed Return");
	}
}

void Test_wString_Replace_Ch_Str(TestInst &self) {
	{
		wString a = "hello there is good";
		wString b = "hell0 there is g00d";
		SizeL Rtn = a.Replace('o', "0");
		self.assertEqual(a, b, "1st ReplaceChStr !!Failed!!");
		self.assertTrue(Rtn == 3, "1st ReplaceChStr Failed Return");
	}
	{
		wString a = "hello there is good";
		wString b = "h3@llo th3@r3@ is good";
		SizeL Rtn = a.Replace('e', "3@");
		self.assertEqual(a, b, "2nd ReplaceChStr !!Failed!!");
		self.assertTrue(Rtn == 3, "2nd ReplaceChStr Failed Return");
	}
	{
		wString a = "hello there is good";
		wString b = "he$12$12o there is good";
		SizeL Rtn = a.Replace('l', "$12");
		self.assertEqual(a, b, "3rd ReplaceChStr !!Failed!!");
		self.assertTrue(Rtn == 2, "3rd ReplaceChStr Failed Return");
	}
}

void Test_wString_Replace_Str_Ch(TestInst &self) {
	{
		wString a = "hell0 there is g00d";
		wString b = "hello there is good";
		SizeL Rtn = a.Replace("0", 'o');
		self.assertEqual(a, b, "1st ReplaceStrCh !!Failed!!");
		self.assertTrue(Rtn == 3, "1st ReplaceStrCh Failed Return");
	}
	{
		wString a = "h3@llo th3@r3@ is good";
		wString b = "hello there is good";
		SizeL Rtn = a.Replace("3@", 'e');
		self.assertEqual(a, b, "2nd ReplaceStrCh !!Failed!!");
		self.assertTrue(Rtn == 3, "2nd ReplaceStrCh Failed Return");
	}
	{
		wString a = "he$12$12o there is good";
		wString b = "hello there is good";
		SizeL Rtn = a.Replace("$12", 'l');
		self.assertEqual(a, b, "3rd ReplaceStrCh !!Failed!!");
		self.assertTrue(Rtn == 2, "3rd ReplaceStrCh Failed Return");
	}
}

void Test_wString_Find_Ch(TestInst &self) {
	wString a = "__Hello there Hellish";
	{
		SizeL Pos = 0;
		bool Rtn = a.Find(Pos, 'H');
		self.assertTrue(Pos == 2, "1st Find_Ch !!Failed!!");
		self.assertTrue(Rtn, "1st Find_Ch Failed Return");
	}
	{
		SizeL Pos = 12;
		bool Rtn = a.Find(Pos, 'l');
		self.assertTrue(Pos == 4, "2nd Find_Ch !!Failed!!");
		self.assertTrue(Rtn, "2nd Find_Ch Failed Return");
	}
	{
		SizeL Pos = 4;
		bool Rtn = a.Find(Pos, 'l', true);
		self.assertTrue(Pos == 4, "3rd Find_Ch !!Failed!!");
		self.assertTrue(Rtn, "3rd Find_Ch Failed Return");
	}
	{
		SizeL Pos = 4;
		bool Rtn = a.Find(Pos, 'e', true);
		self.assertTrue(Pos == 10, "4th Find_Ch !!Failed!!");
		self.assertTrue(Rtn, "4th Find_Ch Failed Return");
	}
	{
		SizeL Pos = 2;
		bool Rtn = a.Find(Pos, 'z');
		self.assertFalse(Rtn, "5th Find_Ch Failed Return");
	}
	{
		SizeL Pos = 4;
		bool Rtn = a.Find(Pos, 'z', true);
		self.assertFalse(Rtn, "6th Find_Ch Failed Return");
	}
	{
		SizeL Pos = 3;
		bool Rtn = a.Find(Pos, 'H', true);
		self.assertTrue(Pos == 14, "7th Find_Ch !!Failed!!");
		self.assertTrue(Rtn, "7th Find_Ch Failed Return");
	}
}

void Test_wString_Find_Str(TestInst &self) {
	wString a = "__Hello there Hellish";
	{
		SizeL Pos = 0;
		bool Rtn = a.Find(Pos, "He");
		self.assertTrue(Pos == 2, "1st Find_Str !!Failed!!");
		self.assertTrue(Rtn, "1st Find_Str Failed Return");
	}
	{
		SizeL Pos = 12;
		bool Rtn = a.Find(Pos, "_He");
		self.assertTrue(Pos == 1, "2nd Find_Str !!Failed!!");
		self.assertTrue(Rtn, "2nd Find_Str Failed Return");
	}
	{
		SizeL Pos = 4;
		bool Rtn = a.Find(Pos, "llo", true);
		self.assertTrue(Pos == 4, "3rd Find_Str !!Failed!!");
		self.assertTrue(Rtn, "3rd Find_Str Failed Return");
	}
	{
		SizeL Pos = 4;
		bool Rtn = a.Find(Pos, "he", true);
		self.assertTrue(Pos == 9, "4th Find_Str !!Failed!!");
		self.assertTrue(Rtn, "4th Find_Str Failed Return");
	}
	{
		SizeL Pos = 2;
		bool Rtn = a.Find(Pos, " tH");
		self.assertFalse(Rtn, "5th Find_Str Failed Return");
	}
	{
		SizeL Pos = 4;
		bool Rtn = a.Find(Pos, "_H", true);
		self.assertFalse(Rtn, "6th Find_Str Failed Return");
	}
	{
		SizeL Pos = 3;
		bool Rtn = a.Find(Pos, "Hell", true);
		self.assertTrue(Pos == 14, "7th Find_Str !!Failed!!");
		self.assertTrue(Rtn, "7th Find_Str Failed Return");
	}
	{
		a = "elhelhelf";
		SizeL Pos = 0;
		bool Rtn = a.Find(Pos, "elhelf");
		self.assertTrue(Pos == 3, "8th Find_Str !!Failed!!");
		self.assertTrue(Rtn, "8th Find_Str Failed Return");
	}
}

void Test_wString_RFind_Ch(TestInst &self) {
	wString a = "__Hello there Hellish";
	{
		SizeL Pos = 0;
		bool Rtn = a.RFind(Pos, 'H');
		self.assertTrue(Pos == 14, "1st RFind_Ch !!Failed!!");
		self.assertTrue(Rtn, "1st RFind_Ch Failed Return");
	}
	{
		SizeL Pos = 12;
		bool Rtn = a.RFind(Pos, 'l');
		self.assertTrue(Pos == 17, "2nd RFind_Ch !!Failed!!");
		self.assertTrue(Rtn, "2nd RFind_Ch Failed Return");
	}
	{
		SizeL Pos = 4;
		bool Rtn = a.RFind(Pos, 'l', true);
		self.assertTrue(Pos == 4, "3rd RFind_Ch !!Failed!!");
		self.assertTrue(Rtn, "3rd RFind_Ch Failed Return");
	}
	{
		SizeL Pos = 11;
		bool Rtn = a.RFind(Pos, 'e', true);
		self.assertTrue(Pos == 10, "4th RFind_Ch !!Failed!!");
		self.assertTrue(Rtn, "4th RFind_Ch Failed Return");
	}
	{
		SizeL Pos = 2;
		bool Rtn = a.RFind(Pos, 'z');
		self.assertFalse(Rtn, "5th RFind_Ch Failed Return");
	}
	{
		SizeL Pos = 4;
		bool Rtn = a.RFind(Pos, 'o', true);
		self.assertFalse(Rtn, "6th RFind_Ch Failed Return");
	}
	{
		SizeL Pos = 5;
		bool Rtn = a.RFind(Pos, 'H', true);
		self.assertTrue(Pos == 2, "7th RFind_Ch !!Failed!!");
		self.assertTrue(Rtn, "7th RFind_Ch Failed Return");
	}
}

void Test_wString_RFind_Str(TestInst &self) {
	wString a = "__Hello there Hellish";
	{
		SizeL Pos = 0;
		bool Rtn = a.RFind(Pos, "He");
		self.assertTrue(Pos == 14, "1st RFind_Str !!Failed!!");
		self.assertTrue(Rtn, "1st RFind_Str Failed Return");
	}
	{
		SizeL Pos = 12;
		bool Rtn = a.RFind(Pos, "_He");
		self.assertTrue(Pos == 1, "2nd RFind_Str !!Failed!!");
		self.assertTrue(Rtn, "2nd RFind_Str Failed Return");
	}
	{
		SizeL Pos = 4;
		bool Rtn = a.RFind(Pos, "llo", true);
		self.assertFalse(Rtn, "3rd RFind_Str Failed Return");
	}
	{
		SizeL Pos = 12;
		bool Rtn = a.RFind(Pos, "he", true);
		self.assertTrue(Pos == 9, "4th RFind_Str !!Failed!!");
		self.assertTrue(Rtn, "4th RFind_Str Failed Return");
	}
	{
		SizeL Pos = 2;
		bool Rtn = a.RFind(Pos, " tH");
		self.assertFalse(Rtn, "5th RFind_Str Failed Return");
	}
	{
		SizeL Pos = 4;
		bool Rtn = a.RFind(Pos, "there", true);
		self.assertFalse(Rtn, "6th RFind_Str Failed Return");
	}
	{
		SizeL Pos = 15;
		bool Rtn = a.RFind(Pos, "Hell", true);
		self.assertTrue(Pos == 2, "7th RFind_Str !!Failed!!");
		self.assertTrue(Rtn, "7th RFind_Str Failed Return");
	}
	{
		a = "flehlehle";
		SizeL Pos = 2;
		bool Rtn = a.RFind(Pos, "flehle");
		self.assertTrue(Pos == 0, "8th RFind_Str !!Failed!!");
		self.assertTrue(Rtn, "8th RFind_Str Failed Return");
	}
}

void Test_String_Split(TestInst &self) {
	String a("hello there spaces");
	{
		Array<String> Cmp("", 3);
		Cmp[0] = "hello";
		Cmp[1] = "there";
		Cmp[2] = "spaces";
		self.assertEqual(a.Split(" "), Cmp, "1st Split Failed");
	}
	{
		Array<String> Cmp("", 2);
		Cmp[0] = "hello";
		Cmp[1] = "there spaces";
		self.assertEqual(a.Split(" ", 1), Cmp, "2nd Split Failed");
	}
	{
		Array<String> Cmp("", 3);
		Cmp[0] = "hello";
		Cmp[1] = "there";
		Cmp[2] = "spaces";
		self.assertEqual(a.Split(" ", 2), Cmp, "3rd Split Failed");
	}
}

void Test_String_RSplit(TestInst &self) {
	String a("hello there spaces");
	{
		Array<String> Cmp("", 3);
		Cmp[0] = "hello";
		Cmp[1] = "there";
		Cmp[2] = "spaces";
		self.assertEqual(a.RSplit(" "), Cmp, "1st RSplit Failed");
	}
	{
		Array<String> Cmp("", 2);
		Cmp[0] = "hello there";
		Cmp[1] = "spaces";
		self.assertEqual(a.RSplit(" ", 1), Cmp, "2nd RSplit Failed");
	}
	{
		Array<String> Cmp("", 3);
		Cmp[0] = "hello";
		Cmp[1] = "there";
		Cmp[2] = "spaces";
		self.assertEqual(a.RSplit(" ", 2), Cmp, "3rd RSplit Failed");
	}
}

void Test_String_Join(TestInst &self) {
	Array<String> a("", 5);
	a[0] = "hello";
	a[1] = "1234";
	a[2] = "there";
	a[3] = "5678";
	a[4] = "spaces";
	self.assertEqual(
		String(",").Join(a),
		String("hello,1234,there,5678,spaces"),
		"1st Join Failed");
	self.assertEqual(
		String(", ").Join(a),
		String("hello, 1234, there, 5678, spaces"),
		"2nd Join Failed");
	self.assertEqual(
		String("| |").Join(a),
		String("hello| |1234| |there| |5678| |spaces"),
		"3rd Join Failed");
}

void Test_String_Replace_Ch_Ch(TestInst &self) {
	{
		String a = "hello there is good";
		String b = "hell0 there is g00d";
		SizeL Rtn = a.Replace('o', '0');
		self.assertEqual(a, b, "1st ReplaceChCh !!Failed!!");
		self.assertTrue(Rtn == 3, "1st ReplaceChCh Failed Return");
	}
	{
		String a = "hello there is good";
		String b = "h3llo th3r3 is good";
		SizeL Rtn = a.Replace('e', '3');
		self.assertEqual(a, b, "2nd ReplaceChCh !!Failed!!");
		self.assertTrue(Rtn == 3, "2nd ReplaceChCh Failed Return");
	}
	{
		String a = "hello there is good";
		String b = "he11o there is good";
		SizeL Rtn = a.Replace('l', '1');
		self.assertEqual(a, b, "3rd ReplaceChCh !!Failed!!");
		self.assertTrue(Rtn == 2, "3rd ReplaceChCh Failed Return");
	}
}

void Test_String_Replace_Ch_Str(TestInst &self) {
	{
		String a = "hello there is good";
		String b = "hell0 there is g00d";
		SizeL Rtn = a.Replace('o', "0");
		self.assertEqual(a, b, "1st ReplaceChCh !!Failed!!");
		self.assertTrue(Rtn == 3, "1st ReplaceChCh Failed Return");
	}
	{
		String a = "hello there is good";
		String b = "h3@llo th3@r3@ is good";
		SizeL Rtn = a.Replace('e', "3@");
		self.assertEqual(a, b, "2nd ReplaceChStr !!Failed!!");
		self.assertTrue(Rtn == 3, "2nd ReplaceChStr Failed Return");
	}
	{
		String a = "hello there is good";
		String b = "he$12$12o there is good";
		SizeL Rtn = a.Replace('l', "$12");
		self.assertEqual(a, b, "3rd ReplaceChStr !!Failed!!");
		self.assertTrue(Rtn == 2, "3rd ReplaceChStr Failed Return");
	}
}

void Test_String_Replace_Str_Ch(TestInst &self) {
	{
		String a = "hell0 there is g00d";
		String b = "hello there is good";
		SizeL Rtn = a.Replace("0", 'o');
		self.assertEqual(a, b, "1st ReplaceStrCh !!Failed!!");
		self.assertTrue(Rtn == 3, "1st ReplaceStrCh Failed Return");
	}
	{
		String a = "h3@llo th3@r3@ is good";
		String b = "hello there is good";
		SizeL Rtn = a.Replace("3@", 'e');
		self.assertEqual(a, b, "2nd ReplaceStrCh !!Failed!!");
		self.assertTrue(Rtn == 3, "2nd ReplaceStrCh Failed Return");
	}
	{
		String a = "he$12$12o there is good";
		String b = "hello there is good";
		SizeL Rtn = a.Replace("$12", 'l');
		self.assertEqual(a, b, "3rd ReplaceStrCh !!Failed!!");
		self.assertTrue(Rtn == 2, "3rd ReplaceStrCh Failed Return");
	}
}

void Test_String_Find_Ch(TestInst &self) {
	String a = "__Hello there Hellish";
	{
		SizeL Pos = 0;
		bool Rtn = a.Find(Pos, 'H');
		self.assertTrue(Pos == 2, "1st Find_Ch !!Failed!!");
		self.assertTrue(Rtn, "1st Find_Ch Failed Return");
	}
	{
		SizeL Pos = 12;
		bool Rtn = a.Find(Pos, 'l');
		self.assertTrue(Pos == 4, "2nd Find_Ch !!Failed!!");
		self.assertTrue(Rtn, "2nd Find_Ch Failed Return");
	}
	{
		SizeL Pos = 4;
		bool Rtn = a.Find(Pos, 'l', true);
		self.assertTrue(Pos == 4, "3rd Find_Ch !!Failed!!");
		self.assertTrue(Rtn, "3rd Find_Ch Failed Return");
	}
	{
		SizeL Pos = 4;
		bool Rtn = a.Find(Pos, 'e', true);
		self.assertTrue(Pos == 10, "4th Find_Ch !!Failed!!");
		self.assertTrue(Rtn, "4th Find_Ch Failed Return");
	}
	{
		SizeL Pos = 2;
		bool Rtn = a.Find(Pos, 'z');
		self.assertFalse(Rtn, "5th Find_Ch Failed Return");
	}
	{
		SizeL Pos = 4;
		bool Rtn = a.Find(Pos, 'z', true);
		self.assertFalse(Rtn, "6th Find_Ch Failed Return");
	}
	{
		SizeL Pos = 3;
		bool Rtn = a.Find(Pos, 'H', true);
		self.assertTrue(Pos == 14, "7th Find_Ch !!Failed!!");
		self.assertTrue(Rtn, "7th Find_Ch Failed Return");
	}
}

void Test_String_Find_Str(TestInst &self) {
	String a = "__Hello there Hellish";
	{
		SizeL Pos = 0;
		bool Rtn = a.Find(Pos, "He");
		self.assertTrue(Pos == 2, "1st Find_Str !!Failed!!");
		self.assertTrue(Rtn, "1st Find_Str Failed Return");
	}
	{
		SizeL Pos = 12;
		bool Rtn = a.Find(Pos, "_He");
		self.assertTrue(Pos == 1, "2nd Find_Str !!Failed!!");
		self.assertTrue(Rtn, "2nd Find_Str Failed Return");
	}
	{
		SizeL Pos = 4;
		bool Rtn = a.Find(Pos, "llo", true);
		self.assertTrue(Pos == 4, "3rd Find_Str !!Failed!!");
		self.assertTrue(Rtn, "3rd Find_Str Failed Return");
	}
	{
		SizeL Pos = 4;
		bool Rtn = a.Find(Pos, "he", true);
		self.assertTrue(Pos == 9, "4th Find_Str !!Failed!!");
		self.assertTrue(Rtn, "4th Find_Str Failed Return");
	}
	{
		SizeL Pos = 2;
		bool Rtn = a.Find(Pos, " tH");
		self.assertFalse(Rtn, "5th Find_Str Failed Return");
	}
	{
		SizeL Pos = 4;
		bool Rtn = a.Find(Pos, "_H", true);
		self.assertFalse(Rtn, "6th Find_Str Failed Return");
	}
	{
		SizeL Pos = 3;
		bool Rtn = a.Find(Pos, "Hell", true);
		self.assertTrue(Pos == 14, "7th Find_Str !!Failed!!");
		self.assertTrue(Rtn, "7th Find_Str Failed Return");
	}
	{
		a = "elhelhelf";
		SizeL Pos = 0;
		bool Rtn = a.Find(Pos, "elhelf");
		self.assertTrue(Pos == 3, "8th Find_Str !!Failed!!");
		self.assertTrue(Rtn, "8th Find_Str Failed Return");
	}
}

void Test_String_RFind_Ch(TestInst &self) {
	String a = "__Hello there Hellish";
	{
		SizeL Pos = 0;
		bool Rtn = a.RFind(Pos, 'H');
		self.assertTrue(Pos == 14, "1st RFind_Ch !!Failed!!");
		self.assertTrue(Rtn, "1st RFind_Ch Failed Return");
	}
	{
		SizeL Pos = 12;
		bool Rtn = a.RFind(Pos, 'l');
		self.assertTrue(Pos == 17, "2nd RFind_Ch !!Failed!!");
		self.assertTrue(Rtn, "2nd RFind_Ch Failed Return");
	}
	{
		SizeL Pos = 4;
		bool Rtn = a.RFind(Pos, 'l', true);
		self.assertTrue(Pos == 4, "3rd RFind_Ch !!Failed!!");
		self.assertTrue(Rtn, "3rd RFind_Ch Failed Return");
	}
	{
		SizeL Pos = 11;
		bool Rtn = a.RFind(Pos, 'e', true);
		self.assertTrue(Pos == 10, "4th RFind_Ch !!Failed!!");
		self.assertTrue(Rtn, "4th RFind_Ch Failed Return");
	}
	{
		SizeL Pos = 2;
		bool Rtn = a.RFind(Pos, 'z');
		self.assertFalse(Rtn, "5th RFind_Ch Failed Return");
	}
	{
		SizeL Pos = 4;
		bool Rtn = a.RFind(Pos, 'o', true);
		self.assertFalse(Rtn, "6th RFind_Ch Failed Return");
	}
	{
		SizeL Pos = 5;
		bool Rtn = a.RFind(Pos, 'H', true);
		self.assertTrue(Pos == 2, "7th RFind_Ch !!Failed!!");
		self.assertTrue(Rtn, "7th RFind_Ch Failed Return");
	}
}

void Test_String_RFind_Str(TestInst &self) {
	String a = "__Hello there Hellish";
	{
		SizeL Pos = 0;
		bool Rtn = a.RFind(Pos, "He");
		self.assertTrue(Pos == 14, "1st RFind_Str !!Failed!!");
		self.assertTrue(Rtn, "1st RFind_Str Failed Return");
	}
	{
		SizeL Pos = 12;
		bool Rtn = a.RFind(Pos, "_He");
		self.assertTrue(Pos == 1, "2nd RFind_Str !!Failed!!");
		self.assertTrue(Rtn, "2nd RFind_Str Failed Return");
	}
	{
		SizeL Pos = 4;
		bool Rtn = a.RFind(Pos, "llo", true);
		self.assertFalse(Rtn, "3rd RFind_Str Failed Return");
	}
	{
		SizeL Pos = 12;
		bool Rtn = a.RFind(Pos, "he", true);
		self.assertTrue(Pos == 9, "4th RFind_Str !!Failed!!");
		self.assertTrue(Rtn, "4th RFind_Str Failed Return");
	}
	{
		SizeL Pos = 2;
		bool Rtn = a.RFind(Pos, " tH");
		self.assertFalse(Rtn, "5th RFind_Str Failed Return");
	}
	{
		SizeL Pos = 4;
		bool Rtn = a.RFind(Pos, "there", true);
		self.assertFalse(Rtn, "6th RFind_Str Failed Return");
	}
	{
		SizeL Pos = 15;
		bool Rtn = a.RFind(Pos, "Hell", true);
		self.assertTrue(Pos == 2, "7th RFind_Str !!Failed!!");
		self.assertTrue(Rtn, "7th RFind_Str Failed Return");
	}
	{
		a = "flehlehle";
		SizeL Pos = 2;
		bool Rtn = a.RFind(Pos, "flehle");
		self.assertTrue(Pos == 0, "8th RFind_Str !!Failed!!");
		self.assertTrue(Rtn, "8th RFind_Str Failed Return");
	}
}

TestData Tests[] = {
	{ "Test_wString_Split", Test_wString_Split },
	{ "Test_wString_RSplit", Test_wString_RSplit },
	{ "Test_wString_Join", Test_wString_Join },
	{ "Test_wString_Replace_Ch_Ch", Test_wString_Replace_Ch_Ch },
	{ "Test_wString_Replace_Ch_Str", Test_wString_Replace_Ch_Str },
	{ "Test_wString_Replace_Str_Ch", Test_wString_Replace_Str_Ch },
	{ "Test_wString_Find_Ch", Test_wString_Find_Ch },
	{ "Test_wString_Find_Str", Test_wString_Find_Str },
	{ "Test_wString_RFind_Ch", Test_wString_RFind_Ch },
	{ "Test_wString_RFind_Str", Test_wString_RFind_Str },
	{ "Test_String_Split", Test_String_Split },
	{ "Test_String_RSplit", Test_String_RSplit },
	{ "Test_String_Join", Test_String_Join },
	{ "Test_String_Replace_Ch_Ch", Test_String_Replace_Ch_Ch },
	{ "Test_String_Replace_Ch_Str", Test_String_Replace_Ch_Str },
	{ "Test_String_Replace_Str_Ch", Test_String_Replace_Str_Ch },
	{ "Test_String_Find_Ch", Test_String_Find_Ch },
	{ "Test_String_Find_Str", Test_String_Find_Str },
	{ "Test_String_RFind_Ch", Test_String_RFind_Ch },
	{ "Test_String_RFind_Str", Test_String_RFind_Str },
};
SizeL NumTests = sizeof(Tests) / sizeof(TestData);
struct NameRes {
	wString Name;
	bool Passed;
	NameRes &operator=(const NameRes &Other) {
		Name = Other.Name;
		Passed = Other.Passed;
		return *this;
	}
	NameRes &operator=(NameRes &&Other) {
		Name = (wString &&)Other.Name;
		Passed = Other.Passed;
		return *this;
	}
	NameRes(const NameRes &Other) {
		Name = Other.Name;
		Passed = Other.Passed;
	}
	NameRes(NameRes &&Other) {
		Name = (wString &&)Other.Name;
		Passed = Other.Passed;
	}
	bool operator==(const NameRes &Cmp) {
		return Passed == Cmp.Passed && Name == Cmp.Name;
	}
	bool operator!=(const NameRes &Cmp) {
		return Passed != Cmp.Passed || Name != Cmp.Name;
	}
	NameRes() {
		Passed = false;
	}
	NameRes(const wString &TheName, bool IsPassed) {
		Name = TheName;
		Passed = IsPassed;
	}
	NameRes(wString &&TheName, bool IsPassed) {
		Name = TheName;
		Passed = IsPassed;
	}
};

int main()
{
	Array<TestInst*> TestInsts((TestInst*)0, NumTests);
	for (SizeL c = 0; c < NumTests; ++c) {
		TestInsts[c] = new TestInst(Tests[c]);
	}
	SizeL Passed = 0, Failed = 0;
	SizeL MaxName = 0;
	Array<NameRes> TestRes(NameRes(), TestInsts.Length());
	for (SizeL c = 0; c < TestInsts.Length(); ++c) {
		TestInst *Inst = TestInsts[c];
		TestRes[c] = NameRes(Inst->GetName(), Inst->Run(wcout));
		TestRes[c].Passed ? ++Passed : ++Failed;
		SizeL Len = TestRes[c].Name.Length();
		if (Len > MaxName) MaxName = Len;
	}
	MaxName += 1;
	wcout << "Passed: " << Passed << "\nFailed: " << Failed << endl;
	wcout << "+" << wString("-") * (MaxName + 1) << "+--------+" << endl;
	for (SizeL c = 0; c < TestInsts.Length(); ++c) {
		TestInst *Inst = TestInsts[c];
		delete Inst;
		wString Disp = TestRes[c].Name + wString(" ") * (MaxName - TestRes[c].Name.Length()) + "| " + (TestRes[c].Passed ? "PASSED" : "FAILED");
		wcout << "| " << Disp << " |" << endl;
		wcout << "+" << wString("-") * (MaxName + 1) << "+--------+" << endl;
	}
	system("PAUSE");
    return 0;
}

