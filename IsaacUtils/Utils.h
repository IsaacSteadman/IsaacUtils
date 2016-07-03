#include "TemplateUtils.h"
namespace Utils{
	typedef unsigned char Byte;
	typedef Array<Byte> ISAACUTILS_API ByteArray;
	class wString;
	class ISAACUTILS_API String{
	private:
		SizeL AllocNum;
		char *Data;
	public:
		class ISAACUTILS_API Iterator{
		private:
			String *Str;
			SizeL Pos;
		public:
			Iterator(String *Iter, SizeL CharPos);
			Iterator &operator++();
			bool operator==(Iterator &Cmp);
			bool operator!=(Iterator &Cmp);
			char &operator*();
		};
		String();
		String(String &&Cpy);
		String(const wchar_t *StrIn);
		String(const char *StrIn);
		String(const wchar_t *StrIn, SizeL Len);
		String(const char *StrIn, SizeL Len);
		String(const String &StrCp);
		String(const char ChFill, const SizeL Len);
		~String();
		void SetLength(SizeL Len);
		bool operator==(const String &Cmp) const;
		bool operator!=(const String &Cmp) const;
		bool operator<(const String &Cmp) const;
		bool operator<=(const String &Cmp) const;
		bool operator>(const String &Cmp) const;
		bool operator>=(const String &Cmp) const;
		String operator+(const String &Add) const;
		String &operator=(const String &Cp);
		String &operator=(String &&Cp);
		String &operator=(ByteArray &&Cp);
		String &operator+=(const String &Add);
		String &operator+=(const char Add);
		explicit operator ByteArray &();
		void CopyTo(char *To, SizeL LenTo) const;
		void CopyTo(wchar_t *To, SizeL LenTo) const;
		bool Contains(const char Val) const;
		SizeL GetNumCh(const char Val) const;
		//null terminated string allocated using new wchar_t[len + 1]
		char *GetCString() const;
		//non null terminated wide character string, less computation for higher performance. NOTE: do NOT deallocate or the string object becomes invalid
		const char *GetData() const;
		bool Insert(SizeL Pos, char Val);
		bool Insert(SizeL Pos, String &Str);
		bool Remove(SizeL Pos, SizeL Num = 1);
		bool Find(SizeL &Pos, char Ch, bool PosIsStart = false) const;
		bool RFind(SizeL &Pos, char Ch, bool PosIsStart = false);
		bool Find(SizeL &Pos, String Ch, bool PosIsStart = false) const;
		bool RFind(SizeL &Pos, String Ch, bool PosIsStart = false);
		SizeL Replace(const String &FindStr, const String &Write);
		SizeL Replace(const char FindStr, const String &Write);
		SizeL Replace(const String &FindStr, const char Write);
		SizeL Replace(const char FindStr, const char Write);
		const String SubStr(SizeL Start, SizeL Stop = MAX_INT, SnzL Step = 0) const;//if actual step is positive then set Step to (actual step) - 1
		char &AtEnd();
		void ToLower();
		void ToUpper();
		String Lower() const;
		String Upper() const;
		bool IsDigit(bool Hex = false) const;
		bool IsAlpha() const;
		bool IsAlnum() const;
		char &operator[](const SizeL Pos);
		char operator[](const SizeL Pos) const;
		SizeL Length() const;
		String &operator*=(SizeL Num);
		String operator*(SizeL Num) const;
		wString wStr() const;
		Iterator begin();
		Iterator end();
	};
	class ISAACUTILS_API wString{
	private:
		SizeL AllocNum;
		wchar_t *Data;
	public:
		class ISAACUTILS_API Iterator{
		private:
			wString *Str;
			SizeL Pos;
		public:
			Iterator(wString *Iter, SizeL CharPos);
			Iterator &operator++();
			bool operator==(Iterator &Cmp);
			bool operator!=(Iterator &Cmp);
			wchar_t &operator*();
		};
		wString();
		wString(wString &&Cpy);
		wString(const wchar_t *StrIn);
		wString(const char *StrIn);
		wString(const wchar_t *StrIn, SizeL Len);
		wString(const char *StrIn, SizeL Len);
		wString(const wString &StrCp);
		wString(const wchar_t ChFill, const SizeL Len);
		~wString();
		void SetLength(SizeL Len);
		bool operator==(const wString &Cmp) const;
		bool operator!=(const wString &Cmp) const;
		bool operator<(const wString &Cmp) const;
		bool operator<=(const wString &Cmp) const;
		bool operator>(const wString &Cmp) const;
		bool operator>=(const wString &Cmp) const;
		wString operator+(const wString &Add) const;
		wchar_t operator[](const SizeL Pos) const;
		wString &operator=(const wString &Cp);
		wString &operator=(wString &&Cp);
		wString &operator+=(const wString &Add);
		wString &operator+=(const wchar_t Add);
		void CopyTo(wchar_t *To, SizeL LenTo) const;
		bool Contains(const wchar_t Val) const;
		SizeL GetNumCh(const wchar_t Val) const;
		//null terminated string allocated using new wchar_t[len + 1]
		wchar_t *GetCString() const;
		//non null terminated wide character string, less cpu intensive for higher performance. NOTE: do NOT deallocate or the string object becomes invalid
		const wchar_t *GetData() const;
		bool Insert(SizeL Pos, wchar_t Val);
		bool Insert(SizeL Pos, wString &Str);
		bool Remove(SizeL Pos, SizeL Num = 1);
		bool Find(SizeL &Pos, wchar_t Ch, bool PosIsStart = false) const;
		bool RFind(SizeL &Pos, wchar_t Ch, bool PosIsStart = false);
		bool Find(SizeL &Pos, wString Ch, bool PosIsStart = false) const;
		bool RFind(SizeL &Pos, wString Ch, bool PosIsStart = false);
		SizeL Replace(const wString &FindStr, const wString &Write);
		SizeL Replace(const wchar_t FindStr, const wString &Write);
		SizeL Replace(const wString &FindStr, const wchar_t Write);
		SizeL Replace(const wchar_t FindStr, const wchar_t Write);
		const wString SubStr(SizeL Start, SizeL Stop = MAX_INT, SnzL Step = 0) const;//if actual step is positive then set Step to (actual step) - 1
		wchar_t &AtEnd();
		void ToLower();
		void ToUpper();
		wString Lower() const;
		wString Upper() const;
		bool IsDigit(bool Hex = false) const;
		bool IsAlpha() const;
		bool IsAlnum() const;
		wchar_t &operator[](const SizeL Pos);
		SizeL Length() const;
		wString &operator*=(SizeL Num);
		wString operator*(SizeL Num) const;
		String Str() const;
		Iterator begin();
		Iterator end();
	};
	class ISAACUTILS_API BigLong {
	private:
		Array<UInt32> Longs;
		bool Sign;
	public:
		static BigLong NomMul(const BigLong &Bl1, const BigLong &Bl2);
		static BigLong KarMul(const BigLong &Bl1, const BigLong &Bl2);
		static void KarSplit(BigLong &Bl1, BigLong &Bl2, SizeL &Power, BigLong &BlRem1, BigLong &BlRem2);
		BigLong();
		BigLong(UInt32 Ul);
		BigLong(UInt64 Ull);
		BigLong(BigLong &&Cpy);
		BigLong(const BigLong &Cpy);
		BigLong(const BigLong &Cpy, SizeL BtCopy);
		BigLong(const Byte *Data, SizeL LenData);
		void IMulPow(SizeL Num);
		BigLong MulPow(SizeL Num);
		ByteArray ToByteArray();
		//LimNum is the maximum number of bytes to provide
		BigLong &IMulLim(const BigLong &MulBy, SizeL LimNum);
		SizeL RemNulls();
		Array<UInt32> &GetLongs();
		Byte &GetByte(SizeL Pos);
		bool GetBit(UInt64 Pos);
		void SetBit(UInt64 Pos, bool Set);
		BigLong PostInc(UInt32 Num);
		BigLong &PreInc(UInt32 Num);
		//Not Tested
		BigLong PostDec(UInt32 Num);
		//Not Tested
		BigLong &PreDec(UInt32 Num);
		void Minus();
		BigLong Negate() const;
		bool FromStr(const String &Str, Byte Radix = 10);
		void ToStr(String &Str, Byte Radix = 10);
		bool FromwStr(const wString &wStr, Byte Radix = 10);
		void TowStr(wString &wStr, Byte Radix = 10);
		bool IsPow2() const;
		SizeL ToSizeL() const;
		SInt64 ToLL() const;
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
		UInt64 BitLength() const;
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
		BigLong operator<<(const SizeL Shift) const;
		BigLong operator>>(const SizeL Shift) const;
		BigLong &operator<<=(const SizeL Shift);
		BigLong &operator>>=(const SizeL Shift);
		BigLong operator|(const BigLong Num) const;
		BigLong &operator|=(const BigLong Num);
		BigLong operator&(const BigLong Num) const;
		BigLong &operator&=(const BigLong Num);
		BigLong operator^(const BigLong Num) const;
		BigLong &operator^=(const BigLong Num);
		//tests for zeroness, if zero returns 1, if greater than zero returns 2, if less than zero returns 0
		Byte Zero() const;
	};
	BigLong ISAACUTILS_API Pow(BigLong Base, BigLong Exp, BigLong Mod = (UInt32)0);

