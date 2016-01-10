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
class AbsObjPool {
public:
	virtual Utils::wString GetName() = 0;
	virtual bool HasObject(void *Obj) = 0;
	virtual bool RemDelObject(void *Obj) = 0;
	virtual bool RemObject(void *Obj) = 0;
	virtual void CleanPool() = 0;
	virtual ~AbsObjPool();
};
AbsObjPool::~AbsObjPool() {}
template<typename T>
class ObjPool : public AbsObjPool{
public:
	Utils::Mutex *Lk;
	Utils::Array<void *> Objects;

	ObjPool();
	//O(1)
	Utils::wString GetName();
	//O(n log(n))
	void *AddObject(T *Obj);
	//O(log(n))
	bool HasObject(void *Obj);
	//O(n log(n))
	bool RemDelObject(void *Obj);
	//O(n log(n))
	bool RemObject(void *Obj);
	//O(n)
	void CleanPool();
	~ObjPool();
};
template<typename T>
ObjPool<T>::ObjPool() {
	Lk = Utils::GetRWMutex();
}
template<typename T>
Utils::wString ObjPool<T>::GetName() {
	return "UNKNOWN";
}
template<>
Utils::wString ObjPool<Utils::wString>::GetName() {
	return "wString";
}
template<>
Utils::wString ObjPool<Utils::BigLong>::GetName() {
	return "BigLong";
}
template<>
Utils::wString ObjPool<Utils::ByteArray>::GetName() {
	return "ByteArray";
}
template<>
Utils::wString ObjPool<Utils::sock::SockAddr>::GetName() {
	return "SockAddr";
}
template<>
Utils::wString ObjPool<Utils::sock::Socket>::GetName() {
	return "Socket";
}
template<>
Utils::wString ObjPool<Utils::Mutex>::GetName() {
	return "Lock";
}
template<>
Utils::wString ObjPool<Utils::fs::FileBase>::GetName() {
	return "FileObj";
}
template<>
Utils::wString ObjPool<Utils::EncProt>::GetName() {
	return "EncProt";
}
template<typename T>
void *ObjPool<T>::AddObject(T *Obj) {
	Utils::Lock InstLk(Lk, true);
	SizeL NewPos = Utils::BinaryApprox((void **)Objects.GetData(), Objects.Length(), (void *&)Obj);
	Objects.Insert(NewPos, Obj);
	return Obj;
}
template<typename T>
bool ObjPool<T>::HasObject(void *Obj) {
	Utils::Lock InstLk(Lk, false);
	return Utils::BinarySearch((void **)Objects.GetData(), Objects.Length(), Obj) < Objects.Length();
}
template<typename T>
bool ObjPool<T>::RemDelObject(void *Obj) {
	if (Obj == 0) return false;
	else
	{
		Utils::Lock InstLk(Lk, true);
		SizeL Pos = Utils::BinarySearch((void **)Objects.GetData(), Objects.Length(), Obj);
		if (Pos >= Objects.Length()) return false;
		Objects.Remove(Pos);
		delete (T *)Obj;
		return true;
	}
}
template<>
bool ObjPool<Utils::EncProt>::RemDelObject(void *Obj) {
	if (Obj == 0) return false;
	else
	{
		Utils::Lock InstLk(Lk, true);
		SizeL Pos = Utils::BinarySearch((void **)Objects.GetData(), Objects.Length(), Obj);
		if (Pos >= Objects.Length()) return false;
		Objects.Remove(Pos);
		delete ((Utils::EncProt *)Obj)->Enc;
		((Utils::EncProt *)Obj)->Enc = 0;
		delete (Utils::EncProt *)Obj;
		return true;
	}
}
template<typename T>
bool ObjPool<T>::RemObject(void *Obj) {
	if (Obj == 0) return false;
	else
	{
		Utils::Lock InstLk(Lk, true);
		SizeL Pos = Utils::BinarySearch((void **)Objects.GetData(), Objects.Length(), Obj);
		if (Pos >= Objects.Length()) return false;
		Objects.Remove(Pos);
		return true;
	}
}
template<typename T>
void ObjPool<T>::CleanPool() {
	Utils::Lock InstLk(Lk, true);
	for (void *&Obj : Objects) {
		delete (T *)Obj;
	}
	Objects.SetLength(0);
}
template<>
void ObjPool<Utils::EncProt>::CleanPool() {
	Utils::Lock InstLk(Lk, true);
	for (void *&Obj : Objects) {
		delete ((Utils::EncProt *)Obj)->Enc;
		((Utils::EncProt *)Obj)->Enc = 0;
		delete (Utils::EncProt *)Obj;
	}
	Objects.SetLength(0);
}
template<typename T>
ObjPool<T>::~ObjPool() {
	CleanPool();
	Utils::DestroyMutex(Lk);
	Lk = 0;
}


ObjPool<Utils::wString> wStrPool;
ObjPool<Utils::BigLong> BigLongPool;
ObjPool<Utils::ByteArray> BArrPool;
ObjPool<Utils::sock::SockAddr> SockAddrPool;
ObjPool<Utils::sock::Socket> SocketPool;
ObjPool<Utils::Mutex> LockPool;
ObjPool<Utils::fs::FileBase> FlPool;
ObjPool<Utils::EncProt> EncProtPool;
Utils::Array<AbsObjPool *> TypePool;

void *AddObject(Utils::wString *Obj) {
	return wStrPool.AddObject(Obj);
}
void *AddObject(Utils::BigLong *Obj) {
	return BigLongPool.AddObject(Obj);
}
void *AddObject(Utils::ByteArray *Obj) {
	return BArrPool.AddObject(Obj);
}
void *AddObject(Utils::sock::SockAddr *Obj) {
	return SockAddrPool.AddObject(Obj);
}
void *AddObject(Utils::sock::Socket *Obj) {
	return SocketPool.AddObject(Obj);
}
void *AddObject(Utils::Mutex *Obj) {
	return LockPool.AddObject(Obj);
}
void *AddObject(Utils::fs::FileBase *Obj) {
	return FlPool.AddObject(Obj);
}
void *AddObject(Utils::EncProt *Obj) {
	return EncProtPool.AddObject(Obj);
}


