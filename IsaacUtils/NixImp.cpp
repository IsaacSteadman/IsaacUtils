#include "stdafx.h"
#include "Utils.h"
namespace Utils {
	//=====================================================BEGIN RANDOM============================================================
	class CryptRandom : public Random {
	protected:
		bool IsValid;
		fs::FileBase *Data;
	public:
		CryptRandom();
		//NoLessBitLen if true means that no numbers of lesser bit length are produced
		BigLong RandBit(SizeL BitLen, bool NoLessBitLen = false);
		BigLong GetRand(SizeL ByteLen);
		void Seed(BigLong Val);
		~CryptRandom();
	};
	//#GC-CHECK delete
	Random *GetCryptoRand() {
		return new CryptRandom();
	}
	void DestroyRand(Random *Obj) {
		delete Obj;
	}
	CryptRandom::CryptRandom() {
		Data = 0;
		Data = fs::GetFileObj(String("/dev/urandom"), fs::F_IN|fs::F_BIN);
		IsValid = Data != 0;
	}
	BigLong CryptRandom::GetRand(SizeL ByteLen) {
		if (!IsValid) return (UInt32)0;
		ByteArray Bytes = Data->Read(ByteLen);
		return BigLong(Bytes.GetData(), Bytes.Length());
	}
	BigLong CryptRandom::RandBit(SizeL BitLen, bool NoLessBitLen) {
		if (!IsValid || BitLen == 0) return (UInt32)0;
		if (NoLessBitLen && (BitLen == 1)) return (UInt32)1;
		if (NoLessBitLen) BitLen -= 1;
		UInt32 Len = (BitLen + 7) / 8;
		ByteArray Bytes = Data->Read(Len);
		// if (BitLen % 8 != 0) use an inverted mask to keep only the part of
		//   the byte that is within BitLen
		if (BitLen % 8 != 0) Bytes.AtEnd() &= (1 << BitLen % 8) - 1;
		BigLong Rtn(Bytes.GetData(), Bytes.Length());
		// since the BitLen was -= 1 if NoLessBitLen (Orig BitLen) - 1 == BitLen
		if (NoLessBitLen) Rtn.SetBit(BitLen, true);
		return Rtn;
	}
	void CryptRandom::Seed(BigLong Val) {}
	CryptRandom::~CryptRandom() {
		if (IsValid) Data->Close();
	}
	//======================================================END RANDOM=============================================================

	//=======================================================BEGIN FS==============================================================

	namespace fs {
		wString ExtPath = "\\\\?\\";
		String ExtPathA = ExtPath.Str();
		String SrchPath = "/*.*";
		FileDesc FileDescFromStat(const wString &Path, const struct stat &Stat) {
			FileDesc Rtn = {
				Path,
				Stat.st_mode,
				0,
				Stat.st_size,
				FileTime(Stat.st_ctim.tv_sec, Stat.st_ctim.tv_nsec),
				FileTime(Stat.st_mtim.tv_sec, Stat.st_mtim.tv_nsec),
				FileTime(Stat.st_atim.tv_sec, Stat.st_atim.tv_nsec)
			};
			switch (Stat.st_mode & S_IFMT) {
			case S_IFBLK: Rtn.Attr |= FileAttr::FILE_ATTR_DEVICE; break;
			case S_IFCHR: Rtn.Attr |= FileAttr::FILE_ATTR_DEVICE; break;
			case S_IFDIR: Rtn.Attr |= FileAttr::FILE_ATTR_DIRECTORY; break;
			case S_IFIFO: Rtn.Attr |= FileAttr::FILE_ATTR_VIRTUAL; break;
			case S_IFLNK: Rtn.Attr |= FileAttr::FILE_ATTR_SYSTEM; break;
			case S_IFREG: Rtn.Attr |= FileAttr::FILE_ATTR_NORMAL; break;
			case S_IFSOCK: Rtn.Attr |= FileAttr::FILE_ATTR_DEVICE; break;
			}
			return Rtn;
		}
		FileDescA FileDescFromStat(const String &Path, const struct stat &Stat) {
			FileDescA Rtn = {
				Path,
				Stat.st_mode,
				0,
				Stat.st_size,
				FileTime(Stat.st_ctim.tv_sec, Stat.st_ctim.tv_nsec),
				FileTime(Stat.st_mtim.tv_sec, Stat.st_mtim.tv_nsec),
				FileTime(Stat.st_atim.tv_sec, Stat.st_atim.tv_nsec)
			};
			switch (Stat.st_mode & S_IFMT) {
			case S_IFBLK: Rtn.Attr |= FileAttr::FILE_ATTR_DEVICE; break;
			case S_IFCHR: Rtn.Attr |= FileAttr::FILE_ATTR_DEVICE; break;
			case S_IFDIR: Rtn.Attr |= FileAttr::FILE_ATTR_DIRECTORY; break;
			case S_IFIFO: Rtn.Attr |= FileAttr::FILE_ATTR_VIRTUAL; break;
			case S_IFLNK: Rtn.Attr |= FileAttr::FILE_ATTR_SYSTEM; break;
			case S_IFREG: Rtn.Attr |= FileAttr::FILE_ATTR_NORMAL; break;
			case S_IFSOCK: Rtn.Attr |= FileAttr::FILE_ATTR_DEVICE; break;
			}
			return Rtn;
		}
		String GetcwdA() {
			const SizeL ChunkSz = 1024;
			String Rtn(char(0), ChunkSz);
			while (::getcwd((char *)Rtn.GetData(), Rtn.Length()) == NULL) {
				if (errno != ERANGE) {
					Rtn.SetLength(0);
					return Rtn;
				}
				Rtn.SetLength(Rtn.Length() + ChunkSz);
			}
			SizeL Pos = 0;
			if (Rtn.Find(Pos, '\0')) Rtn.SetLength(Pos);
			return Rtn;
		}
		bool Setcwd(String Path) {
			Path.Insert(Path.Length(), 0);
			return chdir(Path.GetData());
		}
		wString Getcwd() {
			return GetcwdA().wStr();
		}
		bool Setcwd(wString Path) {
			return Setcwd(Path.Str());
		}
		class NixFile : public FileBase {
		private:
			SInt32 hFile;
			wString fName;
			UInt32 Md;
		public:
			NixFile(wchar_t *fName, UInt32 Mode);
			NixFile(char *fName, UInt32 Mode);
			virtual ByteArray Read();
			virtual ByteArray Read(UInt32 Num);
			virtual UInt32 Read(ByteArray &Data);
			virtual bool Seek(SInt64 Pos, SInt32 From = SK_SET);
			virtual SInt64 Tell();
			virtual UInt32 Write(const ByteArray &Data);
			virtual void Close();
			virtual void Flush();
			virtual wString GetName();
			virtual UInt32 GetMode();
		};
		class NixDrive : public DefUniDrive {
		private:
			wString Name;
			String PathPartA;
			bool AllocPath(const String &Path, char *&Ptr, SizeL &Len);
		public:
			NixDrive(String Mnt, wString Name);