	class ISAACUTILS_API Random{
	public:
		Random();
		virtual BigLong RandBit(SizeL BitLen, bool NoLessBitLen = false) = 0;
		virtual BigLong GetRand(SizeL ByteLen) = 0;
		virtual double GetRand();
		virtual BigLong GetRand(BigLong a, BigLong b) = 0;
		virtual void Seed(BigLong Val) = 0;
		virtual ~Random();
	};
	ISAACUTILS_API Random *GetCryptoRand();
	ISAACUTILS_API void DestroyRand(Random *Obj);
	//TODO: big errors in cryptography and primes
	ISAACUTILS_API BigLong RandNoMultiple(Random * Rnd, Array<BigLong> &Against, BigLong a, BigLong b, unsigned short NumQuit = 0);

	ISAACUTILS_API bool FermatBaseTest(Random *, const BigLong &Num, UInt32 NumTest = 16);

	ISAACUTILS_API BigLong GetRandPrimeProb(Random *Rnd, bool(*Test)(Random *, const BigLong &, UInt32), SizeL BitLen, UInt32 NumTimes);
	ISAACUTILS_API BigLong GetRandPrime(Random *Rnd, bool(*Test)(Random *, const BigLong &), SizeL BitLen);
	ISAACUTILS_API void XorBytes(Byte *Obj, const Byte *Param, SizeL Len);
	struct BlkCiph;
	class ISAACUTILS_API MidEncSt {
	public:
		typedef void(*CiphMode)(ByteArray &, MidEncSt *, bool);
		Array<BigLong> InstBls;
		ByteArray CurVec[2];
		BlkCiph *BlkFunc;
		CiphMode ModeFunc;
		ByteArray Key;
		void Init(ByteArray KeyIn, BlkCiph *Cipher, CiphMode Func, ByteArray InitVec);
		void Encrypt(ByteArray &Data);
		void Decrypt(ByteArray &Data);
		~MidEncSt();
	};
	ISAACUTILS_API void CmpxCBCCrypt(ByteArray &Data, MidEncSt *EncSt, bool IsEnc);
	typedef void(*CryptFunc)(MidEncSt *EncSt, Byte *Data);
	struct ISAACUTILS_API BlkCiph {
		void(*InitEncSt)(MidEncSt *EncSt);
		CryptFunc Encrypt;
		CryptFunc Decrypt;
	};
	extern ISAACUTILS_API BlkCiph BlkCipheron;
	extern ISAACUTILS_API BlkCiph BlkCipheros;
	extern ISAACUTILS_API Array<BlkCiph *> LstCiph;
	extern ISAACUTILS_API Array<MidEncSt::CiphMode> LstCiphModes;
	class ISAACUTILS_API Clock{
	private:
		UInt32 NumTimes;
		double TotTime;
		SInt64 CurClk;
		Clock(const Clock &Clk);
		Clock &operator=(const Clock &Cpy);
	public:
		static SInt64 Tps;//ticks per seconds
		Clock();
		~Clock();
		UInt32 GetNumTimes();
		double GetAvgTime();
		double GetTotalTime();
		void StartTime();
		void EndTime();
	};
	extern ISAACUTILS_API Clock *BlMulTm, *BlDivTm, *RandTm;
	class ISAACUTILS_API FuncTimer{
	private:
		Clock *Clk;
	public:
		FuncTimer(Clock *CurClk);
		~FuncTimer();
	};

