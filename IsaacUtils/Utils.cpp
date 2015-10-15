#include "stdafx.h"
#include "Utils.h"
namespace Utils{
	unsigned long BlMulTm = 0, BlDivTm = 0, RandTm = 0;

	unsigned long GetMost(unsigned long a, unsigned long b){
		return (a > b) ? a : b;
	}
	unsigned long GetLeast(unsigned long a, unsigned long b){
		return (a < b) ? a : b;
	}
	bool IsGreaterThan(const Utils::Array<unsigned long> &Par1, const Utils::Array<unsigned long> &Par2){
		if (Par1.Length() > Par2.Length())
		{
			unsigned long c = Par2.Length();
			while (c < Par1.Length()){
				if (Par1[c] != 0) return true;
				++c;
			}
		}
		else
		{
			unsigned long c = Par1.Length();
			while (c < Par2.Length()){
				if (Par2[c] != 0) return false;
				++c;
			}
		}
		unsigned long c = GetLeast(Par1.Length(), Par2.Length());
		while (c > 0){
			--c;
			if (Par1[c] > Par2[c]) return true;
			else if (Par2[c] > Par1[c]) return false;
		}
		return false;
	}

	BigLong::BigLong(){
		//DtorProc = 0;
		Longs = Array<unsigned long>((unsigned long)0, 1);
		Sign = false;
	}
	BigLong::BigLong(unsigned long Ul){
		//DtorProc = 0;
		Longs = Array<unsigned long>(Ul, 1);
		Sign = false;
	}
	BigLong::BigLong(unsigned long long Ull){
		//DtorProc = 0;
		Longs = Array<unsigned long>((unsigned long)(Ull & 0x00000000FFFFFFFF), 2);
		Longs[1] = (Ull & 0xFFFFFFFF00000000) >> 32;
		Sign = false;
	}
	BigLong::BigLong(BigLong &&Cpy){
		Longs = (Array<unsigned long> &&)Cpy.Longs;
		Sign = Cpy.Sign;
	}
	BigLong::BigLong(const BigLong &Cpy){
		Longs = Cpy.Longs;
		Sign = Cpy.Sign;
	}
	BigLong::BigLong(const BigLong &Cpy, unsigned long BtCopy){
		unsigned long MaskEnd = 1 << (BtCopy % 32);
		--MaskEnd;
		unsigned long NumLongCpy = BtCopy / 32;
		Longs.SetLength(NumLongCpy);
		for (unsigned long c = 0; c < NumLongCpy && c < Cpy.Longs.Length(); ++c){
			Longs[c] = Cpy.Longs[c];
		}
		if (MaskEnd > 0 && NumLongCpy < Cpy.Longs.Length())
			Longs += Cpy.Longs[NumLongCpy] & MaskEnd;
		Sign = Cpy.Sign;
	}
	void BigLong::IMulPow(unsigned long Num){
		Longs.AddBeg(Num, 0);
	}
	BigLong BigLong::MulPow(unsigned long Num){
		BigLong Rtn = *this;
		Rtn.Longs.AddBeg(Num, 0);
		return Rtn;
	}
	unsigned long BigLong::RemNulls(){
		unsigned long LenNoNull = Longs.Length();
		while (LenNoNull > 0){
			--LenNoNull;
			if (Longs[LenNoNull] != 0) break;
		}
		++LenNoNull;
		if (LenNoNull < Longs.Length()) Longs = Array<unsigned long>(Longs, LenNoNull);
		return LenNoNull;
	}
	Array<unsigned long> &BigLong::GetLongs(){
		return Longs;
	}
	Byte &BigLong::GetByte(unsigned long Pos){
		if (IsBigEnd) return ((Byte*)Longs.GetData())[Pos];
		return ((Byte *)(&Longs.GetData()[Pos / 4]))[4 - (Pos % 4)];
	}
	bool BigLong::GetBit(unsigned long long Pos){
		unsigned long BytePos = Pos / 8;
		Byte Mask = 1 << (Pos % 8), RtnMid = IsBigEnd ? ((Byte*)Longs.GetData())[Pos] : ((Byte *)(&Longs.GetData()[Pos / 4]))[4 - (Pos % 4)];
		return (RtnMid & Mask) > 0;
	}
	void BigLong::SetBit(unsigned long long Pos, bool Set){
		unsigned long BytePos = Pos / 8;
		Byte Mask = 1 << (Pos % 8);
		Byte &ByteMod = IsBigEnd ? ((Byte*)Longs.GetData())[Pos] : ((Byte *)(&Longs.GetData()[Pos / 4]))[4 - (Pos % 4)];
		if (Set) ByteMod |= Mask;
		else
		{
			Mask = ~Mask;
			ByteMod &= Mask;
		}
	}
	BigLong BigLong::PostInc(unsigned long Num){
		BigLong Rtn = *this;
		unsigned long long Mid = Num;
		for (unsigned long &Long : Longs){
			Mid += Long;
			Long = 0x00000000FFFFFFFF & Mid;
			if (Mid & 0xFFFFFFFF00000000 == 0) break;
			Mid >>= 32;
		}
		return Rtn;
	}
	BigLong &BigLong::PreInc(unsigned long Num){
		unsigned long long Mid = Num;
		for (unsigned long &Long : Longs){
			Mid += Long;
			Long = 0x00000000FFFFFFFF & Mid;
			if (Mid & 0xFFFFFFFF00000000 == 0) break;
			Mid >>= 32;
		}
		return (*this);
	}
	BigLong BigLong::PostDec(unsigned long Num){
		BigLong Rtn = *this;
		if (Longs.Length() == 1)
		{
			if (Longs[0] < Num)
			{
				Longs[0] = Num - Longs[0];
				Sign = !Sign;
			}
			else Longs[0] -= Num;
		}
		else if (Longs.Length() == 0)
		{
			Sign = !Sign;
			Longs[0] = Num;
		}
		else
		{
			bool IsBorrow = Num > Longs[0];
			unsigned long c = IsBorrow ? 1 : 0;
			while (IsBorrow){
				if (Longs[c] != 0) IsBorrow = false;
				else ++c;
			}
			if (c > 0)
			{
				unsigned long LeftOver = Longs[0];
				--Longs[c];
				while (c > 0){
					Longs[c - 1] = 0xFFFFFFFF;
					--c;
				}
				Longs[0] -= Num - 1;
				Longs[0] += LeftOver;
			}
			else Longs[0] -= Num;
		}
		return Rtn;
	}
	BigLong &BigLong::PreDec(unsigned long Num){
		if (Longs.Length() == 1)
		{
			if (Longs[0] < Num)
			{
				Longs[0] = Num - Longs[0];
				Sign = !Sign;
			}
			else Longs[0] -= Num;
		}
		else if (Longs.Length() == 0)
		{
			Sign = !Sign;
			Longs[0] = Num;
		}
		else
		{
			bool IsBorrow = Num > Longs[0];
			unsigned long c = IsBorrow ? 1 : 0;
			while (IsBorrow){
				if (Longs[c] != 0) IsBorrow = false;
				else ++c;
			}
			if (c > 0)
			{
				unsigned long LeftOver = Longs[0];
				--Longs[c];
				while (c > 0){
					Longs[c - 1] = 0xFFFFFFFF;
					--c;
				}
				Longs[0] -= Num - 1;
				Longs[0] += LeftOver;
			}
			else Longs[0] -= Num;
		}
		return *this;
	}
	bool BigLong::FromwStr(wString &wStr, Byte Radix){
		unsigned long c = wStr.Length();
		BigLong Power = (unsigned long)1;
		while (c > 0){
			--c;
			if ((c == 0) && (wStr[0] == '-'))
			{
				Sign = true;
				return true;
			}
			if ((wStr[c] >= '0') && (wStr[c] <= (Radix < 10) ? '0' + Radix - 1 : '9'))
				(*this) += Power * (unsigned long)(wStr[c] - '0');
			else if (Radix <= 36)
			{
				if ((wStr[c] >= 'a') && (wStr[c] <= (Radix < 36) ? 'a' + (Radix - 10) - 1 : 'z'))
					(*this) += Power * (unsigned long)((wStr[c] - 'a') + 10);
				else if ((wStr[c] >= 'A') && (wStr[c] <= (Radix < 36) ? 'A' + (Radix - 10) - 1 : 'Z'))
					(*this) += Power * (unsigned long)((wStr[c] - 'A') + 10);
				else return false;
			}
			else if (Radix <= 64)
			{
				if ((wStr[c] >= 'a') && (wStr[c] <= (Radix < 36) ? 'a' + (Radix - 10) - 1 : 'z'))
					(*this) += Power * (unsigned long)((wStr[c] - 'a') + 10);
				else if ((wStr[c] >= 'A') && (wStr[c] <= (Radix < 62) ? 'A' + (Radix - 36) - 1 : 'Z'))
					(*this) += Power * (unsigned long)((wStr[c] - 'A') + 36);
				else if ((wStr[c] == '_') && (Radix >= 63))
					(*this) += Power * (unsigned long)62;
				else if ((wStr[c] == '$') && (Radix >= 64))
					(*this) += Power * (unsigned long)63;
				else return false;
			}
			Power *= (unsigned long)Radix;
			Power.RemNulls();
		}
		RemNulls();
		return true;
	}
	bool BigLong::FromStr(String &Str, Byte Radix){
		unsigned long c = Str.Length();
		BigLong Power = (unsigned long)1;
		while (c > 0){
			--c;
			if ((c == 0) && (Str[0] == '-'))
			{
				Sign = true;
				return true;
			}
			if ((Str[c] >= '0') && (Str[c] <= (Radix < 10) ? '0' + Radix - 1 : '9'))
				(*this) += Power * (unsigned long)(Str[c] - '0');
			else if (Radix <= 36)
			{
				if ((Str[c] >= 'a') && (Str[c] <= (Radix < 36) ? 'a' + (Radix - 10) - 1 : 'z'))
					(*this) += Power * (unsigned long)((Str[c] - 'a') + 10);
				else if ((Str[c] >= 'A') && (Str[c] <= (Radix < 36) ? 'A' + (Radix - 10) - 1 : 'Z'))
					(*this) += Power * (unsigned long)((Str[c] - 'A') + 10);
				else return false;
			}
			else if (Radix <= 64)
			{
				if ((Str[c] >= 'a') && (Str[c] <= (Radix < 36) ? 'a' + (Radix - 10) - 1 : 'z'))
					(*this) += Power * (unsigned long)((Str[c] - 'a') + 10);
				else if ((Str[c] >= 'A') && (Str[c] <= (Radix < 62) ? 'A' + (Radix - 36) - 1 : 'Z'))
					(*this) += Power * (unsigned long)((Str[c] - 'A') + 36);
				else if ((Str[c] == '_') && (Radix >= 63))
					(*this) += Power * (unsigned long)62;
				else if ((Str[c] == '$') && (Radix >= 64))
					(*this) += Power * (unsigned long)63;
				else return false;
			}
			Power *= (unsigned long)Radix;
			Power.RemNulls();
		}
		RemNulls();
		return true;
	}
	void BigLong::TowStr(wString &wStr, Byte Radix){
		if (Radix == 0) return;
		BigLong Tmp = (*this);
		char Digits[] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_$";
		wStr = "";
		while (Tmp > (unsigned long)0){
			BigLong *Val = Tmp.DivRem((unsigned long)Radix);
			Tmp -= Val[1];
			wStr.Insert(0, Digits[Val[1].GetByte(0)]);
			Tmp = Val[0];
			delete[] Val;
		}
		if (Sign) wStr.Insert(0, '-');
	}
	void BigLong::ToStr(String &Str, Byte Radix){
		if (Radix == 0) return;
		BigLong Tmp = (*this);
		char Digits[] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_$";
		Str = "";
		while (Tmp > (unsigned long)0){
			BigLong *Val = Tmp.DivRem((unsigned long)Radix);
			Tmp -= Val[1];
			Str.Insert(0, Digits[Val[1].GetByte(0)]);
			Tmp = Val[0];
			delete[] Val;
		}
		if (Sign) Str.Insert(0, '-');
	}
	bool BigLong::IsPow2() const{
		if (Longs.Length() == 0) return false;
		unsigned long long BitLen = BitLength();
		unsigned long LongEnd = 1 << (BitLen % 32);
		if (Longs.AtEnd() != LongEnd) return false;
		for (unsigned long c = 0; c < (Longs.Length() - 1); ++c){
			if (Longs[c] != 0) return false;
		}
		return true;
	}
	void BigLong::Minus(){
		Sign = !Sign;
	}
	BigLong BigLong::Negate() const{
		BigLong Rtn = *this;
		Rtn.Minus();
		return Rtn;
	}
	BigLong Pow(BigLong Base, BigLong Exp, BigLong Mod){
		if (Mod.Zero() != 1)
		{
			BigLong Rtn = One;
			Base %= Mod;
			while (Exp.Zero() > 1){
				if ((Exp.GetLongs()[0] % 2) == 1) Rtn = (Rtn * Base) % Mod;
				Exp >>= 1;
				Base = (Base * Base) % Mod;
			}
			return Rtn;
		}
		BigLong Rtn = One;
		if (Exp.Zero() == 0) Exp.Minus();
		while (Exp.Zero() > 1){
			Rtn *= Base;
			Exp.PreDec(1);
		}
		return Rtn;
	}
	//All Fixed :)
	// - -  yay! it's all|
	//\___/ FIXED!!      V
	BigLong* BigLong::DivRem(const BigLong &Denom) const{
		if (Denom == (unsigned long)0) return 0;
		FuncTimer Tmr(BlDivTm);
		if (Denom.IsPow2())
		{
			unsigned long ShiftNum = Denom.BitLength() - 1;
			BigLong &Rtn = *new BigLong[2];
			BigLong &Tmp = (&Rtn)[1];
			Tmp = BigLong(*this, ShiftNum);
			Rtn = *this;
			Rtn >>= ShiftNum;
			if (Denom.Sign ^ Sign)
			{
				Rtn.Sign = true;
				Tmp.Sign = true;
			}
			else
			{
				Rtn.Sign = false;
				Tmp.Sign = false;
			}
			return &Rtn;
		}
		unsigned long long Shift = 0;
		BigLong e((unsigned long)1);
		{
			unsigned long long TmpA = BitLength(), TmpB = Denom.BitLength();
			if (TmpA > TmpB)
			{
				Shift = TmpA / TmpB;
				e <<= Shift;
			}
			else if (TmpA < TmpB)
			{
				BigLong &Rtn = *(new BigLong[2]);
				BigLong &Tmp = (&Rtn)[1];
				Rtn = (unsigned long)0;
				Tmp = (*this);
				return &Rtn;
			}
		}

		BigLong d = Denom << Shift;
		while ((d < (*this)) && (d > Denom)) {
			e <<= 1;
			d <<= 1;
		}
		if ((d > (*this)) && (d > Denom) && (e > (unsigned long)1)){
			e >>= 1;
			d >>= 1;
		}
		BigLong &Rtn = *(new BigLong[2]);
		BigLong &Tmp = (&Rtn)[1];
		Tmp = (*this);
		//unsigned long long Count0 = 0, Count1 = 0;
		while (Tmp >= Denom){
			if (d <= Tmp){
				Tmp -= d;
				Rtn += e;
			}
			else
			while ((d > Tmp) && (d > Denom)){
				e >>= 1;
				d >>= 1;
				/*unsigned long long PrevC = Count0++;
				if (PrevC > Count0)
				{
				++Count1;
				}
				if (e < (unsigned long)16)
				{
				std::cout << "not the e == 1 thing!at count: ";
				if (Count1 > 0) std::cout << Count1;
				std::cout << Count0 << std::endl;
				std::cin.ignore(2);
				}*/
			}
		}
		if (Denom.Sign ^ Sign)
		{
			Rtn.Sign = true;
			Tmp.Sign = true;
		}
		else
		{
			Rtn.Sign = false;
			Tmp.Sign = false;
		}
		return &Rtn;
	}
	bool BigLong::operator==(const BigLong &Cmp) const{
		if (Sign != Cmp.Sign) return false;
		if ((Longs != Cmp.Longs) && (Longs.Length() == Cmp.Longs.Length())) return false;
		else if (Longs != Cmp.Longs)
		{
			unsigned long c = 0;
			const Array<unsigned long> &Longest = (Longs.Length() > Cmp.Longs.Length()) ? Longs : Cmp.Longs;
			unsigned long LenShort = (Longs.Length() > Cmp.Longs.Length()) ? Cmp.Longs.Length() : Longs.Length(), LenLong = Longest.Length();
			while (c < LenShort){
				if (Longs[c] != Cmp.Longs[c]) return false;
				++c;
			}
			while (c < LenLong){
				if (Longest[c] != 0) return false;
				++c;
			}
		}
		return true;
	}
	bool BigLong::operator!=(const BigLong &Cmp) const{
		if (Sign != Cmp.Sign) return true;
		if ((Longs != Cmp.Longs) && (Longs.Length() == Cmp.Longs.Length())) return true;
		else if (Longs != Cmp.Longs)
		{
			unsigned long c = 0;
			const Array<unsigned long> &Longest = (Longs.Length() > Cmp.Longs.Length()) ? Longs : Cmp.Longs;
			unsigned long LenShort = (Longs.Length() > Cmp.Longs.Length()) ? Cmp.Longs.Length() : Longs.Length(), LenLong = Longest.Length();
			while (c < LenShort){
				if (Longs[c] != Cmp.Longs[c]) return true;
				++c;
			}
			while (c < LenLong){
				if (Longest[c] != 0) return true;
				++c;
			}
		}
		return false;
	}
	bool BigLong::operator<=(const BigLong &Cmp) const{
		if ((Sign == true) && (Cmp.Sign == false)) return true;
		else if ((Cmp.Sign == true) && (Sign == false)) return false;
		else if (Sign == true) return !IsGreaterThan(Cmp.Longs, Longs);
		else return (Longs == Cmp.Longs) || IsGreaterThan(Cmp.Longs, Longs);
	}
	bool BigLong::operator>=(const BigLong &Cmp) const{
		if ((Cmp.Sign == true) && (Sign == false)) return true;
		else if ((Sign == true) && (Cmp.Sign == false)) return false;
		else if (Sign == true) return !IsGreaterThan(Longs, Cmp.Longs);
		else return !IsGreaterThan(Cmp.Longs, Longs);
	}
	bool BigLong::operator<(const BigLong &Cmp) const{
		if ((Cmp.Sign == true) && (Sign == false)) return false;
		else if ((Sign == true) && (Cmp.Sign == false)) return true;
		else if (Sign == true) return IsGreaterThan(Longs, Cmp.Longs);
		else return IsGreaterThan(Cmp.Longs, Longs);
	}
	bool BigLong::operator>(const BigLong &Cmp) const{//DUDE you messed up big time on your semantics here{}{}{}{}{}{[][][][][]} Now it's a maybe
		if ((Sign == true) && (Cmp.Sign == false)) return false;
		else if ((Cmp.Sign == true) && (Sign == false)) return true;
		else if (Sign == true) return IsGreaterThan(Cmp.Longs, Longs);
		else return IsGreaterThan(Longs, Cmp.Longs);
	}
	BigLong &BigLong::operator=(BigLong &&Cpy){
		Sign = Cpy.Sign;
		Longs.operator=((Array<unsigned long> &&)Cpy.Longs);
		return (*this);
	}
	BigLong &BigLong::operator=(const BigLong &Cpy){
		Sign = Cpy.Sign;
		Longs = Cpy.Longs;
		RemNulls();
		return (*this);
	}
	unsigned long long BigLong::BitLength() const{
		unsigned long long Rtn = Longs.Length();
		Rtn *= 32;
		unsigned long Last = Longs[Longs.Length() - 1];
		unsigned long Cmp = 0x80000000;
		while (Cmp > 0){
			if (Last & Cmp) break;
			--Rtn;
			Cmp >>= 1;
		}
		return Rtn;
	}
	BigLong BigLong::operator*(BigLong Num) const{
		FuncTimer Tmr(BlMulTm);
		BigLong Rtn = KarMul(*this, Num);
		Rtn.Sign = Sign ^ Num.Sign;
		return Rtn;
	}
	BigLong &BigLong::operator*=(BigLong Num){
		(*this) = this->operator*(Num);
		RemNulls();
		return (*this);
	}

