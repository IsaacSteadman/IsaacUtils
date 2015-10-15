// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the ISAACUTILS_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// ISAACUTILS_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef ISAACUTILS_EXPORTS
#define ISAACUTILS_API __declspec(dllexport)
#else
#define ISAACUTILS_API __declspec(dllimport)
#endif

#include "Utils.h"

extern "C"{
	ISAACUTILS_API void Init();
	ISAACUTILS_API void *wStr_newWLen(wchar_t *Str, unsigned long Len);
	ISAACUTILS_API void *wStr_newW(wchar_t *Str);
	ISAACUTILS_API void *wStr_newALen(char *Str, unsigned long Len);
	ISAACUTILS_API void *wStr_newA(char *Str);
	ISAACUTILS_API void *wStr_new();
	ISAACUTILS_API unsigned long wStr_Len(void *wStr);
	ISAACUTILS_API void *wStr_DataPtr(void *wStr);
	//ISAACUTILS_API void wStr_del(void *Str);
	ISAACUTILS_API void *ByteArray_new();
	ISAACUTILS_API void *ByteArray_newA(char *Str);
	ISAACUTILS_API void *ByteArray_newW(wchar_t *Str, Utils::Byte ChOpt);
	ISAACUTILS_API void *ByteArray_newWStr(void *wStr, Utils::Byte ChOpt);
	ISAACUTILS_API void *ByteArray_newBigLong(void *BLong);
	ISAACUTILS_API unsigned long ByteArray_Len(void *bArray);
	ISAACUTILS_API void *ByteArray_DataPtr(void *bArray);
	//ISAACUTILS_API void ByteArray_del(void *ByteArray);
	ISAACUTILS_API void *BigLong_newLong(long L);
	ISAACUTILS_API void *BigLong_new();
	ISAACUTILS_API void *BigLong_newByteArray(void *BArray);
	ISAACUTILS_API bool BigLong_FromString(void *Bl, void *wStr, Utils::Byte base);
	ISAACUTILS_API bool BigLong_ToString(void *Bl, void *wStr, Utils::Byte base);
	ISAACUTILS_API void *BigLong_Assign(void *BlThis, void *BlThat);
	ISAACUTILS_API void *BigLong_Add(void *BlThis, void *BlThat);
	ISAACUTILS_API void *BigLong_IAdd(void *BlThis, void *BlThat);
	ISAACUTILS_API void *BigLong_Sub(void *BlThis, void *BlThat);
	ISAACUTILS_API void *BigLong_ISub(void *BlThis, void *BlThat);
	ISAACUTILS_API void *BigLong_Mul(void *BlThis, void *BlThat);
	ISAACUTILS_API void *BigLong_IMul(void *BlThis, void *BlThat);
	ISAACUTILS_API void *BigLong_Div(void *BlThis, void *BlThat);
	ISAACUTILS_API void *BigLong_IDiv(void *BlThis, void *BlThat);
	ISAACUTILS_API void *BigLong_Mod(void *BlThis, void *BlThat);
	ISAACUTILS_API void *BigLong_IMod(void *BlThis, void *BlThat);
	ISAACUTILS_API void *BigLong_LShift(void *BlThis, unsigned long That);
	ISAACUTILS_API void *BigLong_ILShift(void *BlThis, unsigned long That);
	ISAACUTILS_API void *BigLong_RShift(void *BlThis, unsigned long That);
	ISAACUTILS_API void *BigLong_IRShift(void *BlThis, unsigned long That);
	ISAACUTILS_API void *BigLong_And(void *BlThis, void *BlThat);
	ISAACUTILS_API void *BigLong_IAnd(void *BlThis, void *BlThat);
	ISAACUTILS_API void *BigLong_Or(void *BlThis, void *BlThat);
	ISAACUTILS_API void *BigLong_IOr(void *BlThis, void *BlThat);
	ISAACUTILS_API void *BigLong_Xor(void *BlThis, void *BlThat);
	ISAACUTILS_API void *BigLong_IXor(void *BlThis, void *BlThat);
	ISAACUTILS_API bool BigLong_IsEq(void *BlThis, void *BlThat);
	ISAACUTILS_API bool BigLong_IsNe(void *BlThis, void *BlThat);
	ISAACUTILS_API bool BigLong_IsLt(void *BlThis, void *BlThat);
	ISAACUTILS_API bool BigLong_IsLe(void *BlThis, void *BlThat);
	ISAACUTILS_API bool BigLong_IsGt(void *BlThis, void *BlThat);
	ISAACUTILS_API bool BigLong_IsGe(void *BlThis, void *BlThat);
	ISAACUTILS_API unsigned long long BigLong_BitLen(void *Bl);
	//ISAACUTILS_API void BigLong_del(void * Bl);
	ISAACUTILS_API void *BigLong_ModPow(void *Base, void *Exp, void *Mod);
	ISAACUTILS_API void *RegMyHash_10(unsigned long BitLen);
	ISAACUTILS_API void *MyHash_11(void *StrBytes, unsigned long BitLen);
	ISAACUTILS_API bool DelObj(void *Obj);
	ISAACUTILS_API void CleanHeap();
	ISAACUTILS_API void DeInit();
}