	class ISAACUTILS_API UtilsThread {
	private:
		void *hThread;
		UInt32 ThreadId;
		struct ThreadParams {
			void *hThread;
			UInt32 ThreadId;
			UInt32(*Function)(void *, UInt32, void *);
			void *FunctParams;
		};
		static UInt32 OS_CALL ThreadProc(void *Params);
	public:
		UtilsThread();
		UtilsThread(UInt32(*ThreadFunc)(void *, UInt32, void *), void *FunctParams);
		UtilsThread(UInt32 ThreadId);
		UtilsThread &operator=(UtilsThread &&Copy);
		UtilsThread &operator=(const UtilsThread &Copy);
		void Init(UInt32(*ThreadFunc)(void *, UInt32, void *), void *FunctParams);
		void SetThread(UInt32 Id);
		void SetThisToCurr();
		bool Suspend();
		bool Resume();
		bool Join();
		bool ExitCurrentThread(UInt32 ExitCode = 0);
		bool Terminate(UInt32 ExitCode = 0);
		bool IsCallingThread(); // Is the thread that the object represents the same as the thread that called this function
		~UtilsThread();
	};
	extern Byte CipherNums[256];
	ISAACUTILS_API void Cipheros(ByteArray &Data, SizeL &Pos, ByteArray &Key, bool IsEnc = true);

