// IsaacUtils.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "IsaacUtils.h"

struct BitLenNum{
	unsigned long BitLen;
	Utils::BigLong Num;
	bool operator==(BitLenNum &Cmp);
	bool operator!=(BitLenNum &Cmp);
};
bool BitLenNum::operator==(BitLenNum &Cmp){
	return BitLen == Cmp.BitLen && Num == Cmp.Num;
}
bool BitLenNum::operator!=(BitLenNum &Cmp){
	return BitLen != Cmp.BitLen || Num != Cmp.Num;
}
Utils::Array<Utils::BigLong> HashPrimesLst1;
Utils::Array<BitLenNum> HashPrimes1;
Utils::BigLong FastGcd(Utils::BigLong a, Utils::BigLong b){
	Utils::BigLong r;
	while (b.Zero() != 1){
		r = a % b;
		a = b;
		b = r;
	}
	return a;
}
Utils::BigLong Multiplation(Utils::Array<Utils::BigLong> Lst){
	Utils::BigLong Rtn((unsigned long)1);
	for (Utils::BigLong &Num : Lst){
		Rtn *= Num;
	}
	return Rtn;
}
Utils::BigLong Summation(Utils::Array<Utils::BigLong> Lst){
	Utils::BigLong Rtn((unsigned long)0);
	for (Utils::BigLong &Num : Lst){
		Rtn += Num;
	}
	return Rtn;
}
Utils::Array<Utils::BigLong> GenListPrimes(Utils::BigLong UntilNum){
	Utils::BigLong c((unsigned long)2);
	Utils::Array<Utils::BigLong> Rtn(c.PostInc(1), 1);
	Rtn += c.PostInc(2);
	Utils::BigLong Six((unsigned long)6), One((unsigned long)1);
	while (c <= UntilNum){
		if (((c - One) % Six).Zero() == 1 || ((c + One) % Six).Zero() == 1)
		{
			bool IsDiv = false;
			for (Utils::BigLong &Long : Rtn){
				if ((c % Long).Zero() == 1)
				{
					IsDiv = true;
					break;
				}
			}
			if (!IsDiv) Rtn += c;
		}
		c.PreInc(2);
	}
	return Rtn;
}
const Utils::BigLong Two((unsigned long)2), Six((unsigned long)6), One((unsigned long)1);
Utils::Array<Utils::BigLong> GenListnPrimes(unsigned long n){
	Utils::BigLong c = Two;
	Utils::Array<Utils::BigLong> Rtn(c.PostInc(1), 1);
	Rtn += c.PostInc(2);
	while (Rtn.Length() < n){
		if (((c - One) % Six).Zero() == 1 || ((c + One) % Six).Zero() == 1)
		{
			bool IsDiv = false;
			for (Utils::BigLong &Long : Rtn){
				if ((c % Long).Zero() == 1)
				{
					IsDiv = true;
					break;
				}
			}
			if (!IsDiv) Rtn += c;
		}
		c.PreInc(2);
	}
	return Rtn;
}
Utils::Random *SafeRnd = 0;

