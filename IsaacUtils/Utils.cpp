#include "stdafx.h"
#include "Utils.h"
namespace Utils{
	Clock *BlMulTm, *BlDivTm, *RandTm;

	SizeL GetMost(SizeL a, SizeL b){
		return (a > b) ? a : b;
	}
	SizeL GetLeast(SizeL a, SizeL b){
		return (a < b) ? a : b;
	}
	bool IsGreaterThan(const Utils::Array<unsigned long> &Par1, const Utils::Array<unsigned long> &Par2){
		if (Par1.Length() > Par2.Length())
		{
			SizeL c = Par2.Length();
			while (c < Par1.Length()){
				if (Par1[c] != 0) return true;
				++c;
			}
		}
		else
		{
			SizeL c = Par1.Length();
			while (c < Par2.Length()){
				if (Par2[c] != 0) return false;
				++c;
			}
		}
		SizeL c = GetLeast(Par1.Length(), Par2.Length());
		while (c > 0){
			--c;
			if (Par1[c] > Par2[c]) return true;
			else if (Par2[c] > Par1[c]) return false;
		}
		return false;
	}

	BigLong::BigLong(){
		Longs = Array<unsigned long>((unsigned long)0, 1);
		Sign = false;
	}
	BigLong::BigLong(unsigned long Ul){
		Longs = Array<unsigned long>(Ul, 1);
		Sign = false;
	}
	BigLong::BigLong(unsigned long long Ull){
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
	BigLong::BigLong(const BigLong &Cpy, SizeL BtCopy){
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
	BigLong::BigLong(const Byte *Data, SizeL LenData) {
		Longs.SetLength((LenData + 3) / 4);
		Sign = false;
		if (LenData % 4 > 0) Longs.AtEnd() = 0;
		if (IsBigEnd)
		{
			for (SizeL c = 0; c < LenData / 4; ++c) {
				Longs[c] = ((unsigned long *)Data)[c];
			}
			for (SizeL c = LenData - LenData % 4; c < LenData; ++c) {
				((Byte*)Longs.GetData())[c] = Data[c];
			}
		}
		else
		{
			for (SizeL c = 0, c0 = 0; c < LenData / 4; ++c, c0 += 4) {
				Longs[c] = ((unsigned long)Data[c0] << 24) |
					((unsigned long)Data[c0 + 1] << 16) |
					((unsigned long)Data[c0 + 2] << 8) | Data[c0 + 3];
			}
			for (SizeL c = LenData - LenData % 4, c0 = 3; c < LenData; ++c, --c0) {
				Longs[c] |= (unsigned long)Data[c] << (8 * c0);
			}
		}
	}
	void BigLong::IMulPow(SizeL Num){
		Longs.AddBeg(Num, 0);
	}
	BigLong BigLong::MulPow(SizeL Num){
		BigLong Rtn = *this;
		Rtn.Longs.AddBeg(Num, 0);
		return Rtn;
	}
	ByteArray BigLong::ToByteArray() {
		if (Utils::IsBigEnd) return ByteArray((Byte *)Longs.GetData(), Longs.Length() * 4);
		ByteArray Rtn(Utils::Byte(0), Longs.Length() * 4);
		for (SizeL c = 0; c < Longs.Length(); ++c) {
			Rtn[c * 4 + 3] = Longs[c] & 0xFF;
			Rtn[c * 4 + 2] = (Longs[c] >> 8) & 0xFF;
			Rtn[c * 4 + 1] = (Longs[c] >> 16) & 0xFF;
			Rtn[c * 4] = (Longs[c] >> 24) & 0xFF;
		}
		return Rtn;
	}
	SizeL BigLong::RemNulls(){
		SizeL LenNoNull = Longs.Length();
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
	Byte &BigLong::GetByte(SizeL Pos){
		if (IsBigEnd) return ((Byte*)Longs.GetData())[Pos];
		return ((Byte *)(&Longs.GetData()[Pos / 4]))[3 - (Pos % 4)];
	}
	bool BigLong::GetBit(unsigned long long Pos){
		SizeL BytePos = Pos / 8;
		Byte Mask = 1 << (Pos % 8), RtnMid = IsBigEnd ? ((Byte*)Longs.GetData())[Pos] : ((Byte *)(&Longs.GetData()[Pos / 4]))[3 - (Pos % 4)];
		return (RtnMid & Mask) > 0;
	}
	void BigLong::SetBit(unsigned long long Pos, bool Set){
		SizeL BytePos = Pos / 8;
		Byte Mask = 1 << (Pos % 8);
		Byte &ByteMod = IsBigEnd ? ((Byte*)Longs.GetData())[Pos] : ((Byte *)(&Longs.GetData()[Pos / 4]))[3 - (Pos % 4)];
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
			if ((Mid & 0xFFFFFFFF00000000) == 0) break;
			Mid >>= 32;
		}
		return Rtn;
	}
	BigLong &BigLong::PreInc(unsigned long Num){
		unsigned long long Mid = Num;
		for (unsigned long &Long : Longs){
			Mid += Long;
			Long = 0x00000000FFFFFFFF & Mid;
			if ((Mid & 0xFFFFFFFF00000000) == 0) break;
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
	bool BigLong::FromwStr(const wString &wStr, Byte Radix) {
		SizeL c = wStr.Length();
		BigLong Power = (unsigned long)1;
		while (c > 0) {
			--c;
			if ((c == 0) && (wStr[0] == '-'))
			{
				Sign = true;
				return true;
			}
			if ((wStr[c] >= '0') && (wStr[c] <= (Radix < 10 ? '0' + Radix - 1 : '9')))
				(*this) += Power * (unsigned long)(wStr[c] - '0');
			else if (Radix <= 36)
			{
				if ((wStr[c] >= 'a') && (wStr[c] <= (Radix < 36 ? 'a' + (Radix - 10) - 1 : 'z')))
					(*this) += Power * (unsigned long)((wStr[c] - 'a') + 10);
				else if ((wStr[c] >= 'A') && (wStr[c] <= (Radix < 36 ? 'A' + (Radix - 10) - 1 : 'Z')))
					(*this) += Power * (unsigned long)((wStr[c] - 'A') + 10);
				else return false;
			}
			else if (Radix <= 64)
			{
				if ((wStr[c] >= 'a') && (wStr[c] <= (Radix < 36 ? 'a' + (Radix - 10) - 1 : 'z')))
					(*this) += Power * (unsigned long)((wStr[c] - 'a') + 10);
				else if ((wStr[c] >= 'A') && (wStr[c] <= (Radix < 62 ? 'A' + (Radix - 36) - 1 : 'Z')))
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
	bool BigLong::FromStr(const String &Str, Byte Radix) {
		SizeL c = Str.Length();
		BigLong Power = (unsigned long)1;
		while (c > 0) {
			--c;
			if ((c == 0) && (Str[0] == '-'))
			{
				Sign = true;
				return true;
			}
			if ((Str[c] >= '0') && (Str[c] <= (Radix < 10 ? '0' + Radix - 1 : '9')))
				(*this) += Power * (unsigned long)(Str[c] - '0');
			else if (Radix <= 36)
			{
				if ((Str[c] >= 'a') && (Str[c] <= (Radix < 36 ? 'a' + (Radix - 10) - 1 : 'z')))
					(*this) += Power * (unsigned long)((Str[c] - 'a') + 10);
				else if ((Str[c] >= 'A') && (Str[c] <= (Radix < 36 ? 'A' + (Radix - 10) - 1 : 'Z')))
					(*this) += Power * (unsigned long)((Str[c] - 'A') + 10);
				else return false;
			}
			else if (Radix <= 64)
			{
				if ((Str[c] >= 'a') && (Str[c] <= (Radix < 36 ? 'a' + (Radix - 10) - 1 : 'z')))
					(*this) += Power * (unsigned long)((Str[c] - 'a') + 10);
				else if ((Str[c] >= 'A') && (Str[c] <= (Radix < 62 ? 'A' + (Radix - 36) - 1 : 'Z')))
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
		BigLong BlRadix = (unsigned long)Radix;
		BigLong Tmp = (*this);
		char Digits[] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_$";
		wStr = "";
		while (Tmp > (unsigned long)0){
			BigLong *Val = Tmp.DivRem(BlRadix);
			wStr.Insert(0, Digits[Val[1].GetByte(0)]);
			Tmp = (BigLong &&)Val[0];
			delete[] Val;
		}
		if (Sign) wStr.Insert(0, '-');
	}
	void BigLong::ToStr(String &Str, Byte Radix){
		if (Radix == 0) return;
		BigLong BlRadix = (unsigned long)Radix;
		BigLong Tmp = (*this);
		char Digits[] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_$";
		Str = "";
		while (Tmp > (unsigned long)0){
			BigLong *Val = Tmp.DivRem(BlRadix);
			Str.Insert(0, Digits[Val[1].GetByte(0)]);
			Tmp = (BigLong &&)Val[0];
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
	SizeL BigLong::ToSizeL() const {
		if (Longs.Length() == 0) return 0;
#ifdef _WIN64
		if (Longs.Length() == 1) return Longs[0];
		if (IsBigEnd) return ((SizeL *)Longs.GetData())[0];
		else
		{
			SizeL Rtn = Longs[0];
			Rtn <<= 32;
			Rtn |= Longs[1];
			return Rtn;
		}
#else
		return Longs[0];
#endif
	}
	long long BigLong::ToLL() const {
		if (Longs.Length() == 0) return 0;
		long long Rtn = 0;
		if (Longs.Length() == 1) Rtn = Longs[0];
		else if (IsBigEnd) Rtn = ((long long *)Longs.GetData())[0];
		else
		{
			Rtn = Longs[0];
			Rtn <<= 32;
			Rtn |= Longs[1];
			return Rtn;
		}
		if (Sign)
		{
			Rtn -= 1;
			Rtn &= ~0x8000000000000000;
			Rtn = ~Rtn;
		}
		else Rtn &= ~0x8000000000000000;
		return Rtn;
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
	BigLong* BigLong::DivRem(const BigLong &Denom) const{
		if (Denom == (unsigned long)0) return 0;
		FuncTimer Tmr(BlDivTm);
		if (Denom.IsPow2())
		{
			SizeL ShiftNum = Denom.BitLength() - 1;
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
		while (Tmp >= Denom){
			if (d <= Tmp){
				Tmp -= d;
				Rtn += e;
			}
			else
			while ((d > Tmp) && (d > Denom)){
				e >>= 1;
				d >>= 1;
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
			SizeL c = 0;
			const Array<unsigned long> &Longest = (Longs.Length() > Cmp.Longs.Length()) ? Longs : Cmp.Longs;
			SizeL LenShort = (Longs.Length() > Cmp.Longs.Length()) ? Cmp.Longs.Length() : Longs.Length(), LenLong = Longest.Length();
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
			SizeL c = 0;
			const Array<unsigned long> &Longest = (Longs.Length() > Cmp.Longs.Length()) ? Longs : Cmp.Longs;
			SizeL LenShort = (Longs.Length() > Cmp.Longs.Length()) ? Cmp.Longs.Length() : Longs.Length(), LenLong = Longest.Length();
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
	bool BigLong::operator>(const BigLong &Cmp) const{
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
	BigLong &BigLong::IMulLim(const BigLong &MulBy, SizeL LimNum) {
		LimNum = (LimNum + 3) / 4;
		Array<unsigned long> NewLongs((unsigned long)0, LimNum);
		for (SizeL c = 0; c < LimNum; ++c) {
			unsigned long Carry = 0;
			for (SizeL c1 = 0, c0 = c; c1 < LimNum; ++c1, ++c0) {
				unsigned long long Num = Longs[c];
				Num *= MulBy.Longs[c1];
				Num += NewLongs[c0];
				Num += Carry;
				NewLongs[c0] = Num & MAX_INT32;
				Carry = Num >> 32;
			}
		}
		Sign = Sign == MulBy.Sign;
		Longs = (Array<unsigned long> &&)NewLongs;
		return *this;
	}

	BigLong BigLong::operator+(const BigLong Add) const{
		BigLong Rtn;
		if (Sign == Add.Sign)
		{
			SizeL c = 0, Len = GetLeast(Longs.Length(), Add.Longs.Length());
			Rtn.Longs += 0;
			Rtn.Longs *= GetMost(Longs.Length(), Add.Longs.Length());
			SizeL MostLen = Rtn.Longs.Length();
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
			SizeL c = MostLongs.Length();
			while (c > 0){
				--c;
				Rtn.Longs[c] = MostLongs[c];
				if (c >= LeastLongs.Length()) continue;
				else
				{
					if (Rtn.Longs[c] < LeastLongs[c])// since MostLongs always has a greater absolute value
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
	BigLong BigLong::operator<<(const SizeL Shift) const{
		SizeL LongShift = (Shift / 32);
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
	BigLong BigLong::operator>>(const SizeL Shift) const{
		SizeL LongShift = (Shift / 32);
		unsigned short MidShift = Shift % 32;
		BigLong Rtn;
		if (Longs.Length() > LongShift) Rtn.Longs.SetLength(Longs.Length() - LongShift);
		else
		{
			Rtn.Longs.SetLength(1);
			Rtn.Longs[0] = 0;
			return Rtn;
		}

		SizeL c = Longs.Length(), c0 = Longs.Length() - LongShift;
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
	BigLong &BigLong::operator<<=(const SizeL Shift){
		(*this) = this->operator<<(Shift);
		return (*this);
	}
	BigLong &BigLong::operator>>=(const SizeL Shift){
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
			SizeL c = 0;
			SizeL Len = Num.Longs.Length();
			while (c < Len){
				Longs[c] |= Num.Longs[c];
				++c;
			}
		}
		else
		{
			SizeL c = 0;
			SizeL Len = Longs.Length();
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
		SizeL Len = (Longs.Length() < Num.Longs.Length()) ? Longs.Length() : Num.Longs.Length();
		if (Longs.Length() > Len) Longs.SetLength(Len);
		for (SizeL c = 0; c < Len; ++c){
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
			SizeL c = 0;
			SizeL Len = Num.Longs.Length();
			while (c < Len){
				Longs[c] ^= Num.Longs[c];
				++c;
			}
		}
		else
		{
			SizeL c = 0;
			SizeL Len = Longs.Length();
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
		SizeL c = 0;
		while (c < Bl1.Longs.Length()){
			BigLong CurrNum;
			CurrNum.Longs = Array<unsigned long>((unsigned long)0, Bl2.Longs.Length() + 1);
			for (SizeL c1 = 0; c1 < Bl2.Longs.Length(); ++c1){
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
		SizeL Power = 0;
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
	void BigLong::KarSplit(BigLong &Bl1, BigLong &Bl2, SizeL &Power, BigLong &BlRem1, BigLong &BlRem2){
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


	
	Clock::Clock(){
		CurClk = 0;
		NumTimes = 0;
		TotTime = 0;
	}
	Clock::Clock(const Clock &Clk){
		CurClk = 0;
		NumTimes = Clk.NumTimes;
		TotTime = Clk.TotTime;
	}
	Clock &Clock::operator=(const Clock &Cpy){
		NumTimes = Cpy.NumTimes;
		TotTime = Cpy.TotTime;
		return (*this);
	}
	Clock::~Clock(){
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
	FuncTimer::FuncTimer(Clock *CurClk){
		Clk = CurClk;
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
		SizeL Len = Against.Length();
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

	BigLong GetRandPrimeProb(Random *Rnd, bool(*Test)(Random *, const BigLong &, unsigned long), SizeL BitLen, unsigned long NumTimes){
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
	BigLong GetRandPrime(Random *Rnd, bool(*Test)(Random *, const BigLong &), SizeL BitLen){
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
	ConQueue::ConQueue() {
		LastLock = GetSingleMutex();
		LastCond = GetCondVar(LastLock);
		First = 0;
		Last = 0;
		TotBytes = 0;
	}
	ByteArray ConQueue::GetBytes(SizeL NumBytes) {
		ByteArray Rtn(Byte(0), NumBytes);
		GetBytes(Rtn, NumBytes, 0);
		return Rtn;
	}
	ByteArray ConQueue::TryGetBytes(SizeL NumBytes) {
		ByteArray Rtn(Byte(0), NumBytes);
		Rtn.SetLength(TryGetBytes(Rtn, NumBytes, 0));
		return Rtn;
	}
	void ConQueue::GetBytes(ByteArray &Into, SizeL NumBytes, SizeL At) {
		QBlk *MyLast = 0;
		SizeL PrevRead = 0;
		while (true) {
			LastLock->Acquire();
			//TotBytes -= PrevRead;
			AtomicSub(TotBytes, PrevRead);
			if (GetFunc != 0 && PrevRead > 0)
			{
				try {
					GetFunc(CbObj, PrevRead);
				}
				catch (...) {
					LastLock->Release();
					throw;
				}
			}
			PrevRead = 0;
			if (NumBytes == 0) break;
			if (MyLast != Last) MyLast = Last;
			else break;//break while the lock is held to preserve the current synchronized state
			LastLock->Release();
			while (First != MyLast)
			{
				QBlk *Next = First->Next;
				if (First->Data.Length() <= NumBytes)
				{
					ByteArray Tmp((ByteArray &&)First->Data);
					Into.WriteFromAt(Tmp, At);
					SizeL CurRd = Tmp.Length();
					PrevRead += CurRd;
					NumBytes -= CurRd;
					At += CurRd;
					delete First;
					First = Next;
				}
				else
				{
					Into.WriteFromAt(First->Data.SubArr(0, NumBytes), At);
					First->Data.RemBeg(NumBytes);
					PrevRead += NumBytes;
					At += NumBytes;
					NumBytes = 0;
				}
				if (NumBytes == 0) break;
			}
		}
		while (NumBytes > 0) {
			while (!First) LastCond->wait();//If there isn't a next then wait for one
			LastLock->Release();
			if (First->Data.Length() <= NumBytes)
			{
				PrevRead = First->Data.Length();
				ByteArray Tmp((ByteArray &&)First->Data);
				Into.WriteFromAt(Tmp, At);
				NumBytes -= Tmp.Length();
				LastLock->Acquire();
				QBlk *Next = First->Next;
				delete First;
				First = Next;
				if (!First) Last = 0;
			}
			else
			{
				PrevRead = NumBytes;
				Into.WriteFromAt(First->Data.SubArr(0, NumBytes), At);
				First->Data.RemBeg(NumBytes);
				NumBytes = 0;
				LastLock->Acquire();
			}
			At += PrevRead;
			//TotBytes -= PrevRead;
			AtomicSub(TotBytes, PrevRead);
			if (GetFunc != 0 && PrevRead > 0)
			{
				try {
					GetFunc(CbObj, PrevRead);
				}
				catch (...) {
					LastLock->Release();
					throw;
				}
			}
		}
		LastLock->Release();
	}
	SizeL ConQueue::TryGetBytes(ByteArray &Into, SizeL NumBytes, SizeL At) {
		QBlk *MyLast = 0;
		SizeL PrevRead = 0;
		while (true) {
			LastLock->Acquire();
			TotBytes -= PrevRead;
			if (GetFunc != 0 && PrevRead > 0)
			{
				try {
					GetFunc(CbObj, PrevRead);
				}
				catch (...) {
					LastLock->Release();
					throw;
				}
			}
			PrevRead = 0;
			if (NumBytes == 0) break;
			if (MyLast != Last) MyLast = Last;
			else break;//break while the lock is held to preserve the current synchronized state
			LastLock->Release();
			while (First != MyLast)
			{
				QBlk *Next = First->Next;
				if (First->Data.Length() <= NumBytes)
				{
					ByteArray Tmp((ByteArray &&)First->Data);
					Into.WriteFromAt(Tmp, At);
					SizeL CurRd = Tmp.Length();
					PrevRead += CurRd;
					NumBytes -= CurRd;
					At += CurRd;
					delete First;
					First = Next;
				}
				else
				{
					Into.WriteFromAt(First->Data.SubArr(0, NumBytes), At);
					First->Data.RemBeg(NumBytes);
					PrevRead += NumBytes;
					At += NumBytes;
					NumBytes = 0;
				}
			}
		}
		if (!First) PrevRead = 0;
		else if (First->Data.Length() <= NumBytes)
		{
			PrevRead = First->Data.Length();
			ByteArray Tmp((ByteArray &&)First->Data);
			Into.WriteFromAt(Tmp, At);
			NumBytes -= Tmp.Length();
			delete First;
			First = 0;
			Last = 0;
		}
		else if (NumBytes)
		{
			PrevRead = NumBytes;
			Into.WriteFromAt(First->Data.SubArr(0, NumBytes), At);
			First->Data.RemBeg(NumBytes);
			NumBytes = 0;
		}
		else PrevRead = 0;
		TotBytes -= PrevRead;
		LastLock->Release();
		return At;
	}
	ByteArray ConQueue::PeekBytes(SizeL NumBytes) {
		ByteArray Rtn;
		QBlk *MyLast = 0;
		QBlk *PeekFirst = First;
		while (true) {
			LastLock->Acquire();
			if (MyLast != Last) MyLast = Last;
			else break;//break while the lock is held to preserve the current synchronized state
			LastLock->Release();
			while (PeekFirst != MyLast)
			{
				QBlk *Next = PeekFirst->Next;
				if (PeekFirst->Data.Length() <= NumBytes)
				{
					Rtn += PeekFirst->Data;
					NumBytes -= PeekFirst->Data.Length();
					PeekFirst = Next;
				}
				else
				{
					Rtn += PeekFirst->Data.SubArr(0, NumBytes);
					NumBytes = 0;
				}
				if (NumBytes == 0) return Rtn;
			}
		}
		while (NumBytes > 0) {
			if (PeekFirst->Data.Length() <= NumBytes)
			{
				Rtn += PeekFirst->Data;
				NumBytes -= PeekFirst->Data.Length();
			}
			else
			{
				Rtn += PeekFirst->Data.SubArr(0, NumBytes);
				NumBytes = 0;
			}
			if (NumBytes == 0) break;
			while (!PeekFirst->Next) LastCond->wait();
			PeekFirst = PeekFirst->Next;
		}
		LastLock->Release();
		return Rtn;
	}
	void ConQueue::PutBytes(const ByteArray &Bytes) {
		QBlk &Add = *new QBlk();
		Add.Data = Bytes;
		Add.Next = 0;
		LastLock->Acquire();
		//TotBytes += Add.Data.Length();
		AtomicAdd(TotBytes, Add.Data.Length());
		if (Last != 0)
		{
			Last->Next = &Add;
			Last = Last->Next;
		}
		else
		{
			Last = &Add;
			First = Last;
		}
		LastCond->notify();
		LastLock->Release();
	}
	void ConQueue::PutBytes(ByteArray &&Bytes) {
		QBlk &Add = *new QBlk();
		Add.Data = (ByteArray &&)Bytes;
		Add.Next = 0;
		LastLock->Acquire();
		//TotBytes += Add.Data.Length();
		AtomicAdd(TotBytes, Add.Data.Length());
		if (Last != 0)
		{
			Last->Next = &Add;
			Last = Last->Next;
		}
		else
		{
			Last = &Add;
			First = Last;
		}
		LastCond->notify();
		LastLock->Release();
	}
	void ConQueue::Clear(SizeL NumBytes) {
		Lock Lk(LastLock);
		if (NumBytes >= TotBytes)
		{
			while (First != Last)
			{
				QBlk *Next = First->Next;
				delete First;
				First = Next;
			}
			if (Last != 0) delete Last;
			TotBytes = 0;
			First = 0;
			Last = 0;
			return;
		}
		while (First != Last) {
			SizeL NumClr = First->Data.Length();
			if (NumBytes > NumClr)
			{
				//TotBytes -= NumClr;
				AtomicSub(TotBytes, NumClr);
				NumBytes -= NumClr;
				QBlk *Next = First->Next;
				delete First;
				First = Next;
			}
			else if (NumBytes == NumClr)
			{
				QBlk *Next = First->Next;
				delete First;
				First = Next;
				//TotBytes -= NumClr;
				AtomicSub(TotBytes, NumClr);
				break;
			}
			else
			{
				First->Data.RemBeg(NumBytes);
				//TotBytes -= NumBytes;
				AtomicSub(TotBytes, NumBytes);
				break;
			}
		}
	}
	SizeL ConQueue::Length() {
		return TotBytes;
	}
	ConQueue::~ConQueue() {
		LastCond->IsLockRef = false;
		DestroyCond(LastCond);
		QBlk *Cur = First;
		while (First) {
			Cur = First->Next;
			delete First;
			First = Cur;
		}
	}
	Lock::Lock(Mutex *Obj, bool Access) {
		LockObj = Obj;
		Attr = Access;
		LockObj->Acquire(Attr);
	}
	Lock::Lock(Lock &&Cpy) {
		LockObj = Cpy.LockObj;
		Attr = Cpy.Attr;
		Cpy.LockObj = 0;
	}
	Lock &Lock::operator=(Lock &&Cpy) {
		{
			Mutex *Tmp = LockObj;
			LockObj = Cpy.LockObj;
			Cpy.LockObj = Tmp;
		}
		bool Tmp = Attr;
		Attr = Cpy.Attr;
		Cpy.Attr = Tmp;
		return *this;
	}
	Lock::~Lock() {
		if (LockObj) LockObj->Release(Attr);
	}

	Random::Random(){}
	Random::~Random(){}
	SizeL wStringHash(wString wStr, SizeL Range) {
		register SizeL len = wStr.Length();
		register wchar_t *p;
		register SizeL x;

		p = (wchar_t *)wStr.GetData();
		x = *p << 7;
		while (len-- > 0)
			x = (1000003 * x) ^ *p++;
		x ^= wStr.Length() * 2;
		if (x == MAX_INT) x = MAX_INT - 1;
		return x % Range;
	}
	SizeL StringHash(String Str, SizeL Range) {
		register SizeL len = Str.Length();
		register unsigned char *p;
		register SizeL x;

		p = (unsigned char *)Str.GetData();
		x = *p << 7;
		while (len-- > 0)
			x = (1000003 * x) ^ *p++;
		x ^= Str.Length();
		if (x == MAX_INT) x = MAX_INT - 1;
		return x % Range;
	}
	SizeL wStringHash(const wString &wStr, SizeL Range) {
		register SizeL len = wStr.Length();
		register wchar_t *p;
		register SizeL x;

		p = (wchar_t *)wStr.GetData();
		x = *p << 7;
		while (len-- > 0)
			x = (1000003 * x) ^ *p++;
		x ^= wStr.Length() * 2;
		if (x == MAX_INT) x = MAX_INT - 1;
		return x % Range;
	}
	SizeL StringHash(const String &Str, SizeL Range) {
		register SizeL len = Str.Length();
		register unsigned char *p;
		register SizeL x;

		p = (unsigned char *)Str.GetData();
		x = *p << 7;
		while (len-- > 0)
			x = (1000003 * x) ^ *p++;
		x ^= Str.Length();
		if (x == MAX_INT) x = MAX_INT - 1;
		return x % Range;
	}
	SizeL CmpxNumHash(const SizeL &In, SizeL Range) {
		register SizeL x;
		register SizeL len = 0;
		register SizeL TmpIn = In;
		x = In & 0xFF;
		while (TmpIn > 0) {
			x = (1000003 * x) ^ (TmpIn & 0xFF);
			TmpIn >>= 8;
			++len;
		}
		x ^= len;
		if (x == MAX_INT) x = MAX_INT - 1;
		return x % Range;
	}
	SizeL NumHash(const SizeL &In, SizeL Range) {
		return In % Range;
	}
	wString FuncNames[FUNC_LAST] = { "Not a Function", "GetDrvNPath", "OpenFile", "Stat", "ListDirStats", "ListDir", "GetFileExt"};
	wString UtilsGetError() {
		return wString("In function: ") + FuncNames[ErrorFuncId] + " Error: " + LastError;
	}
	bool UtilsGetIsErr() {
		if (ErrIsRead) return false;
		ErrIsRead = true;
		return true;
	}
	void UtilsSetError(wString Err, unsigned long FuncErrId) {
		ErrIsRead = false;
		LastError = Err;
		if (FuncErrId != 0) ErrorFuncId = FuncErrId;
	}
	wString LastError;
	unsigned long ErrorFuncId = 0;
	bool ErrIsRead = true;
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
	BigLong GetNumStrTestB(ByteArray Str){
		BigLong Rtn;
		BigLong Power((unsigned long)1);
		for (wchar_t Ch : Str){
			Rtn += Power * ((unsigned long)1 + Ch);
			Power <<= 16;
		}
		return Rtn;
	}
	ByteArray GetStrNumTestB(BigLong Bl){
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
	bool IsBigEnd;
	Array<BlkCiph *> LstCiph;
	Array<MidEncSt::CiphMode> LstCiphModes;
	void Init(){
		union BtuLongConv {
			SizeL Num;
			Byte Bts[sizeof(SizeL)];
		} Test;
		Test.Num = 0;
		Test.Bts[0] = 0xFF;
		IsBigEnd = (Test.Num == 0xFF);
		fs::DriveMap.SetHashFunc(wStringHash, false);
		sock::ErrCodes.SetHashFunc(NumHash, false);
		OsInit();
		BlMulTm = new Clock();
		BlDivTm = new Clock();
		RandTm = new Clock();
		for (SizeL c = 1; c < 257; ++c) {
			for (SizeL Test = 1; Test < 257; ++Test) {
				if ((c * Test) % 257 == 1)
				{
					CipherNums[c - 1] = Test - 1;
					break;
				}
			}
		}
		LstCiph.SetLength(2);
		LstCiph[0] = &BlkCipheros;
		LstCiph[1] = &BlkCipheron;
		LstCiphModes.SetLength(1);
		LstCiphModes[0] = CmpxCBCCrypt;
	}
	void DeInit() {
		OsDeInit();
	}
	SizeL wStrLen(wchar_t *wStr){
		SizeL c = 0;
		while (wStr[c] != 0)++c;
		return c;
	}
	SizeL BtToL(const ByteArray &Bts, SizeL &Pos, SizeL Sz) {
		if (Pos >= Bts.Length()) return 0;
		SizeL Rtn = 0;
		SizeL c = sizeof(SizeL) + Pos;
		if (c > Sz) c = Sz + Pos;
		if (c > Bts.Length()) c = Bts.Length();
		while (Pos < c) {
			Rtn <<= 8;
			Rtn |= Bts[Pos];
			++Pos;
		}
		return Rtn;
	}
	SizeL BtToL(const ByteArray &Bts) {
		SizeL Rtn = 0;
		SizeL c = sizeof(SizeL);
		if (c > Bts.Length()) c = Bts.Length();
		while (c-- > 0) {
			Rtn <<= 8;
			Rtn |= Bts[c];
		}
		return Rtn;
	}
	ByteArray LToBt(SizeL Num, SizeL Align) {
		if (Align == 0)
		{
			ByteArray Rtn(Byte(0), sizeof(Num));
			for (SizeL c = 0; c < sizeof(Num); ++c) {
				if (Num == 0)
				{
					Rtn.SetLength(c);
					break;
				}
				Rtn[c] = Num & 0xFF;
				Num >>= 8;
			}
			return Rtn;
		}
		else
		{
			ByteArray Rtn(Byte(0), Align);
			while (Align-- > 0) {
				Rtn[Align] = Num & 0xFF;
				Num >>= 8;
			}
			return Rtn;
		}
	}
	void WriteLToBt(SizeL Num, ByteArray &Dest, SizeL &Pos, SizeL Align) {
		if (Align == 0)
		{
			do {
				if (Dest.Length() >= Pos) Dest += Num & 0xFF;
				else Dest[Pos] = Num & 0xFF;
				++Pos;
				Num >>= 8;
			} while (Num > 0);
		}
		else
		{
			if (Align + Pos < Dest.Length()) Dest.SetLength(Align + Pos);
			while (Align-- > 0) {
				Dest[Pos++] = Num & 0xFF;
				Num >>= 8;
			}
		}
	}
	void PackStrLen(String &Dest, SizeL &Pos, const String &StrSrc, SizeL HeadLen) {
		if (Dest.Length() < Pos + StrSrc.Length() + HeadLen) Dest.SetLength(Pos + StrSrc.Length() + HeadLen);
		WriteLToBt(StrSrc.Length(), (ByteArray &)Dest, Pos, HeadLen);
		for (SizeL c = 0; c < Dest.Length(); ++c, ++Pos) Dest[c] = StrSrc[Pos];
	}
	String UnpackStrLen(const String &Str, SizeL HeadLen, SizeL &Pos) {
		SizeL Len = BtToL((const ByteArray &)Str, Pos, HeadLen);
		return Str.SubStr(Pos, Pos += Len);
	}
	Array<String> UnpackListStrFl(fs::FileBase *Fl, SizeL HeadLen, SizeL StrHeadLen) {
		Array<String> Rtn("", BtToL(Fl->Read(HeadLen)));
		for (String &Elem : Rtn) Elem = Fl->Read(BtToL(Fl->Read(StrHeadLen)));
		return Rtn;
	}
	Array<String> UnpackListStr(const String &StrSrc, SizeL HeadLen, SizeL StrHeadLen) {
		Array<String> Rtn;
		SizeL Pos = 0;
		Rtn.SetLength(BtToL((const ByteArray &)StrSrc, Pos, HeadLen));
		Pos += HeadLen;
		for (String &Elem : Rtn) Elem = UnpackStrLen(StrSrc, StrHeadLen, Pos);
		return Rtn;
	}
	Array<String> UnpackListStr(const String &StrSrc, SizeL HeadLen, SizeL StrHeadLen, SizeL &Pos) {
		Array<String> Rtn;
		Rtn.SetLength(BtToL((const ByteArray &)StrSrc, Pos, HeadLen));
		Pos += HeadLen;
		for (String &Elem : Rtn) Elem = UnpackStrLen(StrSrc, StrHeadLen, Pos);
		return Rtn;
	}
	void PackListStrFl(fs::FileBase *Fl, const Array<String> &LstStr, SizeL HeadLen, SizeL StrHeadLen) {
		Fl->Write(LToBt(LstStr.Length(), HeadLen));
		for (SizeL c = 0; c < LstStr.Length(); ++c) {
			Fl->Write(LToBt(LstStr[c].Length(), StrHeadLen));
			Fl->Write((const ByteArray &)LstStr[c]);
		}
	}
	void PackListStr(String &Dest, SizeL &Pos, const Array<String> &LstStr, SizeL HeadLen, SizeL StrHeadLen) {
		{
			SizeL LookAheadLen = HeadLen + StrHeadLen * LstStr.Length();
			for (SizeL c = 0; c < LstStr.Length(); ++c) LookAheadLen += LstStr[c].Length();
			if (Dest.Length() < Pos + LookAheadLen) Dest.SetLength(Pos + LookAheadLen);
		}
		WriteLToBt(LstStr.Length(), (ByteArray &)Dest, Pos, HeadLen);
		for (SizeL c = 0; c < LstStr.Length(); ++c) {
			WriteLToBt(LstStr[c].Length(), (ByteArray &)Dest, Pos, StrHeadLen);
			LstStr[c].CopyTo((char *)Dest.GetData() + Pos, LstStr[c].Length());
			Pos += LstStr[c].Length();
		}
	}
	Array<String> SplitStr(const String &Str, SizeL HeadLen) {
		Array<String> Rtn(String(), Str.Length() / (HeadLen + 1));
		SizeL CurPos = 0;
		for (SizeL c = 0; c < Rtn.Length(); ++c) {
			SizeL LenPart = BigLong((Byte *)(Str.GetData() + CurPos), HeadLen).ToSizeL();
			Rtn[c] = Str.SubStr(CurPos, CurPos += LenPart + 1);//Not a mistake to increment CurPos in the second parameter to provide the future position
			if (CurPos >= Str.Length())
			{
				Rtn.SetLength(c + 1);
				break;
			}
		}
		return Rtn;
	}
	Mutex::~Mutex() {}
	const Utils::BigLong Two((unsigned long)2), Six((unsigned long)6), One((unsigned long)1), Zero((unsigned long)0);
	AbsFile::AbsFile() {
		Pos = 0;
		Meta[0] = 0;
		Meta[1] = 1;
		TheData = 0;
	}
	AbsFile::AbsFile(fs::FileBase *Fl) {
		Pos = 0;
		Meta[0] = 1;
		Meta[1] = 1;
		TheData = Fl;
	}
	AbsFile::AbsFile(ByteArray &BArr) {
		Pos = 0;
		Meta[0] = 2;
		Meta[1] = 1;
		TheData = &BArr;
	}
	AbsFile::AbsFile(String &Str) {
		Pos = 0;
		Meta[0] = 2;
		Meta[1] = 1;
		TheData = &Str;
	}
	AbsFile::AbsFile(const AbsFile &Cpy) {
		Pos = Cpy.Pos;
		Meta[0] = Cpy.Meta[0];
		Meta[1] = Cpy.Meta[1];
		TheData = Cpy.TheData;
	}
	void AbsFile::SetData(fs::FileBase *Fl) {
		Pos = 0;
		Meta[0] = 1;
		Meta[1] = 1;
		TheData = Fl;
	}
	void AbsFile::SetData(ByteArray *BArr) {
		Pos = 0;
		Meta[0] = 2;
		Meta[1] = 1;
		TheData = BArr;
	}
	void AbsFile::PreAlloc(BigLong NumAlloc) {
		if (Meta[0] == 2) ((ByteArray *)TheData)->SetLength(NumAlloc.ToSizeL());
	}
	void AbsFile::Write(const ByteArray &Data) {
		if (Meta[0] == 2)
		{
			ByteArray &CurData = *(ByteArray *)TheData;
			if (CurData.Length() < Pos + Data.Length())
				CurData.SetLength(((Pos + Data.Length() - 1 + Meta[1]) / Meta[1]) * Meta[1]);
			SizeL c;
			for (c = 0; c < Data.Length(); ++c, ++Pos) {
				CurData[Pos] = Data[c];
			}
			for (c = Pos; c < CurData.Length(); ++c) {
				CurData[c] = 0;
			}
		}
		else if (Meta[0] == 1)
		{
			fs::FileBase *CurData = (fs::FileBase *)TheData;
			CurData->Write(Data);
		}
	}
	void AbsFile::Read(ByteArray &Data, SizeL Len) const{
		if (Meta[0] == 2)
		{
			if (Pos < ((ByteArray *)TheData)->Length())
				Data = ((ByteArray *)TheData)->SubArr(Pos, Pos + Len);
			else Data.SetLength(0);
			Pos += Data.Length();
		}
		else if (Meta[0] == 1) Data = ((fs::FileBase *)TheData)->Read(Len);
		else Data.SetLength(0);
	}
	BigLong AbsFile::GetLen() const {
		if (Meta[0] == 2) return ((ByteArray *)TheData)->Length();
		else if (Meta[0] == 1)
		{
			fs::FileBase *CurData = (fs::FileBase *)TheData;
			long long PrevPos = CurData->Tell();
			CurData->Seek(0, fs::SK_END);
			BigLong Rtn((unsigned long long)CurData->Tell() - PrevPos);
			CurData->Seek(PrevPos);
			return Rtn;
		}
		else return (unsigned long)0;
	}
}