			virtual wString GetName();
			virtual String GetNameA();
			virtual FileBase * OpenFile(const String &Path, UInt32 Mode);
			virtual Array<String> ListDir(const String &Path);
			virtual FileDescA Stat(const String &Path);
		};
		//#GC-CHECK Ptr: delete[]
		bool NixDrive::AllocPath(const String &Path, char *&Ptr, SizeL &Len) {
			Len = Path.Length() + PathPartA.Length();
			Ptr = new char[Len+1];
			PathPartA.CopyTo(Ptr, PathPartA.Length());
			Ptr += PathPartA.Length();
			Path.CopyTo(Ptr, Path.Length());
			Ptr[Len] = 0;
			Ptr -= PathPartA.Length();
			return true;
		}
		NixDrive::NixDrive(String Mnt, wString FullName) {
			PathPartA = Mnt;
			Name = FullName;
		}
		wString NixDrive::GetName() {
			return Name;
		}
		String NixDrive::GetNameA() {
			return Name.Str();
		}
		//#GC-CHECK delete
		FileBase *NixDrive::OpenFile(const String &Path, UInt32 Mode) {
			SizeL Len = 0;
			char *Str = 0;
			if (!AllocPath(Path, Str, Len))
				throw FileErrorN(MAX_INT32, "Max Path exceded");
			FileBase *Rtn = new NixFile(Str, Mode);
			delete[] Str;
			return Rtn;
		}
		/*bool NixDrive::IsFile(const String &Path) {
			char *cPathA = 0;
			SizeL Len = 0;
			if (!AllocPath(Path, cPathA, Len))
				throw FileErrorN(MAX_INT32, "Max Path exceded");
			struct stat ResStat;
			if (::stat(cPathA, &ResStat) != 0)
			{
				delete[] cPathA;
				if (errno != ENOENT) throw FileErrorN(errno, "Bad Stat");
				return false;
			}
			delete[] cPathA;
			return (ResStat.st_mode & S_IFMT) == S_IFREG;
		}
		bool NixDrive::Exists(const String &Path) {
			char *cPathA = 0;
			SizeL Len = 0;
			if (!AllocPath(Path, cPathA, Len))
				throw FileErrorN(MAX_INT32, "Max Path exceded");
			struct stat ResStat;
			if (::stat(cPathA, &ResStat) != 0)
			{
				delete[] cPathA;
				if (errno != ENOENT) throw FileErrorN(errno, "Bad Stat");
				return false;
			}
			delete[] cPathA;
			return true;
		}
		bool NixDrive::IsDir(const String &Path) {
			char *cPathA = 0;
			SizeL Len = 0;
			if (!AllocPath(Path, cPathA, Len))
				throw FileErrorN(MAX_INT32, "Max Path exceded");
			struct stat ResStat;
			if (::stat(cPathA, &ResStat) != 0)
			{
				delete[] cPathA;
				if (errno != ENOENT) throw FileErrorN(errno, "Bad Stat");
				return false;
			}
			delete[] cPathA;
			return (ResStat.st_mode & S_IFMT) == S_IFDIR;
		}*/

		Array<String> NixDrive::ListDir(const String &Path) {
			char *cPathA = 0;
			SizeL Len = SrchPath.Length();
			SizeL ChunkSzN = 64;
			Array<String> Rtn;
			if (!AllocPath(Path, cPathA, Len))
				throw FileErrorN(MAX_INT32, "Max Path exceded");
			DIR *TheDir = ::opendir(cPathA);
			delete[] cPathA;
			if (TheDir == NULL) throw FileErrorN(errno, "Bad Dir");
			dirent *Cur;
			SizeL c = 0;
			while (Cur = ::readdir(TheDir)) {
				if (Cur->d_name[0] == '.' && (Cur->d_name[1] == '\0' || Cur->d_name[1] == '.' && Cur->d_name[2] == '\0'))
					continue;
				if (Rtn.Length() <= c)
					Rtn.SetLength(Rtn.Length() + ChunkSzN);
				Rtn[c] = Cur->d_name;
				++c;
			}
			int Err = errno;
			::closedir(TheDir);
			if (Err) throw FileErrorN(Err, "Bad ReadDir");
			if (errno) throw FileErrorN(errno, "Bad CloseDir");
			return Rtn;
		}
		FileDescA NixDrive::Stat(const String &Path) {
			char *cPath = 0;
			SizeL Len = 0;
			AllocPath(Path, cPath, Len);
			FileDescA Rtn;
			Rtn.Mode = 0;
			Rtn.CreateTime = 0;
			Rtn.LastAccessTime = 0;
			Rtn.LastWriteTime = 0;
			Rtn.Attr = 0xFFFFFFFF;
			struct stat Data;
			if (::stat(cPath, &Data))
			{
				delete[] cPath;
				throw FileErrorN(errno, "Bad Stat");
			}
			delete[] cPath;
			SizeL Pos = 0;
			Rtn = FileDescFromStat(((wString &)Path).RFind(Pos, '/') ? Path.SubStr(Pos + 1) : Path, Data);
			return Rtn;
		}
		NixFile::NixFile(wchar_t *Path, UInt32 Mode) {//OLD Version
			void;
		}
		NixFile::NixFile(char *Path, UInt32 Mode) {
			if ((Mode & F_APP) > 0 && (Mode & F_TRUNC) > 0)
				throw FileError("OpenModeError", "Cannot open file with append and truncate");
			fName = Path;
			Md = Mode;
			SInt32 Flags = 0;
			if (Mode & F_APP) Flags |= O_APPEND;
			switch (Mode & (F_IN | F_OUT)) {
			case 0:
				throw FileErrorN(EINVAL, "Bad Open: Mode must specify reading and/or writing");
				break;
			case F_IN:
				Flags = O_RDONLY;
				break;
			case F_OUT:
				Flags = O_WRONLY;
				break;
			default:
				Flags = O_RDWR;
				break;
			}
			if (Mode & F_APP) Flags |= O_APPEND;
			if (Mode & F_TRUNC) Flags |= O_TRUNC;
			if (Mode & F_NOREPLACE == 0) Flags |= O_CREAT;
			if ((hFile = ::open(Path, Flags)) == -1)
				throw FileErrorN(errno, "Bad Open");
			if (Mode & F_ATE) ::lseek(hFile, 0, SEEK_END);
		}
		ByteArray NixFile::Read() {
			ByteArray Rtn;
			struct stat Inf;
			if (::fstat(hFile, &Inf))
				throw FileErrorN(errno, "Bad Read: Bad Stat");
			UInt64 LenToEnd = Inf.st_size - Tell();
			if (LenToEnd > MAX_INT)
				throw FileErrorN(ENOMEM, "Bad Read: Not Enough Memory");
			Rtn.SetLength(LenToEnd);
			SizeL Sz = ::read(hFile, (void *)Rtn.GetData(), Rtn.Length());
			if (Sz == MAX_INT) throw FileErrorN(errno, "Bad Read");
			else if (Sz < Rtn.Length()) Rtn.SetLength(Sz);
			return Rtn;
		}
		ByteArray NixFile::Read(UInt32 Num) {
			ByteArray Rtn;
			Rtn.SetLength(Num);
			SizeL Sz = ::read(hFile, (void *)Rtn.GetData(), Rtn.Length());
			if (Sz == MAX_INT) throw FileErrorN(errno, "Bad Read");
			Rtn.SetLength(Sz);
			return Rtn;
		}
		UInt32 NixFile::Read(ByteArray &Data) {
			SizeL Sz = ::read(hFile, (void *)Data.GetData(), Data.Length());
			if (Sz == MAX_INT) throw FileErrorN(errno, "Bad Read");
			return Sz;
		}
		bool NixFile::Seek(SInt64 Pos, SInt32 From) {
			switch (From) {
			case SK_SET: From = SEEK_SET; break;
			case SK_CUR: From = SEEK_CUR; break;
			case SK_END: From = SEEK_END; break;
			default: throw FileErrorN(EINVAL, "Bad Seek: Invalid From");
			}
			return ::lseek(hFile, Pos, From) != (off_t)-1;
		}
		SInt64 NixFile::Tell() {
			return ::lseek(hFile, 0, SEEK_CUR);
		}
		UInt32 NixFile::Write(const ByteArray &Data) {
			return ::write(hFile, Data.GetData(), Data.Length());
		}
		void NixFile::Close() {
			::close(hFile);
		}
		void NixFile::Flush() {
			::fsync(hFile);
		}
		wString NixFile::GetName() {
			return fName;
		}
		UInt32 NixFile::GetMode() {
			return Md;
		}
		wString GetFullPath(wString Path) {
			String PathA = Path.Str();
			char *cPathA = PathA.GetCString();
			char *Rtn = realpath(cPathA, NULL);
			delete[] cPathA;
			PathA = Rtn;
			free(Rtn);
			return PathA.wStr();
		}
	}
	//========================================================END FS===============================================================

