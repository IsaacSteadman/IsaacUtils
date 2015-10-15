#include "TemplateUtils.h"
#if defined(_WIN64)
typedef unsigned long long HCRYPTPROV;
#else
typedef unsigned long HCRYPTPROV;
#endif

#ifndef ISAACUTILS_API
#ifdef ISAACUTILS_EXPORTS
#define ISAACUTILS_API __declspec(dllexport)
#else
#define ISAACUTILS_API __declspec(dllimport)
#endif
#endif

namespace Utils{
	class wString;
	class ISAACUTILS_API String{
	private:
		unsigned long AllocNum;
		char *Data;
	public:
		class Iterator{
		private:
			String *Str;
			unsigned long Pos;
		public:
			Iterator(String *Iter, unsigned long CharPos);
			Iterator &operator++();
			bool operator==(Iterator &Cmp);
			bool operator!=(Iterator &Cmp);
			char &operator*();
		};
		String();
		String(String &&Cpy);
		String(const wchar_t *StrIn);
		String(const char *StrIn);
		String(const wchar_t *StrIn, unsigned long Len);
		String(const char *StrIn, unsigned long Len);
		String(const String &StrCp);
		String(const char ChFill, const unsigned long Len);
		~String();
		bool operator==(const String &Cmp) const;
		bool operator!=(const String &Cmp) const;
		bool operator<(const String &Cmp) const;
		bool operator<=(const String &Cmp) const;
		bool operator>(const String &Cmp) const;
		bool operator>=(const String &Cmp) const;
		String operator+(const String &Add) const;
		String &operator=(const String &Cp);
		String &operator=(String &&Cp);
		String &operator+=(const String &Add);
		String &operator+=(const char Add);
		bool Contains(const char Val) const;
		unsigned long GetNumCh(const char Val) const;
		//null terminated string allocated using new wchar_t[len + 1]
		char *GetCString() const;
		//non null terminated wide character string, less cpu intensive for higher performance. NOTE: do NOT deallocate or the string object becomes invalid
		const char *GetData() const;
		bool Insert(unsigned long Pos, char Val);
		bool Insert(unsigned long Pos, String &Str);
		bool Remove(unsigned long Pos, unsigned long Num = 1);
		bool Find(unsigned long &Pos, char Ch, bool PosIsStart = false) const;
		bool RFind(unsigned long &Pos, char Ch, bool PosIsStart = false);
		bool Find(unsigned long &Pos, String Ch, bool PosIsStart = false) const;
		bool RFind(unsigned long &Pos, String Ch, bool PosIsStart = false);
		String SubStr(unsigned long Start, unsigned long Stop = 0xFFFFFFFF, signed long Step = 0) const;//if actual step is positive then set Step to (actual step) - 1
		char &AtEnd();
		void ToLower();
		void ToUpper();
		String Lower() const;
		String Upper() const;
		bool IsDigit(bool Hex = false) const;
		bool IsAlpha() const;
		bool IsAlnum() const;
		char &operator[](const unsigned long Pos);
		char operator[](const unsigned long Pos) const;
		unsigned long Length() const;
		String &operator*=(unsigned long Num);
		String operator*(unsigned long Num) const;
		wString wStr() const;
		Iterator begin();
		Iterator end();
	};
	class ISAACUTILS_API wString{
	private:
		unsigned long AllocNum;
		wchar_t *Data;
	public:
		class Iterator{
		private:
			wString *Str;
			unsigned long Pos;
		public:
			Iterator(wString *Iter, unsigned long CharPos);
			Iterator &operator++();
			bool operator==(Iterator &Cmp);
			bool operator!=(Iterator &Cmp);
			wchar_t &operator*();
		};
		wString();
		wString(wString &&Cpy);
		wString(const wchar_t *StrIn);
		wString(const char *StrIn);
		wString(const wchar_t *StrIn, unsigned long Len);
		wString(const char *StrIn, unsigned long Len);
		wString(const wString &StrCp);
		wString(const wchar_t ChFill, const unsigned long Len);
		~wString();
		bool operator==(const wString &Cmp) const;
		bool operator!=(const wString &Cmp) const;
		bool operator<(const wString &Cmp) const;
		bool operator<=(const wString &Cmp) const;
		bool operator>(const wString &Cmp) const;
		bool operator>=(const wString &Cmp) const;
		wString operator+(const wString &Add) const;
		wchar_t operator[](const unsigned long Pos) const;
		wString &operator=(const wString &Cp);
		wString &operator=(wString &&Cp);
		wString &operator+=(const wString &Add);
		wString &operator+=(const wchar_t Add);
		bool Contains(const wchar_t Val) const;
		unsigned long GetNumCh(const wchar_t Val) const;
		//null terminated string allocated using new wchar_t[len + 1]
		wchar_t *GetCString() const;
		//non null terminated wide character string, less cpu intensive for higher performance. NOTE: do NOT deallocate or the string object becomes invalid
		const wchar_t *GetData() const;
		bool Insert(unsigned long Pos, wchar_t Val);
		bool Insert(unsigned long Pos, wString &Str);
		bool Remove(unsigned long Pos, unsigned long Num = 1);
		bool Find(unsigned long &Pos, wchar_t Ch, bool PosIsStart = false) const;
		bool RFind(unsigned long &Pos, wchar_t Ch, bool PosIsStart = false);
		bool Find(unsigned long &Pos, wString Ch, bool PosIsStart = false) const;
		bool RFind(unsigned long &Pos, wString Ch, bool PosIsStart = false);
		wString SubStr(unsigned long Start, unsigned long Stop = 0xFFFFFFFF, signed long Step = 0) const;//if actual step is positive then set Step to (actual step) - 1
		wchar_t &AtEnd();
		void ToLower();
		void ToUpper();
		wString Lower() const;
		wString Upper() const;
		bool IsDigit(bool Hex = false) const;
		bool IsAlpha() const;
		bool IsAlnum() const;
		wchar_t &operator[](const unsigned long Pos);
		unsigned long Length() const;
		wString &operator*=(unsigned long Num);
		wString operator*(unsigned long Num) const;
		String Str() const;
		Iterator begin();
		Iterator end();
	};
	typedef unsigned char Byte;
	class ISAACUTILS_API BigLong{
	private:
		Array<unsigned long> Longs;
		bool Sign;
		//void (*DtorProc)(Array<unsigned long> &, bool &); //First is the reference to the array of longs, second is the refence to the sign byte/bit
	public:
		static BigLong NomMul(const BigLong &Bl1, const BigLong &Bl2);
		static BigLong KarMul(const BigLong &Bl1, const BigLong &Bl2);
		static void KarSplit(BigLong &Bl1, BigLong &Bl2, unsigned long &Power, BigLong &BlRem1, BigLong &BlRem2);
		BigLong();
		BigLong(unsigned long Ul);
		BigLong(unsigned long long Ull);
		BigLong(BigLong &&Cpy);
		BigLong(const BigLong &Cpy);
		BigLong(const BigLong &Cpy, unsigned long BtCopy);
		void IMulPow(unsigned long Num);
		BigLong MulPow(unsigned long Num);
		unsigned long RemNulls();
		Array<unsigned long> &GetLongs();
		//~BigLong();
		//void SetDtorProc(void (*Proc)(Array<unsigned long> &, bool &));
		Byte &GetByte(unsigned long Pos);
		bool GetBit(unsigned long long Pos);
		void SetBit(unsigned long long Pos, bool Set);
		BigLong PostInc(unsigned long Num);
		BigLong &PreInc(unsigned long Num);
		//Not Tested
		BigLong PostDec(unsigned long Num);
		//Not Tested
		BigLong &PreDec(unsigned long Num);
		void Minus();
		BigLong Negate() const;
		bool FromStr(String &Str, Byte Radix = 10);
		void ToStr(String &Str, Byte Radix = 10);
		bool FromwStr(wString &wStr, Byte Radix = 10);
		void TowStr(wString &wStr, Byte Radix = 10);
		bool IsPow2() const;
		//returns an array of 2 BigLongs that must be deallocated with delete []
		//the array returned is in the form {Quotient, Remainder}
		BigLong* DivRem(const BigLong &Denom) const;
		bool operator==(const BigLong &Cmp) const;
		bool operator!=(const BigLong &Cmp) const;
		bool operator<=(const BigLong &Cmp) const;
		bool operator>=(const BigLong &Cmp) const;
		bool operator<(const BigLong &Cmp) const;
		bool operator>(const BigLong &Cmp) const;
		BigLong &operator=(BigLong &&Cpy);
		BigLong &operator=(const BigLong &Cpy);
		unsigned long long BitLength() const;
		BigLong &operator*=(BigLong Num);
		BigLong operator*(BigLong Num) const;
		BigLong operator+(const BigLong Add) const;
		BigLong &operator+=(const BigLong Add);
		BigLong &operator%=(const BigLong Num);
		BigLong operator%(const BigLong Num) const;
		BigLong &operator/=(const BigLong Num);
		BigLong operator/(const BigLong Num) const;
		BigLong &operator-=(const BigLong Num);
		BigLong operator-(BigLong Num) const;
		BigLong operator<<(const unsigned long Shift) const;
		BigLong operator>>(const unsigned long Shift) const;
		BigLong &operator<<=(const unsigned long Shift);
		BigLong &operator>>=(const unsigned long Shift);
		BigLong operator|(const BigLong Num) const;
		BigLong &operator|=(const BigLong Num);
		BigLong operator&(const BigLong Num) const;
		BigLong &operator&=(const BigLong Num);
		BigLong operator^(const BigLong Num) const;
		BigLong &operator^=(const BigLong Num);
		//tests for zeroness, if zero returns 1, if greater than zero returns 2, if less than zero returns 0
		Byte Zero() const;
	};
	BigLong ISAACUTILS_API Pow(BigLong Base, BigLong Exp, BigLong Mod = (unsigned long)0);