bool FastNoRepTest(Utils::BigLong Num, unsigned long NumBases){
	return Utils::FermatBaseTest(SafeRnd, Num, NumBases);
}
Utils::BigLong GetHighestPrime(unsigned long BitLen, unsigned long NumPTest){
	if (BitLen < 2) return (unsigned long)0;
	Utils::BigLong Rtn = One;
	Rtn <<= BitLen;
	Rtn.PreDec(1);
	Utils::ShowError("hello Level", "we are at Pos 1");
	Utils::Array<Utils::BigLong> Primes = GenListnPrimes(NumPTest);
	Utils::BigLong Test = Multiplation(Primes);
	while (true){
		if (Rtn.BitLength() < BitLen) return One.Negate();
		if (FastGcd(Rtn, Test) != One)
			Rtn.PreDec(2);
		else if (FastNoRepTest(Rtn, 16)) return Rtn;
		else Rtn.PreDec(2);
	}
}
Utils::Array<void *> Objects;//wStrPos is at 0
SizeL BigLongPos = 0;
SizeL ByteArrayPos = 0;
void *AddObject(Utils::wString *Obj){
	Objects.Insert(0, Obj);
	++BigLongPos;
	++ByteArrayPos;
	return Obj;
}
void *AddObject(Utils::BigLong *Obj){
	Objects.Insert(BigLongPos, Obj);
	++ByteArrayPos;
	return Obj;
}
void *AddObject(Utils::ByteArray *Obj){
	Objects.Insert(ByteArrayPos, Obj);
	return Obj;
}
void *RemoveNoDel(Utils::wString *Obj){
	SizeL Pos = 0;
	if (!Objects.Find(Pos, Obj)) return 0;
	void *Rtn = Objects[Pos];
	Objects.Remove(Pos);
	--BigLongPos;
	--ByteArrayPos;
	return Rtn;
}
void *RemoveNoDel(Utils::BigLong *Obj){
	SizeL Pos = 0;
	if (!Objects.Find(Pos, Obj)) return 0;
	void *Rtn = Objects[Pos];
	Objects.Remove(Pos);
	--ByteArrayPos;
	return Rtn;
}
void *RemoveNoDel(Utils::ByteArray *Obj){
	SizeL Pos = 0;
	if (!Objects.Find(Pos, Obj)) return 0;
	void *Rtn = Objects[Pos];
	Objects.Remove(Pos);
	return Rtn;
}