	void ShowError(wString Caption, wString Text) {
		String Disp = (wString("echo ") + Caption + "\necho " + Text + "\npython2 -c 'raw_input(\"press Enter to continue\"'").Str();
		char *Cmd = Disp.GetCString();
		::system(Cmd);
		delete[] Cmd;
	}
	SInt64 Clock::Tps = 0;
//	extern fs::FileBase *DbgLog;
	void OsInit() {
		sock::ErrCodes.SetHashFunc(NumHash);
		sock::ErrCodes.Put(21, "Invalid address format.");
		sock::ErrCodes.Put(10004, "The operation was interrupted.");
		sock::ErrCodes.Put(10009, "A bad file handle was passed.");
		sock::ErrCodes.Put(10013, "Permission denied.");
		sock::ErrCodes.Put(10014, "A fault occurred on the network?? or in memory");
		sock::ErrCodes.Put(10022, "An invalid operation was attempted.");
		sock::ErrCodes.Put(10035, "The socket operation would block");
		sock::ErrCodes.Put(10036, "A blocking operation is already in progress.");
		sock::ErrCodes.Put(10038, "The socket was not valid");
		sock::ErrCodes.Put(10048, "The network address is in use.");
		sock::ErrCodes.Put(10054, "The connection has been reset.");
		sock::ErrCodes.Put(10058, "The network has been shut down.");
		sock::ErrCodes.Put(10060, "The operation timed out.");
		sock::ErrCodes.Put(10061, "Connection refused.");
		sock::ErrCodes.Put(10063, "The name is too long.");
		sock::ErrCodes.Put(10064, "The host is down.");
		sock::ErrCodes.Put(10065, "The host is unreachable.");
		sock::ErrCodes.Put(11001, "Encryption state not provided for encrypted data.");
		timespec Tmp;
		clock_getres(CLOCK_MONOTONIC, &Tmp);
		if (Tmp.tv_sec != 0) Clock::Tps = 1e9 / (Tmp.tv_sec * 1e9 + Tmp.tv_nsec);
		else Clock::Tps = 1000000000 / Tmp.tv_nsec;
		fs::DriveMap.Put("/", new fs::NixDrive("", ""));
		fs::DriveMap.Put("C", new fs::NixDrive("", "C"));
//		DbgLog = fs::GetFileObj(String("C:/Users/UserName/Documents/ReFiSyS/KCltSockLog.txt"), fs::F_BIN|fs::F_OUT|fs::F_TRUNC);
	}
	void OsDeInit() {
//		DbgLog->Close();
	}
	void Clock::StartTime() {
		++NumTimes;
		timespec Numer, Denom;
		clock_gettime(CLOCK_MONOTONIC, &Numer);
		clock_getres(CLOCK_MONOTONIC, &Denom);
		if (Denom.tv_sec == 0) CurClk = double(Numer.tv_nsec) / Denom.tv_nsec + double(Numer.tv_sec) * 1e9 / Denom.tv_nsec;
		else CurClk = (Numer.tv_sec * 1e9 + Numer.tv_nsec) / (Denom.tv_sec * 1e9 + Denom.tv_nsec);
	}
	void Clock::EndTime() {
		SInt64 Later = 0;
		timespec Numer, Denom;
		clock_gettime(CLOCK_MONOTONIC, &Numer);
		clock_getres(CLOCK_MONOTONIC, &Denom);
		if (Denom.tv_sec == 0) Later = double(Numer.tv_nsec) / Denom.tv_nsec + double(Numer.tv_sec) * 1e9 / Denom.tv_nsec;
		else Later = (Numer.tv_sec * 1e9 + Numer.tv_nsec) / (Denom.tv_sec * 1e9 + Denom.tv_nsec);
		Later -= CurClk;
		double Add = Later;
		Add /= Tps;
		TotTime += Add;
	}

	//=====================================================BEGIN THREAD============================================================

	UInt32 __stdcall UtilsThread::ThreadProc(void *Params) {
		UInt32 Rtn = 0;
		{
			ThreadParams &Parameters = *((ThreadParams *)Params);
			Rtn = Parameters.Function(Parameters.hThread, Parameters.ThreadId, Parameters.FunctParams);
		}
		delete Params;
		return Rtn;
	}

	UtilsThread::UtilsThread() {
		hThread = 0;
		ThreadId = 0;
	}
	UtilsThread::UtilsThread(UInt32(*ThreadFunc)(void *, UInt32, void *), void *FunctParams) {
		ThreadParams &Params = *(new ThreadParams);
		Params.Function = ThreadFunc;
		Params.FunctParams = FunctParams;
		{
			SECURITY_ATTRIBUTES SecurityAttrib;
			SecurityAttrib.bInheritHandle = TRUE;
			SecurityAttrib.lpSecurityDescriptor = NULL;
			SecurityAttrib.nLength = sizeof(SECURITY_ATTRIBUTES);
			hThread = CreateThread(&SecurityAttrib, NULL, ThreadProc, &Params, CREATE_SUSPENDED, &ThreadId);
			Params.hThread = hThread;
			Params.ThreadId = ThreadId;
			ResumeThread(HANDLE(hThread));
		}
	}
	UtilsThread::UtilsThread(UInt32 Id) {
		ThreadId = Id;
		if (Id != 0) hThread = OpenThread(THREAD_ALL_ACCESS, TRUE, Id);
		else hThread = 0;
	}
	void UtilsThread::SetThisToCurr() {
		HANDLE hThreadTemp = GetCurrentThread();
		DuplicateHandle(GetCurrentProcess(), hThreadTemp, GetCurrentProcess(),
			&(HANDLE)hThread, THREAD_ALL_ACCESS, TRUE, DUPLICATE_SAME_ACCESS);
		ThreadId = GetCurrentThreadId();
	}
	bool UtilsThread::ExitCurrentThread(UInt32 ExitCode) {
		if (hThread == 0) return false;
		ExitThread(ExitCode);
	}
	bool UtilsThread::Resume() {
		if (hThread == 0) return false;
		ResumeThread(HANDLE(hThread));
		return true;
	}
	bool UtilsThread::Suspend() {
		if (hThread == 0) return false;
		SuspendThread(HANDLE(hThread));
		return true;
	}
	bool UtilsThread::Terminate(UInt32 ExitCode) {
		if (hThread == 0) return false;
		TerminateThread(HANDLE(hThread), ExitCode);
		return true;
	}
	UtilsThread &UtilsThread::operator=(UtilsThread &&Copy) {
		hThread = Copy.hThread;
		ThreadId = Copy.ThreadId;
		Copy.hThread = 0;
		Copy.ThreadId = 0;
		return (*this);
	}
	UtilsThread &UtilsThread::operator=(const UtilsThread &Copy) {
		ThreadId = Copy.ThreadId;
		DuplicateHandle(GetCurrentProcess(), Copy.hThread, GetCurrentProcess(),
			&(HANDLE)hThread, THREAD_ALL_ACCESS, TRUE, DUPLICATE_SAME_ACCESS);
		return *this;
	}
	void UtilsThread::Init(UInt32(*ThreadFunc)(void *, UInt32, void *), void *FunctParams) {
		ThreadParams &Params = *(new ThreadParams);
		Params.Function = ThreadFunc;
		Params.FunctParams = FunctParams;
		{
			SECURITY_ATTRIBUTES SecurityAttrib;
			SecurityAttrib.bInheritHandle = TRUE;
			SecurityAttrib.lpSecurityDescriptor = NULL;
			SecurityAttrib.nLength = sizeof(SECURITY_ATTRIBUTES);
			hThread = CreateThread(&SecurityAttrib, NULL, ThreadProc, &Params, CREATE_SUSPENDED, &ThreadId);
			Params.hThread = hThread;
			Params.ThreadId = ThreadId;
			ResumeThread(HANDLE(hThread));
		}
	}
	UtilsThread::~UtilsThread() {
		if (hThread != 0) CloseHandle((HANDLE)hThread);
	}
	bool UtilsThread::IsCallingThread() {
		return ThreadId == GetCurrentThreadId();
	}
	void UtilsThread::SetThread(UInt32 Id) {
		if (hThread != 0) CloseHandle((HANDLE)hThread);
		ThreadId = Id;
		if (Id != 0) hThread = OpenThread(THREAD_ALL_ACCESS, TRUE, Id);
		else hThread = 0;
	}
	bool UtilsThread::Join() {
		if (GetCurrentThreadId() == ThreadId) return false;
		return WaitForSingleObject(hThread, INFINITE) != 0xFFFFFFFF;
	}
	void AtomicInc(UInt32 &Num) {
		InterlockedIncrement(&Num);
	}
	void AtomicInc(UInt64 &Num) {
		InterlockedIncrement(&Num);
	}
	void AtomicDec(UInt32 &Num) {
		InterlockedDecrement(&Num);
	}
	void AtomicDec(UInt64 &Num) {
		InterlockedDecrement(&Num);
	}
	void AtomicAdd(UInt32 &Num, UInt32 Add) {
		InterlockedExchangeAdd(&Num, Add);
	}
	void AtomicAdd(UInt64 &Num, UInt64 Add) {
		InterlockedExchangeAdd(&Num, Add);
	}
	void AtomicSub(UInt32 &Num, UInt32 Add) {
		Add -= 1;
		Add = ~Add;
		InterlockedExchangeAdd(&Num, Add);
	}
	void AtomicSub(UInt64 &Num, UInt64 Add) {
		Add -= 1;
		Add = ~Add;
		InterlockedExchangeAdd(&Num, Add);
	}