	BigLong BigLong::operator+(const BigLong Add) const{
		BigLong Rtn;
		if (Sign == Add.Sign)
		{
			unsigned long Next = 0;
			unsigned long c = 0, Len = GetLeast(Longs.Length(), Add.Longs.Length());
			Rtn.Longs += 0;
			Rtn.Longs *= GetMost(Longs.Length(), Add.Longs.Length());
			unsigned long MostLen = Rtn.Longs.Length();
			Rtn.Longs += 0;
			bool ThisIsMost = Longs.Length() > Add.Longs.Length();
			while (c < Len){
				unsigned long long Tmp = Longs[c];
				Tmp += Add.Longs[c];
				Rtn.Longs[c] += Tmp & 0x00000000FFFFFFFF;
				Rtn.Longs[c + 1] = (Tmp & 0xFFFFFFFF00000000) >> 32;
				++c;
			}
			if (ThisIsMost)
			{
				while (c < MostLen){
					unsigned long long Tmp = Longs[c];
					Rtn.Longs[c] += Tmp & 0x00000000FFFFFFFF;
					Rtn.Longs[c + 1] = (Tmp & 0xFFFFFFFF00000000) >> 32;
					++c;
				}
			}
			else
			{
				while (c < MostLen){
					unsigned long long Tmp = Add.Longs[c];
					Rtn.Longs[c] += Tmp & 0x00000000FFFFFFFF;
					Rtn.Longs[c + 1] = (Tmp & 0xFFFFFFFF00000000) >> 32;
					++c;
				}
			}
			Rtn.Sign = Sign;
		}
		else
		{
			bool ThisMost = IsGreaterThan(Longs, Add.Longs);
			if (Add.Sign) Rtn.Sign = !ThisMost;
			else Rtn.Sign = ThisMost;
			const Array<unsigned long> &MostLongs = ThisMost ? Longs : Add.Longs;
			const Array<unsigned long> &LeastLongs = ThisMost ? Add.Longs : Longs;
			Rtn.Longs += 0;
			Rtn.Longs *= MostLongs.Length();
			unsigned long c = MostLongs.Length();
			while (c > 0){
				--c;
				Rtn.Longs[c] = MostLongs[c];
				if (c >= LeastLongs.Length()) continue;
				else
				{
					if (/*(*/Rtn.Longs[c] < LeastLongs[c]/*) && ((c + 1) < MostLongs.Length())*/)// since MostLongs always has a greater absolute value
					{
						unsigned long long Tmp = Rtn.Longs[c];
						Tmp += 0x0000000100000000;
						--Rtn.Longs[c + 1];
						Rtn.Longs[c] = Tmp - LeastLongs[c];
					}
					else Rtn.Longs[c] -= LeastLongs[c];
				}
			}
		}
		return Rtn;
	}
	BigLong &BigLong::operator+=(const BigLong Add){
		(*this) = this->operator+(Add);
		RemNulls();
		return (*this);
	}
	BigLong BigLong::operator-(BigLong Add) const{
		Add.Minus();
		return operator+(Add);
	}
	BigLong &BigLong::operator-=(const BigLong Num){
		(*this) = this->operator-(Num);
		RemNulls();
		return (*this);
	}
	BigLong &BigLong::operator%=(const BigLong Num){
		if (Num.IsPow2())
		{
			return *this &= (Num - One);
		}
		BigLong *Tmp = this->DivRem(Num);
		operator=(Tmp[1]);
		delete[] Tmp;
		RemNulls();
		return (*this);
	}
	BigLong BigLong::operator%(const BigLong Num) const{
		if (Num.IsPow2())
		{
			return *this & (Num - One);
		}
		BigLong *Tmp = this->DivRem(Num);
		BigLong Rtn = Tmp[1];
		delete[] Tmp;
		return Rtn;
	}
	BigLong &BigLong::operator/=(const BigLong Num){
		if (Num.IsPow2())
		{
			return *this >>= Num.BitLength();
		}
		BigLong *Tmp = this->DivRem(Num);
		operator=(Tmp[0]);
		delete[] Tmp;
		RemNulls();
		return (*this);
	}
	BigLong BigLong::operator/(const BigLong Num) const{
		if (Num.IsPow2())
		{
			return *this >> Num.BitLength();
		}
		BigLong *Tmp = this->DivRem(Num);
		BigLong Rtn = Tmp[0];
		delete[] Tmp;
		return Rtn;
	}
	BigLong BigLong::operator<<(const unsigned long Shift) const{
		unsigned long LongShift = (Shift / 32);
		unsigned short MidShift = Shift % 32;
		BigLong Rtn;
		Rtn.Longs.SetLength(Longs.Length() + 1);
		unsigned long Next = 0;
		for (unsigned long c = 0; c < Longs.Length(); ++c){
			unsigned long long Tmp = Longs[c];
			Tmp <<= MidShift;
			Rtn.Longs[c] |= (Tmp & 0x00000000FFFFFFFF);
			Rtn.Longs[c + 1] = (Tmp & 0xFFFFFFFF00000000) >> 32;
		}
		Rtn.Longs.AddBeg(LongShift, 0);
		Rtn.RemNulls();
		return Rtn;
	}
	BigLong BigLong::operator>>(const unsigned long Shift) const{
		unsigned long LongShift = (Shift / 32);
		unsigned short MidShift = Shift % 32;
		BigLong Rtn;
		if (Longs.Length() > LongShift) Rtn.Longs.SetLength(Longs.Length() - LongShift);
		else
		{
			Rtn.Longs.SetLength(1);
			Rtn.Longs[0] = 0;
			return Rtn;
		}

		unsigned long c = Longs.Length(), c0 = Longs.Length() - LongShift;
		if (c0 > 0) Rtn.Longs[c0 - 1] = 0;
		while (c > LongShift){
			--c;
			--c0;
			unsigned long long Tmp = (unsigned long long)Longs[c] << 32;
			Tmp >>= MidShift;
			Rtn.Longs[c0] |= (Tmp & 0xFFFFFFFF00000000) >> 32;
			if (c0 > 0) Rtn.Longs[c0 - 1] = (Tmp & 0x00000000FFFFFFFF);
		}
		Rtn.RemNulls();
		return Rtn;
	}
	BigLong &BigLong::operator<<=(const unsigned long Shift){
		(*this) = this->operator<<(Shift);
		return (*this);
	}
	BigLong &BigLong::operator>>=(const unsigned long Shift){
		(*this) = this->operator>>(Shift);
		return (*this);
	}
	BigLong BigLong::operator|(const BigLong Num) const{
		BigLong Rtn = *this;
		Rtn |= Num;
		return Rtn;
	}
	BigLong &BigLong::operator|=(const BigLong Num){
		Sign |= Num.Sign;
		if (Longs.Length() >= Num.Longs.Length())
		{
			unsigned long c = 0;
			unsigned long Len = Num.Longs.Length();
			while (c < Len){
				Longs[c] |= Num.Longs[c];
				++c;
			}
		}
		else
		{
			unsigned long c = 0;
			unsigned long Len = Longs.Length();
			while (c < Len){
				Longs[c] |= Num.Longs[c];
				++c;
			}
			Longs.AddMissing(Num.Longs);
		}
		return (*this);
	}
	BigLong BigLong::operator&(const BigLong Num) const{
		BigLong Rtn = *this;
		Rtn &= Num;
		return Rtn;
	}
	BigLong &BigLong::operator&=(const BigLong Num){
		Sign &= Num.Sign;
		unsigned long Len = (Longs.Length() < Num.Longs.Length()) ? Longs.Length() : Num.Longs.Length();
		if (Longs.Length() > Len) Longs.SetLength(Len);
		for (unsigned long c = 0; c < Len; ++c){
			Longs[c] &= Num.Longs[c];
		}
		return (*this);
	}
	BigLong BigLong::operator^(const BigLong Num) const{
		BigLong Rtn = *this;
		Rtn ^= Num;
		return Rtn;
	}
	BigLong &BigLong::operator^=(const BigLong Num){
		Sign ^= Num.Sign;
		if (Longs.Length() >= Num.Longs.Length())
		{
			unsigned long c = 0;
			unsigned long Len = Num.Longs.Length();
			while (c < Len){
				Longs[c] ^= Num.Longs[c];
				++c;
			}
		}
		else
		{
			unsigned long c = 0;
			unsigned long Len = Longs.Length();
			while (c < Len){
				Longs[c] ^= Num.Longs[c];
				++c;
			}
			Longs.AddMissing(Num.Longs);
		}
		return (*this);
	}