	class ISAACUTILS_API Mutex {
	public:
		enum Access {
			ACCESS_READ = 0,
			ACCESS_WRITE = 1,
			ACCESS_QUICK = 2
		};
		//Access true for write
		virtual bool TryAcquire(bool Access = false) = 0;
		//Access true for write
		virtual void Acquire(bool Access = false) = 0;
		//Access true for write
		virtual void Release(bool Access = false) = 0;
		virtual SizeL GetType() = 0;
		virtual ~Mutex();
	};
	class ISAACUTILS_API CondVar {
	public:
		bool IsLockRef;
		bool PreWtNtfy;//Allow notifies to affect waits later on NOTE: only works on single notifies
		virtual void notify() = 0;
		virtual void notifyAll() = 0;
		virtual void wait(UInt32 Timeout = MAX_INT32, bool Access = false) = 0;
		virtual Mutex *GetInternLock() = 0;
		virtual ~CondVar();
	};
	ISAACUTILS_API CondVar *GetCondVar(Mutex *TheLock = 0);
	ISAACUTILS_API Mutex *GetSingleMutex();
	ISAACUTILS_API Mutex *GetRWMutex();
	ISAACUTILS_API void DestroyMutex(Mutex *Obj);
	ISAACUTILS_API void DestroyCond(CondVar *Obj);
	class ISAACUTILS_API Lock {
	private:
		bool Attr;
		Mutex *LockObj;
	public:
		Lock(Mutex *Obj, bool Access = false);
		Lock(Lock &&Cpy);
		Lock &operator=(Lock &&Cpy);
		~Lock();
	};
	ISAACUTILS_API void AtomicInc(UInt32 &Num);
	ISAACUTILS_API void AtomicInc(UInt64 &Num);
	ISAACUTILS_API void AtomicDec(UInt32 &Num);
	ISAACUTILS_API void AtomicDec(UInt64 &Num);
	ISAACUTILS_API void AtomicAdd(UInt32 &Num, UInt32 Add);
	ISAACUTILS_API void AtomicAdd(UInt64 &Num, UInt64 Add);
	ISAACUTILS_API void AtomicSub(UInt32 &Num, UInt32 Add);
	ISAACUTILS_API void AtomicSub(UInt64 &Num, UInt64 Add);
	//Concurrent Queue: allows one to insert stuff at the end while simultaneously retreiving from the beginning
	class ISAACUTILS_API ConQueue {//Not Tested
	public:
		struct QBlk {
			QBlk *Next;
			ByteArray Data;
		};
	private:
		Mutex *LastLock;
		CondVar *LastCond;
		QBlk *Last;
		QBlk *First;
		SizeL TotBytes;
		void ChgBytes(SizeL Amt, bool IsAdd = true);
	public:
		ConQueue();
		ByteArray GetBytes(SizeL NumBytes);
		void GetBytes(ByteArray &Into, SizeL NumBytes, SizeL At = 0);
		SizeL TryGetBytes(ByteArray &Into, SizeL NumBytes, SizeL At = 0);//Returns the position where it left off (NumBytesRead = Rtn - At and maxed at NumBytes)
		ByteArray PeekBytes(SizeL NumBytes);
		ByteArray TryGetBytes(SizeL NumBytes);
		void PutBytes(const ByteArray &Bytes);
		void PutBytes(ByteArray &&Bytes);
		//Use from the getter thread, it will interfere with the getbytes functions otherwise
		void Clear(SizeL NumBytes);
		SizeL Length();
		~ConQueue();

		void *CbObj;
		void(*GetFunc)(void *Obj, SizeL NumBytes);
	};

