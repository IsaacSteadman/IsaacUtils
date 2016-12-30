#include "stdafx.h"
#include "Utils.h"
namespace Utils {
	void XorBytes(Byte *Modify, const Byte *Param, SizeL Len) {
		SizeL c;
		if (Len % sizeof(SizeL) > 0)
		{
			for (c = Len - Len % sizeof(SizeL); c < Len; ++c) {
				Modify[c] ^= Param[c];
			}
		}
		Len /= sizeof(SizeL);
		for (c = 0; c < Len; ++c) {
			((SizeL *)Modify)[c] ^= ((SizeL *)Param)[c];
		}
	}
	Byte CipherNums[256];
	void Cipheros(ByteArray &Data, SizeL &Pos, ByteArray &Key, bool IsEnc) {
		if (IsEnc)
		{
			for (SizeL c = 0; c < Key.Length(); ++c, ++Pos) {
				UInt32 Num = Data[Pos];
				Num += 1;
				UInt32 NumK = Key[c];
				NumK += 1;
				Num *= NumK;
				Num %= 257;
				Data[Pos] = Num - 1;
			}
		}
		else
		{
			for (SizeL c = 0; c < Key.Length(); ++c, ++Pos) {
				UInt32 Num = Data[Pos];
				Num += 1;
				UInt32 NumK = CipherNums[Key[c]];
				NumK += 1;
				Num *= NumK;
				Num %= 257;
				Data[Pos] = Num - 1;
			}
		}
	}
	void CipherosEnc(MidEncSt *EncSt, Byte *Data) {
		for (SizeL c = 0; c < EncSt->Key.Length(); ++c) {
			UInt32 Num = Data[c];
			Num += 1;
			UInt32 NumK = EncSt->Key[c];
			NumK += 1;
			Num *= NumK;
			Num %= 257;
			Data[c] = Num - 1;
		}
	}
	void CipherosDec(MidEncSt *EncSt, Byte *Data) {
		for (SizeL c = 0; c < EncSt->Key.Length(); ++c) {
			UInt32 Num = Data[c];
			Num += 1;
			UInt32 NumK = CipherNums[EncSt->Key[c]];
			NumK += 1;
			Num *= NumK;
			Num %= 257;
			Data[c] = Num - 1;
		}
	}
	//#GC-CHECK delete
	BigLong *ExtEuclidAlg(const BigLong &a, const BigLong &b) {
		BigLong *Rtn = new BigLong[3];
		if (b.Zero() == 1)
		{
			Rtn[0] = One;
			Rtn[1] = Zero;
			Rtn[2] = a;
		}
		else
		{
			BigLong *Get = ExtEuclidAlg(b, a % b);
			Rtn[2] = (BigLong &&)Get[2];
			Rtn[1] = Rtn[0] - Rtn[1] * (a / b);
			Rtn[0] = (BigLong &&)Rtn[1];
			delete[] Get;
		}
		return Rtn;
	}
	BigLong ModInvEuclid(const BigLong &a, const BigLong &m) {
		BigLong *Get = ExtEuclidAlg(a, m);
		BigLong Rtn = Get[0] % Get[1];
		if (Rtn.Zero() == 0) Rtn += Get[1];
		delete[] Get;
		return Rtn;
	}
	UInt32 CipheronMasks[4] = { 0x00000001, 0x000001FF, 0x0001FFFF, 0x01FFFFFF };
	void CipheronBase(BigLong &Data, const BigLong &OptKey, SizeL ByteLenKey) {
		SizeL Lim = Data.GetLongs().Length() * 4;
		Data <<= 1;
		Data.GetLongs()[0] |= 1;
		Data.IMulLim(OptKey, Lim);
		Array<UInt32> &Mine = Data.GetLongs();
		Mine.AtEnd() &= CipheronMasks[ByteLenKey % 4];
		Data >>= 1;
	}
	void CipheronEnc(MidEncSt *EncSt, Byte *Data) {
		BigLong BlData(Data, EncSt->Key.Length());
		CipheronBase(BlData, EncSt->InstBls[0], EncSt->Key.Length());
	}
	void CipheronDec(MidEncSt *EncSt, Byte *Data) {
		BigLong BlData(Data, EncSt->Key.Length());
		CipheronBase(BlData, EncSt->InstBls[1], EncSt->Key.Length());
	}
	void CipheronInit(MidEncSt *EncSt) {
		EncSt->InstBls.SetLength(3);
		EncSt->InstBls[0] = BigLong(EncSt->Key.GetData(), EncSt->Key.Length());
		EncSt->InstBls[0] <<= 1;
		EncSt->InstBls[0] += One;
		EncSt->InstBls[2] = Two;
		EncSt->InstBls[2].IMulPow(EncSt->Key.Length() / 4);
		EncSt->InstBls[2] <<= (EncSt->Key.Length() % 4 * 8);
		EncSt->InstBls[1] = ModInvEuclid(EncSt->InstBls[0], EncSt->InstBls[2]);
	}
	BlkCiph BlkCipheron = { CipheronInit, CipheronEnc, CipheronDec };
	BlkCiph BlkCipheros = { 0, CipherosEnc, CipherosDec };
	MidEncSt::~MidEncSt() {}
	void MidEncSt::Init(ByteArray KeyIn, BlkCiph *Cipher, CiphMode Func, ByteArray InitVec) {
		Key = KeyIn;
		BlkFunc = Cipher;
		ModeFunc = Func;
		CurVec[0] = InitVec;
		CurVec[1] = InitVec;
		if (BlkFunc->InitEncSt) BlkFunc->InitEncSt(this);
	}
	void MidEncSt::Encrypt(ByteArray &Data) {
		ModeFunc(Data, this, true);
	}
	void MidEncSt::Decrypt(ByteArray &Data) {
		ModeFunc(Data, this, false);
	}
	void CmpxCBCCrypt(ByteArray &Data, MidEncSt *EncSt, bool IsEnc) {
		if (IsEnc)
		{
			for (SizeL c = 0; c < Data.Length(); c += EncSt->Key.Length()) {
				XorBytes(&Data[c], EncSt->CurVec[0].GetData(), EncSt->CurVec[0].Length());
				EncSt->BlkFunc->Encrypt(EncSt, &Data[c]);
				EncSt->CurVec[0] = ByteArray(&Data[c], EncSt->Key.Length());
			}
		}
		else
		{
			for (SizeL c = 0; c < Data.Length(); c += EncSt->Key.Length()) {
				ByteArray Tmp = EncSt->CurVec[1];
				EncSt->CurVec[1] = ByteArray(&Data[c], EncSt->Key.Length());
				EncSt->BlkFunc->Decrypt(EncSt, &Data[c]);
				XorBytes(&Data[c], Tmp.GetData(), Tmp.Length());
			}
		}
	}
}