Utils::wString LastError;
unsigned long LastErrCode = 0;
bool AssertOn = true;

void SetErrStr(const Utils::wString &TypeName, unsigned long ParamNum, const Utils::wString &FuncName) {
	LastErrCode = 11000;
	LastError = FuncName + ": Assertion failed. Expected a " + TypeName + " Object at Parameter Number: " + Utils::FromNumber(ParamNum);
}

/*template<typename T>
bool AssertType(void *Obj, unsigned long ParamNum, const Utils::wString &FuncName) {
	return false;
}
template<>
bool AssertType<Utils::wString>(void *Obj, unsigned long ParamNum, const Utils::wString &FuncName) {
	if (!AssertOn || wStrPool.HasObject(Obj)) return true;
	else SetErrStr("wString", ParamNum, FuncName);
}
template<>
bool AssertType<Utils::BigLong>(void *Obj, unsigned long ParamNum, const Utils::wString &FuncName) {
	if (!AssertOn || BigLongPool.HasObject(Obj)) return true;
	else SetErrStr("BigLong", ParamNum, FuncName);
	return false;
}
template<>
bool AssertType<Utils::ByteArray>(void *Obj, unsigned long ParamNum, const Utils::wString &FuncName) {
	if (!AssertOn || BArrPool.HasObject(Obj)) return true;
	else SetErrStr("ByteArray", ParamNum, FuncName);
	return false;
}
template<>
bool AssertType<Utils::sock::SockAddr>(void *Obj, unsigned long ParamNum, const Utils::wString &FuncName) {
	if (!AssertOn || SockAddrPool.HasObject(Obj)) return true;
	else SetErrStr("SockAddr", ParamNum, FuncName);
	return false;
}
template<>
bool AssertType<Utils::sock::Socket>(void *Obj, unsigned long ParamNum, const Utils::wString &FuncName) {
	if (!AssertOn || SocketPool.HasObject(Obj)) return true;
	else SetErrStr("Socket", ParamNum, FuncName);
	return false;
}
template<>
bool AssertType<Utils::fs::FileBase>(void *Obj, unsigned long ParamNum, const Utils::wString &FuncName) {
	if (!AssertOn || FlPool.HasObject(Obj)) return true;
	else SetErrStr("FileObj", ParamNum, FuncName);
	return false;
}
template<>
bool AssertType<Utils::EncProt>(void *Obj, unsigned long ParamNum, const Utils::wString &FuncName) {
	if (!AssertOn || EncProtPool.HasObject(Obj)) return true;
	else SetErrStr("EncProt", ParamNum, FuncName);
	return false;
}*/

bool AssertType(void *Obj, AbsObjPool *Type, unsigned long ParamNum, const Utils::wString &FuncName) {//TODO Add Assertions in functions
	if (LastErrCode)
	{
		LastErrCode = 0;
		LastError.SetLength(0);
	}
	if (!AssertOn) return true;
	if (Type->HasObject(Obj)) return true;
	LastErrCode = 11000;
	LastError = FuncName + ": Assertion failed, Parameter Number: " + Utils::FromNumber(ParamNum) + " was not of type: " + Type->GetName();
	return false;
}