	Byte BigLong::Zero() const{
		if (Longs.Length() == 0) return 1;
		else if (BitLength() > 0) return Sign ? 0 : 2;
		return 1;
	}
	BigLong BigLong::NomMul(const BigLong &Bl1, const BigLong &Bl2){
		BigLong Sum;
		unsigned long c = 0;
		while (c < Bl1.Longs.Length()){
			BigLong CurrNum;
			CurrNum.Longs = Array<unsigned long>((unsigned long)0, Bl2.Longs.Length() + 1);
			for (unsigned long c1 = 0; c1 < Bl2.Longs.Length(); ++c1){
				unsigned long long Tmp = (unsigned long long)Bl1.Longs[c] * (unsigned long long)Bl2.Longs[c1];
				CurrNum.Longs[c1] += Tmp & 0x00000000FFFFFFFF;
				CurrNum.Longs[c1 + 1] = (Tmp & 0xFFFFFFFF00000000) >> 32;
			}
			CurrNum.Longs = Array<unsigned long>((unsigned long)0, c) + CurrNum.Longs;
			Sum = Sum + CurrNum;
			++c;
		}
		Sum.RemNulls();
		return Sum;
	}
	BigLong BigLong::KarMul(const BigLong &Bl1, const BigLong &Bl2){
		if (Bl1.Longs.Length() <= 2 || Bl2.Longs.Length() <= 2) return NomMul(Bl1, Bl2);
		else if (Bl1.Zero() == 1 || Bl2.Zero() == 1) return Utils::Zero;
		BigLong High1 = Bl1, High2 = Bl2, Low1, Low2;
		unsigned long Power = 0;
		KarSplit(High1, High2, Power, Low1, Low2);
		High1.Sign = false;
		High2.Sign = false;
		Low1.Sign = false;
		Low2.Sign = false;
		BigLong z0, z1, z2;
		z0 = (Low1.Longs.Length() <= 1 || Low2.Longs.Length() <= 1) ? NomMul(Low1, Low2) : KarMul(Low1, Low2);
		z2 = (High1.Longs.Length() <= 1 || High2.Longs.Length() <= 1) ? NomMul(High1, High2) : KarMul(High1, High2);
		z1 = KarMul((Low1 + High1), (Low2 + High2)) - (z2 + z0);
		return z2.MulPow(2 * Power) + z1.MulPow(Power) + z0;
	}
	void BigLong::KarSplit(BigLong &Bl1, BigLong &Bl2, unsigned long &Power, BigLong &BlRem1, BigLong &BlRem2){
		Power = ((Bl1.Longs.Length() > Bl2.Longs.Length()) ? Bl1.Longs.Length() : Bl2.Longs.Length()) / 2;
		BlRem1.Longs.SetLength(0);
		BlRem2.Longs.SetLength(0);
		BlRem1.Longs.AddMissing(Bl1.Longs, Power < Bl1.Longs.Length() ? Power : Bl1.Longs.Length());
		BlRem2.Longs.AddMissing(Bl2.Longs, Power < Bl2.Longs.Length() ? Power : Bl2.Longs.Length());
		Bl1.Longs.RemBeg(BlRem1.Longs.Length());
		Bl2.Longs.RemBeg(BlRem2.Longs.Length());
		Bl1.RemNulls();
		Bl2.RemNulls();
		BlRem1.RemNulls();
		BlRem2.RemNulls();
	}


	
	Array<Clock *> Clock::Clks;
	Clock::Clock(unsigned long ClkId){
		CurClk = 0;
		Id = ClkId;
		NumTimes = 0;
		TotTime = 0;
	}
	Clock::Clock(const Clock &Clk){
		CurClk = 0;
		Id = Clk.Id;
		NumTimes = Clk.NumTimes;
		TotTime = Clk.TotTime;
	}
	Clock &Clock::operator=(const Clock &Cpy){
		Id = Cpy.Id;
		NumTimes = Cpy.NumTimes;
		TotTime = Cpy.TotTime;
		return (*this);
	}
	Clock *Clock::GetNewClock(){
		Clks += new Clock(Clks.Length());
		return Clks.AtEnd();
	}
	Clock *Clock::GetClock(unsigned long Id){
		if (Id < Clks.Length()) return Clks[Id];
		else return 0;
	}
	Clock::~Clock(){
		Clks.Remove(Id);
		for (unsigned long c = Id; c < Clks.Length(); ++c){
			Clks[c]->Id = c;
		}
	}