	class SingleMutex : public Mutex {
	public:
		CRITICAL_SECTION Data;
		SingleMutex();
		bool TryAcquire(bool Access);
		void Acquire(bool Access);
		void Release(bool Access);
		SizeL GetType();
		~SingleMutex();
	};
	SingleMutex::SingleMutex() {
		InitializeCriticalSection(&Data);
	}
	bool SingleMutex::TryAcquire(bool Access) {
		return TryEnterCriticalSection(&Data) != 0;
	}
	void SingleMutex::Acquire(bool Access) {
		EnterCriticalSection(&Data);
	}
	void SingleMutex::Release(bool Access) {
		LeaveCriticalSection(&Data);
	}
	SizeL SingleMutex::GetType() {
		return 1;
	}
	SingleMutex::~SingleMutex() {
		DeleteCriticalSection(&Data);
	}
	class RWMutex : public Mutex {
	public:
		SRWLOCK Data;
		RWMutex();
		bool TryAcquire(bool Access);
		void Acquire(bool Access);
		void Release(bool Access);
		SizeL GetType();
		~RWMutex();
	};
	RWMutex::RWMutex() {
		InitializeSRWLock(&Data);
	}
	bool RWMutex::TryAcquire(bool Access) {
		return (Access ? TryAcquireSRWLockExclusive(&Data) : TryAcquireSRWLockShared(&Data)) != 0;
	}
	void RWMutex::Acquire(bool Access) {
		if (Access) AcquireSRWLockExclusive(&Data);
		else AcquireSRWLockShared(&Data);
	}
	void RWMutex::Release(bool Access) {
		if (Access) ReleaseSRWLockExclusive(&Data);
		else ReleaseSRWLockShared(&Data);
	}
	SizeL RWMutex::GetType() {
		return 2;
	}
	RWMutex::~RWMutex() {}
	//#GC-CHECK DestroyMutex(Rtn)
	Mutex *GetSingleMutex() {
		return new SingleMutex();
	}
	//#GC-CHECK DestroyMutex(Rtn)
	Mutex *GetRWMutex() {
		return new RWMutex();
	}
	void DestroyMutex(Mutex *Obj) {
		delete Obj;
	}
	void DestroyCond(CondVar *Obj) {
		delete Obj;
	}
	CondVar::~CondVar() {}
	class RWCondVar : public CondVar {
	private:
		CONDITION_VARIABLE Data;
		RWMutex *RWLock;
		SizeL NumWaiting;
		SizeL NumAllowWake;
		SingleMutex InfLock;
	public:
		RWCondVar(RWMutex *TheLock);
		void notify();
		void notifyAll();
		void wait(UInt32 Timeout = MAX_INT32, bool Access = false);
		Mutex *GetInternLock();
		~RWCondVar();
	};
	RWCondVar::RWCondVar(RWMutex *TheLock) {
		IsLockRef = true;
		PreWtNtfy = false;
		NumWaiting = 0;
		NumAllowWake = 0;
		RWLock = TheLock;
		InitializeConditionVariable(&Data);
	}
	void RWCondVar::notify() {
		InfLock.Acquire(false);
		if (PreWtNtfy) NumAllowWake += 1;
		else if (NumWaiting > 0 && NumAllowWake < NumWaiting) NumAllowWake += 1;
		InfLock.Release(false);
		WakeConditionVariable(&Data);
	}
	void RWCondVar::notifyAll() {
		InfLock.Acquire(false);
		NumAllowWake = NumWaiting;
		InfLock.Release(false);
		WakeAllConditionVariable(&Data);
	}
	void RWCondVar::wait(UInt32 Timeout, bool Access) {
		InfLock.Acquire(false);
		++NumWaiting;
		while (true) {
			if (NumAllowWake) break;
			InfLock.Release(false);
			SleepConditionVariableSRW(&Data, &RWLock->Data, Timeout, Access ? 0 : (CONDITION_VARIABLE_LOCKMODE_SHARED));
			InfLock.Acquire(false);
		}
		--NumAllowWake;
		--NumWaiting;
		InfLock.Release(false);
	}
	Mutex *RWCondVar::GetInternLock() {
		return RWLock;
	}
	RWCondVar::~RWCondVar() {
		if (!IsLockRef) DestroyMutex(RWLock);
	}
	class CSCondVar : public CondVar {
	private:
		CONDITION_VARIABLE Data;
		SingleMutex *CSLock;
		SizeL NumWaiting;
		SizeL NumAllowWake;
	public:
		CSCondVar(SingleMutex *TheLock);
		void notify();
		void notifyAll();
		void wait(UInt32 Timeout = MAX_INT32, bool Access = false);
		Mutex *GetInternLock();
		~CSCondVar();
	};
	CSCondVar::CSCondVar(SingleMutex *TheLock) {
		IsLockRef = true;
		PreWtNtfy = false;
		NumWaiting = 0;
		NumAllowWake = 0;
		CSLock = TheLock;
		InitializeConditionVariable(&Data);
	}
	void CSCondVar::notify() {
		if (PreWtNtfy) AtomicInc(NumAllowWake);
		else if (NumWaiting > 0 && NumAllowWake < NumWaiting) AtomicInc(NumAllowWake);
		WakeConditionVariable(&Data);
	}
	void CSCondVar::notifyAll() {
		NumAllowWake = NumWaiting;
		WakeAllConditionVariable(&Data);
	}
	void CSCondVar::wait(UInt32 Timeout, bool Access) {
		AtomicInc(NumWaiting);
		while (NumAllowWake == 0) SleepConditionVariableCS(&Data, &CSLock->Data, Timeout);
		AtomicDec(NumAllowWake);
		AtomicDec(NumWaiting);
	}
	Mutex *CSCondVar::GetInternLock() {
		return CSLock;
	}
	CSCondVar::~CSCondVar() {
		if (!IsLockRef) DestroyMutex(CSLock);
	}
	//#GC-CHECK DestroyCond(Rtn)
	CondVar *GetCondVar(Mutex *TheLock) {
		if (TheLock == 0)
		{
			CondVar *Rtn = new CSCondVar(new SingleMutex());
			Rtn->IsLockRef = false;
		}
		switch (TheLock->GetType()) {
		case 1:
			return new CSCondVar((SingleMutex *)TheLock);
		case 2:
			return new RWCondVar((RWMutex *)TheLock);
		default:
			return 0;
		}
	}
	void ConQueue::ChgBytes(SizeL Amt, bool IsAdd) {
		if (!IsAdd)
		{
			Amt ^= Amt;
			Amt += 1;
		}
#if defined(_WIN64)
		InterlockedExchangeAdd64((SnzL *)&TotBytes, (SnzL)Amt);
#else
		InterlockedExchangeAdd((SnzL *)&TotBytes, (SnzL)Amt);
#endif
	}
	//======================================================END THREAD=============================================================