extern "C" {
	void Init() {
		if (SafeRnd == 0)
		{
			Utils::Init();
			SafeRnd = Utils::GetCryptoRand();
			HashPrimesLst1 = GenListnPrimes(18);
			TypePool.SetLength(7);
			TypePool[0] = &wStrPool;
			TypePool[1] = &BigLongPool;
			TypePool[2] = &BArrPool;
			TypePool[3] = &SockAddrPool;
			TypePool[4] = &SocketPool;
			TypePool[5] = &FlPool;
			TypePool[6] = &EncProtPool;
		}
	}
	void *wStr_newWLen(wchar_t *Str, SizeL Len) {
		return AddObject(new Utils::wString(Str, Len));
	}
	void *wStr_newW(wchar_t *Str) {
		return AddObject(new Utils::wString(Str));
	}
	void *wStr_newALen(char *Str, SizeL Len) {
		return AddObject(new Utils::wString(Str, Len));
	}
	void *wStr_newA(char *Str) {
		return AddObject(new Utils::wString(Str));
	}
	void *wStr_new() {
		return AddObject(new Utils::wString());
	}
	unsigned long wStr_Len(void *wStr) {
		if (!AssertType(wStr, &wStrPool, 0, __FUNCTION__)) return 0;
		return ((Utils::wString *)wStr)->Length();
	}
	void *wStr_DataPtr(void *wStr) {
		if (!AssertType(wStr, &wStrPool, 0, __FUNCTION__)) return 0;
		return (void *)((Utils::wString *)wStr)->GetData();
	}
	void *ByteArray_new() {
		return AddObject(new Utils::ByteArray());
	}
	void *ByteArray_newA(char *Str) {
		return AddObject(new Utils::Array<Utils::Byte>((unsigned char *)Str, strlen(Str)));
	}
	void *ByteArray_newALen(char *Str, SizeL Len) {
		return AddObject(new Utils::Array<Utils::Byte>((unsigned char *)Str, Len));
	}
	void *ByteArray_newW(wchar_t *Str, Utils::Byte ChOpt) {
		if (ChOpt == 0) return new Utils::ByteArray((unsigned char *)Str, 2 * Utils::wStrLen(Str));
		Utils::ByteArray *Rtn = new Utils::ByteArray(Utils::Byte(0), Utils::wStrLen(Str));
		for (SizeL c = 0; c < Rtn->Length(); ++c) {
			(*Rtn)[c] = ChOpt == 1 ? Str[c] & 0xFF : (Str[c] & 0xFF00) >> 8;
		}
		return AddObject(Rtn);
	}
	void *ByteArray_newWStr(void *wStr, Utils::Byte ChOpt) {
		if (!AssertType(wStr, &wStrPool, 0, __FUNCTION__)) return 0;
		Utils::wString &Str = *(Utils::wString *)wStr;
		if (ChOpt == 0) return new Utils::ByteArray((unsigned char *)Str.GetData(), 2 * Str.Length());
		Utils::ByteArray *Rtn = new Utils::ByteArray(Utils::Byte(0), Str.Length());
		for (SizeL c = 0; c < Rtn->Length(); ++c) {
			(*Rtn)[c] = ChOpt == 1 ? Str[c] & 0xFF : (Str[c] & 0xFF00) >> 8;
		}
		return AddObject(Rtn);
	}
	void *ByteArray_newBigLong(void *BLong) {
		if (!AssertType(BLong, &BigLongPool, 0, __FUNCTION__)) return 0;
		Utils::BigLong &Bl = *(Utils::BigLong*)BLong;
		if (Utils::IsBigEnd) return new Utils::ByteArray((Utils::Byte *)Bl.GetLongs().GetData(), Bl.GetLongs().Length() * 4);
		Utils::Array<unsigned long> &Longs = Bl.GetLongs();
		Utils::ByteArray &Rtn = *new Utils::ByteArray(Utils::Byte(0), Longs.Length() * 4);
		for (SizeL c = 0; c < Longs.Length(); ++c) {
			Rtn[c * 4 + 3] = Longs[c] & 0xFF;
			Rtn[c * 4 + 2] = (Longs[c] >> 8) & 0xFF;
			Rtn[c * 4 + 1] = (Longs[c] >> 16) & 0xFF;
			Rtn[c * 4] = (Longs[c] >> 24) & 0xFF;
		}
		return AddObject(&Rtn);
	}
	SizeL ByteArray_Len(void *bArray) {
		if (!AssertType(bArray, &BArrPool, 0, __FUNCTION__)) return 0;
		return ((Utils::ByteArray *)bArray)->Length();
	}
	void *ByteArray_DataPtr(void *bArray) {
		if (!AssertType(bArray, &BArrPool, 0, __FUNCTION__)) return 0;
		return (void *)((Utils::ByteArray *)bArray)->GetData();
	}
	void *BigLong_newLong(long L) {
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
	void *BigLong_DataPtr(void *Bl) {
		if (!AssertType(Bl, &BigLongPool, 0, __FUNCTION__)) return 0;
		return (void *)((Utils::BigLong *)Bl)->GetLongs().GetData();
	}
	SizeL BigLong_Len(void *Bl) {
		if (!AssertType(Bl, &BigLongPool, 0, __FUNCTION__)) return 0;
		return ((Utils::BigLong *)Bl)->GetLongs().Length() * 4;
	}
	void *BigLong_new() {
		return AddObject(new Utils::BigLong());
	}
	void *BigLong_newByteArray(void *bArray) {
		if (!AssertType(bArray, &BArrPool, 0, __FUNCTION__)) return 0;
		Utils::BigLong &Rtn = *new Utils::BigLong();
		Utils::ByteArray &BArr = *(Utils::ByteArray *)bArray;
		Rtn.GetLongs().SetLength((BArr.Length() + 3) / 4);
		for (SizeL c = 0; c < BArr.Length(); ++c) {
			Rtn.GetByte(c) = BArr[c];
		}
		return AddObject(&Rtn);
	}
	void *BigLong_newALen(char *Str, SizeL Len) {
		Utils::BigLong &Rtn = *new Utils::BigLong();
		Utils::ByteArray Tmp((Utils::Byte *)Str, Len);
		Rtn.GetLongs().SetLength((Tmp.Length() + 3) / 4);
		for (SizeL c = 0; c < Tmp.Length(); ++c) {
			Rtn.GetByte(c) = Tmp[c];
		}
		return AddObject(&Rtn);
	}
	bool BigLong_FromwString(void *Bl, void *wStr, Utils::Byte base) {
		if (!AssertType(Bl, &BigLongPool, 0, __FUNCTION__)) return 0;
		if (!AssertType(wStr, &wStrPool, 1, __FUNCTION__)) return 0;
		if (base > 64 || base == 0) return false;
		Utils::BigLong &Var = *(Utils::BigLong *)Bl;
		return Var.FromwStr(*(Utils::wString *)wStr, base);
	}
	bool BigLong_TowString(void *Bl, void *wStr, Utils::Byte base) {
		if (!AssertType(Bl, &BigLongPool, 0, __FUNCTION__)) return 0;
		if (!AssertType(wStr, &wStrPool, 1, __FUNCTION__)) return 0;
		if (base > 64 || base == 0) return false;
		Utils::BigLong &Var = *(Utils::BigLong *)Bl;
		Var.TowStr(*(Utils::wString *)wStr, base);
		return true;
	}
	void *BigLong_Assign(void *BlThis, void *BlThat) {
		if (!AssertType(BlThis, &BigLongPool, 0, __FUNCTION__)) return 0;
		if (!AssertType(BlThat, &BigLongPool, 1, __FUNCTION__)) return 0;
		return &(((Utils::BigLong *)BlThis)->operator=(*(Utils::BigLong *)BlThat));
	}
	void *BigLong_Add(void *BlThis, void *BlThat) {
		if (!AssertType(BlThis, &BigLongPool, 0, __FUNCTION__)) return 0;
		if (!AssertType(BlThat, &BigLongPool, 1, __FUNCTION__)) return 0;
		return AddObject(new Utils::BigLong(((Utils::BigLong *)BlThis)->operator+(*(Utils::BigLong *)BlThat)));
	}
	void *BigLong_IAdd(void *BlThis, void *BlThat) {
		if (!AssertType(BlThis, &BigLongPool, 0, __FUNCTION__)) return 0;
		if (!AssertType(BlThat, &BigLongPool, 1, __FUNCTION__)) return 0;
		return &(((Utils::BigLong *)BlThis)->operator+=(*(Utils::BigLong *)BlThat));
	}
	void *BigLong_Sub(void *BlThis, void *BlThat) {
		if (!AssertType(BlThis, &BigLongPool, 0, __FUNCTION__)) return 0;
		if (!AssertType(BlThat, &BigLongPool, 1, __FUNCTION__)) return 0;
		return AddObject(new Utils::BigLong(((Utils::BigLong *)BlThis)->operator-(*(Utils::BigLong *)BlThat)));
	}
	void *BigLong_ISub(void *BlThis, void *BlThat) {
		if (!AssertType(BlThis, &BigLongPool, 0, __FUNCTION__)) return 0;
		if (!AssertType(BlThat, &BigLongPool, 1, __FUNCTION__)) return 0;
		return &(((Utils::BigLong *)BlThis)->operator-=(*(Utils::BigLong *)BlThat));
	}
	void *BigLong_Mul(void *BlThis, void *BlThat) {
		if (!AssertType(BlThis, &BigLongPool, 0, __FUNCTION__)) return 0;
		if (!AssertType(BlThat, &BigLongPool, 1, __FUNCTION__)) return 0;
		return AddObject(new Utils::BigLong(((Utils::BigLong *)BlThis)->operator*(*(Utils::BigLong *)BlThat)));
	}
	void *BigLong_IMul(void *BlThis, void *BlThat) {
		if (!AssertType(BlThis, &BigLongPool, 0, __FUNCTION__)) return 0;
		if (!AssertType(BlThat, &BigLongPool, 1, __FUNCTION__)) return 0;
		return &((Utils::BigLong *)BlThis)->operator*=(*(Utils::BigLong *)BlThat);
	}
	void *BigLong_IMulLim(void *BlThis, void *BlThat, SizeL LimNum) {
		if (!AssertType(BlThis, &BigLongPool, 0, __FUNCTION__)) return 0;
		if (!AssertType(BlThat, &BigLongPool, 1, __FUNCTION__)) return 0;
		return &((Utils::BigLong *)BlThis)->IMulLim(*(Utils::BigLong *)BlThat, LimNum);
	}
	void *BigLong_Div(void *BlThis, void *BlThat) {
		if (!AssertType(BlThis, &BigLongPool, 0, __FUNCTION__)) return 0;
		if (!AssertType(BlThat, &BigLongPool, 1, __FUNCTION__)) return 0;
		return AddObject(new Utils::BigLong(((Utils::BigLong *)BlThis)->operator/(*(Utils::BigLong *)BlThat)));
	}
	void *BigLong_IDiv(void *BlThis, void *BlThat) {
		if (!AssertType(BlThis, &BigLongPool, 0, __FUNCTION__)) return 0;
		if (!AssertType(BlThat, &BigLongPool, 1, __FUNCTION__)) return 0;
		return &((Utils::BigLong *)BlThis)->operator/=(*(Utils::BigLong *)BlThat);
	}
	void *BigLong_Mod(void *BlThis, void *BlThat) {
		if (!AssertType(BlThis, &BigLongPool, 0, __FUNCTION__)) return 0;
		if (!AssertType(BlThat, &BigLongPool, 1, __FUNCTION__)) return 0;
		return AddObject(new Utils::BigLong(((Utils::BigLong *)BlThis)->operator%(*(Utils::BigLong *)BlThat)));
	}
	void *BigLong_IMod(void *BlThis, void *BlThat) {
		if (!AssertType(BlThis, &BigLongPool, 0, __FUNCTION__)) return 0;
		if (!AssertType(BlThat, &BigLongPool, 1, __FUNCTION__)) return 0;
		return &((Utils::BigLong *)BlThis)->operator%=(*(Utils::BigLong *)BlThat);
	}
	void *BigLong_LShift(void *BlThis, unsigned long That) {
		if (!AssertType(BlThis, &BigLongPool, 0, __FUNCTION__)) return 0;
		return AddObject(new Utils::BigLong(((Utils::BigLong *)BlThis)->operator<<(That)));
	}
	void *BigLong_ILShift(void *BlThis, unsigned long That) {
		if (!AssertType(BlThis, &BigLongPool, 0, __FUNCTION__)) return 0;
		return &((Utils::BigLong *)BlThis)->operator<<=(That);
	}
	void *BigLong_RShift(void *BlThis, unsigned long That) {
		if (!AssertType(BlThis, &BigLongPool, 0, __FUNCTION__)) return 0;
		return AddObject(new Utils::BigLong(((Utils::BigLong *)BlThis)->operator>>(That)));
	}
	void *BigLong_IRShift(void *BlThis, unsigned long That) {
		if (!AssertType(BlThis, &BigLongPool, 0, __FUNCTION__)) return 0;
		return &((Utils::BigLong *)BlThis)->operator>>=(That);
	}
	void *BigLong_And(void *BlThis, void *BlThat) {
		if (!AssertType(BlThis, &BigLongPool, 0, __FUNCTION__)) return 0;
		if (!AssertType(BlThat, &BigLongPool, 1, __FUNCTION__)) return 0;
		return AddObject(new Utils::BigLong(((Utils::BigLong *)BlThis)->operator&(*(Utils::BigLong *)BlThat)));
	}
	void *BigLong_IAnd(void *BlThis, void *BlThat) {
		if (!AssertType(BlThis, &BigLongPool, 0, __FUNCTION__)) return 0;
		if (!AssertType(BlThat, &BigLongPool, 1, __FUNCTION__)) return 0;
		return &((Utils::BigLong *)BlThis)->operator&=(*(Utils::BigLong *)BlThat);
	}
	void *BigLong_Or(void *BlThis, void *BlThat) {
		if (!AssertType(BlThis, &BigLongPool, 0, __FUNCTION__)) return 0;
		if (!AssertType(BlThat, &BigLongPool, 1, __FUNCTION__)) return 0;
		return AddObject(new Utils::BigLong(((Utils::BigLong *)BlThis)->operator|(*(Utils::BigLong *)BlThat)));
	}
	void *BigLong_IOr(void *BlThis, void *BlThat) {
		if (!AssertType(BlThis, &BigLongPool, 0, __FUNCTION__)) return 0;
		if (!AssertType(BlThat, &BigLongPool, 1, __FUNCTION__)) return 0;
		return &((Utils::BigLong *)BlThis)->operator|=(*(Utils::BigLong *)BlThat);
	}
	void *BigLong_Xor(void *BlThis, void *BlThat) {
		if (!AssertType(BlThis, &BigLongPool, 0, __FUNCTION__)) return 0;
		if (!AssertType(BlThat, &BigLongPool, 1, __FUNCTION__)) return 0;
		return AddObject(new Utils::BigLong(((Utils::BigLong *)BlThis)->operator^(*(Utils::BigLong *)BlThat)));
	}
	void *BigLong_IXor(void *BlThis, void *BlThat) {
		if (!AssertType(BlThis, &BigLongPool, 0, __FUNCTION__)) return 0;
		if (!AssertType(BlThat, &BigLongPool, 1, __FUNCTION__)) return 0;
		return &((Utils::BigLong *)BlThis)->operator^=(*(Utils::BigLong *)BlThat);
	}
	bool BigLong_IsEq(void *BlThis, void *BlThat) {
		if (!AssertType(BlThis, &BigLongPool, 0, __FUNCTION__)) return 0;
		if (!AssertType(BlThat, &BigLongPool, 1, __FUNCTION__)) return 0;
		return ((Utils::BigLong *)BlThis)->operator==(*(Utils::BigLong *)BlThat);
	}
	bool BigLong_IsNe(void *BlThis, void *BlThat) {
		if (!AssertType(BlThis, &BigLongPool, 0, __FUNCTION__)) return 0;
		if (!AssertType(BlThat, &BigLongPool, 1, __FUNCTION__)) return 0;
		return ((Utils::BigLong *)BlThis)->operator!=(*(Utils::BigLong *)BlThat);
	}
	bool BigLong_IsLt(void *BlThis, void *BlThat) {
		if (!AssertType(BlThis, &BigLongPool, 0, __FUNCTION__)) return 0;
		if (!AssertType(BlThat, &BigLongPool, 1, __FUNCTION__)) return 0;
		return ((Utils::BigLong *)BlThis)->operator<(*(Utils::BigLong *)BlThat);
	}
	bool BigLong_IsLe(void *BlThis, void *BlThat) {
		if (!AssertType(BlThis, &BigLongPool, 0, __FUNCTION__)) return 0;
		if (!AssertType(BlThat, &BigLongPool, 1, __FUNCTION__)) return 0;
		return ((Utils::BigLong *)BlThis)->operator<=(*(Utils::BigLong *)BlThat);
	}
	bool BigLong_IsGt(void *BlThis, void *BlThat) {
		if (!AssertType(BlThis, &BigLongPool, 0, __FUNCTION__)) return 0;
		if (!AssertType(BlThat, &BigLongPool, 1, __FUNCTION__)) return 0;
		return ((Utils::BigLong *)BlThis)->operator>(*(Utils::BigLong *)BlThat);
	}
	bool BigLong_IsGe(void *BlThis, void *BlThat) {
		if (!AssertType(BlThis, &BigLongPool, 0, __FUNCTION__)) return 0;
		if (!AssertType(BlThat, &BigLongPool, 1, __FUNCTION__)) return 0;
		return ((Utils::BigLong *)BlThis)->operator>=(*(Utils::BigLong *)BlThat);
	}
	unsigned long long BigLong_BitLen(void *Bl) {
		if (!AssertType(Bl, &BigLongPool, 0, __FUNCTION__)) return 0;
		return ((Utils::BigLong *)Bl)->BitLength();
	}
	void *BigLong_ModPow(void *Base, void *Exp, void *Mod) {
		if (!AssertType(Base, &BigLongPool, 0, __FUNCTION__)) return 0;
		if (!AssertType(Exp, &BigLongPool, 1, __FUNCTION__)) return 0;
		if (Mod == 0) return AddObject(new Utils::BigLong(Utils::Pow(*(Utils::BigLong *)Base, *(Utils::BigLong *)Exp)));
		if (!AssertType(Mod, &BigLongPool, 2, __FUNCTION__)) return 0;
		return AddObject(new Utils::BigLong(Utils::Pow(*(Utils::BigLong *)Base, *(Utils::BigLong *)Exp, *(Utils::BigLong *)Mod)));
	}
	void *LastErrorDataPtrW() {
		return (void *)LastError.GetData();
	}
	SizeL LastErrorLenW() {
		return LastError.Length();
	}

	unsigned long UlLastError() {
		return LastErrCode;
	}
	void *Socket_newAfTpProt(int af, int type, int prot) {
		Utils::sock::Socket *Rtn = new Utils::sock::Socket();
		LastErrCode = 0;
		try {
			Rtn->Init(af, type, prot);
			return AddObject(Rtn);
		}
		catch (Utils::sock::SockErr &Exc) {
			LastError = Exc.Msg;
			LastErrCode = Exc.ErrCode;
			return 0;
		}
	}
	bool Socket_bind(void *Sock, void *Addr) {
		if (!AssertType(Sock, &SocketPool, 0, __FUNCTION__)) return 0;
		if (!AssertType(Addr, &SockAddrPool, 1, __FUNCTION__)) return 0;
		Utils::sock::SockAddr *TheAddr = (Utils::sock::SockAddr *)Addr;
		LastErrCode = 0;
		try {
			((Utils::sock::Socket *)Sock)->bind(*TheAddr);
		}
		catch (Utils::sock::SockErr &Exc) {
			LastError = Exc.Msg;
			LastErrCode = Exc.ErrCode;
			return false;
		}
		return true;
	}
	bool Socket_connect(void *Sock, void *Addr) {
		if (!AssertType(Sock, &SocketPool, 0, __FUNCTION__)) return 0;
		if (!AssertType(Addr, &SockAddrPool, 1, __FUNCTION__)) return 0;
		Utils::sock::SockAddr *TheAddr = (Utils::sock::SockAddr *)Addr;
		LastErrCode = 0;
		try {
			((Utils::sock::Socket *)Sock)->connect(*TheAddr);
		}
		catch (Utils::sock::SockErr &Exc) {
			LastError = Exc.Msg;
			LastErrCode = Exc.ErrCode;
			return false;
		}
		return true;
	}
	bool Socket_setsockopt(void *Sock, int Lvl, int OptName, void *bArray) {
		if (!AssertType(Sock, &SocketPool, 0, __FUNCTION__)) return 0;
		if (!AssertType(bArray, &BArrPool, 3, __FUNCTION__)) return 0;
		Utils::sock::Socket *TheSock = (Utils::sock::Socket *)Sock;
		Utils::ByteArray *ThebArr = (Utils::ByteArray *)bArray;
		LastErrCode = 0;
		try {
			TheSock->setsockopt(Lvl, OptName, (void *)ThebArr->GetData(), ThebArr->Length());
		}
		catch (Utils::sock::SockErr &Exc) {
			LastError = Exc.Msg;
			LastErrCode = Exc.ErrCode;
			return false;
		}
		return true;
	}
	bool Socket_accept(void *Sock, void *Conn) {
		if (!AssertType(Sock, &SocketPool, 0, __FUNCTION__)) return 0;
		if (!AssertType(Conn, &SocketPool, 1, __FUNCTION__)) return 0;
		Utils::sock::Socket *TheSock = (Utils::sock::Socket *)Sock;
		Utils::sock::Socket *TheConn = (Utils::sock::Socket *)Conn;
		LastErrCode = 0;
		try {
			TheSock->accept(*TheConn);
		}
		catch (Utils::sock::SockErr &Exc) {
			LastError = Exc.Msg;
			LastErrCode = Exc.ErrCode;
			return false;
		}
		return true;
	}
	bool Socket_listen(void *Sock, int Backlog) {
		if (!AssertType(Sock, &SocketPool, 0, __FUNCTION__)) return 0;
		LastErrCode = 0;
		try {
			((Utils::sock::Socket *)Sock)->listen(Backlog);
		}
		catch (Utils::sock::SockErr &Exc) {
			LastError = Exc.Msg;
			LastErrCode = Exc.ErrCode;
			return false;
		}
		return true;
	}
	bool Socket_close(void *Sock) {
		if (!AssertType(Sock, &SocketPool, 0, __FUNCTION__)) return 0;
		LastErrCode = 0;
		try {
			((Utils::sock::Socket *)Sock)->close();
		}
		catch (Utils::sock::SockErr &Exc) {
			LastError = Exc.Msg;
			LastErrCode = Exc.ErrCode;
			return false;
		}
		return true;
	}
	SizeL Socket_send(void *Sock, void *bArray, int Flags) {
		if (!AssertType(Sock, &SocketPool, 0, __FUNCTION__)) return 0;
		if (!AssertType(bArray, &BArrPool, 1, __FUNCTION__)) return 0;
		Utils::ByteArray *bArr = (Utils::ByteArray *)bArray;
		LastErrCode = 0;
		try {
			SizeL Rtn = ((Utils::sock::Socket *)Sock)->send(*bArr, Flags);
			return Rtn;
		}
		catch (Utils::sock::SockErr &Exc) {
			LastError = Exc.Msg;
			LastErrCode = Exc.ErrCode;
			return 0;
		}
	}
	SizeL Socket_sendto(void *Sock, void *Addr, void *bArray, int Flags) {
		if (!AssertType(Sock, &SocketPool, 0, __FUNCTION__)) return 0;
		if (!AssertType(Addr, &SockAddrPool, 1, __FUNCTION__)) return 0;
		if (!AssertType(bArray, &BArrPool, 2, __FUNCTION__)) return 0;
		Utils::sock::SockAddr *TheAddr = (Utils::sock::SockAddr *)Addr;
		Utils::ByteArray *bArr = (Utils::ByteArray *)bArray;
		LastErrCode = 0;
		try {
			SizeL Rtn = ((Utils::sock::Socket *)Sock)->sendto(*bArr, *TheAddr, Flags);
			return Rtn;
		}
		catch (Utils::sock::SockErr &Exc) {
			LastError = Exc.Msg;
			LastErrCode = Exc.ErrCode;
			return 0;
		}
	}
	void *Socket_recv(void *Sock, SizeL Num, int Flags) {
		if (!AssertType(Sock, &SocketPool, 0, __FUNCTION__)) return 0;
		Utils::sock::Socket *TheSock = (Utils::sock::Socket *)Sock;
		LastErrCode = 0;
		try {
			return AddObject(new Utils::ByteArray(TheSock->recv(Num, Flags)));
		}
		catch (Utils::sock::SockErr &Exc) {
			LastError = Exc.Msg;
			LastErrCode = Exc.ErrCode;
			return 0;
		}
	}
	void *Socket_recvfrom(void *Sock, void *Addr, SizeL Num, int Flags) {
		if (!AssertType(Sock, &SocketPool, 0, __FUNCTION__)) return 0;
		if (!AssertType(Addr, &SockAddrPool, 1, __FUNCTION__)) return 0;
		Utils::sock::SockAddr *TheAddr = (Utils::sock::SockAddr *)Addr;
		Utils::sock::Socket *TheSock = (Utils::sock::Socket *)Sock;
		LastErrCode = 0;
		try {
			return AddObject(new Utils::ByteArray(TheSock->recvfrom(*TheAddr, Num, Flags)));
		}
		catch (Utils::sock::SockErr &Exc) {
			LastError = Exc.Msg;
			LastErrCode = Exc.ErrCode;
			return 0;
		}
	}
	void *Socket_getsockname(void *Sock) {
		if (!AssertType(Sock, &SocketPool, 0, __FUNCTION__)) return 0;
		LastErrCode = 0;
		Utils::sock::Socket *TheSock = (Utils::sock::Socket *)Sock;
		return AddObject(new Utils::sock::SockAddr(TheSock->getsockname()));
	}
	void *Socket_getpeername(void *Sock) {
		if (!AssertType(Sock, &SocketPool, 0, __FUNCTION__)) return 0;
		LastErrCode = 0;
		Utils::sock::Socket *TheSock = (Utils::sock::Socket *)Sock;
		return AddObject(new Utils::sock::SockAddr(TheSock->getpeername()));
	}
	bool Socket_settimeout(void *Sock, double Time) {
		if (!AssertType(Sock, &SocketPool, 0, __FUNCTION__)) return 0;
		LastErrCode = 0;
		try {
			((Utils::sock::Socket *)Sock)->settimeout(Time);
		}
		catch (Utils::sock::SockErr &Exc) {
			LastError = Exc.Msg;
			LastErrCode = Exc.ErrCode;
			return 0;
		}
		return true;
	}
	double Socket_gettimeout(void *Sock) {
		if (!AssertType(Sock, &SocketPool, 0, __FUNCTION__)) return 0;
		return ((Utils::sock::Socket *)Sock)->gettimeout();
	}
	bool Socket_InitMngd(void *Sock, unsigned long MidTmOut) {
		if (!AssertType(Sock, &SocketPool, 0, __FUNCTION__)) return 0;
		((Utils::sock::Socket *)Sock)->InitMngd(MidTmOut);
		return true;
	}
	void *SockAddr_newA(char *Str, unsigned short Port, unsigned long FlowInf, unsigned long ScopeId) {
		Utils::String TheStr(Str);
		Utils::sock::SockAddr *Rtn = new Utils::sock::SockAddr();
		LastErrCode = 0;
		try {
			Rtn->Init(TheStr, Port, FlowInf, ScopeId);
			return AddObject(Rtn);
		}
		catch (Utils::sock::SockErr &Exc) {
			LastError = Exc.Msg;
			LastErrCode = Exc.ErrCode;
			delete Rtn;
			return 0;
		}
	}

	void *EncProt_newS(void *Sock, void *Key, void *InitVec, unsigned long CipherNum, unsigned long ModeNum) {
		if (!AssertType(Sock, &SocketPool, 0, __FUNCTION__)) return 0;
		if (Key == 0)
		{
			Utils::sock::Socket *Tmp = (Utils::sock::Socket *)Sock;
			return AddObject(new Utils::EncProt(0, Tmp));
		}
		if (!AssertType(Key, &BArrPool, 1, __FUNCTION__)) return 0;
		if (!AssertType(InitVec, &BArrPool, 2, __FUNCTION__)) return 0;
		Utils::sock::Socket *Tmp = (Utils::sock::Socket *)Sock;
		Utils::MidEncSt *EncSt = new Utils::MidEncSt();
		EncSt->Init(*(Utils::ByteArray *)Key, Utils::LstCiph[CipherNum], Utils::LstCiphModes[ModeNum], *(Utils::ByteArray *)InitVec);
		return AddObject(new Utils::EncProt(EncSt, Tmp));
	}

	bool EncProt_Init(void *Prot, void *Key, void *InitVec, unsigned long CipherNum, unsigned long ModeNum) {
		if (!AssertType(Prot, &EncProtPool, 0, __FUNCTION__)) return 0;
		if (!AssertType(Key, &BArrPool, 1, __FUNCTION__)) return 0;
		if (!AssertType(InitVec, &BArrPool, 2, __FUNCTION__)) return 0;
		Utils::MidEncSt *EncSt = new Utils::MidEncSt();
		EncSt->Init(*(Utils::ByteArray *)Key, Utils::LstCiph[CipherNum], Utils::LstCiphModes[ModeNum], *(Utils::ByteArray *)InitVec);
		Utils::EncProt *MyProt = (Utils::EncProt *)Prot;
		if (MyProt->Enc != 0) delete MyProt->Enc;
		MyProt->Enc = EncSt;
		return true;
	}
	
	bool EncProt_Send(void *Prot, void *NoEncDat, void *EncDat) {
		if (!AssertType(Prot, &EncProtPool, 0, __FUNCTION__)) return 0;
		bool NoEncFl = false;
		if (FlPool.HasObject(NoEncDat)) NoEncFl = true;
		else if (!BArrPool.HasObject(NoEncDat))
		{
			SetErrStr("ByteArray or File", 1, __FUNCTION__);
			return false;
		}
		bool EncFl = false;
		if (FlPool.HasObject(EncDat)) EncFl = true;
		else if (!BArrPool.HasObject(EncDat))
		{
			SetErrStr("ByteArray or File", 2, __FUNCTION__);
			return false;
		}
		Utils::AbsFile NoEncAbs, EncAbs;
		if (NoEncFl) NoEncAbs.SetData((Utils::fs::FileBase *)NoEncDat);
		else if (NoEncDat) NoEncAbs.SetData((Utils::ByteArray *)NoEncDat);
		if (EncFl) EncAbs.SetData((Utils::fs::FileBase *)EncDat);
		else if (EncDat) EncAbs.SetData((Utils::ByteArray *)EncDat);
		try {
			((Utils::EncProt *)Prot)->Send(NoEncAbs, EncAbs);
		}
		catch (Utils::sock::SockErr &Exc) {
			LastError = Exc.Msg;
			LastErrCode = Exc.ErrCode;
			return false;
		}
		return true;
	}

	bool EncProt_Recv(void *Prot, void *NoEncDat, void *EncDat) {
		if (!AssertType(Prot, &EncProtPool, 0, __FUNCTION__)) return 0;
		bool NoEncFl = false;
		if (FlPool.HasObject(NoEncDat)) NoEncFl = true;
		else if (!BArrPool.HasObject(NoEncDat))
		{
			SetErrStr("ByteArray or File", 1, __FUNCTION__);
			return false;
		}
		bool EncFl = false;
		if (FlPool.HasObject(EncDat)) EncFl = true;
		else if (!BArrPool.HasObject(EncDat))
		{
			SetErrStr("ByteArray or File", 2, __FUNCTION__);
			return false;
		}
		Utils::AbsFile NoEncAbs, EncAbs;
		if (NoEncFl) NoEncAbs.SetData((Utils::fs::FileBase *)NoEncDat);
		else if (NoEncDat) NoEncAbs.SetData((Utils::ByteArray *)NoEncDat);
		if (EncFl) EncAbs.SetData((Utils::fs::FileBase *)EncDat);
		else if (EncDat) EncAbs.SetData((Utils::ByteArray *)EncDat);
		try {
			((Utils::EncProt *)Prot)->Recv(NoEncAbs, EncAbs);
		}
		catch (Utils::sock::SockErr &Exc) {
			LastError = Exc.Msg;
			LastErrCode = Exc.ErrCode;
			return false;
		}
		return true;
	}

	void *RfsFile_newA(void *Prot, char *Str, char *Mode, void *FsLock) {
		if (!AssertType(Prot, &EncProtPool, 0, __FUNCTION__)) return 0;
		if (!AssertType(FsLock, &LockPool, 3, __FUNCTION__)) return 0;
		return AddObject(new Utils::RfsFile((Utils::EncProt *)Prot, Str, Mode, (Utils::Mutex *)FsLock));
	}

	void *RdBuffFile_newF(void *Fl, unsigned long Min, unsigned long Max, unsigned long BlkLen) {
		if (!AssertType(Fl, &FlPool, 0, __FUNCTION__)) return 0;
		return AddObject(new Utils::fRdBuff((Utils::fs::FileBase *)Fl, Min, Max, BlkLen));
	}

	void *File_newA(char *fName, char *Mode) {
		void *Rtn = AddObject(Utils::fs::GetFileObj(Utils::String(fName), Utils::fs::ParseModeStr(Mode)));
		if (Rtn) return Rtn;
		LastErrCode = 24;
		LastError = Utils::UtilsGetError();
		return 0;
	}

	bool File_flush(void *FlObj) {
		if (!AssertType(FlObj, &FlPool, 0, __FUNCTION__)) return 0;
		((Utils::fs::FileBase *)FlObj)->Flush();
		return true;
	}
	
	bool File_seek(void *FlObj, long long Pos, int From) {
		if (!AssertType(FlObj, &FlPool, 0, __FUNCTION__)) return 0;
		return ((Utils::fs::FileBase *)FlObj)->Seek(Pos, From);
	}

	long long File_tell(void *FlObj) {
		if (!AssertType(FlObj, &FlPool, 0, __FUNCTION__)) return 0;
		return ((Utils::fs::FileBase *)FlObj)->Tell();
	}

	void *File_read(void *FlObj, unsigned long Num) {
		if (!AssertType(FlObj, &FlPool, 0, __FUNCTION__)) return 0;
		return AddObject(new Utils::ByteArray(((Utils::fs::FileBase *)FlObj)->Read(Num)));
	}

	unsigned long File_write(void *FlObj, void *bArray) {
		if (!AssertType(FlObj, &FlPool, 0, __FUNCTION__)) return 0;
		if (!AssertType(bArray, &BArrPool, 1, __FUNCTION__)) return 0;
		return ((Utils::fs::FileBase *)FlObj)->Write(*(Utils::ByteArray *)bArray);
	}

	bool File_close(void *FlObj) {
		if (!AssertType(FlObj, &FlPool, 0, __FUNCTION__)) return 0;
		((Utils::fs::FileBase *)FlObj)->Close();
		return true;
	}

	void *SingleMutex_new() {
		return AddObject(Utils::GetSingleMutex());
	}

	void *RWMutex_new() {
		return AddObject(Utils::GetRWMutex());
	}

	bool Lock_TryAcquire(void *Lk, bool Access) {
		if (!AssertType(Lk, &LockPool, 0, __FUNCTION__)) return 0;
		return ((Utils::Mutex *)Lk)->TryAcquire(Access);
	}

	bool Lock_Acquire(void *Lk, bool Access) {
		if (!AssertType(Lk, &LockPool, 0, __FUNCTION__)) return 0;
		((Utils::Mutex *)Lk)->Acquire(Access);
		return true;
	}

	bool Lock_Release(void *Lk, bool Access) {
		if (!AssertType(Lk, &LockPool, 0, __FUNCTION__)) return 0;
		((Utils::Mutex *)Lk)->Release(Access);
		return true;
	}

	void *RegMyHash_10(unsigned long BitLen) {
		for (BitLenNum &Elem : HashPrimes1) {
			if (Elem.BitLen == BitLen) return new Utils::BigLong(Elem.Num);
		}
		Utils::ShowError("hello Level", "we are at Pos 0");
		BitLenNum Add = { BitLen, GetHighestPrime(BitLen, 18) };
		HashPrimes1 += Add;
		return AddObject(new Utils::BigLong(Add.Num));
	}
	void *MyHash_11(void *StrBytes, unsigned long BitLen) {
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

	bool DelObj(void *Obj) {
		for (AbsObjPool *&CurType : TypePool)
			if (CurType->RemDelObject(Obj)) return true;
		return false;
	}
	void CleanHeap() {
		for (AbsObjPool *&CurType : TypePool)
			CurType->CleanPool();
	}
	void DeInit() {
		if (SafeRnd != 0)
		{
			delete SafeRnd;
			SafeRnd = 0;
			CleanHeap();
			Utils::DeInit();
		}
	}
}