	class ISAACUTILS_API Random{
	public:
		Random();
		virtual BigLong RandBit(unsigned long BitLen, bool NoLessBitLen = false) = 0;
		virtual BigLong GetRand(unsigned long ByteLen) = 0;
		virtual double GetRand() = 0;
		virtual BigLong GetRand(BigLong a, BigLong b) = 0;
		virtual void Seed(BigLong Val) = 0;
		virtual ~Random();
	};
	//TODO: big errors in cryptography and primes
	BigLong ISAACUTILS_API RandNoMultiple(Random * Rnd, Array<BigLong> &Against, BigLong a, BigLong b, unsigned short NumQuit = 0);

	bool ISAACUTILS_API TrialDivTest(Random *, const BigLong &Num);
	bool ISAACUTILS_API FermatBaseTest(Random *, const BigLong &Num, unsigned long NumTest = 16);

	BigLong ISAACUTILS_API GetRandPrimeProb(Random *Rnd, bool(*Test)(Random *, const BigLong &, unsigned long), unsigned long BitLen, unsigned long NumTimes);
	BigLong ISAACUTILS_API GetRandPrime(Random *Rnd, bool(*Test)(Random *, const BigLong &), unsigned long BitLen);

	class ISAACUTILS_API Clock{
	private:
		unsigned long NumTimes;
		double TotTime;
		unsigned long Id;
		long long CurClk;
		static Array<Clock *> Clks;
		Clock(unsigned long ClkId);
		Clock(const Clock &Clk);
		Clock &operator=(const Clock &Cpy);
		static long long Tps;//ticks per seconds
	public:
		static Clock *GetNewClock();
		static Clock *GetClock(unsigned long Id);
		~Clock();
		unsigned long GetNumTimes();
		unsigned long GetId();
		double GetAvgTime();
		double GetTotalTime();
		void StartTime();
		void EndTime();
		friend void OsInit();
	};
	class ISAACUTILS_API FuncTimer{
	private:
		Clock *Clk;
	public:
		FuncTimer(unsigned long ClkId);
		~FuncTimer();
	};

