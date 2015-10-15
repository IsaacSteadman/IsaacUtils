#include "stdafx.h"
#include "Utils.h"

namespace Utils{
	//character string class

	String::String(){
		AllocNum = 0;
		Data = 0;
	}
	String::String(String &&Cpy){
		Data = Cpy.Data;
		AllocNum = Cpy.AllocNum;
		Cpy.Data = 0;
		Cpy.AllocNum = 0;
	}
	String::String(const char *StrIn){
		unsigned long c = 0;
		while (StrIn[c] != 0){
			++c;
		}
		AllocNum = c;
		Data = new char[AllocNum];
		c = 0;
		while (c < AllocNum){
			Data[c] = StrIn[c];
			++c;
		}
	}
	String::String(const wchar_t *StrIn){
		unsigned long c = 0;
		while (StrIn[c] != 0){
			++c;
		}
		AllocNum = c;
		Data = new char[AllocNum];
		c = 0;
		while (c < AllocNum){
			Data[c] = StrIn[c];
			++c;
		}
	}
	String::String(const char *StrIn, unsigned long Len){
		AllocNum = Len;
		unsigned long c = 0;
		Data = new char[AllocNum];
		while (c < AllocNum){
			Data[c] = StrIn[c];
			++c;
		}
	}
	String::String(const wchar_t *StrIn, unsigned long Len){
		AllocNum = Len;
		unsigned long c = 0;
		Data = new char[AllocNum];
		while (c < AllocNum){
			Data[c] = StrIn[c];
			++c;
		}
	}
	String::String(const String &StrCp){
		AllocNum = StrCp.AllocNum;
		unsigned long c = 0;
		Data = new char[AllocNum];
		while (c < AllocNum){
			Data[c] = StrCp.Data[c];
			++c;
		}
	}
	String::String(const char ChFill, const unsigned long Len){
		AllocNum = Len;
		unsigned long c = 0;
		if (AllocNum == 0)
		{
			Data = 0;
			return;
		}
		Data = new char[AllocNum];
		while (c < AllocNum){
			Data[c] = ChFill;
			++c;
		}
	}
	String::~String(){
		if (Data != 0) delete[] Data;
	}
	bool String::operator==(const String &Cmp) const{
		if (AllocNum != Cmp.AllocNum) return false;
		unsigned long c = 0;
		while (c < AllocNum){
			if (Data[c] != Cmp.Data[c]) return false;
			++c;
		}
		return true;
	}
	bool String::operator!=(const String &Cmp) const{
		if (AllocNum != Cmp.AllocNum) return true;
		unsigned long c = 0;
		while (c < AllocNum){
			if (Data[c] != Cmp.Data[c]) return true;
			++c;
		}
		return false;
	}
	bool String::operator<(const String &Cmp) const{
		bool FinalRtn = AllocNum < Cmp.AllocNum;
		unsigned long Shortest = FinalRtn ? AllocNum : Cmp.AllocNum;
		for (unsigned long c = 0; c < Shortest; ++c){
			if (Data[c] != Cmp.Data[c]) return Data[c] < Cmp.Data[c];
		}
		return FinalRtn;
	}
	bool String::operator<=(const String &Cmp) const{
		bool FinalRtn = AllocNum <= Cmp.AllocNum;
		unsigned long Shortest = FinalRtn ? AllocNum : Cmp.AllocNum;
		for (unsigned long c = 0; c < Shortest; ++c){
			if (Data[c] != Cmp.Data[c]) return Data[c] < Cmp.Data[c];
		}
		return FinalRtn;
	}
	bool String::operator>(const String &Cmp) const{
		bool FinalRtn = AllocNum > Cmp.AllocNum;
		unsigned long Shortest = FinalRtn ? Cmp.AllocNum : AllocNum;
		for (unsigned long c = 0; c < Shortest; ++c){
			if (Data[c] != Cmp.Data[c]) return Data[c] > Cmp.Data[c];
		}
		return FinalRtn;
	}
	bool String::operator>=(const String &Cmp) const{
		bool FinalRtn = AllocNum >= Cmp.AllocNum;
		unsigned long Shortest = FinalRtn ? Cmp.AllocNum : AllocNum;
		for (unsigned long c = 0; c < Shortest; ++c){
			if (Data[c] != Cmp.Data[c]) return Data[c] > Cmp.Data[c];
		}
		return FinalRtn;
	}
	String String::operator+(const String &Add) const{
		String Rtn(*this);
		return Rtn.operator+=(Add);
	}
	String &String::operator=(const String &Cp){
		if (Data != 0) delete[] Data;
		AllocNum = Cp.AllocNum;
		unsigned long c = 0;
		Data = new char[AllocNum];
		while (c < AllocNum){
			Data[c] = Cp.Data[c];
			++c;
		}
		return (*this);
	}
	String &String::operator=(String &&Cp){
		if (Data != 0) delete[] Data;
		AllocNum = Cp.AllocNum;
		Data = Cp.Data;
		Cp.AllocNum = 0;
		Cp.Data = 0;
		return (*this);
	}
	String &String::operator+=(const String &Add){
		char *NewData = new char[AllocNum + Add.AllocNum];
		unsigned long c = 0;
		while (c < AllocNum){
			NewData[c] = Data[c];
			++c;
		}
		c = 0;
		while (c < Add.AllocNum){
			NewData[AllocNum + c] = Add.Data[c];
			++c;
		}
		if (Data != 0) delete[] Data;
		AllocNum += Add.AllocNum;
		Data = NewData;
		return (*this);
	}
	String &String::operator+=(const char Add){
		char *NewData = new char[AllocNum + 1];
		unsigned long c = 0;
		while (c < AllocNum){
			NewData[c] = Data[c];
			++c;
		}
		c = 0;
		NewData[AllocNum] = Add;
		if (Data != 0) delete[] Data;
		++AllocNum;
		Data = NewData;
		return (*this);
	}
	char String::operator[](const unsigned long Pos) const{
		if (Pos < AllocNum) return Data[Pos];
		else return 0;
	}
	char &String::operator[](const unsigned long Pos){
		return Data[Pos];
	}
	bool String::Contains(const char Val) const{
		unsigned long c = 0;
		while (c < AllocNum){
			if (Data[c] == Val) return true;
			++c;
		}
		return false;
	}
	unsigned long String::GetNumCh(const char Val) const{
		unsigned long c = 0;
		unsigned long Rtn = 0;
		while (c < AllocNum){
			if (Data[c] == Val) ++Rtn;
			++c;
		}
		return Rtn;
	}
	unsigned long String::Length() const{
		return AllocNum;
	}
	char *String::GetCString() const{
		char *Rtn = new char[AllocNum + 1];
		unsigned long c = 0;
		while (c < AllocNum){
			Rtn[c] = Data[c];
			++c;
		}
		Rtn[c] = 0;
		return Rtn;
	}
	bool String::Insert(unsigned long Pos, char Val){
		if (Pos > AllocNum) return false;
		unsigned long c = 0;
		++AllocNum;
		char *NewData = new char[AllocNum];
		while (c < Pos){
			NewData[c] = Data[c];
			++c;
		}
		NewData[c] = Val;
		++c;
		while (c < AllocNum){
			NewData[c] = Data[c - 1];
			++c;
		}
		delete[] Data;
		Data = NewData;
		return true;
	}
	bool String::Insert(unsigned long Pos, String &Str){
		if (Pos > AllocNum) return false;
		unsigned long c = 0;
		AllocNum += Str.AllocNum;
		char *NewData = new char[AllocNum];
		while (c < Pos){
			NewData[c] = Data[c];
			++c;
		}
		unsigned long Pos2 = Pos + Str.AllocNum;
		while (c < Pos2){
			NewData[c] = Str.Data[c - Pos];
			++c;
		}
		while (c < AllocNum){
			NewData[c] = Data[c - Str.AllocNum];
			++c;
		}
		delete[] Data;
		Data = NewData;
		return true;
	}
	bool String::Remove(unsigned long Pos, unsigned long Num){
		if (Pos >= AllocNum) return false;
		unsigned long c = 0;
		AllocNum -= Num;
		char *NewData = new char[AllocNum];
		while (c < Pos){
			NewData[c] = Data[c];
			++c;
		}
		while (c < AllocNum){
			NewData[c] = Data[c + Num];
			++c;
		}
		delete[] Data;
		Data = NewData;
		return true;
	}
	bool String::Find(unsigned long &Pos, char Ch, bool PosIsStart) const{
		for (unsigned long c = PosIsStart ? Pos : 0; c < AllocNum; ++c){
			if (Data[c] == Ch)
			{
				Pos = c;
				return true;
			}
		}
		return false;
	}
	bool String::RFind(unsigned long &Pos, char Ch, bool PosIsStart){
		--Data;//to shift Data so that (Old)Data[0] is the same as (New)Data[1]
		unsigned long Until = PosIsStart ? Pos : 0;
		for (unsigned long c = AllocNum; c > Until; --c){
			if (Data[c] == Ch)
			{
				Pos = c;
				++Data;
				return true;
			}
		}
		++Data;
		return false;
	}
	bool String::Find(unsigned long &Pos, String Str, bool PosIsStart) const{
		if (Str.AllocNum == 0) return false;
		unsigned long MidPos = 0;
		for (unsigned long c = PosIsStart ? Pos : 0; c < AllocNum; ++c){
			if (Data[c] == Str.Data[MidPos])
			{
				++MidPos;
				if (MidPos == Str.AllocNum)
				{
					Pos = c;
					return true;
				}
			}
			else MidPos = 0;
		}
		return false;
	}
	bool String::RFind(unsigned long &Pos, String Str, bool PosIsStart){
		if (Str.AllocNum == 0) return false;
		--Data;//to shift Data so that (Old)Data[0] is the same as (New)Data[1]
		--Str.Data;
		unsigned long MidPos = Str.AllocNum;
		unsigned long Until = PosIsStart ? Pos : 0;
		for (unsigned long c = AllocNum; c > Until; --c){
			if (Data[c] == Str.Data[MidPos])
			{
				--MidPos;
				if (MidPos == 0)
				{
					++Data;
					++Str.Data;
					Pos = c;
					return true;
				}
			}
			else MidPos = Str.AllocNum;
		}
		++Data;
		++Str.Data;
		return false;
	}
	String String::SubStr(unsigned long Start, unsigned long Stop, signed long Step) const{
		if (AllocNum == 0) return "";
		if ((Stop > AllocNum) && Stop != 0xFFFFFFFF) Stop = AllocNum;
		if (Start > AllocNum) Start = AllocNum - 1;
		if (Step < 0)
		{
			bool StopMax = false;
			if (Stop == 0xFFFFFFFF) StopMax = true;
			else if (Start <= Stop) return "";
			if (Start >= AllocNum) Start = AllocNum - 1;
			String Rtn(char(0), ((Start - Stop) - (StopMax ? Step + 1 : Step)) / -Step);
			unsigned long c = 0;
			for (char &Ch : Rtn){
				Ch = Data[Start + c * Step];
				++c;
			}
			return Rtn;
		}
		else
		{
			if (Start >= Stop) return "";
			if (Stop == 0xFFFFFFFF) Stop = AllocNum;
			unsigned long Step1 = Step + 1;
			unsigned long c1 = 0;
			String Rtn(char(0), (Start - Stop - 1) / Step1 + 1);
			for (unsigned long c = 0; c < Stop; c += Step1){
				Rtn[c1] = Data[c];
				++c1;
			}
			return Rtn;
		}
	}
	char &String::AtEnd(){
		return Data[AllocNum - 1];
	}
	void String::ToLower(){
		for (unsigned long c = 0; c < AllocNum; ++c){
			if (Data[c] >= 'A' && Data[c] <= 'Z') Data[c] += 32;
		}
	}
	void String::ToUpper(){
		for (unsigned long c = 0; c < AllocNum; ++c){
			if (Data[c] >= 'a' && Data[c] <= 'z') Data[c] -= 32;
		}
	}
	String String::Lower() const{
		String Rtn = (*this);
		Rtn.ToLower();
		return Rtn;
	}
	String String::Upper() const{
		String Rtn = (*this);
		Rtn.ToUpper();
		return Rtn;
	}
	bool String::IsDigit(bool Hex) const{
		for (unsigned long c = 0; c < AllocNum; ++c){
			if (Data[c] <= '9' && Data[c] >= '0') continue;
			else if (Hex && ((Data[c] <= 'f' && Data[c] >= 'a') || (Data[c] <= 'F' && Data[c] >= 'A'))) continue;
			else return false;
		}
		return true;
	}
	bool String::IsAlpha() const{
		for (unsigned long c = 0; c < AllocNum; ++c){
			if ((Data[c] <= 'Z' && Data[c] >= 'A') || (Data[c] <= 'z' && Data[c] >= 'a')) continue;
			else return false;
		}
		return true;
	}
	bool String::IsAlnum() const{
		for (unsigned long c = 0; c < AllocNum; ++c){
			if ((Data[c] <= 'Z' && Data[c] >= 'A') || (Data[c] <= 'z' && Data[c] >= 'a')) continue;
			else if (Data[c] <= '9' && Data[c] >= '0') continue;
			else return false;
		}
		return true;
	}
	String &String::operator*=(unsigned long Num){
		char *NewData = new char[AllocNum * Num];
		for (unsigned long c = 0; c < Num; ++c){
			for (unsigned long c1 = 0; c1 < AllocNum; ++c1){
				NewData[(c * AllocNum) + c1] = Data[c1];
			}
		}
		AllocNum *= Num;
		delete[] Data;
		Data = NewData;
		return (*this);
	}
	String String::operator*(unsigned long Num) const{
		String Rtn;
		Rtn.Data = new char[AllocNum * Num];
		for (unsigned long c = 0; c < Num; ++c){
			for (unsigned long c1 = 0; c1 < AllocNum; ++c1){
				Rtn.Data[(c * AllocNum) + c1] = Data[c1];
			}
		}
		Rtn.AllocNum = AllocNum * Num;
		return Rtn;
	}
	wString String::wStr() const{
		return wString(Data, AllocNum);
	}
	const char *String::GetData() const{
		return Data;
	}