	ISAACUTILS_API wString FromNumber(UInt32 Num, unsigned char Radix = 10);
	ISAACUTILS_API SizeL StringHash(String Str, SizeL Range);
	ISAACUTILS_API SizeL wStringHash(wString wStr, SizeL Range);
	ISAACUTILS_API SizeL StringHash(const String &Str, SizeL Range);
	ISAACUTILS_API SizeL wStringHash(const wString &wStr, SizeL Range);
	ISAACUTILS_API SizeL CmpxNumHash(const SizeL &In, SizeL Range);
	ISAACUTILS_API SizeL NumHash(const SizeL &In, SizeL Range);
	enum ERROR_ID {
		FUNC_NONE = 0,
		FUNC_GETDRVPATH,
		FUNC_OPENFILE,
		FUNC_FSTAT,
		FUNC_LS_FSTAT,
		FUNC_LSDIR,
		FUNC_GETF_EXT,
		FUNC_LAST
	};
	extern ISAACUTILS_API wString LastError;
	extern ISAACUTILS_API UInt32 ErrorFuncId;
	extern ISAACUTILS_API bool ErrIsRead;
	ISAACUTILS_API wString UtilsGetError();
	ISAACUTILS_API bool UtilsGetIsErr();
	ISAACUTILS_API void UtilsSetError(wString Err, UInt32 FuncErrId = 0);
	/* File System namespace:
	*   Can be hooked to expose virtual file systems in the same process
	*/
	namespace fs{
		struct ISAACUTILS_API FileDesc{
			wString fName;
			UInt32 Attr;
			UInt64 Size;
			UInt64 CreateTime;
			UInt64 LastWriteTime;
			UInt64 LastAccessTime;
		};
		struct ISAACUTILS_API FileDescA{
			String fName;
			UInt32 Attr;
			UInt64 Size;
			UInt64 CreateTime;
			UInt64 LastWriteTime;
			UInt64 LastAccessTime;
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
		ISAACUTILS_API Array<wString> ListDir(wString Path);
		ISAACUTILS_API bool Exists(wString Path);
		ISAACUTILS_API bool IsFile(wString Path);
		ISAACUTILS_API bool IsDir(wString Path);
		/**
		* RETURNS: an array of wString filenames. to get the absolute path of each filename prepend the string with the [search path followed by a '/']
		*   This function will search through all the files in the path and in any sub-directories and produce the filenames
		*    with the extension Ext
		* EXAMPLE: To find all mp3 files in the user's Music directory
		*   Array<wString> Mp3Files = GetFileExt("C:/Users/John.Doe/Music", "mp3");
		*   Linux/Unix: Array<String> Mp3Files = GetFileExt("C:/home/John.Doe/Music", "mp3");
		*/
		ISAACUTILS_API Array<wString> GetFileExt(wString Path, const Array<wString> &Exts);
		ISAACUTILS_API FileDesc Stat(wString Path);
		ISAACUTILS_API Array<FileDesc> ListDirStats(wString Path);
		ISAACUTILS_API wString Getcwd();
		ISAACUTILS_API bool Setcwd(wString Path);
		ISAACUTILS_API Array<String> ListDir(String Path);
		ISAACUTILS_API bool Exists(String Path);
		ISAACUTILS_API bool IsFile(String Path);
		ISAACUTILS_API bool IsDir(String Path);
		/**
		* RETURNS: an array of String filenames. to get the absolute path of each filename prepend the search path with a '/' between
		*   the search path and the filename
		*   This function will search through all the files in the path and in any sub-directories and produce the filenames
		*    with the extension Ext
		* EXAMPLE: To find all mp3 files in the user's Music directory
		*   Windows: Array<String> Mp3Files = GetFileExt("C:/Users/John.Doe/Music", "mp3");
		*   Linux/Unix: Array<String> Mp3Files = GetFileExt("C:/home/John.Doe/Music", "mp3");
		*/
		Array<String> ISAACUTILS_API GetFileExt(String Path, const Array<String> &Exts);
		FileDescA ISAACUTILS_API Stat(String Path);
		Array<FileDescA> ISAACUTILS_API ListDirStats(String Path);
		String ISAACUTILS_API GetcwdA();
		bool ISAACUTILS_API Setcwd(String Path);
		wString ISAACUTILS_API GetFullPath(wString Path);
		enum SEEK_POS {
			SK_SET = 0,
			SK_CUR = 1,
			SK_END = 2
		};
		class ISAACUTILS_API FileError {
		public:
			FileError();
			wString Msg;
			wString Type;
			FileError(wString Cap, wString Txt);
		};
		//abstract file class representing a file from a drive
		class ISAACUTILS_API FileBase {
		public:
//			FileBase();
			virtual ByteArray Read() = 0;
			virtual ByteArray Read(UInt32 Num) = 0;
			// Does not modify the length of Data
			virtual UInt32 Read(ByteArray &Data) = 0;
			virtual bool Seek(SInt64 Pos, SInt32 From = SK_SET) = 0;
			virtual SInt64 Tell() = 0;
			virtual UInt32 Write(const ByteArray &Data) = 0;
			virtual void Close() = 0;
			virtual void Flush() = 0;
			virtual wString GetName() = 0;
			virtual UInt32 GetMode() = 0;
			virtual ~FileBase();
		};
		class ISAACUTILS_API DriveBase {
		public:
//			DriveBase();
			FileError Err;
			bool ErrNotRead;
			virtual wString GetName() = 0;
			virtual String GetNameA() = 0;
			virtual FileBase *OpenFile(const wString &Path, UInt32 Mode) = 0;
			virtual FileBase *OpenFile(const String &Path, UInt32 Mode) = 0;
			virtual bool IsFile(const wString &Path) = 0;
			virtual bool IsFile(const String &Path) = 0;
			virtual bool Exists(const wString &Path) = 0;
			virtual bool Exists(const String &Path) = 0;
			virtual bool IsDir(const wString &Path) = 0;
			virtual bool IsDir(const String &Path) = 0;
			virtual Array<wString> ListDir(const wString &Path) = 0;
			virtual Array<String> ListDir(const String &Path) = 0;
			virtual FileDesc Stat(const wString &Path) = 0;
			virtual FileDescA Stat(const String &Path) = 0;
			virtual Array<FileDesc> ListDirSt(const wString &Path) = 0;
			virtual Array<FileDescA> ListDirSt(const String &Path) = 0;
			virtual Array<wString> GetFileExt(const wString &Path, const Array<wString> &Ext, bool Invert = false, bool RtnBegDots = false) = 0;
			virtual Array<String> GetFileExt(const String &Path, const Array<String> &Ext, bool Invert = false, bool RtnBegDots = false) = 0;
		};
		bool ISAACUTILS_API GetIsError(DriveBase *Drv);
		enum OpenMode {
			F_IN = 1,
			F_OUT = 2,
			F_BIN = 4,
			F_APP = 8,
			F_ATE = 16,
			F_NOREPLACE = 32,
			F_TRUNC = 64
		};
		extern ISAACUTILS_API HashMap<wString, DriveBase *> DriveMap;
		ISAACUTILS_API FileBase *GetFileObj(wString Path, UInt32 Mode = F_IN);
		ISAACUTILS_API FileBase *GetFileObj(String Path, UInt32 Mode = F_IN);
		ISAACUTILS_API SInt32 GetDrvNPath(String &Path, DriveBase *&Drv);
		ISAACUTILS_API SInt32 GetDrvNPath(wString &Path, DriveBase *&Drv);
		ISAACUTILS_API UInt32 ParseModeStr(const String &ModeStr);
		ISAACUTILS_API String ParseModeLong(UInt32 Md);
	}
	namespace sock {
		enum AddrFam {
			UNSPEC = 0,
			INET = 2,
			IPX = 6,
			APPLETALK = 16,
			NETBIOS = 17,
			INET6 = 23,
			IRDA = 26,
			BTH = 32
		};
		enum Types {
			STREAM = 1,
			DGRAM = 2,
			RAW = 3,
			RDM = 4,
			SEQPACKET = 5
		};
		enum Protos {
			IP = 0,
			ICMP = 1,
			IGMP = 2,
			//Bluetooth
			RFCOMM = 3,
			TCP = 6,
			UDP = 17,
			ICMPV6 = 58,
			RM = 113
		};
		extern ISAACUTILS_API bool Usable;
		extern ISAACUTILS_API HashMap<SizeL, String> ErrCodes;
		class ISAACUTILS_API SockAddr {
		private:
			void *Data;
		public:
			void DeInitData();
			SockAddr();
			SockAddr(const SockAddr &Cpy);
			SockAddr(SockAddr &&Cpy);
			void Init(const wString &AddrStr, unsigned short Port, UInt32 FlowInf = 0, UInt32 ScopeId = 0);
			void Init(const String &AddrStr, unsigned short Port, UInt32 FlowInf = 0, UInt32 ScopeId = 0);
			SockAddr &operator=(const SockAddr &Cpy);
			SockAddr &operator=(SockAddr &&Cpy);
			SockAddr(const String &Addr, unsigned short Port);
			SockAddr(void *Bytes);
			void Init(void *Bytes);
			SInt32 GetSize() const;
			void *GetData() const;
			~SockAddr();
			String GetAddrStr();
			unsigned short GetPort();
		};
		class ISAACUTILS_API Socket {
		private:
			union DatType {
				SInt32 Fd;
				SizeL Ptr;
			} Data;
			SockAddr SockName;
			SockAddr PeerName;
			UInt32 TmOuts[2];
			Mutex *SockLock;
			//base timeout [0]; high byte is seconds, low word and other byte is microseconds
			//[1]; NumAccess except for highest bit: IsReqClose
			UInt32 MngdTmOut[2];
		public:
			Socket();
			~Socket();
			void Init(SInt32 af = UNSPEC, SInt32 Type = STREAM, SInt32 Prot = 0);
			void bind(const SockAddr &Addr);
			void connect(const SockAddr &Addr);
			void setsockopt(SInt32 Lvl, SInt32 OptName, void *Data, UInt32 DataLen);
			void accept(Socket &Sock);
			void listen(SInt32 AllowBuff);
			void close();
			void InitMngd(UInt32 MidTmOut);
			SizeL sendBase(const char *SendDat, SizeL LenDat, SInt32 Flags = 0);
			SizeL sendtoBase(const char *SendDat, SizeL LenDat, const SockAddr &Addr, SInt32 Flags = 0);
			SizeL send(const ByteArray &Bytes, SInt32 Flags = 0);
			SizeL send(const String &Bytes, SInt32 Flags = 0);
			SizeL sendto(const ByteArray &Bytes, const SockAddr &Addr, SInt32 Flags = 0);
			SizeL sendto(const String &Bytes, const SockAddr &Addr, SInt32 Flags = 0);
			SizeL recvBase(char *RecvDat, SizeL LenDat, SInt32 Flags = 0);
			//Support for reading until LenDat bytes have been read, when managed, is limited due to recvfrom behavior with other source addresses
			SizeL recvfromBase(SockAddr &Addr, char *RecvDat, SizeL LenDat, SInt32 Flags = 0);
			ByteArray recv(SizeL Num, SInt32 Flags = 0);
			String recvS(SizeL Num, SInt32 Flags = 0);
			ByteArray recvfrom(SockAddr &Addr, SizeL Num, SInt32 Flags = 0);
			String recvfromS(SockAddr &Addr, SizeL Num, SInt32 Flags = 0);
			void settimeout(double Sec);
			void setblocking(bool IsBlk);
			double gettimeout();
			SockAddr getsockname();
			SockAddr getpeername();
		};
		class ISAACUTILS_API SockErr {
		public:
			wString Msg;
			SizeL ErrCode;
			SockErr();
			SockErr(SInt32 SockErrCode);
		};

	}
	ISAACUTILS_API SizeL BtToL(const ByteArray &Bts, SizeL &Pos, SizeL Sz = MAX_INT);
	ISAACUTILS_API SizeL BtToL(const ByteArray &Bts);
	ISAACUTILS_API void WriteLToBt(SizeL Num, ByteArray &Dest, SizeL &Pos, SizeL Align = 0);
	ISAACUTILS_API ByteArray LToBt(SizeL Num, SizeL Align = 0);
	ISAACUTILS_API Array<String> SplitStr(const String &Str, SizeL HeadLen);
	ISAACUTILS_API String UnpackStrLen(const String &Str, SizeL HeadLen, SizeL &Pos);
	ISAACUTILS_API Array<String> UnpackListStrFl(fs::FileBase *Fl, SizeL HeadLen, SizeL StrHeadLen);
	ISAACUTILS_API Array<String> UnpackListStr(const String &StrSrc, SizeL HeadLen, SizeL StrHeadLen);
	ISAACUTILS_API Array<String> UnpackListStr(const String &StrSrc, SizeL HeadLen, SizeL StrHeadLen, SizeL &Pos);
	ISAACUTILS_API void PackStrLen(String &Dest, SizeL &Pos, const String &StrSrc, SizeL HeadLen);
	ISAACUTILS_API void PackListStrFl(fs::FileBase *Fl, const Array<String> &LstStr, SizeL HeadLen, SizeL StrHeadLen);
	ISAACUTILS_API void PackListStr(String &Dest, SizeL &Pos, const Array<String> &LstStr, SizeL HeadLen, SizeL StrHeadLen);
	ISAACUTILS_API bool DefDelFl(void *Fl);
	class ISAACUTILS_API fRdBuff : public fs::FileBase {
	private:
		ConQueue Buff;
		SInt64 Pos;
		UInt64 FlLen;
		fs::FileBase *FlObj;
		UInt32 MinMax[2];
		CondVar *TheCond;
		UInt32 BlkLen;
		UtilsThread Thrd;
		fs::FileError *ThrdErr;
		bool(*FreeFlCb)(void *);
	public:
		static UInt32 BuffWorker(void *hThread, UInt32 Id, void *Params);
		static void BuffGetFunc(void *Obj, SizeL NumBytes);
		fRdBuff(fs::FileBase *Fl, UInt32 Min, UInt32 Max, UInt32 BlkLen, bool Direct = false);
		void SetFlDelFunc(bool(*DelFunc)(void *));
		ByteArray Read();
		ByteArray Read(UInt32 Num);
		UInt32 Read(ByteArray &Data);
		bool Seek(SInt64 SkPos, SInt32 From = fs::SK_SET);
		SInt64 Tell();
		UInt32 Write(const ByteArray &Data);
		void Close();
		void Flush();
		void SetBuffMode(bool Direct = false);
		wString GetName();
		UInt32 GetMode();
		~fRdBuff();
	};
	class ISAACUTILS_API AbsFile {
	public:
		void *TheData;
		mutable SizeL Pos;
		UInt32 Meta[2]; //Meta[0] is Type, Meta[1] is the Allocation BlockSize or zero for allocations for exact needed length
		AbsFile();
		AbsFile(fs::FileBase *Fl);
		AbsFile(ByteArray &BArr);
		AbsFile(String &Str);
		AbsFile(const AbsFile &Cpy);
		void SetData(fs::FileBase *Fl);
		void SetData(ByteArray *BArr);
		void PreAlloc(BigLong NumAlloc);
		void Write(const ByteArray &Data);
		void Read(ByteArray &Data, SizeL Len) const;
		BigLong GetLen() const;
	};
	// if Enc == 0 the EncProt will not send EncDat
	class ISAACUTILS_API EncProt {
	public:
		sock::Socket *Sock;
		MidEncSt *Enc;
		SizeL BlkLen;
		double Timeout;
		EncProt(MidEncSt *EncSt, sock::Socket *Conn, double TmOut = 30);
		void Recv(AbsFile NoEnc, AbsFile EncDat);
		void Send(const AbsFile NoEnc, const AbsFile EncDat);
	};
	enum ReFiSys {
		FL_OPEN, FL_READ, FL_WRITE, FL_SEEK, FL_TELL, FL_LEN,
		FL_READ_DELIM, FL_CLOSE, FL_UPLOAD, FL_DOWNLOAD,
		FL_LIST, FL_DEL,
		OPT_LIST,
		OPT_EXIT,
		OPT_CHANGE_PASS,
		OPT_LST_PERMS,
		OPT_MOD_PERMS,
		SYS_RESTART,
		FL_EXIST,
		FL_ISDIR,
		FL_ISFILE,
		SERVICE_EXEC,
		SERVICE_EXECFL,
		SERVICE_LIST,
		FL_LIST_EXTS
	};
	enum gfeOpts {
		FL_LS_OPT_NOT = 0x01,
		FL_LS_OPT_BEGDOTS = 0x02
	};
	class RfsFile : public fs::FileBase {
	private:
		EncProt *Prot;
		SInt64 Pos;
		ByteArray IdStr;
		Mutex *FsLock;
		String ThisfName;
		String Md;
	private:
		void InternRead(ByteArray &Data, UInt32 Num);
		bool InternSeek(SInt64 PosIn, SInt32 From);
		SInt64 InternTell();
		UInt32 InternWrite(const ByteArray &Data);
		void InternClose();
		UInt64 InternGetLen();
	public:
		RfsFile(EncProt *Serv, const String &fName, const String &Mode, Mutex *RfsLock);
		ByteArray Read();
		ByteArray Read(UInt32 Num);
		UInt32 Read(ByteArray &Data);
		bool Seek(SInt64 PosIn, SInt32 From = fs::SK_SET);
		SInt64 Tell();
		UInt32 Write(const ByteArray &Data);
		void Close();
		void Flush();
		wString GetName();
		UInt32 GetMode();
	};
	class RfsDrv : public fs::DriveBase {//Decent TODO(Unicode support) but should(maybe) work
	private:
		void UniStrNi();
		void NoImp();
	public:
		EncProt *Prot;
		String Name;
		Mutex *FsLock;