	unsigned long Clock::GetId(){
		return Id;
	}
	unsigned long Clock::GetNumTimes(){
		return NumTimes;
	}
	double Clock::GetAvgTime(){
		return TotTime / NumTimes;
	}
	double Clock::GetTotalTime(){
		return TotTime;
	}
	FuncTimer::FuncTimer(unsigned long ClkId){
		Clk = Clock::GetClock(ClkId);
		Clk->StartTime();
	}

	FuncTimer::~FuncTimer(){
		Clk->EndTime();
	}

	BigLong RandNoMultiple(Random * Rnd, Array<BigLong> &Against, BigLong a, BigLong b, unsigned short NumQuit){
		FuncTimer Tmr(RandTm);
		if (NumQuit == 0)
		{
			if (Against.Length() > 4) NumQuit = 2 * Against.Length();
			else NumQuit = 8;
		}
		unsigned long Len = Against.Length();
		for (unsigned short c = 0; c < NumQuit; ++c){
			BigLong Rtn = Rnd->GetRand(a, b);
			bool Cont = false;
			for (BigLong &Long : Against){
				if ((Rtn % Long).Zero() == 1)
				{
					Cont = true;
					break;
				}
			}
			if (Cont) continue;
			else return Rtn;
		}
		return (unsigned long)0;
	}

