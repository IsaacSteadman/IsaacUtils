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
		SizeL c = 0;
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
		SizeL c = 0;
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
	String::String(const char *StrIn, SizeL Len){
		AllocNum = Len;
		SizeL c = 0;
		Data = new char[AllocNum];
		while (c < AllocNum){
			Data[c] = StrIn[c];
			++c;
		}
	}
	String::String(const wchar_t *StrIn, SizeL Len){
		AllocNum = Len;
		SizeL c = 0;
		Data = new char[AllocNum];
		while (c < AllocNum){
			Data[c] = StrIn[c];
			++c;
		}
	}
	String::String(const String &StrCp){
		AllocNum = StrCp.AllocNum;
		SizeL c = 0;
		Data = new char[AllocNum];
		while (c < AllocNum){
			Data[c] = StrCp.Data[c];
			++c;
		}
	}
	String::String(const char ChFill, const SizeL Len){
		AllocNum = Len;
		if (AllocNum == 0)
		{
			Data = 0;
			return;
		}
		SizeL c = 0;
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
		SizeL c = 0;
		while (c < AllocNum){
			if (Data[c] != Cmp.Data[c]) return false;
			++c;
		}
		return true;
	}
	bool String::operator!=(const String &Cmp) const{
		if (AllocNum != Cmp.AllocNum) return true;
		SizeL c = 0;
		while (c < AllocNum){
			if (Data[c] != Cmp.Data[c]) return true;
			++c;
		}
		return false;
	}
	bool String::operator<(const String &Cmp) const{
		bool FinalRtn = AllocNum < Cmp.AllocNum;
		SizeL Shortest = FinalRtn ? AllocNum : Cmp.AllocNum;
		for (SizeL c = 0; c < Shortest; ++c){
			if (Data[c] != Cmp.Data[c]) return Data[c] < Cmp.Data[c];
		}
		return FinalRtn;
	}
	bool String::operator<=(const String &Cmp) const{
		bool FinalRtn = AllocNum <= Cmp.AllocNum;
		SizeL Shortest = FinalRtn ? AllocNum : Cmp.AllocNum;
		for (SizeL c = 0; c < Shortest; ++c){
			if (Data[c] != Cmp.Data[c]) return Data[c] < Cmp.Data[c];
		}
		return FinalRtn;
	}
	bool String::operator>(const String &Cmp) const{
		bool FinalRtn = AllocNum > Cmp.AllocNum;
		SizeL Shortest = FinalRtn ? Cmp.AllocNum : AllocNum;
		for (SizeL c = 0; c < Shortest; ++c){
			if (Data[c] != Cmp.Data[c]) return Data[c] > Cmp.Data[c];
		}
		return FinalRtn;
	}
	bool String::operator>=(const String &Cmp) const{
		bool FinalRtn = AllocNum >= Cmp.AllocNum;
		SizeL Shortest = FinalRtn ? Cmp.AllocNum : AllocNum;
		for (SizeL c = 0; c < Shortest; ++c){
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
		SizeL c = 0;
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
		SizeL c = 0;
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
		SizeL c = 0;
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
	char String::operator[](const SizeL Pos) const{
		if (Pos < AllocNum) return Data[Pos];
		else return 0;
	}
	char &String::operator[](const SizeL Pos){
		return Data[Pos];
	}
	bool String::Contains(const char Val) const{
		SizeL c = 0;
		while (c < AllocNum){
			if (Data[c] == Val) return true;
			++c;
		}
		return false;
	}
	SizeL String::GetNumCh(const char Val) const{
		SizeL c = 0;
		SizeL Rtn = 0;
		while (c < AllocNum){
			if (Data[c] == Val) ++Rtn;
			++c;
		}
		return Rtn;
	}
	SizeL String::Length() const{
		return AllocNum;
	}
	char *String::GetCString() const{
		char *Rtn = new char[AllocNum + 1];
		SizeL c = 0;
		while (c < AllocNum){
			Rtn[c] = Data[c];
			++c;
		}
		Rtn[c] = 0;
		return Rtn;
	}
	bool String::Insert(SizeL Pos, char Val){
		if (Pos > AllocNum) return false;
		SizeL c = 0;
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
	bool String::Insert(SizeL Pos, String &Str){
		if (Pos > AllocNum) return false;
		SizeL c = 0;
		AllocNum += Str.AllocNum;
		char *NewData = new char[AllocNum];
		while (c < Pos){
			NewData[c] = Data[c];
			++c;
		}
		SizeL Pos2 = Pos + Str.AllocNum;
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
	bool String::Remove(SizeL Pos, SizeL Num){
		if (Pos >= AllocNum) return false;
		SizeL c = 0;
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
	bool String::Find(SizeL &Pos, char Ch, bool PosIsStart) const{
		for (SizeL c = PosIsStart ? Pos : 0; c < AllocNum; ++c){
			if (Data[c] == Ch)
			{
				Pos = c;
				return true;
			}
		}
		return false;
	}
	bool String::RFind(SizeL &Pos, char Ch, bool PosIsStart){
		--Data;//to shift Data so that (Old)Data[0] is the same as (New)Data[1]
		SizeL Until = PosIsStart ? Pos : 0;
		for (SizeL c = AllocNum; c > Until; --c){
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
	bool String::Find(SizeL &Pos, String Str, bool PosIsStart) const{
		if (Str.AllocNum == 0) return false;
		SizeL MidPos = 0;
		for (SizeL c = PosIsStart ? Pos : 0; c < AllocNum; ++c){
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
	bool String::RFind(SizeL &Pos, String Str, bool PosIsStart){
		if (Str.AllocNum == 0) return false;
		--Data;//to shift Data so that (Old)Data[0] is the same as (New)Data[1]
		--Str.Data;
		SizeL MidPos = Str.AllocNum;
		SizeL Until = PosIsStart ? Pos : 0;
		for (SizeL c = AllocNum; c > Until; --c){
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
	SizeL String::Replace(const String &FindStr, const String &Write) {
		if (FindStr.Length() == 0) return 0;
		if (FindStr.Length() == 1 && Write.Length() == 1) return Replace(FindStr[0], Write[0]);
		Array<SizeL> PosFinds;
		SizeL Pos = 0;
		while (Find(Pos, FindStr, true)) {
			PosFinds += Pos;
		}
		if (PosFinds.Length() == 0) return 0;
		char *NewData = 0;
		SizeL NewLen = 0;
		if (FindStr.Length() > Write.Length()) {
			SizeL DifLen = FindStr.Length() - Write.Length();
			NewLen = AllocNum - PosFinds.Length() * DifLen;
			NewData = new char[NewLen];
			SizeL CurPosFind = 0;
			SizeL CurOff = 0;
			for (SizeL c = 0; c < AllocNum; ++c) {
				if (c < PosFinds.Length() && c == PosFinds[CurPosFind])
				{
					for (SizeL c0 = 0; c0 < Write.Length(); ++c, ++c0) {
						NewData[c - CurOff] = Write[c0];
					}
					CurOff += DifLen;
					++CurPosFind;
				}
				NewData[c - CurOff] = Data[c];
			}
		}
		else
		{
			SizeL DifLen = Write.Length() - FindStr.Length();
			NewLen = AllocNum + PosFinds.Length() * DifLen;
			NewData = new char[NewLen];
			SizeL CurPosFind = 0;
			SizeL CurOff = 0;
			for (SizeL c = 0; c < AllocNum; ++c) {
				if (c < PosFinds.Length() && c == PosFinds[CurPosFind])
				{
					for (SizeL c0 = 0; c0 < Write.Length(); ++c0) {
						NewData[c + CurOff + c0] = Write[c0];
					}
					CurOff += DifLen;
					++CurPosFind;
				}
				NewData[c + CurOff] = Data[c];
			}
		}
		if (AllocNum > 0) delete[] Data;
		Data = NewData;
		AllocNum = NewLen;
		return PosFinds.Length();// return the number of replacements we did
	}
	SizeL String::Replace(const char FindStr, const String &Write) {
		if (Write.Length() == 1) return Replace(FindStr, Write[0]);
		Array<SizeL> PosFinds;
		SizeL Pos = 0;
		while (Find(Pos, FindStr, true)) {
			PosFinds += Pos;
		}
		if (PosFinds.Length() == 0) return 0;
		SizeL DifLen = Write.Length() - 1;
		SizeL NewLen = AllocNum + PosFinds.Length() * DifLen;
		char *NewData = new char[NewLen];
		SizeL CurPosFind = 0;
		SizeL CurOff = 0;
		for (SizeL c = 0; c < AllocNum; ++c) {
			if (c < PosFinds.Length() && c == PosFinds[CurPosFind])
			{
				for (SizeL c0 = 0; c0 < Write.Length(); ++c0) {
					NewData[c + CurOff + c0] = Write[c0];
				}
				CurOff += DifLen;
				++CurPosFind;
			}
			NewData[c + CurOff] = Data[c];
		}
		return PosFinds.Length();
	}
	SizeL String::Replace(const String &FindStr, const char Write) {
		if (FindStr.Length() == 0) return 0;
		if (FindStr.Length() == 1) return Replace(FindStr[0], Write);
		Array<SizeL> PosFinds;
		SizeL Pos = 0;
		while (Find(Pos, FindStr, true)) {
			PosFinds += Pos;
		}
		if (PosFinds.Length() == 0) return 0;
		//length of FindStr is always greater than 1 (length of Write) at this point
		SizeL DifLen = FindStr.Length() - 1;
		SizeL NewLen = AllocNum - PosFinds.Length() * DifLen;
		char *NewData = new char[NewLen];
		SizeL CurPosFind = 0;
		SizeL CurOff = 0;
		for (SizeL c = 0; c < AllocNum; ++c) {
			if (c < PosFinds.Length() && c == PosFinds[CurPosFind])
			{
				NewData[c - CurOff] = Write;
				CurOff += DifLen;
				++CurPosFind;
			}
			NewData[c - CurOff] = Data[c];
		}
		if (AllocNum > 0) delete[] Data;
		Data = NewData;
		AllocNum = NewLen;
		return PosFinds.Length();
	}
	SizeL String::Replace(const char FindStr, const char Write) {
		SizeL Rtn = 0;
		for (SizeL c = 0; c < AllocNum; ++c) {
			if (Data[c] == FindStr)
			{
				++Rtn;
				Data[c] = Write;
			}
		}
		return Rtn;
	}
	const String String::SubStr(SizeL Start, SizeL Stop, SnzL Step) const{
		if (AllocNum == 0) return "";
		if ((Stop > AllocNum) && Stop != MAX_INT) Stop = AllocNum;
		if (Start > AllocNum) Start = AllocNum - 1;
		if (Step < 0)
		{
			bool StopMax = false;
			if (Start >= AllocNum) Start = AllocNum - 1;
			if (Stop == MAX_INT)
			{
				Stop = 0;
				StopMax = true;
			}
			else if (Start <= Stop) return "";
			SizeL Range = Start - Stop;
			SizeL Step0 = -Step;
			String Rtn(char(0), ((Start - Stop) + (StopMax ? Step0 : Step0 - 1)) / Step0);
			SizeL c = 0;
			for (char &Ch : Rtn){
				Ch = Data[Start - c * Step0];
				++c;
			}
			return Rtn;
		}
		else
		{
			if (Stop > AllocNum) Stop = AllocNum;
			if (Start >= Stop) return "";
			SizeL Step1 = Step + 1;
			SizeL c1 = 0;
			String Rtn(char(0), (Stop - Start - 1) / Step1 + 1);//Error in this line
			for (SizeL c = Start; c < Stop; c += Step1, ++c1){
				Rtn[c1] = Data[c];
			}
			return Rtn;
		}
	}
	char &String::AtEnd(){
		return Data[AllocNum - 1];
	}
	void String::ToLower(){
		for (SizeL c = 0; c < AllocNum; ++c){
			if (Data[c] >= 'A' && Data[c] <= 'Z') Data[c] += 32;
		}
	}
	void String::ToUpper(){
		for (SizeL c = 0; c < AllocNum; ++c){
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
		for (SizeL c = 0; c < AllocNum; ++c){
			if (Data[c] <= '9' && Data[c] >= '0') continue;
			else if (Hex && ((Data[c] <= 'f' && Data[c] >= 'a') || (Data[c] <= 'F' && Data[c] >= 'A'))) continue;
			else return false;
		}
		return true;
	}
	bool String::IsAlpha() const{
		for (SizeL c = 0; c < AllocNum; ++c){
			if ((Data[c] <= 'Z' && Data[c] >= 'A') || (Data[c] <= 'z' && Data[c] >= 'a')) continue;
			else return false;
		}
		return true;
	}
	bool String::IsAlnum() const{
		for (SizeL c = 0; c < AllocNum; ++c){
			if ((Data[c] <= 'Z' && Data[c] >= 'A') || (Data[c] <= 'z' && Data[c] >= 'a')) continue;
			else if (Data[c] <= '9' && Data[c] >= '0') continue;
			else return false;
		}
		return true;
	}
	String &String::operator*=(SizeL Num){
		char *NewData = new char[AllocNum * Num];
		for (SizeL c = 0; c < Num; ++c){
			for (SizeL c1 = 0; c1 < AllocNum; ++c1){
				NewData[(c * AllocNum) + c1] = Data[c1];
			}
		}
		AllocNum *= Num;
		delete[] Data;
		Data = NewData;
		return (*this);
	}
	String String::operator*(SizeL Num) const{
		String Rtn;
		Rtn.Data = new char[AllocNum * Num];
		for (SizeL c = 0; c < Num; ++c){
			for (SizeL c1 = 0; c1 < AllocNum; ++c1){
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
	String::Iterator::Iterator(String *Iter, SizeL CharPos){
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
		SizeL c = 0;
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
		SizeL c = 0;
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
	wString::wString(const char *StrIn, SizeL Len){
		AllocNum = Len;
		SizeL c = 0;
		Data = new wchar_t[AllocNum];
		while (c < AllocNum){
			Data[c] = StrIn[c];
			++c;
		}
	}
	wString::wString(const wchar_t *StrIn, SizeL Len){
		AllocNum = Len;
		SizeL c = 0;
		Data = new wchar_t[AllocNum];
		while (c < AllocNum){
			Data[c] = StrIn[c];
			++c;
		}
	}
	wString::wString(const wString &StrCp){
		AllocNum = StrCp.AllocNum;
		SizeL c = 0;
		Data = new wchar_t[AllocNum];
		while (c < AllocNum){
			Data[c] = StrCp.Data[c];
			++c;
		}
	}
	wString::wString(const wchar_t ChFill, const SizeL Len){
		AllocNum = Len;
		SizeL c = 0;
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
		SizeL c = 0;
		while (c < AllocNum){
			if (Data[c] != Cmp.Data[c]) return false;
			++c;
		}
		return true;
	}
	bool wString::operator!=(const wString &Cmp) const{
		if (AllocNum != Cmp.AllocNum) return true;
		SizeL c = 0;
		while (c < AllocNum){
			if (Data[c] != Cmp.Data[c]) return true;
			++c;
		}
		return false;
	}
	bool wString::operator<(const wString &Cmp) const{
		bool FinalRtn = AllocNum < Cmp.AllocNum;
		SizeL Shortest = FinalRtn ? AllocNum : Cmp.AllocNum;
		for (SizeL c = 0; c < Shortest; ++c){
			if (Data[c] != Cmp.Data[c]) return Data[c] < Cmp.Data[c];
		}
		return FinalRtn;
	}
	bool wString::operator<=(const wString &Cmp) const{
		bool FinalRtn = AllocNum <= Cmp.AllocNum;
		SizeL Shortest = FinalRtn ? AllocNum : Cmp.AllocNum;
		for (SizeL c = 0; c < Shortest; ++c){
			if (Data[c] != Cmp.Data[c]) return Data[c] < Cmp.Data[c];
		}
		return FinalRtn;
	}
	bool wString::operator>(const wString &Cmp) const{
		bool FinalRtn = AllocNum > Cmp.AllocNum;
		SizeL Shortest = FinalRtn ? Cmp.AllocNum : AllocNum;
		for (SizeL c = 0; c < Shortest; ++c){
			if (Data[c] != Cmp.Data[c]) return Data[c] > Cmp.Data[c];
		}
		return FinalRtn;
	}
	bool wString::operator>=(const wString &Cmp) const{
		bool FinalRtn = AllocNum >= Cmp.AllocNum;
		SizeL Shortest = FinalRtn ? Cmp.AllocNum : AllocNum;
		for (SizeL c = 0; c < Shortest; ++c){
			if (Data[c] != Cmp.Data[c]) return Data[c] > Cmp.Data[c];
		}
		return FinalRtn;
	}
	wString wString::operator+(const wString &Add) const{
		wString Rtn(*this);
		return Rtn.operator+=(Add);
	}
	wchar_t wString::operator[](const SizeL Pos) const{
		if (Pos < AllocNum) return Data[Pos];
		else return 0;
	}
	wString &wString::operator=(const wString &Cp){
		if (Data != 0) delete[] Data;
		AllocNum = Cp.AllocNum;
		SizeL c = 0;
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
		SizeL c = 0;
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
		SizeL c = 0;
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
	wchar_t &wString::operator[](const SizeL Pos){
		return Data[Pos];
	}
	bool wString::Contains(const wchar_t Val) const{
		SizeL c = 0;
		while (c < AllocNum){
			if (Data[c] == Val) return true;
			++c;
		}
		return false;
	}
	SizeL wString::GetNumCh(const wchar_t Val) const{
		SizeL c = 0;
		SizeL Rtn = 0;
		while (c < AllocNum){
			if (Data[c] == Val) ++Rtn;
			++c;
		}
		return Rtn;
	}
	SizeL wString::Length() const{
		return AllocNum;
	}
	wchar_t *wString::GetCString() const{
		wchar_t *Rtn = new wchar_t[AllocNum + 1];
		SizeL c = 0;
		while (c < AllocNum){
			Rtn[c] = Data[c];
			++c;
		}
		Rtn[c] = 0;
		return Rtn;
	}
	bool wString::Insert(SizeL Pos, wchar_t Val){
		if (Pos > AllocNum) return false;
		SizeL c = 0;
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
	bool wString::Insert(SizeL Pos, wString &Str){
		if (Pos > AllocNum) return false;
		SizeL c = 0;
		AllocNum += Str.AllocNum;
		wchar_t *NewData = new wchar_t[AllocNum];
		while (c < Pos){
			NewData[c] = Data[c];
			++c;
		}
		SizeL Pos2 = Pos + Str.AllocNum;
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
	bool wString::Remove(SizeL Pos, SizeL Num){
		if (Pos >= AllocNum) return false;
		SizeL c = 0;
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
	bool wString::Find(SizeL &Pos, wchar_t Ch, bool PosIsStart) const{
		for (SizeL c = PosIsStart ? Pos : 0; c < AllocNum; ++c){
			if (Data[c] == Ch)
			{
				Pos = c;
				return true;
			}
		}
		return false;
	}
	bool wString::RFind(SizeL &Pos, wchar_t Ch, bool PosIsStart){
		--Data;//to shift Data so that (Old)Data[0] is the same as (New)Data[1]
		SizeL Until = PosIsStart ? Pos : 0;
		for (SizeL c = AllocNum; c > Until; --c){
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
	bool wString::Find(SizeL &Pos, wString Ch, bool PosIsStart) const{
		if (Ch.AllocNum == 0) return false;
		SizeL MidPos = 0;
		for (SizeL c = PosIsStart ? Pos : 0; c < AllocNum; ++c){
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
	bool wString::RFind(SizeL &Pos, wString Ch, bool PosIsStart){
		if (Ch.AllocNum == 0) return false;
		--Data;//to shift Data so that (Old)Data[0] is the same as (New)Data[1]
		--Ch.Data;
		SizeL MidPos = Ch.AllocNum;
		SizeL Until = PosIsStart ? Pos : 0;
		for (SizeL c = AllocNum; c > Until; --c){
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
	SizeL wString::Replace(const wString &FindStr, const wString &Write) {
		if (FindStr.Length() == 0) return 0;
		if (FindStr.Length() == 1 && Write.Length() == 1) return Replace(FindStr[0], Write[0]);
		Array<SizeL> PosFinds;
		SizeL Pos = 0;
		while (Find(Pos, FindStr, true)) {
			PosFinds += Pos;
		}
		if (PosFinds.Length() == 0) return 0;
		wchar_t *NewData = 0;
		SizeL NewLen = 0;
		if (FindStr.Length() > Write.Length()) {
			SizeL DifLen = FindStr.Length() - Write.Length();
			NewLen = AllocNum - PosFinds.Length() * DifLen;
			NewData = new wchar_t[NewLen];
			SizeL CurPosFind = 0;
			SizeL CurOff = 0;
			for (SizeL c = 0; c < AllocNum; ++c) {
				if (c < PosFinds.Length() && c == PosFinds[CurPosFind])
				{
					for (SizeL c0 = 0; c0 < Write.Length(); ++c, ++c0) {
						NewData[c - CurOff] = Write[c0];
					}
					CurOff += DifLen;
					++CurPosFind;
				}
				NewData[c - CurOff] = Data[c];
			}
		}
		else
		{
			SizeL DifLen = Write.Length() - FindStr.Length();
			NewLen = AllocNum + PosFinds.Length() * DifLen;
			NewData = new wchar_t[NewLen];
			SizeL CurPosFind = 0;
			SizeL CurOff = 0;
			for (SizeL c = 0; c < AllocNum; ++c) {
				if (c < PosFinds.Length() && c == PosFinds[CurPosFind])
				{
					for (SizeL c0 = 0; c0 < Write.Length(); ++c0) {
						NewData[c + CurOff + c0] = Write[c0];
					}
					CurOff += DifLen;
					++CurPosFind;
				}
				NewData[c + CurOff] = Data[c];
			}
		}
		if (AllocNum > 0) delete [] Data;
		Data = NewData;
		AllocNum = NewLen;
		return PosFinds.Length();// return the number of replacements we did
	}
	SizeL wString::Replace(const wchar_t FindStr, const wString &Write) {
		if (Write.Length() == 1) return Replace(FindStr, Write[0]);
		Array<SizeL> PosFinds;
		SizeL Pos = 0;
		while (Find(Pos, FindStr, true)) {
			PosFinds += Pos;
		}
		if (PosFinds.Length() == 0) return 0;
		SizeL DifLen = Write.Length() - 1;
		SizeL NewLen = AllocNum + PosFinds.Length() * DifLen;
		wchar_t *NewData = new wchar_t[NewLen];
		SizeL CurPosFind = 0;
		SizeL CurOff = 0;
		for (SizeL c = 0; c < AllocNum; ++c) {
			if (c < PosFinds.Length() && c == PosFinds[CurPosFind])
			{
				for (SizeL c0 = 0; c0 < Write.Length(); ++c0) {
					NewData[c + CurOff + c0] = Write[c0];
				}
				CurOff += DifLen;
				++CurPosFind;
			}
			NewData[c + CurOff] = Data[c];
		}
		return PosFinds.Length();
	}
	SizeL wString::Replace(const wString &FindStr, const wchar_t Write) {
		if (FindStr.Length() == 0) return 0;
		if (FindStr.Length() == 1) return Replace(FindStr[0], Write);
		Array<SizeL> PosFinds;
		SizeL Pos = 0;
		while (Find(Pos, FindStr, true)) {
			PosFinds += Pos;
		}
		if (PosFinds.Length() == 0) return 0;
		//length of FindStr is always greater than 1 (length of Write) at this point
		SizeL DifLen = FindStr.Length() - 1;
		SizeL NewLen = AllocNum - PosFinds.Length() * DifLen;
		wchar_t *NewData = new wchar_t[NewLen];
		SizeL CurPosFind = 0;
		SizeL CurOff = 0;
		for (SizeL c = 0; c < AllocNum; ++c) {
			if (c < PosFinds.Length() && c == PosFinds[CurPosFind])
			{
				NewData[c - CurOff] = Write;
				CurOff += DifLen;
				++CurPosFind;
			}
			NewData[c - CurOff] = Data[c];
		}
		if (AllocNum > 0) delete[] Data;
		Data = NewData;
		AllocNum = NewLen;
		return PosFinds.Length();
	}
	SizeL wString::Replace(const wchar_t FindStr, const wchar_t Write) {
		SizeL Rtn = 0;
		for (SizeL c = 0; c < AllocNum; ++c) {
			if (Data[c] == FindStr)
			{
				++Rtn;
				Data[c] = Write;
			}
		}
		return Rtn;
	}
	const wString wString::SubStr(SizeL Start, SizeL Stop, SnzL Step) const{
		if (AllocNum == 0) return "";
		if ((Stop > AllocNum) && Stop != MAX_INT) Stop = AllocNum;
		if (Start > AllocNum) Start = AllocNum - 1;
		if (Step < 0)
		{
			bool StopMax = false;
			if (Start >= AllocNum) Start = AllocNum - 1;
			if (Stop == MAX_INT)
			{
				Stop = 0;
				StopMax = true;
			}
			else if (Start <= Stop) return "";
			SizeL Range = Start - Stop;
			SizeL Step0 = -Step;
			wString Rtn(wchar_t(0), ((Start - Stop) + (StopMax ? Step0 : Step0 - 1)) / Step0);
			SizeL c = 0;
			for (wchar_t &Ch : Rtn) {
				Ch = Data[Start - c * Step0];
				++c;
			}
			return Rtn;
		}
		else
		{
			if (Stop > AllocNum) Stop = AllocNum;
			if (Start >= Stop) return "";
			SizeL Step1 = Step + 1;
			SizeL c1 = 0;
			wString Rtn(wchar_t(0), (Stop - Start - 1) / Step1 + 1);//Error in this line
			for (SizeL c = 0; c < Stop; c += Step1, ++c1) {
				Rtn[c1] = Data[c];
			}
			return Rtn;
		}
	}
	wchar_t &wString::AtEnd(){
		return Data[AllocNum - 1];
	}
	void wString::ToLower(){
		for (SizeL c = 0; c < AllocNum; ++c){
			if (Data[c] >= 'A' && Data[c] <= 'Z') Data[c] += 32;
		}
	}
	void wString::ToUpper(){
		for (SizeL c = 0; c < AllocNum; ++c){
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
		for (SizeL c = 0; c < AllocNum; ++c){
			if (Data[c] <= '9' && Data[c] >= '0') continue;
			else if (Hex && ((Data[c] <= 'f' && Data[c] >= 'a') || (Data[c] <= 'F' && Data[c] >= 'A'))) continue;
			else return false;
		}
		return true;
	}
	bool wString::IsAlpha() const{
		for (SizeL c = 0; c < AllocNum; ++c){
			if ((Data[c] <= 'Z' && Data[c] >= 'A') || (Data[c] <= 'z' && Data[c] >= 'a')) continue;
			else return false;
		}
		return true;
	}
	bool wString::IsAlnum() const{
		for (SizeL c = 0; c < AllocNum; ++c){
			if ((Data[c] <= 'Z' && Data[c] >= 'A') || (Data[c] <= 'z' && Data[c] >= 'a')) continue;
			else if (Data[c] <= '9' && Data[c] >= '0') continue;
			else return false;
		}
		return true;
	}
	wString &wString::operator*=(SizeL Num){
		wchar_t *NewData = new wchar_t[AllocNum * Num];
		for (SizeL c = 0; c < Num; ++c){
			for (SizeL c1 = 0; c1 < AllocNum; ++c1){
				NewData[(c * AllocNum) + c1] = Data[c1];
			}
		}
		AllocNum *= Num;
		delete[] Data;
		Data = NewData;
		return (*this);
	}
	wString wString::operator*(SizeL Num) const{
		wString Rtn;
		Rtn.Data = new wchar_t[AllocNum * Num];
		for (SizeL c = 0; c < Num; ++c){
			for (SizeL c1 = 0; c1 < AllocNum; ++c1){
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
	wString::Iterator::Iterator(wString *Iter, SizeL CharPos){
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