		RfsDrv(EncProt *Serv, const String &Username);
		wString GetName();
		String GetNameA();
		fs::FileBase *OpenFile(const wString &Path, UInt32 Mode);
		fs::FileBase *OpenFile(const String &Path, UInt32 Mode);
		bool IsFile(const wString &Path);
		bool IsFile(const String &Path);
		bool Exists(const wString &Path);
		bool Exists(const String &Path);
		bool IsDir(const wString &Path);
		bool IsDir(const String &Path);
		Array<wString> ListDir(const wString &Path);
		Array<String> ListDir(const String &Path);
		fs::FileDesc Stat(const wString &Path);
		fs::FileDescA Stat(const String &Path);
		Array<fs::FileDesc> ListDirSt(const wString &Path);
		Array<fs::FileDescA> ListDirSt(const String &Path);
		Array<wString> GetFileExt(const wString &Path, const Array<wString> &Ext, bool Invert = false, bool RtnBegDots = false);
		Array<String> GetFileExt(const String &Path, const Array<String> &Ext, bool Invert = false, bool RtnBegDots = false);
	};
	ISAACUTILS_API wString GetStrNumTest(BigLong Bl);
	ISAACUTILS_API BigLong GetNumStrTest(wString Str);
	ISAACUTILS_API BigLong GetNumStrTestB(ByteArray Str);
	ISAACUTILS_API ByteArray GetStrNumTestB(BigLong Bl);
	extern ISAACUTILS_API bool IsBigEnd;
	ISAACUTILS_API void Init();
	ISAACUTILS_API void OsInit();
	ISAACUTILS_API void OsDeInit();
	ISAACUTILS_API void DeInit();
	ISAACUTILS_API SizeL wStrLen(wchar_t *wStr);
	ISAACUTILS_API SizeL StrLen(char *Str);
	ISAACUTILS_API void ShowError(wString Caption, wString Text);
	extern ISAACUTILS_API const Utils::BigLong Two;
	extern ISAACUTILS_API const Utils::BigLong Six;
	extern ISAACUTILS_API const Utils::BigLong One;
	extern ISAACUTILS_API const Utils::BigLong Zero;
}