	bool FermatBaseTest(Random *Rnd, const BigLong &Num, unsigned long NumTest){
		if (((Num - One) % Six != Zero) && ((Num + One) % Six != Zero)) return false;
		Array<BigLong> LstBases;
		for (unsigned long c = 0; c < NumTest; ++c){
			BigLong Base = RandNoMultiple(Rnd, LstBases, Two, Num - Two);
			if (Pow(Base, Num - One, Num) != One) return false;
			LstBases += Base;
		}
		return true;
	}

	BigLong GetRandPrimeProb(Random *Rnd, bool(*Test)(Random *, const BigLong &, unsigned long), unsigned long BitLen, unsigned long NumTimes){
		BigLong LowerK = (unsigned long)1;
		LowerK <<= BitLen - 1;
		LowerK += (unsigned long)1;
		LowerK /= (unsigned long)6;
		BigLong UpperK = LowerK << (unsigned long)1;
		UpperK -= (unsigned long)1;
		UpperK /= (unsigned long)6;
		while (true){
			BigLong Rtn0 = Rnd->GetRand(LowerK, UpperK);
			bool First, Second;
			Rtn0 *= (unsigned long)6;
			BigLong Rtn1 = Rtn0 + (unsigned long)1;
			Rtn0 -= (unsigned long)1;
			First = Test(Rnd, Rtn0, NumTimes);
			Second = Test(Rnd, Rtn1, NumTimes);
			if (First != Second) return First ? Rtn0 : Rtn1;
			else if (First == true)
			{
				if (Rnd->RandBit(1) == (unsigned long)0) return Rtn0;
				else return Rtn1;
			}
		}
	}
	BigLong GetRandPrime(Random *Rnd, bool(*Test)(Random *, const BigLong &), unsigned long BitLen){
		BigLong LowerK = (unsigned long)1;
		LowerK <<= BitLen - 1;
		LowerK += (unsigned long)1;
		LowerK /= (unsigned long)6;
		BigLong UpperK = LowerK << (unsigned long)1;
		UpperK -= (unsigned long)1;
		UpperK /= (unsigned long)6;
		while (true){
			BigLong Rtn0 = Rnd->GetRand(LowerK, UpperK);
			bool First, Second;
			Rtn0 *= (unsigned long)6;
			BigLong Rtn1 = Rtn0 + (unsigned long)1;
			Rtn0 -= (unsigned long)1;
			First = Test(Rnd, Rtn0);
			Second = Test(Rnd, Rtn1);
			if (First != Second) return First ? Rtn0 : Rtn1;
			else if (First == true)
			{
				if (Rnd->RandBit(1) == (unsigned long)0) return Rtn0;
				else return Rtn1;
			}
		}
	}