	//==========================================================begin windows specific===============================================================

	class ISAACUTILS_API CryptRandom : public Random{
	protected:
		bool IsValid;
		HCRYPTPROV Data;
	public:
		CryptRandom();
		//NoLessBitLen if true means that no numbers of lesser bit length are produced
		BigLong RandBit(unsigned long BitLen, bool NoLessBitLen = false);
		BigLong GetRand(unsigned long ByteLen);
		BigLong GetRand(BigLong a, BigLong b);
		double GetRand();
		void Seed(BigLong Val);
		~CryptRandom();
	};

	//===========================================================end windows specific================================================================

	wString FromNumber(unsigned long Num, unsigned char Radix = 10);

	namespace fs{
		struct ISAACUTILS_API FileDesc{
			wString fName;
			unsigned long Attr;
			unsigned long long Size;
			unsigned long long CreateTime;
			unsigned long long LastWriteTime;
			unsigned long long LastAccessTime;
		};
		struct ISAACUTILS_API FileDescA{
			String fName;
			unsigned long Attr;
			unsigned long long Size;
			unsigned long long CreateTime;
			unsigned long long LastWriteTime;
			unsigned long long LastAccessTime;
		};
		enum FileAttr{
			FILE_ATTR_READONLY = 0x1,
			FILE_ATTR_HIDDEN = 0x2,
			FILE_ATTR_SYSTEM = 0x4,
			FILE_ATTR_DIRECTORY = 0x10,
			FILE_ATTR_ARCHIVE = 0x20,
			FILE_ATTR_DEVICE = 0x40,
			FILE_ATTR_NORMAL = 0x80,
			FILE_ATTR_TEMPORARY = 0x100,
			FILE_ATTR_SPARSE_FILE = 0x200,
			FILE_ATTR_REPARSE_POINT = 0x400,
			FILE_ATTR_COMPRESSED = 0x800,
			FILE_ATTR_OFFLINE = 0x1000,
			FILE_ATTR_NOT_CONTENT_INDEXED = 0x2000,
			FILE_ATTR_ENCRYPTED = 0x4000,
			FILE_ATTR_INTEGRITY_STREAM = 0x8000,
			FILE_ATTR_VIRTUAL = 0x10000,
			FILE_ATTR_NO_SCRUB_DATA = 0x20000
		};
		Array<wString> ISAACUTILS_API ListDir(wString Path);
		bool ISAACUTILS_API Exists(wString Path);
		bool ISAACUTILS_API IsFile(wString Path);
		bool ISAACUTILS_API IsDir(wString Path);
		Array<wString> ISAACUTILS_API GetFileExt(wString Path, wString Ext);
		FileDesc ISAACUTILS_API Stat(wString Path);
		Array<FileDesc> ISAACUTILS_API ListDirStats(wString Path);
		wString ISAACUTILS_API Getcwd();
		bool ISAACUTILS_API Setcwd(wString Path);
		Array<String> ISAACUTILS_API ListDir(String Path);
		bool ISAACUTILS_API Exists(String Path);
		bool ISAACUTILS_API IsFile(String Path);
		bool ISAACUTILS_API IsDir(String Path);
		Array<String> ISAACUTILS_API GetFileExt(String Path, String Ext);
		FileDescA ISAACUTILS_API Stat(String Path);
		Array<FileDescA> ISAACUTILS_API ListDirStats(String Path);
		String ISAACUTILS_API GetcwdA();
		bool ISAACUTILS_API Setcwd(String Path);
	}
	wString ISAACUTILS_API GetStrNumTest(BigLong Bl);
	BigLong ISAACUTILS_API GetNumStrTest(wString Str);
	BigLong ISAACUTILS_API GetNumStrTestB(Array<Byte> Str);
	Array<Byte> ISAACUTILS_API GetStrNumTestB(BigLong Bl);
	extern bool ISAACUTILS_API IsBigEnd;
	void ISAACUTILS_API Init();
	void OsInit();
	unsigned long ISAACUTILS_API wStrLen(wchar_t *wStr);
	typedef Array<Byte> ISAACUTILS_API ByteArray;
	void ShowError(wString Caption, wString Text);
	extern const Utils::BigLong ISAACUTILS_API Two;
	extern const Utils::BigLong ISAACUTILS_API Six;
	extern const Utils::BigLong ISAACUTILS_API One;
	extern const Utils::BigLong ISAACUTILS_API Zero;
}