	//======================================================BEGIN SOCK=============================================================
/*	fs::FileBase *DbgLog;
	UInt64 GetSysTimeNum() {
		UInt64 Rtn = 0;
		FILETIME SysTm;
		GetSystemTimeAsFileTime(&SysTm);
		Rtn = SysTm.dwHighDateTime;
		Rtn <<= 32;
		Rtn |= SysTm.dwLowDateTime;
		return Rtn;
	}
	void DbgLogData(const ByteArray &Data, Byte InfId) {
		if (!DbgLog) return;
		ByteArray HeadDat(Byte(0), 14);
		HeadDat[1] = InfId;
		HeadDat.WriteFromAt(BigLong(GetSysTimeNum()).ToByteArray(), 2, 10);
		HeadDat[10] = Data.Length() & 0xFF;
		HeadDat[11] = (Data.Length() >> 8) & 0xFF;
		HeadDat[12] = (Data.Length() >> 16) & 0xFF;
		HeadDat[13] = (Data.Length() >> 24) & 0xFF;
		DbgLog->Write(HeadDat);
		DbgLog->Write(Data);
		DbgLog->Flush();
	}
	void DbgLogData(UInt64 Data, Byte InfId) {
		if (!DbgLog) return;
		ByteArray HeadDat(Byte(0), 18);
		HeadDat[0] = 1;
		HeadDat[1] = InfId;
		HeadDat.WriteFromAt(BigLong(GetSysTimeNum()).ToByteArray(), 2, 10);
		HeadDat.WriteFromAt(BigLong(Data).ToByteArray(), 10, 18);
		DbgLog->Write(HeadDat);
		DbgLog->Flush();
	}*/
	namespace sock {
		enum DbgInfs {
			DBGINF_RECV = 0,
			DBGINF_SEND = 1,
			DBGINF_EXCRECV = 2,
			DBGINF_EXCSEND = 3,
			DBGINF_OKRECV = 4,
			DBGINF_OKSEND = 5,
			DBGINF_KYRECV = 6,
			DBGINF_KYSEND = 7,
			DBGINF_EXCKYRECV = 8,
			DBGINF_EXCKYSEND = 9,
			DBGINF_OKKYRECV = 10,
			DBGINF_OKKYSEND = 11,
			DBGINF_DHXCHG = 12
		};
		Socket::Socket() {
			Data.Ptr = INVALID_SOCKET;
			TmOuts[0] = MAX_INT32;
			TmOuts[1] = 0xFFFFFFFF;
			SockLock = 0;
			MngdTmOut[0] = 0;
			MngdTmOut[1] = 0;
		}
		Socket::~Socket() {
			if (Data.Ptr != INVALID_SOCKET) close();
			if (SockLock) DestroyMutex(SockLock);
			SockLock = 0;
		}
		void Socket::Init(SInt32 af, SInt32 Type, SInt32 Prot) {
			if ((Data.Ptr = socket(af, Type, Prot)) == INVALID_SOCKET)
				throw SockErr(WSAGetLastError());
		}
		void Socket::bind(const SockAddr &Addr) {
			if ((MngdTmOut[1] & 0xC0000000) || Data.Ptr == INVALID_SOCKET) throw SockErr(10038);
			if (::bind(Data.Ptr, (sockaddr *)Addr.GetData(), Addr.GetSize()) == SOCKET_ERROR)
				throw SockErr(WSAGetLastError());
			SockName = Addr;
		}
		void Socket::connect(const SockAddr &Addr) {
			if ((MngdTmOut[1] & 0xC0000000) || Data.Ptr == INVALID_SOCKET) throw SockErr(10038);
			if (TmOuts[0] != MAX_INT32)
			{
				fd_set Tmp;
				FD_ZERO(&Tmp);
				FD_SET(Data.Ptr, &Tmp);
				timeval TimeOut;
				TimeOut.tv_sec = TmOuts[0];
				TimeOut.tv_usec = TmOuts[1];
				SInt32 Rtn = select(0, 0, &Tmp, 0, &TimeOut);
				if (Rtn == 0) throw SockErr(10060);
				else if (Rtn == SOCKET_ERROR) throw SockErr(WSAGetLastError());
			}
			if (::connect(Data.Ptr, (sockaddr *)Addr.GetData(), Addr.GetSize()) == SOCKET_ERROR)
				throw SockErr(WSAGetLastError());
			SOCKADDR_STORAGE Mine;
			SInt32 MineLen = sizeof(Mine);
			if (::getsockname(Data.Ptr, (sockaddr *)&Mine, &MineLen) == 0) SockName.Init(&Mine);
			PeerName = Addr;
		}
		void Socket::setsockopt(SInt32 Lvl, SInt32 OptName, void *OptData, UInt32 DataLen) {
			if ((MngdTmOut[1] & 0xC0000000) || Data.Ptr == INVALID_SOCKET) throw SockErr(10038);
			if (::setsockopt(Data.Ptr, Lvl, OptName, (char*)OptData, DataLen) == SOCKET_ERROR)
				throw SockErr(WSAGetLastError());
		}
		void Socket::accept(Socket &Sock) {
			if ((MngdTmOut[1] & 0xC0000000) || Data.Ptr == INVALID_SOCKET) throw SockErr(10038);
			if (TmOuts[0] != MAX_INT32)
			{
				fd_set Tmp;
				FD_ZERO(&Tmp);
				FD_SET(Data.Ptr, &Tmp);
				timeval TimeOut;
				TimeOut.tv_sec = TmOuts[0];
				TimeOut.tv_usec = TmOuts[1];
				SInt32 Rtn = select(0, &Tmp, 0, 0, &TimeOut);
				if (Rtn == 0) throw SockErr(10060);
				else if (Rtn == SOCKET_ERROR) throw SockErr(WSAGetLastError());
			}
			SOCKADDR_STORAGE Tmp;
			SInt32 AddrLen = sizeof(Tmp);
			if ((Sock.Data.Ptr = ::accept(Data.Ptr, (sockaddr *)&Tmp, &AddrLen)) == INVALID_SOCKET)
				throw SockErr(WSAGetLastError());
			Sock.SockName = SockName;
			Sock.PeerName.Init(&Tmp);
		}
		void Socket::listen(SInt32 AllowBuff) {
			if ((MngdTmOut[1] & 0xC0000000) || Data.Ptr == INVALID_SOCKET) throw SockErr(10038);
			if (::listen(Data.Ptr, AllowBuff) == SOCKET_ERROR)
				throw SockErr(WSAGetLastError());
		}
		void Socket::close() {
			if (Data.Ptr == INVALID_SOCKET) throw SockErr(10038);
			if (SockLock)
			{
				if (SockLock->TryAcquire()) MngdTmOut[1] |= 0x40000000;
				else
				{
					MngdTmOut[1] |= 0x80000000;
					SockLock->Acquire();
				}
				SockLock->Release();
			}
			if (::closesocket(Data.Ptr) == SOCKET_ERROR)
				throw SockErr(WSAGetLastError());
			Data.Ptr = INVALID_SOCKET;
		}
		void Socket::InitMngd(UInt32 MidTmOut) {
			if ((MngdTmOut[1] & 0xC0000000) || Data.Ptr == INVALID_SOCKET) throw SockErr(10038);
			if (SockLock == 0)
			{
				UInt32 IsNoBlk = 1;
				SockLock = GetSingleMutex();
				SInt32 Rtn = ioctlsocket(Data.Ptr, FIONBIO, &IsNoBlk);
				if (Rtn == SOCKET_ERROR) throw SockErr(WSAGetLastError());
			}
			MngdTmOut[0] = MidTmOut;
		}
		SizeL Socket::sendBase(const char *SendDat, SizeL LenDat, SInt32 Flags) {
			if ((MngdTmOut[1] & 0xC0000000) || Data.Ptr == INVALID_SOCKET) throw SockErr(10038);
			if (LenDat == 0) return LenDat;
			if (SockLock)
			{
				Lock Lk(SockLock);//Released upon exiting this scope(like when throw or return)
				if (MngdTmOut[1] & 0xC0000000) throw SockErr(10054);
//				DbgLogData(ByteArray((Byte *)SendDat, LenDat), DBGINF_KYSEND);
				timeval TimeOut;
				TimeOut.tv_sec = MngdTmOut[0] >> 24;
				TimeOut.tv_usec = MngdTmOut[0] & 0x00FFFFFF;
				SInt64 OrigTmOut = TmOuts[0];
				OrigTmOut *= 1000000;
				OrigTmOut += TmOuts[1];
				SInt64 MyTmOut = TimeOut.tv_sec;
				MyTmOut *= 1000000;
				MyTmOut += TimeOut.tv_usec;
				SInt64 TmLeft = OrigTmOut;
				SizeL CurLenDat = LenDat;
				while (TmLeft > 0) {
					if (TmOuts[0] != MAX_INT32) TmLeft -= MyTmOut;
					fd_set Tmp;
					FD_ZERO(&Tmp);
					FD_SET(Data.Ptr, &Tmp);
					SInt32 Rtn = select(0, 0, &Tmp, 0, &TimeOut);
					if (MngdTmOut[1] & 0x80000000) throw SockErr(10054);
					if (Rtn == 0) continue;
					else if (Rtn == SOCKET_ERROR)
					{
						SockErr Exc(WSAGetLastError());
//						DbgLogData(Exc.ErrCode, DBGINF_EXCKYSEND);
//						DbgLogData((const ByteArray &)Exc.Msg.Str(), DBGINF_EXCKYSEND);
						throw Exc;
					}
					else
					{
						SizeL TmpRtn = 0;
						if ((Rtn = ::send(Data.Ptr, SendDat, CurLenDat, Flags)) == SOCKET_ERROR)
						{
							SockErr Exc(WSAGetLastError());
//							DbgLogData(Exc.ErrCode, DBGINF_EXCKYSEND);
//							DbgLogData((const ByteArray &)Exc.Msg.Str(), DBGINF_EXCKYSEND);
							throw Exc;
						}
						if (Rtn != 0)
						{
							TmLeft = OrigTmOut;
							CurLenDat -= Rtn;
							SendDat += Rtn;
//							DbgLogData(Rtn, DBGINF_KYSEND);
							if (!CurLenDat)
							{
//								DbgLogData(LenDat, DBGINF_OKKYSEND);
								return LenDat;
							}
						}
					}
				}
				SockErr Exc(10060);
//				DbgLogData(Exc.ErrCode, DBGINF_EXCKYSEND);
//				DbgLogData((const ByteArray &)Exc.Msg.Str(), DBGINF_EXCKYSEND);
				throw Exc;
			}
			if (TmOuts[0] != MAX_INT32)
			{
				fd_set Tmp;
				FD_ZERO(&Tmp);
				FD_SET(Data.Ptr, &Tmp);
				timeval TimeOut;
				TimeOut.tv_sec = TmOuts[0];
				TimeOut.tv_usec = TmOuts[1];
				SInt32 Rtn = select(0, 0, &Tmp, 0, &TimeOut);
				if (Rtn == 0) throw SockErr(10060);
				else if (Rtn == SOCKET_ERROR) throw SockErr(WSAGetLastError());
			}
			SizeL Rtn = 0;
			if ((Rtn = ::send(Data.Ptr, SendDat, LenDat, Flags)) == SOCKET_ERROR)
				throw SockErr(WSAGetLastError());
			return Rtn;
		}
		SizeL Socket::sendtoBase(const char *SendDat, SizeL LenDat, const SockAddr &Addr, SInt32 Flags) {
			if ((MngdTmOut[1] & 0xC0000000) || Data.Ptr == INVALID_SOCKET) throw SockErr(10038);
			if (LenDat == 0) return LenDat;
			if (SockLock)
			{
				Lock Lk(SockLock);//Released upon exiting this scope(like when throw or return)
				if (MngdTmOut[1] & 0xC0000000) throw SockErr(10054);
				timeval TimeOut;
				TimeOut.tv_sec = MngdTmOut[0] >> 24;
				TimeOut.tv_usec = MngdTmOut[0] & 0x00FFFFFF;
				SInt64 OrigTmOut = TmOuts[0];
				OrigTmOut *= 1000000;
				OrigTmOut += TmOuts[1];
				SInt64 MyTmOut = TimeOut.tv_sec;
				MyTmOut *= 1000000;
				MyTmOut += TimeOut.tv_usec;
				SInt64 TmLeft = OrigTmOut;
				SizeL CurLenDat = LenDat;
				while (TmLeft > 0) {
					if (TmOuts[0] != MAX_INT32) TmLeft -= MyTmOut;
					fd_set Tmp;
					FD_ZERO(&Tmp);
					FD_SET(Data.Ptr, &Tmp);
					SInt32 Rtn = select(0, 0, &Tmp, 0, &TimeOut);
					if (MngdTmOut[1] & 0x80000000) throw SockErr(10054);
					if (Rtn == 0) continue;
					else if (Rtn == SOCKET_ERROR)
						throw SockErr(WSAGetLastError());
					else
					{
						SizeL TmpRtn = 0;
						if ((Rtn = ::sendto(Data.Ptr, SendDat, CurLenDat, Flags, (sockaddr *)Addr.GetData(), Addr.GetSize())) == SOCKET_ERROR)
							throw SockErr(WSAGetLastError());
						if (Rtn != 0)
						{
							TmLeft = OrigTmOut;
							SendDat += Rtn;
							CurLenDat -= Rtn;
							if (!CurLenDat) return LenDat;
						}
					}
				}
				throw SockErr(10060);
			}
			if (TmOuts[0] != MAX_INT32)
			{
				fd_set Tmp;
				FD_ZERO(&Tmp);
				FD_SET(Data.Ptr, &Tmp);
				timeval TimeOut;
				TimeOut.tv_sec = TmOuts[0];
				TimeOut.tv_usec = TmOuts[1];
				SInt32 Rtn = select(0, 0, &Tmp, 0, &TimeOut);
				if (Rtn == 0) throw SockErr(10060);
				else if (Rtn == SOCKET_ERROR) throw SockErr(WSAGetLastError());
			}
			SizeL Rtn = 0;
			if ((Rtn = ::sendto(Data.Ptr, SendDat, LenDat, Flags, (sockaddr *)Addr.GetData(), Addr.GetSize())) == SOCKET_ERROR)
				throw SockErr(WSAGetLastError());
			return Rtn;
		}
		SizeL Socket::recvBase(char *RecvDat, SizeL LenDat, SInt32 Flags) {
			if ((MngdTmOut[1] & 0xC0000000) || Data.Ptr == INVALID_SOCKET) throw SockErr(10038);
			if (SockLock)
			{
				Lock Lk(SockLock);//Released upon exiting this scope(like when throw or return)
				if (MngdTmOut[1] & 0xC0000000) throw SockErr(10054);
//				DbgLogData(LenDat, DBGINF_KYRECV);
				timeval TimeOut;
				TimeOut.tv_sec = MngdTmOut[0] >> 24;
				TimeOut.tv_usec = MngdTmOut[0] & 0x00FFFFFF;
				SInt64 OrigTmOut = TmOuts[0];
				OrigTmOut *= 1000000;
				OrigTmOut += TmOuts[1];
				SInt64 MyTmOut = TimeOut.tv_sec;
				MyTmOut *= 1000000;
				MyTmOut += TimeOut.tv_usec;
				SInt64 TmLeft = OrigTmOut;
				SizeL CurLenDat = LenDat;
				SizeL NumZeroLeft = 16;
				while (TmLeft > 0) {
					if (!NumZeroLeft)
					{
						SockErr Exc(10054);
//						DbgLogData(Exc.ErrCode, DBGINF_EXCKYRECV);
//						DbgLogData((const ByteArray &)Exc.Msg.Str(), DBGINF_EXCKYRECV);
						throw Exc;
					}
					if (TmOuts[0] != MAX_INT32) TmLeft -= MyTmOut;
					fd_set Tmp;
					FD_ZERO(&Tmp);
					FD_SET(Data.Ptr, &Tmp);
					SInt32 Rtn = select(0, &Tmp, 0, 0, &TimeOut);
					if (MngdTmOut[1] & 0x80000000) throw SockErr(10054);
					if (Rtn == 0) continue;
					else if (Rtn == SOCKET_ERROR)
					{
						SockErr Exc(WSAGetLastError());
//						DbgLogData(Exc.ErrCode, DBGINF_EXCKYRECV);
//						DbgLogData((const ByteArray &)Exc.Msg.Str(), DBGINF_EXCKYRECV);
						throw Exc;
					}
					else
					{
						SizeL TmpRtn = 0;
						if ((Rtn = ::recv(Data.Ptr, RecvDat, CurLenDat, Flags)) == SOCKET_ERROR)
						{
							SockErr Exc(WSAGetLastError());
//							DbgLogData(Exc.ErrCode, DBGINF_EXCKYRECV);
//							DbgLogData((const ByteArray &)Exc.Msg.Str(), DBGINF_EXCKYRECV);
							throw Exc;
						}
						if (Rtn != 0)
						{
							NumZeroLeft = 16;
							TmLeft = OrigTmOut;
							RecvDat += Rtn;
							CurLenDat -= Rtn;
							if (!CurLenDat)
							{
//								DbgLogData(ByteArray((Byte *)RecvDat - LenDat, LenDat), DBGINF_OKKYRECV);
								return LenDat;
							}
						}
						else --NumZeroLeft;
					}
				}
				SockErr Exc(10060);
//				DbgLogData(Exc.ErrCode, DBGINF_EXCKYRECV);
//				DbgLogData((const ByteArray &)Exc.Msg.Str(), DBGINF_EXCKYRECV);
				throw Exc;
			}
			if (TmOuts[0] != MAX_INT32)
			{
				fd_set Tmp;
				FD_ZERO(&Tmp);
				FD_SET(Data.Ptr, &Tmp);
				timeval TimeOut;
				TimeOut.tv_sec = TmOuts[0];
				TimeOut.tv_usec = TmOuts[1];
				SInt32 Rtn = select(0, &Tmp, 0, 0, &TimeOut);
				if (Rtn == 0) throw SockErr(10060);
				else if (Rtn == SOCKET_ERROR) throw SockErr(WSAGetLastError());
			}
			SizeL Rtn = 0;
			if ((Rtn = ::recv(Data.Ptr, RecvDat, LenDat, Flags)) == SOCKET_ERROR)
				throw SockErr(WSAGetLastError());
			return Rtn;
		}
		SizeL Socket::recvfromBase(SockAddr &Addr, char *RecvDat, SizeL LenDat, SInt32 Flags) {
			if ((MngdTmOut[1] & 0xC0000000) || Data.Ptr == INVALID_SOCKET) throw SockErr(10038);
			if (SockLock)
			{
				Lock Lk(SockLock);//Released upon exiting this scope(like when throw or return)
				if (MngdTmOut[1] & 0xC0000000) throw SockErr(10054);
				timeval TimeOut;
				TimeOut.tv_sec = MngdTmOut[0] >> 24;
				TimeOut.tv_usec = MngdTmOut[0] & 0x00FFFFFF;
				SInt64 OrigTmOut = TmOuts[0];
				OrigTmOut *= 1000000;
				OrigTmOut += TmOuts[1];
				SInt64 MyTmOut = TimeOut.tv_sec;
				MyTmOut *= 1000000;
				MyTmOut += TimeOut.tv_usec;
				SInt64 TmLeft = OrigTmOut;
				SizeL NumZeroLeft = 16;
				while (TmLeft > 0) {
					if (!NumZeroLeft) throw SockErr(10054);
					if (TmOuts[0] != MAX_INT32) TmLeft -= MyTmOut;
					fd_set Tmp;
					FD_ZERO(&Tmp);
					FD_SET(Data.Ptr, &Tmp);
					SInt32 Rtn = select(0, &Tmp, 0, 0, &TimeOut);
					if (MngdTmOut[1] & 0x80000000) throw SockErr(10054);
					if (Rtn == 0) continue;
					else if (Rtn == SOCKET_ERROR)
						throw SockErr(WSAGetLastError());
					else
					{
						SOCKADDR_STORAGE Tmp;
						SInt32 TmpLen = sizeof(Tmp);
						SizeL TmpRtn = 0;
						if ((Rtn = ::recvfrom(Data.Ptr, RecvDat, LenDat, Flags, (sockaddr *)&Tmp, &TmpLen)) == SOCKET_ERROR)
							throw SockErr(WSAGetLastError());
						if (Rtn != 0)
						{
							NumZeroLeft = 16;
							TmLeft = OrigTmOut;
							Addr.Init(&Tmp);
							return Rtn;
						}
						else --NumZeroLeft;
					}
				}
				throw SockErr(10060);
			}
			if (TmOuts[0] != MAX_INT32)
			{
				fd_set Tmp;
				FD_ZERO(&Tmp);
				FD_SET(Data.Ptr, &Tmp);
				timeval TimeOut;
				TimeOut.tv_sec = TmOuts[0];
				TimeOut.tv_usec = TmOuts[1];
				SInt32 Rtn = select(0, &Tmp, 0, 0, &TimeOut);
				if (Rtn == 0) throw SockErr(10060);
				else if (Rtn == SOCKET_ERROR) throw SockErr(WSAGetLastError());
			}
			SizeL Rtn = 0;
			SOCKADDR_STORAGE Tmp;
			SInt32 TmpLen = sizeof(Tmp);
			if ((Rtn = ::recvfrom(Data.Ptr, RecvDat, LenDat, Flags, (sockaddr *)&Tmp, &TmpLen)) == SOCKET_ERROR)
				throw SockErr(WSAGetLastError());
			return Rtn;
		}
		void Socket::settimeout(double Sec) {
			if ((MngdTmOut[1] & 0xC0000000) || Data.Ptr == INVALID_SOCKET) throw SockErr(10038);
			SInt32 Rtn = 0;
			UInt32 IsNoBlk = 0;
			if (Sec == -1)
			{
				if (SockLock) IsNoBlk = 1;
				TmOuts[0] = MAX_INT32;
				TmOuts[1] = 0;
			}
			else
			{
				IsNoBlk = 1;
				TmOuts[0] = Sec;
				TmOuts[1] = Sec * 100000;
				TmOuts[1] %= 1000000;
			}
			Rtn = ioctlsocket(Data.Ptr, FIONBIO, &IsNoBlk);
			if (Rtn == SOCKET_ERROR) throw SockErr(WSAGetLastError());
		}
		SockAddr::SockAddr(void *Bytes) {
			Data = 0;
			if (Bytes != 0)
			{
				switch (((sockaddr_in *)Bytes)->sin_family) {
				case AF_INET:
				{
					sockaddr_in *Repr = (sockaddr_in *)Bytes;
					sockaddr_in *NewDat = new sockaddr_in;
					NewDat->sin_family = Repr->sin_family;
					NewDat->sin_addr = Repr->sin_addr;
					NewDat->sin_port = Repr->sin_port;
					Data = NewDat;
					return;
				}
				case AF_INET6:
				{
					sockaddr_in6 *Repr = (sockaddr_in6 *)Bytes;
					sockaddr_in6 *NewDat = new sockaddr_in6;
					NewDat->sin6_family = Repr->sin6_family;
					NewDat->sin6_addr = Repr->sin6_addr;
					NewDat->sin6_flowinfo = Repr->sin6_flowinfo;
					NewDat->sin6_port = Repr->sin6_port;
					NewDat->sin6_scope_id = Repr->sin6_scope_id;
					Data = NewDat;
					return;
				}
				default:
					return;
				}
			}
		}
		void SockAddr::Init(void *Bytes) {
			DeInitData();
			if (Bytes != 0)
			{
				switch (((sockaddr_in *)Bytes)->sin_family) {
				case AF_INET:
				{
					sockaddr_in *Repr = (sockaddr_in *)Bytes;
					sockaddr_in *NewDat = new sockaddr_in;
					NewDat->sin_family = Repr->sin_family;
					NewDat->sin_addr = Repr->sin_addr;
					NewDat->sin_port = Repr->sin_port;
					Data = NewDat;
					return;
				}
				case AF_INET6:
				{
					sockaddr_in6 *Repr = (sockaddr_in6 *)Bytes;
					sockaddr_in6 *NewDat = new sockaddr_in6;
					NewDat->sin6_family = Repr->sin6_family;
					NewDat->sin6_addr = Repr->sin6_addr;
					NewDat->sin6_flowinfo = Repr->sin6_flowinfo;
					NewDat->sin6_port = Repr->sin6_port;
					NewDat->sin6_scope_id = Repr->sin6_scope_id;
					Data = NewDat;
					return;
				}
				default:
					return;
				}
			}
		}
		SockAddr::SockAddr(const String &Addr, unsigned short Port) {
			Data = 0;
			sockaddr_in *TmpDat = new sockaddr_in;
			char *TmpStr = Addr.GetCString();
			if (inet_pton(AF_INET, TmpStr, &TmpDat->sin_addr) == 1)
			{
				TmpDat->sin_port = htons(Port);
				TmpDat->sin_family = AF_INET;
				delete[] TmpStr;
				Data = TmpDat;
				return;
			}
			delete TmpDat;
			sockaddr_in6 *TmpDat1 = new sockaddr_in6;
			if (inet_pton(AF_INET6, TmpStr, &TmpDat1->sin6_addr) == 1)
			{
				TmpDat1->sin6_port = htons(Port);
				TmpDat1->sin6_family = AF_INET6;
				TmpDat1->sin6_flowinfo = 0;
				TmpDat1->sin6_scope_id = 0;
				delete[] TmpStr;
				Data = TmpDat1;
				return;
			}
			delete[] TmpStr;
			delete TmpDat1;
		}
		SockAddr::SockAddr(const SockAddr &Cpy) {
			Data = 0;
			if (Cpy.Data != 0)
			{
				switch (((sockaddr_in *)Cpy.Data)->sin_family) {
				case AF_INET:
					Data = new sockaddr_in(*(const sockaddr_in *)Cpy.Data);
					break;
				case AF_INET6:
					Data = new sockaddr_in6(*(const sockaddr_in6 *)Cpy.Data);
					break;
				}
			}
		}
		void SockAddr::DeInitData() {
			if (Data != 0)
			{
				switch (((sockaddr_in *)Data)->sin_family) {
				case AF_INET:
					delete (sockaddr_in *)Data;
					break;
				case AF_INET6:
					delete (sockaddr_in6 *)Data;
					break;
				default:
					delete Data;
				}
				Data = 0;
			}
		}
		void SockAddr::Init(const wString &AddrStr, unsigned short Port, UInt32 FlowInf, UInt32 ScopeId) {
			DeInitData();
			if (AddrStr.Contains('.'))// we are going to say it is AF_INET (IPV4)
			{
				sockaddr_in *Tmp = new sockaddr_in;
				wchar_t *TheStr = AddrStr.GetCString();
				SInt32 Rtn = InetPtonW(AF_INET, TheStr, &Tmp->sin_addr);
				delete TheStr;
				if (Rtn != 1)
					delete Tmp;
				if (Rtn == 0) throw SockErr(10014);
				if (Rtn == -1) throw SockErr(WSAGetLastError());
				Data = Tmp;
				Tmp->sin_family = AF_INET;
				Tmp->sin_port = htons(Port);
				Tmp->sin_zero[0] = 0;
				Tmp->sin_zero[1] = 0;
				Tmp->sin_zero[2] = 0;
				Tmp->sin_zero[3] = 0;
				Tmp->sin_zero[4] = 0;
				Tmp->sin_zero[5] = 0;
				Tmp->sin_zero[6] = 0;
				Tmp->sin_zero[7] = 0;
			}
			else // AF_INET6 (IPV6)
			{
				sockaddr_in6 *Tmp = new sockaddr_in6;
				wchar_t *TheStr = AddrStr.GetCString();
				SInt32 Rtn = InetPtonW(AF_INET6, TheStr, &Tmp->sin6_addr);
				delete TheStr;
				if (Rtn != 1)
					delete Tmp;
				if (Rtn == 0) throw SockErr(10014);
				if (Rtn == -1) throw SockErr(WSAGetLastError());
				Data = Tmp;
				Tmp->sin6_family = AF_INET6;
				Tmp->sin6_port = htons(Port);
				Tmp->sin6_flowinfo = FlowInf;
				Tmp->sin6_scope_id = ScopeId;
			}
		}
		void SockAddr::Init(const String &AddrStr, unsigned short Port, UInt32 FlowInf, UInt32 ScopeId) {
			DeInitData();
			if (AddrStr.Contains('.'))// we are going to say it is AF_INET (IPV4)
			{
				sockaddr_in *Tmp = new sockaddr_in;
				char *TheStr = AddrStr.GetCString();
				SInt32 Rtn = InetPtonA(AF_INET, TheStr, &Tmp->sin_addr);
				delete TheStr;
				if (Rtn != 1)
					delete Tmp;
				if (Rtn == 0) throw SockErr(21);
				if (Rtn == -1) throw SockErr(WSAGetLastError());
				Data = Tmp;
				Tmp->sin_family = AF_INET;
				Tmp->sin_port = htons(Port);
				Tmp->sin_zero[0] = 0;
				Tmp->sin_zero[1] = 0;
				Tmp->sin_zero[2] = 0;
				Tmp->sin_zero[3] = 0;
				Tmp->sin_zero[4] = 0;
				Tmp->sin_zero[5] = 0;
				Tmp->sin_zero[6] = 0;
				Tmp->sin_zero[7] = 0;
			}
			else // AF_INET6 (IPV6)
			{
				sockaddr_in6 *Tmp = new sockaddr_in6;
				char *TheStr = AddrStr.GetCString();
				SInt32 Rtn = InetPtonA(AF_INET6, TheStr, &Tmp->sin6_addr);
				delete TheStr;
				if (Rtn != 1)
					delete Tmp;
				if (Rtn == 0) throw SockErr(21);
				if (Rtn == -1) throw SockErr(WSAGetLastError());
				Data = Tmp;
				Tmp->sin6_family = AF_INET6;
				Tmp->sin6_port = htons(Port);
				Tmp->sin6_flowinfo = FlowInf;
				Tmp->sin6_scope_id = ScopeId;
			}
		}
		String SockAddr::GetAddrStr() {
			String Rtn;
			if (Data == 0) return Rtn;
			switch (((sockaddr_in *)Data)->sin_family) {
			case AF_INET:
			{
				sockaddr_in *Repr = (sockaddr_in *)Data;
				Rtn = String(char(0), 16);
				inet_ntop(AF_INET, &Repr->sin_addr, (char*)Rtn.GetData(), Rtn.Length());
				return String(Rtn.GetData());
			}
			case AF_INET6:
			{
				sockaddr_in6 *Repr = (sockaddr_in6 *)Data;
				Rtn = String(char(0), 46);
				inet_ntop(AF_INET6, &Repr->sin6_addr, (char*)Rtn.GetData(), Rtn.Length());
				return String(Rtn.GetData());
			}
			}
			return Rtn;
		}
		unsigned short SockAddr::GetPort() {
			if (Data == 0) return 0;
			switch (((sockaddr_in *)Data)->sin_family) {
			case AF_INET:
				return ((sockaddr_in *)Data)->sin_port;
			case AF_INET6:
				return ((sockaddr_in6 *)Data)->sin6_port;
			}
			return 0;
		}
		SockAddr &SockAddr::operator=(const SockAddr &Cpy) {
			DeInitData();
			if (Cpy.Data != 0)
			{
				switch (((sockaddr_in *)Cpy.Data)->sin_family) {
				case AF_INET:
					Data = new sockaddr_in(*(const sockaddr_in *)Cpy.Data);
					break;
				case AF_INET6:
					Data = new sockaddr_in6(*(const sockaddr_in6 *)Cpy.Data);
					break;
				default:
					Data = 0;
					break;
				}
			}
			return *this;
		}
		SInt32 SockAddr::GetSize() const{
			if (Data == 0) return 0;
			switch (((sockaddr_in *)Data)->sin_family) {
			case AF_INET:
				return sizeof(sockaddr_in);
			case AF_INET6:
				return sizeof(sockaddr_in6);
			default:
				return 0;
			}
			return 0;
		}
	}
	//=======================================================END SOCK==============================================================
}