extern "C"{
	void Init(){
		if (SafeRnd == 0)
		{
			Utils::Init();
			SafeRnd = new Utils::CryptRandom();
			HashPrimesLst1 = GenListnPrimes(18);
		}
	}
	void *wStr_newWLen(wchar_t *Str, SizeL Len){
		return AddObject(new Utils::wString(Str, Len));
	}
	void *wStr_newW(wchar_t *Str){
		return AddObject(new Utils::wString(Str));
	}
	void *wStr_newALen(char *Str, SizeL Len){
		return AddObject(new Utils::wString(Str, Len));
	}
	void *wStr_newA(char *Str){
		return AddObject(new Utils::wString(Str));
	}
	void *wStr_new(){
		return AddObject(new Utils::wString());
	}
	unsigned long wStr_Len(void *wStr){
		return ((Utils::wString *)wStr)->Length();
	}
	void *wStr_DataPtr(void *wStr){
		return (void *)((Utils::wString *)wStr)->GetData();
	}/*
	void wStr_del(void *Str){
		DelObj(Str);
	}*/
	void *ByteArray_new(){
		return AddObject(new Utils::Array<Utils::Byte>());
	}
	void *ByteArray_newA(char *Str){
		return AddObject(new Utils::Array<Utils::Byte>((unsigned char *)Str, strlen(Str)));
	}
	void *ByteArray_newW(wchar_t *Str, Utils::Byte ChOpt){
		if (ChOpt == 0) return new Utils::Array<Utils::Byte>((unsigned char *)Str, 2 * Utils::wStrLen(Str));
		Utils::ByteArray *Rtn = new Utils::Array<Utils::Byte>(Utils::Byte(0), Utils::wStrLen(Str));
		for (SizeL c = 0; c < Rtn->Length(); ++c){
			(*Rtn)[c] = ChOpt == 1 ? Str[c] & 0xFF : (Str[c] & 0xFF00) >> 8;
		}
		return AddObject(Rtn);
	}
	void *ByteArray_newWStr(void *wStr, Utils::Byte ChOpt){
		Utils::wString &Str = *(Utils::wString *)wStr;
		if (ChOpt == 0) return new Utils::Array<Utils::Byte>((unsigned char *)Str.GetData(), 2 * Str.Length());
		Utils::ByteArray *Rtn = new Utils::ByteArray(Utils::Byte(0), Str.Length());
		for (SizeL c = 0; c < Rtn->Length(); ++c){
			(*Rtn)[c] = ChOpt == 1 ? Str[c] & 0xFF : (Str[c] & 0xFF00) >> 8;
		}
		return AddObject(Rtn);
	}
	void *ByteArray_newBigLong(void *BLong){
		Utils::BigLong &Bl = *(Utils::BigLong*)BLong;
		if (Utils::IsBigEnd) return new Utils::Array<Utils::Byte>((Utils::Byte)Bl.GetLongs().GetData(), Bl.GetLongs().Length() * 4);
		Utils::Array<unsigned long> &Longs = Bl.GetLongs();
		Utils::ByteArray &Rtn = *new Utils::ByteArray(Utils::Byte(0), Longs.Length() * 4);
		for (SizeL c = 0; c < Longs.Length(); ++c){
			if (Utils::IsBigEnd)
			{
				Rtn[c * 4] = Longs[c] & 0xFF;
				Rtn[c * 4 + 1] = (Longs[c] >> 8) & 0xFF;
				Rtn[c * 4 + 2] = (Longs[c] >> 16) & 0xFF;
				Rtn[c * 4 + 3] = (Longs[c] >> 24) & 0xFF;
			}
			else
			{
				Rtn[c * 4 + 3] = Longs[c] & 0xFF;
				Rtn[c * 4 + 2] = (Longs[c] >> 8) & 0xFF;
				Rtn[c * 4 + 1] = (Longs[c] >> 16) & 0xFF;
				Rtn[c * 4] = (Longs[c] >> 24) & 0xFF;
			}
		}
		return AddObject(&Rtn);
	}
	SizeL ByteArray_Len(void *bArray){
		return ((Utils::ByteArray *)bArray)->Length();
	}
	void *ByteArray_DataPtr(void *bArray){
		return (void *)((Utils::ByteArray *)bArray)->GetData();
	}
	void *BigLong_newLong(long L){
		unsigned long Input = 0;
		bool Sign = false;
		if (L < 0)
		{
			Sign = true;
			Input = L;
			Input = ~Input;
			Input += 1;
		}
		else Input = L;
		Utils::BigLong *Rtn = new Utils::BigLong(Input);
		if (Sign) Rtn->Minus();
		return AddObject(Rtn);
	}
	void *BigLong_new(){
		return AddObject(new Utils::BigLong());
	}
	void *BigLong_newByteArray(void *BArray){
		Utils::BigLong &Rtn = *new Utils::BigLong();
		Utils::ByteArray &BArr = *(Utils::ByteArray *)BArray;
		Rtn.GetLongs().SetLength((BArr.Length() + 3) / 4);
		for (SizeL c = 0; c < BArr.Length(); ++c){
			Rtn.GetByte(c) = BArr[c];
		}
		return AddObject(&Rtn);
	}
	bool BigLong_FromwString(void *Bl, void *wStr, Utils::Byte base){
		if (base > 64 || base == 0) return false;
		Utils::BigLong &Var = *(Utils::BigLong *)Bl;
		return Var.FromwStr(*(Utils::wString *)wStr, base);
	}
	bool BigLong_TowString(void *Bl, void *wStr, Utils::Byte base){
		if (base > 64 || base == 0) return false;
		Utils::BigLong &Var = *(Utils::BigLong *)Bl;
		Var.TowStr(*(Utils::wString *)wStr, base);
		return true;
	}
	void *BigLong_Assign(void *BlThis, void *BlThat){
		return &(((Utils::BigLong *)BlThis)->operator=(*(Utils::BigLong *)BlThat));
	}
	void *BigLong_Add(void *BlThis, void *BlThat){
		return AddObject(new Utils::BigLong(((Utils::BigLong *)BlThis)->operator+(*(Utils::BigLong *)BlThat)));
	}
	void *BigLong_IAdd(void *BlThis, void *BlThat){
		return &(((Utils::BigLong *)BlThis)->operator+=(*(Utils::BigLong *)BlThat));
	}
	void *BigLong_Sub(void *BlThis, void *BlThat){
		return AddObject(new Utils::BigLong(((Utils::BigLong *)BlThis)->operator-(*(Utils::BigLong *)BlThat)));
	}
	void *BigLong_ISub(void *BlThis, void *BlThat){
		return &(((Utils::BigLong *)BlThis)->operator-=(*(Utils::BigLong *)BlThat));
	}
	void *BigLong_Mul(void *BlThis, void *BlThat){
		return AddObject(new Utils::BigLong(((Utils::BigLong *)BlThis)->operator*(*(Utils::BigLong *)BlThat)));
	}
	void *BigLong_IMul(void *BlThis, void *BlThat){
		return &(((Utils::BigLong *)BlThis)->operator*=(*(Utils::BigLong *)BlThat));
	}
	void *BigLong_Div(void *BlThis, void *BlThat){
		return AddObject(new Utils::BigLong(((Utils::BigLong *)BlThis)->operator/(*(Utils::BigLong *)BlThat)));
	}
	void *BigLong_IDiv(void *BlThis, void *BlThat){
		return &(((Utils::BigLong *)BlThis)->operator/=(*(Utils::BigLong *)BlThat));
	}
	void *BigLong_Mod(void *BlThis, void *BlThat){
		return AddObject(new Utils::BigLong(((Utils::BigLong *)BlThis)->operator%(*(Utils::BigLong *)BlThat)));
	}
	void *BigLong_IMod(void *BlThis, void *BlThat){
		return &(((Utils::BigLong *)BlThis)->operator%=(*(Utils::BigLong *)BlThat));
	}
	void *BigLong_LShift(void *BlThis, unsigned long That){
		return AddObject(new Utils::BigLong(((Utils::BigLong *)BlThis)->operator<<(That)));
	}
	void *BigLong_ILShift(void *BlThis, unsigned long That){
		return &(((Utils::BigLong *)BlThis)->operator<<=(That));
	}
	void *BigLong_RShift(void *BlThis, unsigned long That){
		return AddObject(new Utils::BigLong(((Utils::BigLong *)BlThis)->operator>>(That)));
	}
	void *BigLong_IRShift(void *BlThis, unsigned long That){
		return &(((Utils::BigLong *)BlThis)->operator>>=(That));
	}
	void *BigLong_And(void *BlThis, void *BlThat){
		return AddObject(new Utils::BigLong(((Utils::BigLong *)BlThis)->operator&(*(Utils::BigLong *)BlThat)));
	}
	void *BigLong_IAnd(void *BlThis, void *BlThat){
		return &(((Utils::BigLong *)BlThis)->operator&=(*(Utils::BigLong *)BlThat));
	}
	void *BigLong_Or(void *BlThis, void *BlThat){
		return AddObject(new Utils::BigLong(((Utils::BigLong *)BlThis)->operator|(*(Utils::BigLong *)BlThat)));
	}
	void *BigLong_IOr(void *BlThis, void *BlThat){
		return &(((Utils::BigLong *)BlThis)->operator|=(*(Utils::BigLong *)BlThat));
	}
	void *BigLong_Xor(void *BlThis, void *BlThat){
		return AddObject(new Utils::BigLong(((Utils::BigLong *)BlThis)->operator^(*(Utils::BigLong *)BlThat)));
	}
	void *BigLong_IXor(void *BlThis, void *BlThat){
		return &(((Utils::BigLong *)BlThis)->operator^=(*(Utils::BigLong *)BlThat));
	}
	bool BigLong_IsEq(void *BlThis, void *BlThat){
		return ((Utils::BigLong *)BlThis)->operator==(*(Utils::BigLong *)BlThat);
	}
	bool BigLong_IsNe(void *BlThis, void *BlThat){
		return ((Utils::BigLong *)BlThis)->operator!=(*(Utils::BigLong *)BlThat);
	}
	bool BigLong_IsLt(void *BlThis, void *BlThat){
		return ((Utils::BigLong *)BlThis)->operator<(*(Utils::BigLong *)BlThat);
	}
	bool BigLong_IsLe(void *BlThis, void *BlThat){
		return ((Utils::BigLong *)BlThis)->operator<=(*(Utils::BigLong *)BlThat);
	}
	bool BigLong_IsGt(void *BlThis, void *BlThat){
		return ((Utils::BigLong *)BlThis)->operator>(*(Utils::BigLong *)BlThat);
	}
	bool BigLong_IsGe(void *BlThis, void *BlThat){
		return ((Utils::BigLong *)BlThis)->operator>=(*(Utils::BigLong *)BlThat);
	}
	unsigned long long BigLong_BitLen(void *Bl){
		return ((Utils::BigLong *)Bl)->BitLength();
	}
	/*
	void BigLong_del(void * Bl){
		DelObj(Bl);
	}*/
	void *BigLong_ModPow(void *Base, void *Exp, void *Mod){
		return AddObject(new Utils::BigLong(Utils::Pow(*(Utils::BigLong *)Base, *(Utils::BigLong *)Exp, *(Utils::BigLong *)Mod)));
	}
	void *RegMyHash_10(unsigned long BitLen){
		for (BitLenNum &Elem : HashPrimes1){
			if (Elem.BitLen == BitLen) return new Utils::BigLong(Elem.Num);
		}
		Utils::ShowError("hello Level", "we are at Pos 0");
		BitLenNum Add = { BitLen, GetHighestPrime(BitLen, 18) };
		HashPrimes1 += Add;
		return AddObject(new Utils::BigLong(Add.Num));
	}
	void *MyHash_11(void *StrBytes, unsigned long BitLen){
		Utils::BigLong &MyPrime = *(Utils::BigLong *)RegMyHash_10(BitLen);
		Utils::BigLong &InLong = *new Utils::BigLong(GetNumStrTestB(*(Utils::Array<Utils::Byte> *)StrBytes) ^ MyPrime);
		if (((InLong + One) % MyPrime).Zero() == 1)
			InLong = (unsigned long)0;
		else
		{
			unsigned long PrimeNum = 0;
			Utils::BigLong Tmp = (InLong / MyPrime) % HashPrimesLst1.Length();
			if (Tmp.GetLongs().Length() == 0) PrimeNum = 0;
			else PrimeNum = Tmp.GetLongs()[0];
			Utils::BigLong *TmpDel = &InLong;
			InLong = Utils::Pow(HashPrimesLst1[PrimeNum], InLong, MyPrime) % (One << BitLen);
			delete TmpDel;
		}
		DelObj(&MyPrime);
		return AddObject(&InLong); // because this was a reference to a 'new' allocated BigLong
	}

	bool DelObj(void *Obj){
		SizeL Pos = 0;
		if (!Objects.Find(Pos, Obj)) return false;
		if (Pos < BigLongPos)
		{
			delete (Utils::wString *)Obj;
			--BigLongPos;
			--ByteArrayPos;
		}
		else if (Pos < ByteArrayPos)
		{
			delete (Utils::BigLong *)Obj;
			--ByteArrayPos;
		}
		else delete (Utils::ByteArray *)Obj;
		Objects.Remove(Pos);
		return true;
	}
	void CleanHeap(){
		for (SizeL c = 0; c < BigLongPos; ++c){
			delete (Utils::wString *)(Objects[c]);
		}
		for (SizeL c = BigLongPos; c < ByteArrayPos; ++c){
			delete (Utils::BigLong *)(Objects[c]);
		}
		SizeL Len = Objects.Length();
		for (SizeL c = ByteArrayPos; c < Len; ++c){
			delete (Utils::Array<Utils::Byte> *)(Objects[c]);
		}
		Objects.SetLength(0);
	}
	void DeInit(){
		if (SafeRnd != 0)
		{
			delete SafeRnd;
			SafeRnd = 0;
			CleanHeap();
		}
	}
}
