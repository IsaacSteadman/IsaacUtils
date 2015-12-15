#include "TemplateUtils.h"

#ifndef ISAACUTILS_API
#ifdef ISAACUTILS_EXPORTS
#define ISAACUTILS_API __declspec(dllexport)
#else
#define ISAACUTILS_API __declspec(dllimport)
#endif
#endif

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
			unsigned long Pos;
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
		Array<unsigned long> Longs;
		bool Sign;
	public:
		static BigLong NomMul(const BigLong &Bl1, const BigLong &Bl2);
		static BigLong KarMul(const BigLong &Bl1, const BigLong &Bl2);
		static void KarSplit(BigLong &Bl1, BigLong &Bl2, SizeL &Power, BigLong &BlRem1, BigLong &BlRem2);
		BigLong();
		BigLong(unsigned long Ul);
		BigLong(unsigned long long Ull);
		BigLong(BigLong &&Cpy);
		BigLong(const BigLong &Cpy);
		BigLong(const BigLong &Cpy, SizeL BtCopy);
		void IMulPow(SizeL Num);
		BigLong MulPow(SizeL Num);
		SizeL RemNulls();
		Array<unsigned long> &GetLongs();
		Byte &GetByte(SizeL Pos);
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
		bool FromStr(const String &Str, Byte Radix = 10);
		void ToStr(String &Str, Byte Radix = 10);
		bool FromwStr(const wString &wStr, Byte Radix = 10);
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
	BigLong ISAACUTILS_API Pow(BigLong Base, BigLong Exp, BigLong Mod = (unsigned long)0);

	class ISAACUTILS_API Random{
	public:
		Random();
		virtual BigLong RandBit(SizeL BitLen, bool NoLessBitLen = false) = 0;
		virtual BigLong GetRand(SizeL ByteLen) = 0;
		virtual double GetRand() = 0;
		virtual BigLong GetRand(BigLong a, BigLong b) = 0;
		virtual void Seed(BigLong Val) = 0;
		virtual ~Random();
	};
	ISAACUTILS_API Random *GetCryptoRand();
	ISAACUTILS_API void DestroyRand(Random *Obj);
	//TODO: big errors in cryptography and primes
	ISAACUTILS_API BigLong RandNoMultiple(Random * Rnd, Array<BigLong> &Against, BigLong a, BigLong b, unsigned short NumQuit = 0);

	ISAACUTILS_API bool FermatBaseTest(Random *, const BigLong &Num, unsigned long NumTest = 16);

	ISAACUTILS_API BigLong GetRandPrimeProb(Random *Rnd, bool(*Test)(Random *, const BigLong &, unsigned long), SizeL BitLen, unsigned long NumTimes);
	ISAACUTILS_API BigLong GetRandPrime(Random *Rnd, bool(*Test)(Random *, const BigLong &), SizeL BitLen);

	class ISAACUTILS_API Clock{
	private:
		unsigned long NumTimes;
		double TotTime;
		long long CurClk;
		Clock(const Clock &Clk);
		Clock &operator=(const Clock &Cpy);
	public:
		static long long Tps;//ticks per seconds
		Clock();
		~Clock();
		unsigned long GetNumTimes();
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
		unsigned long ThreadId;
		struct ThreadParams {
			void *hThread;
			unsigned long ThreadId;
			unsigned long(*Function)(void *, unsigned long, void *);
			void *FunctParams;
		};
		static unsigned long __stdcall ThreadProc(void *Params);
	public:
		UtilsThread();
		UtilsThread(unsigned long(*ThreadFunc)(void *, unsigned long, void *), void *FunctParams);
		UtilsThread(unsigned long ThreadId);
		UtilsThread &operator=(UtilsThread &&Copy);
		UtilsThread &operator=(const UtilsThread &Copy);
		void SetThread(unsigned long Id);
		void SetThisToCurr();
		bool Suspend();
		bool Resume();
		bool Join();
		bool ExitCurrentThread(unsigned long ExitCode = 0);
		bool Terminate(unsigned long ExitCode = 0);
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
		virtual void wait(unsigned long Timeout = MAX_INT32, bool Access = false) = 0;
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
		~Lock();
	};
	//Concurrent Queue: allows one to insert stuff at the end while simultaneously retreiving from the beginning
	class ISAACUTILS_API ConQueue {
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
		ByteArray PeekBytes(SizeL NumBytes);
		ByteArray TryGetBytes(SizeL NumBytes);
		void PutBytes(const ByteArray &Bytes);
		void PutBytes(ByteArray &&Bytes);
		~ConQueue();
	};/*
	class ISAACUTILS_API AsyncTask {
	public:
		bool Done;
		CondVar *TheCond;
		virtual bool TryRun() = 0;
		virtual void WaitOnce() = 0;
		virtual ~AsyncTask();
	};
	extern AsyncTask *OpThrdTasks;
	extern UtilsThread AsyncOpThrd;*/

	ISAACUTILS_API wString FromNumber(unsigned long Num, unsigned char Radix = 10);
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
	extern ISAACUTILS_API unsigned long ErrorFuncId;
	extern ISAACUTILS_API bool ErrIsRead;
	ISAACUTILS_API wString UtilsGetError();
	ISAACUTILS_API bool UtilsGetIsErr();
	ISAACUTILS_API void UtilsSetError(wString Err, unsigned long FuncErrId = 0);
	/* File System namespace:
	*   Can be hooked to expose virtual file systems in the same process
	*/
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
		ISAACUTILS_API Array<wString> GetFileExt(wString Path, wString Ext);
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
		Array<String> ISAACUTILS_API GetFileExt(String Path, String Ext);
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
			FileBase();
			virtual ByteArray Read() = 0;
			virtual ByteArray Read(unsigned long Num) = 0;
			virtual bool Seek(long long Pos, int From = SK_SET) = 0;
			virtual long long Tell() = 0;
			virtual unsigned long Write(ByteArray Data) = 0;
			virtual void Close() = 0;
			virtual wString GetName() = 0;
			virtual unsigned long GetMode() = 0;
			virtual ~FileBase();
		};
		class ISAACUTILS_API DriveBase {
		public:
			DriveBase();
			FileError Err;
			bool ErrNotRead;
			virtual wString GetName() = 0;
			virtual String GetNameA() = 0;
			virtual FileBase *OpenFile(wString Path, unsigned long Mode) = 0;
			virtual FileBase *OpenFile(String Path, unsigned long Mode) = 0;
			virtual bool IsFile(wString Path) = 0;
			virtual bool IsFile(String Path) = 0;
			virtual bool Exists(wString Path) = 0;
			virtual bool Exists(String Path) = 0;
			virtual bool IsDir(wString Path) = 0;
			virtual bool IsDir(String Path) = 0;
			virtual Array<wString> ListDir(wString Path) = 0;
			virtual Array<String> ListDir(String Path) = 0;
			virtual FileDesc Stat(wString Path) = 0;
			virtual FileDescA Stat(String Path) = 0;
			virtual Array<FileDesc> ListDirSt(wString Path) = 0;
			virtual Array<FileDescA> ListDirSt(String Path) = 0;
			virtual Array<wString> GetFileExt(wString Path, wString Ext) = 0;
			virtual Array<String> GetFileExt(String Path, String Ext) = 0;
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
		ISAACUTILS_API FileBase *GetFileObj(wString Path, unsigned long Mode = F_IN);
		ISAACUTILS_API FileBase *GetFileObj(String Path, unsigned long Mode = F_IN);
		ISAACUTILS_API signed long GetDrvNPath(String &Path, DriveBase *&Drv);
		ISAACUTILS_API signed long GetDrvNPath(wString &Path, DriveBase *&Drv);
		ISAACUTILS_API unsigned long ParseModeStr(const String &ModeStr);
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
			void Init(const wString &AddrStr, unsigned short Port, unsigned long FlowInf = 0, unsigned long ScopeId = 0);
			void Init(const String &AddrStr, unsigned short Port, unsigned long FlowInf = 0, unsigned long ScopeId = 0);
			SockAddr &operator=(const SockAddr &Cpy);
			SockAddr &operator=(SockAddr &&Cpy);
			SockAddr(const String &Addr, unsigned short Port);
			SockAddr(void *Bytes);
			void Init(void *Bytes);
			int GetSize() const;
			void *GetData() const;
			~SockAddr();
			String GetAddrStr();
			unsigned short GetPort();
		};
		class ISAACUTILS_API Socket {
		private:
			union DatType {
				int Fd;
				SizeL Ptr;
			} Data;
			SockAddr SockName;
			SockAddr PeerName;
			SizeL NumAccess;
			unsigned long TmOuts[2];
		public:
			Socket();
			void Init(int af = UNSPEC, int Type = STREAM, int Prot = 0);
			void bind(const SockAddr &Addr);
			void connect(const SockAddr &Addr);
			void setsockopt(int Lvl, int OptName, void *Data, unsigned long DataLen);
			void accept(Socket &Sock);
			void listen(int AllowBuff);
			void close();
			SizeL send(const ByteArray &Bytes, int Flags = 0);
			SizeL send(const String &Bytes, int Flags = 0);
			SizeL sendto(const ByteArray &Bytes, const SockAddr &Addr, int Flags = 0);
			SizeL sendto(const String &Bytes, const SockAddr &Addr, int Flags = 0);
			ByteArray recv(SizeL Num, int Flags = 0);
			String recvS(SizeL Num, int Flags = 0);
			ByteArray recvfrom(SockAddr &Addr, SizeL Num, int Flags = 0);
			String recvfromS(SockAddr &Addr, SizeL Num, int Flags = 0);
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
			SockErr(int SockErrCode);
		};

	}
	ISAACUTILS_API wString GetStrNumTest(BigLong Bl);
	ISAACUTILS_API BigLong GetNumStrTest(wString Str);
	ISAACUTILS_API BigLong GetNumStrTestB(ByteArray Str);
	ISAACUTILS_API ByteArray GetStrNumTestB(BigLong Bl);
	extern ISAACUTILS_API bool IsBigEnd;
	ISAACUTILS_API void Init();
	ISAACUTILS_API void OsInit();
	ISAACUTILS_API SizeL wStrLen(wchar_t *wStr);
	ISAACUTILS_API void ShowError(wString Caption, wString Text);
	extern ISAACUTILS_API const Utils::BigLong Two;
	extern ISAACUTILS_API const Utils::BigLong Six;
	extern ISAACUTILS_API const Utils::BigLong One;
	extern ISAACUTILS_API const Utils::BigLong Zero;
}