	String::Iterator String::begin(){
		return Iterator(this, 0);
	}
	String::Iterator String::end(){
		return Iterator(this, AllocNum);
	}
	String::Iterator::Iterator(String *Iter, unsigned long CharPos){
		Str = Iter;
		Pos = CharPos;
	}
	String::Iterator &String::Iterator::operator++(){
		++Pos;
		return (*this);
	}
	bool String::Iterator::operator==(Iterator &Cmp){
		return (Pos == Cmp.Pos) && (Str == Cmp.Str);
	}
	bool String::Iterator::operator!=(Iterator &Cmp){
		return (Pos != Cmp.Pos) || (Str != Cmp.Str);
	}
	char &String::Iterator::operator*(){
		return (*Str)[Pos];
	}



	//wide character string class

	wString::wString(){
		AllocNum = 0;
		Data = 0;
	}
	wString::wString(wString &&Cpy){
		Data = Cpy.Data;
		AllocNum = Cpy.AllocNum;
		Cpy.Data = 0;
		Cpy.AllocNum = 0;
	}
	wString::wString(const char *StrIn){
		unsigned long c = 0;
		while (StrIn[c] != 0){
			++c;
		}
		AllocNum = c;
		Data = new wchar_t[AllocNum];
		c = 0;
		while (c < AllocNum){
			Data[c] = StrIn[c];
			++c;
		}
	}
	wString::wString(const wchar_t *StrIn){
		unsigned long c = 0;
		while (StrIn[c] != 0){
			++c;
		}
		AllocNum = c;
		Data = new wchar_t[AllocNum];
		c = 0;
		while (c < AllocNum){
			Data[c] = StrIn[c];
			++c;
		}
	}
	wString::wString(const char *StrIn, unsigned long Len){
		AllocNum = Len;
		unsigned long c = 0;
		Data = new wchar_t[AllocNum];
		while (c < AllocNum){
			Data[c] = StrIn[c];
			++c;
		}
	}
	wString::wString(const wchar_t *StrIn, unsigned long Len){
		AllocNum = Len;
		unsigned long c = 0;
		Data = new wchar_t[AllocNum];
		while (c < AllocNum){
			Data[c] = StrIn[c];
			++c;
		}
	}
	wString::wString(const wString &StrCp){
		AllocNum = StrCp.AllocNum;
		unsigned long c = 0;
		Data = new wchar_t[AllocNum];
		while (c < AllocNum){
			Data[c] = StrCp.Data[c];
			++c;
		}
	}
	wString::wString(const wchar_t ChFill, const unsigned long Len){
		AllocNum = Len;
		unsigned long c = 0;
		if (AllocNum == 0)
		{
			Data = 0;
			return;
		}
		Data = new wchar_t[AllocNum];
		while (c < AllocNum){
			Data[c] = ChFill;
			++c;
		}
	}
	wString::~wString(){
		if (Data != 0) delete[] Data;
	}
	bool wString::operator==(const wString &Cmp) const{
		if (AllocNum != Cmp.AllocNum) return false;
		unsigned long c = 0;
		while (c < AllocNum){
			if (Data[c] != Cmp.Data[c]) return false;
			++c;
		}
		return true;
	}
	bool wString::operator!=(const wString &Cmp) const{
		if (AllocNum != Cmp.AllocNum) return true;
		unsigned long c = 0;
		while (c < AllocNum){
			if (Data[c] != Cmp.Data[c]) return true;
			++c;
		}
		return false;
	}
	bool wString::operator<(const wString &Cmp) const{
		bool FinalRtn = AllocNum < Cmp.AllocNum;
		unsigned long Shortest = FinalRtn ? AllocNum : Cmp.AllocNum;
		for (unsigned long c = 0; c < Shortest; ++c){
			if (Data[c] != Cmp.Data[c]) return Data[c] < Cmp.Data[c];
		}
		return FinalRtn;
	}
	bool wString::operator<=(const wString &Cmp) const{
		bool FinalRtn = AllocNum <= Cmp.AllocNum;
		unsigned long Shortest = FinalRtn ? AllocNum : Cmp.AllocNum;
		for (unsigned long c = 0; c < Shortest; ++c){
			if (Data[c] != Cmp.Data[c]) return Data[c] < Cmp.Data[c];
		}
		return FinalRtn;
	}
	bool wString::operator>(const wString &Cmp) const{
		bool FinalRtn = AllocNum > Cmp.AllocNum;
		unsigned long Shortest = FinalRtn ? Cmp.AllocNum : AllocNum;
		for (unsigned long c = 0; c < Shortest; ++c){
			if (Data[c] != Cmp.Data[c]) return Data[c] > Cmp.Data[c];
		}
		return FinalRtn;
	}
	bool wString::operator>=(const wString &Cmp) const{
		bool FinalRtn = AllocNum >= Cmp.AllocNum;
		unsigned long Shortest = FinalRtn ? Cmp.AllocNum : AllocNum;
		for (unsigned long c = 0; c < Shortest; ++c){
			if (Data[c] != Cmp.Data[c]) return Data[c] > Cmp.Data[c];
		}
		return FinalRtn;
	}
	wString wString::operator+(const wString &Add) const{
		wString Rtn(*this);
		return Rtn.operator+=(Add);
	}
	wchar_t wString::operator[](const unsigned long Pos) const{
		if (Pos < AllocNum) return Data[Pos];
		else return 0;
	}
	wString &wString::operator=(const wString &Cp){
		if (Data != 0) delete[] Data;
		AllocNum = Cp.AllocNum;
		unsigned long c = 0;
		Data = new wchar_t[AllocNum];
		while (c < AllocNum){
			Data[c] = Cp.Data[c];
			++c;
		}
		return (*this);
	}
	wString &wString::operator=(wString &&Cp){
		if (Data != 0) delete[] Data;
		AllocNum = Cp.AllocNum;
		Data = Cp.Data;
		Cp.AllocNum = 0;
		Cp.Data = 0;
		return (*this);
	}
	wString &wString::operator+=(const wString &Add){
		wchar_t *NewData = new wchar_t[AllocNum + Add.AllocNum];
		unsigned long c = 0;
		while (c < AllocNum){
			NewData[c] = Data[c];
			++c;
		}
		c = 0;
		while (c < Add.AllocNum){
			NewData[AllocNum + c] = Add.Data[c];
			++c;
		}
		if (Data != 0) delete[] Data;
		AllocNum += Add.AllocNum;
		Data = NewData;
		return (*this);
	}
	wString &wString::operator+=(const wchar_t Add){
		wchar_t *NewData = new wchar_t[AllocNum + 1];
		unsigned long c = 0;
		while (c < AllocNum){
			NewData[c] = Data[c];
			++c;
		}
		c = 0;
		NewData[AllocNum] = Add;
		if (Data != 0) delete[] Data;
		++AllocNum;
		Data = NewData;
		return (*this);
	}
	wchar_t &wString::operator[](const unsigned long Pos){
		return Data[Pos];
	}
	bool wString::Contains(const wchar_t Val) const{
		unsigned long c = 0;
		while (c < AllocNum){
			if (Data[c] == Val) return true;
			++c;
		}
		return false;
	}
	unsigned long wString::GetNumCh(const wchar_t Val) const{
		unsigned long c = 0;
		unsigned long Rtn = 0;
		while (c < AllocNum){
			if (Data[c] == Val) ++Rtn;
			++c;
		}
		return Rtn;
	}
	unsigned long wString::Length() const{
		return AllocNum;
	}
	wchar_t *wString::GetCString() const{
		wchar_t *Rtn = new wchar_t[AllocNum + 1];
		unsigned long c = 0;
		while (c < AllocNum){
			Rtn[c] = Data[c];
			++c;
		}
		Rtn[c] = 0;
		return Rtn;
	}
	bool wString::Insert(unsigned long Pos, wchar_t Val){
		if (Pos > AllocNum) return false;
		unsigned long c = 0;
		++AllocNum;
		wchar_t *NewData = new wchar_t[AllocNum];
		while (c < Pos){
			NewData[c] = Data[c];
			++c;
		}
		NewData[c] = Val;
		++c;
		while (c < AllocNum){
			NewData[c] = Data[c - 1];
			++c;
		}
		delete[] Data;
		Data = NewData;
		return true;
	}
	bool wString::Insert(unsigned long Pos, wString &Str){
		if (Pos > AllocNum) return false;
		unsigned long c = 0;
		AllocNum += Str.AllocNum;
		wchar_t *NewData = new wchar_t[AllocNum];
		while (c < Pos){
			NewData[c] = Data[c];
			++c;
		}
		unsigned long Pos2 = Pos + Str.AllocNum;
		while (c < Pos2){
			NewData[c] = Str.Data[c - Pos];
			++c;
		}
		while (c < AllocNum){
			NewData[c] = Data[c - Str.AllocNum];
			++c;
		}
		delete[] Data;
		Data = NewData;
		return true;
	}
	bool wString::Remove(unsigned long Pos, unsigned long Num){
		if (Pos >= AllocNum) return false;
		unsigned long c = 0;
		AllocNum -= Num;
		wchar_t *NewData = new wchar_t[AllocNum];
		while (c < Pos){
			NewData[c] = Data[c];
			++c;
		}
		while (c < AllocNum){
			NewData[c] = Data[c + Num];
			++c;
		}
		delete[] Data;
		Data = NewData;
		return true;
	}
	bool wString::Find(unsigned long &Pos, wchar_t Ch, bool PosIsStart) const{
		for (unsigned long c = PosIsStart ? Pos : 0; c < AllocNum; ++c){
			if (Data[c] == Ch)
			{
				Pos = c;
				return true;
			}
		}
		return false;
	}
	bool wString::RFind(unsigned long &Pos, wchar_t Ch, bool PosIsStart){
		--Data;//to shift Data so that (Old)Data[0] is the same as (New)Data[1]
		unsigned long Until = PosIsStart ? Pos : 0;
		for (unsigned long c = AllocNum; c > Until; --c){
			if (Data[c] == Ch)
			{
				Pos = c;
				++Data;
				return true;
			}
		}
		++Data;
		return false;
	}
	bool wString::Find(unsigned long &Pos, wString Ch, bool PosIsStart) const{
		if (Ch.AllocNum == 0) return false;
		unsigned long MidPos = 0;
		for (unsigned long c = PosIsStart ? Pos : 0; c < AllocNum; ++c){
			if (Data[c] == Ch.Data[MidPos])
			{
				++MidPos;
				if (MidPos == Ch.AllocNum)
				{
					Pos = c;
					return true;
				}
			}
			else MidPos = 0;
		}
		return false;
	}
	bool wString::RFind(unsigned long &Pos, wString Ch, bool PosIsStart){
		if (Ch.AllocNum == 0) return false;
		--Data;//to shift Data so that (Old)Data[0] is the same as (New)Data[1]
		--Ch.Data;
		unsigned long MidPos = Ch.AllocNum;
		unsigned long Until = PosIsStart ? Pos : 0;
		for (unsigned long c = AllocNum; c > Until; --c){
			if (Data[c] == Ch.Data[MidPos])
			{
				--MidPos;
				if (MidPos == 0)
				{
					++Data;
					++Ch.Data;
					Pos = c;
					return true;
				}
			}
			else MidPos = Ch.AllocNum;
		}
		++Data;
		++Ch.Data;
		return false;
	}
	wString wString::SubStr(unsigned long Start, unsigned long Stop, signed long Step) const{
		if (AllocNum == 0) return "";
		if ((Stop > AllocNum) && Stop != 0xFFFFFFFF) Stop = AllocNum;
		if (Start > AllocNum) Start = AllocNum - 1;
		if (Step < 0)
		{
			bool StopMax = false;
			if (Stop == 0xFFFFFFFF) StopMax = true;
			else if (Start <= Stop) return "";
			if (Start >= AllocNum) Start = AllocNum - 1;
			wString Rtn(wchar_t(0), ((Start - Stop) - (StopMax ? Step + 1 : Step)) / -Step);
			unsigned long c = 0;
			for (wchar_t &Ch : Rtn){
				Ch = Data[Start + c * Step];
				++c;
			}
			return Rtn;
		}
		else
		{
			if (Start >= Stop) return "";
			if (Stop == 0xFFFFFFFF) Stop = AllocNum;
			unsigned long Step1 = Step + 1;
			unsigned long c1 = 0;
			wString Rtn(wchar_t(0), (Start - Stop - 1) / Step1 + 1);
			for (unsigned long c = 0; c < Stop; c += Step1){
				Rtn[c1] = Data[c];
				++c1;
			}
			return Rtn;
		}
	}
	wchar_t &wString::AtEnd(){
		return Data[AllocNum - 1];
	}
	void wString::ToLower(){
		for (unsigned long c = 0; c < AllocNum; ++c){
			if (Data[c] >= 'A' && Data[c] <= 'Z') Data[c] += 32;
		}
	}
	void wString::ToUpper(){
		for (unsigned long c = 0; c < AllocNum; ++c){
			if (Data[c] >= 'a' && Data[c] <= 'z') Data[c] -= 32;
		}
	}
	wString wString::Lower() const{
		wString Rtn = (*this);
		Rtn.ToLower();
		return Rtn;
	}
	wString wString::Upper() const{
		wString Rtn = (*this);
		Rtn.ToUpper();
		return Rtn;
	}
	bool wString::IsDigit(bool Hex) const{
		for (unsigned long c = 0; c < AllocNum; ++c){
			if (Data[c] <= '9' && Data[c] >= '0') continue;
			else if (Hex && ((Data[c] <= 'f' && Data[c] >= 'a') || (Data[c] <= 'F' && Data[c] >= 'A'))) continue;
			else return false;
		}
		return true;
	}
	bool wString::IsAlpha() const{
		for (unsigned long c = 0; c < AllocNum; ++c){
			if ((Data[c] <= 'Z' && Data[c] >= 'A') || (Data[c] <= 'z' && Data[c] >= 'a')) continue;
			else return false;
		}
		return true;
	}
	bool wString::IsAlnum() const{
		for (unsigned long c = 0; c < AllocNum; ++c){
			if ((Data[c] <= 'Z' && Data[c] >= 'A') || (Data[c] <= 'z' && Data[c] >= 'a')) continue;
			else if (Data[c] <= '9' && Data[c] >= '0') continue;
			else return false;
		}
		return true;
	}
	wString &wString::operator*=(unsigned long Num){
		wchar_t *NewData = new wchar_t[AllocNum * Num];
		for (unsigned long c = 0; c < Num; ++c){
			for (unsigned long c1 = 0; c1 < AllocNum; ++c1){
				NewData[(c * AllocNum) + c1] = Data[c1];
			}
		}
		AllocNum *= Num;
		delete[] Data;
		Data = NewData;
		return (*this);
	}
	wString wString::operator*(unsigned long Num) const{
		wString Rtn;
		Rtn.Data = new wchar_t[AllocNum * Num];
		for (unsigned long c = 0; c < Num; ++c){
			for (unsigned long c1 = 0; c1 < AllocNum; ++c1){
				Rtn.Data[(c * AllocNum) + c1] = Data[c1];
			}
		}
		Rtn.AllocNum = AllocNum * Num;
		return Rtn;
	}
	String wString::Str() const{
		return String(Data, AllocNum);
	}
	const wchar_t *wString::GetData() const{
		return Data;
	}

	wString::Iterator wString::begin(){
		return Iterator(this, 0);
	}
	wString::Iterator wString::end(){
		return Iterator(this, AllocNum);
	}
	wString::Iterator::Iterator(wString *Iter, unsigned long CharPos){
		Str = Iter;
		Pos = CharPos;
	}
	wString::Iterator &wString::Iterator::operator++(){
		++Pos;
		return (*this);
	}
	bool wString::Iterator::operator==(Iterator &Cmp){
		return (Pos == Cmp.Pos) && (Str == Cmp.Str);
	}
	bool wString::Iterator::operator!=(Iterator &Cmp){
		return (Pos != Cmp.Pos) || (Str != Cmp.Str);
	}
	wchar_t &wString::Iterator::operator*(){
		return (*Str)[Pos];
	}
}