	Random::Random(){}
	Random::~Random(){}
	
	wString FromNumber(unsigned long Num, unsigned char Radix){
		char *Digits = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz$_";
		if ((Radix > 64) || (Radix == 0)) return "/ERROR: invalid radix";
		Utils::wString Rtn;
		while (Num > 0){
			Rtn += Digits[Num % Radix];
			Num /= Radix;
		}
		return Rtn;
	}
	BigLong GetNumStrTest(wString Str){
		BigLong Rtn;
		BigLong Power((unsigned long)1);
		for (wchar_t Ch : Str){
			Rtn += Power * ((unsigned long)1 + Ch);
			Power <<= 16;
		}
		return Rtn;
	}
	wString GetStrNumTest(BigLong Bl){
		if (Bl.Zero() == 1) return "";
		else if (Bl.Zero() == 0) Bl.Minus();
		wString Rtn;
		BigLong Denom = (unsigned long)256;
		while (Bl.Zero() > 1){
			Bl -= (unsigned long)1;
			BigLong *QuoMod = Bl.DivRem(Denom);
			Rtn += QuoMod[1].GetByte(0);
			Bl = QuoMod[0];
			delete[] QuoMod;
		}
		return Rtn;
	}
	BigLong GetNumStrTestB(Array<Byte> Str){
		BigLong Rtn;
		BigLong Power((unsigned long)1);
		for (wchar_t Ch : Str){
			Rtn += Power * ((unsigned long)1 + Ch);
			Power <<= 16;
		}
		return Rtn;
	}
	Array<Byte> GetStrNumTestB(BigLong Bl){
		if (Bl.Zero() == 1) return Array<Byte>();
		else if (Bl.Zero() == 0) Bl.Minus();
		Array<Byte> Rtn;
		BigLong Denom = (unsigned long)256;
		while (Bl.Zero() > 1){
			Bl -= (unsigned long)1;
			BigLong *QuoMod = Bl.DivRem(Denom);
			Rtn += QuoMod[1].GetByte(0);
			Bl = QuoMod[0];
			delete[] QuoMod;
		}
		return Rtn;
	}
	union BtuLongConv{
		unsigned long uLong;
		Byte Bts[4];
	};
	bool IsBigEnd;
	void Init(){
		BtuLongConv Test;
		Test.uLong = 0xFF000000;
		IsBigEnd = Test.Bts[3] > 0;
		OsInit();
		BlMulTm = Clock::GetNewClock()->GetId();
		BlDivTm = Clock::GetNewClock()->GetId();
		RandTm = Clock::GetNewClock()->GetId();
	}
	unsigned long wStrLen(wchar_t *wStr){
		unsigned long c = 0;
		while (wStr[c] != 0)++c;
		return c;
	}
	const Utils::BigLong Two((unsigned long)2), Six((unsigned long)6), One((unsigned long)1), Zero((unsigned long)0);
}