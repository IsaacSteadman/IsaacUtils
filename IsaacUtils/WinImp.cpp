#include "stdafx.h"
#include "WinImp.h"
#include "Utils.h"
namespace Utils{
	CryptRandom::CryptRandom(){
		Data = 0;
		IsValid = CryptAcquireContextW(&Data, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT);
	}
	BigLong CryptRandom::GetRand(SizeL ByteLen){
		if (!IsValid) return (unsigned long)0;
		BigLong Rtn;
		Rtn.GetLongs() = Array<unsigned long>((unsigned long)0, (ByteLen + 3) / 4);
		CryptGenRandom(Data, ByteLen, (Byte*)Rtn.GetLongs().GetData());
		return Rtn;
	}
	BigLong CryptRandom::RandBit(SizeL BitLen, bool NoLessBitLen){
		if (BitLen == 0) return (unsigned long)0;
		if (NoLessBitLen && (BitLen == 1)) return (unsigned long)1;
		BigLong Rtn;
		Rtn.GetLongs() = Array<unsigned long>((unsigned long)0, (BitLen + 31) / 32);
		unsigned long Len = (BitLen + 7) / 8;
		CryptGenRandom(Data, Len, (Byte*)Rtn.GetLongs().GetData());
		unsigned char Mask = 1;
		Mask <<= (BitLen % 8);
		--Mask;
		Rtn.GetByte(Len - 1) &= Mask;
		if (NoLessBitLen) Rtn.SetBit(BitLen - 1, true);
		return Rtn;
	}
	BigLong CryptRandom::GetRand(BigLong a, BigLong b){
		if (a == b) return a;
		else if (a > b) return (unsigned long)0;
		BigLong Off = b - a;
		BigLong Rtn = GetRand((Off.BitLength() + 7) / 8);
		while (Rtn > Off){
			Rtn >>= 1;
		}
		Rtn += a;
		return Rtn;
	}
	double CryptRandom::GetRand(){
		unsigned long long Rnd = 0;
		BigLong Tmp = GetRand(8);
		Utils::Array<unsigned long> &Longs = Tmp.GetLongs();
		for (unsigned char c = 0; c < 2; ++c){
			if (c < Longs.Length()) Rnd |= Longs[c];
			Rnd <<= 32;
		}
		unsigned long long Denom = 0xFFFFFFFFFFFFFFFF;
		double Rtn = Rnd;
		Rtn /= Denom;
		return Rtn;
	}
	void CryptRandom::Seed(BigLong Val){}
	CryptRandom::~CryptRandom(){
		if (IsValid) CryptReleaseContext(Data, 0);
	}

	namespace fs{
		wString ExtPath = "\\\\?\\";
		String ExtPathA = ExtPath.Str();
		/**
		 * RETURNS: an array of wString filenames. to get the absolute path of each filename prepend the string with the [search path followed by a '/']
		 *   This function will search through all the files in the path and in any sub-directories and produce the filenames
		 *    with the extension Ext
		 * EXAMPLE: To find all mp3 files in C:/Users/John.Doe/Music
		 *   Array<wString> Mp3Files = GetFileExt("C:/Users/John.Doe/Music", "mp3");
		 *   
		*/
		wString Getcwd(){
			unsigned long Len = GetCurrentDirectoryW(0, NULL);
			wString Rtn(wchar_t(0), Len);
			GetCurrentDirectoryW(Len, (wchar_t *)Rtn.GetData());
			Rtn.Remove(Rtn.Length() - 1);
			return Rtn;
		}
		bool Setcwd(wString Path){
			Path.Insert(Path.Length(), 0);
			return SetCurrentDirectoryW(Path.GetData());
		}
		String GetcwdA(){
			unsigned long Len = GetCurrentDirectoryA(0, NULL);
			String Rtn(char(0), Len);
			GetCurrentDirectoryA(Len, (char *)Rtn.GetData());
			Rtn.Remove(Rtn.Length() - 1);
			return Rtn;
		}
		bool Setcwd(String Path){
			Path.Insert(Path.Length(), 0);
			return SetCurrentDirectoryA(Path.GetData());
		}
		class WinFile : public FileBase{
		private:
			HFILE hFile;
			wString fName;
			unsigned long Md;
		public:
			WinFile(wString fName, unsigned long Mode);
			WinFile(String fName, unsigned long Mode);
			virtual Array<Byte> Read();
			virtual Array<Byte> Read(unsigned long Num);
			virtual bool Seek(long long Pos, int From = SK_SET);
			virtual long long Tell();
			virtual unsigned long Write(Array<Byte> Data);
			virtual void Close();
			virtual wString GetName();
			virtual unsigned long GetMode();
		};
		class WinDrive : public DriveBase {
		private:
			wString Name;
			wString PathPart;
			String PathPartA;
			void MkFullPath(String &Path);
			void MkFullPath(wString &Path);
		public:
			WinDrive(wchar_t fLabel, wString Name);
			// Inherited via DriveBase
			virtual wString GetName();
			virtual String GetNameA();
			virtual FileBase * OpenFile(wString Path, unsigned long Mode);
			virtual FileBase * OpenFile(String Path, unsigned long Mode);
			virtual bool IsFile(wString Path);
			virtual bool IsFile(String Path);
			virtual bool Exists(wString Path);
			virtual bool Exists(String Path);
			virtual bool IsDir(wString Path);
			virtual bool IsDir(String Path);
			virtual Array<wString> ListDir(wString Path);
			virtual Array<String> ListDir(String Path);
			virtual FileDesc Stat(wString Path);
			virtual FileDescA Stat(String Path);
			virtual Array<FileDesc> ListDirSt(wString Path);
			virtual Array<FileDescA> ListDirSt(String Path);
			virtual Array<wString> GetFileExt(wString Path, wString Ext);
			virtual Array<String> GetFileExt(String Path, String Ext);
		};
		void WinDrive::MkFullPath(String &Path) {
			Path.Insert(0, PathPartA);
			if (Path.Length() > MAX_PATH) Path.Insert(0, ExtPathA);
		}
		void WinDrive::MkFullPath(wString &Path) {
			Path.Insert(0, PathPart);
			if (Path.Length() > MAX_PATH) Path.Insert(0, ExtPath);
		}
		WinDrive::WinDrive(wchar_t fLabel, wString FullName) {
			PathPart = wString(fLabel, (SizeL)1);
			PathPart += ':';
			Name = FullName;
			PathPartA = String(PathPart.GetData(), PathPart.Length());
		}
		wString WinDrive::GetName() {
			return Name;
		}
		String WinDrive::GetNameA() {
			return String(Name.GetData(), Name.Length());
		}
		FileBase *WinDrive::OpenFile(wString Path, unsigned long Mode) {
			MkFullPath(Path);
			return new WinFile(Path, Mode);
		}
		FileBase *WinDrive::OpenFile(String Path, unsigned long Mode) {
			MkFullPath(Path);
			return new WinFile(Path, Mode);
		}
		bool WinDrive::IsFile(wString Path) {
			MkFullPath(Path);
			wchar_t *cPath = Path.GetCString();
			unsigned long Tmp = GetFileAttributesW(cPath);
			delete[] cPath;
			return Tmp != INVALID_FILE_ATTRIBUTES && (Tmp & FILE_ATTR_DIRECTORY) == 0;
		}
		bool WinDrive::IsFile(String Path) {
			if (Path.Length() + 2 > MAX_PATH) return IsFile(wString(Path.GetData(), Path.Length()));
			MkFullPath(Path);
			char *cPath = Path.GetCString();
			unsigned long Tmp = GetFileAttributesA(cPath);
			delete[] cPath;
			return Tmp != INVALID_FILE_ATTRIBUTES && (Tmp & FILE_ATTR_DIRECTORY) == 0;
		}
		bool WinDrive::Exists(wString Path) {
			MkFullPath(Path);
			wchar_t *cPath = Path.GetCString();
			bool Rtn = GetFileAttributesW(cPath) != INVALID_FILE_ATTRIBUTES;
			delete[] cPath;
			return Rtn;
		}
		bool WinDrive::Exists(String Path) {
			if (Path.Length() + 2 > MAX_PATH) return Exists(wString(Path.GetData(), Path.Length()));
			MkFullPath(Path);
			char *cPath = Path.GetCString();
			bool Rtn = GetFileAttributesA(cPath) != INVALID_FILE_ATTRIBUTES;
			delete[] cPath;
			return Rtn;
		}
		bool WinDrive::IsDir(wString Path) {
			MkFullPath(Path);
			wchar_t *cPath = Path.GetCString();
			unsigned long Tmp = GetFileAttributesW(cPath);
			delete[] cPath;
			return (Tmp != INVALID_FILE_ATTRIBUTES) && (Tmp & FILE_ATTR_DIRECTORY);
		}
		bool WinDrive::IsDir(String Path) {
			if (Path.Length() + 2 > MAX_PATH) return IsDir(wString(Path.GetData(), Path.Length()));
			MkFullPath(Path);
			char *cPath = Path.GetCString();
			unsigned long Tmp = GetFileAttributesA(cPath);
			delete[] cPath;
			return (Tmp != INVALID_FILE_ATTRIBUTES) && (Tmp & FILE_ATTR_DIRECTORY);
		}
		Array<wString> WinDrive::ListDir(wString Path) {
			MkFullPath(Path);
			Array<wString> Rtn;
			WIN32_FIND_DATAW fdFile;
			HANDLE hFind = NULL;
			Path += "/*.*";
			wchar_t *cPath = Path.GetCString();
			if ((hFind = FindFirstFileW(cPath, &fdFile)) == INVALID_HANDLE_VALUE)
			{
				delete[] cPath;
				Rtn += "<INVALID SEARCH PATH>";
				return Rtn;
			}
			do {
				if ((fdFile.cFileName != L".") && (fdFile.cFileName != L".."))
					Rtn += fdFile.cFileName;
			} while (FindNextFileW(hFind, &fdFile));
			FindClose(hFind);
			return Rtn;
		}
		Array<String> WinDrive::ListDir(String Path) {
			if (Path.Length() + 2 > MAX_PATH)
			{
				Array<wString> RtnTmp = ListDir(wString(Path.GetData(), Path.Length()));
				Array<String> Rtn;
				Rtn.SetLength(RtnTmp.Length());
				for (unsigned long c = 0; c < Rtn.Length(); ++c) {
					Rtn[c] = String(RtnTmp[c].GetData(), RtnTmp[c].Length());
				}
				return Rtn;
			}
			MkFullPath(Path);
			Array<String> Rtn;
			WIN32_FIND_DATAA fdFile;
			HANDLE hFind = NULL;
			Path += "/*.*";
			char *cPath = Path.GetCString();
			if ((hFind = FindFirstFileA(cPath, &fdFile)) == INVALID_HANDLE_VALUE)
			{
				delete[] cPath;
				Rtn += "<INVALID SEARCH PATH>";
				return Rtn;
			}
			do {
				if ((fdFile.cFileName != ".") && (fdFile.cFileName != ".."))
					Rtn += fdFile.cFileName;
			} while (FindNextFileA(hFind, &fdFile));
			FindClose(hFind);
			return Rtn;
		}
		FileDesc WinDrive::Stat(wString Path) {
			MkFullPath(Path);
			wchar_t *cPath = Path.GetCString();
			FileDesc Rtn;
			Rtn.CreateTime = 0;
			Rtn.LastAccessTime = 0;
			Rtn.LastWriteTime = 0;
			Rtn.Attr = 0xFFFFFFFF;
			WIN32_FILE_ATTRIBUTE_DATA Dat;
			if (GetFileAttributesExW(cPath, GetFileExInfoStandard, &Dat))
			{
				Rtn.Attr = Dat.dwFileAttributes;
				Rtn.fName = Path;
				Rtn.CreateTime = Dat.ftCreationTime.dwLowDateTime | (Dat.ftCreationTime.dwHighDateTime << 32);
				Rtn.LastAccessTime = Dat.ftLastAccessTime.dwLowDateTime | (Dat.ftLastAccessTime.dwHighDateTime << 32);
				Rtn.LastWriteTime = Dat.ftLastWriteTime.dwLowDateTime | (Dat.ftLastWriteTime.dwHighDateTime << 32);
				Rtn.Size = Dat.nFileSizeLow | (Dat.nFileSizeHigh << 32);
			}
			delete[] cPath;
			return Rtn;
		}
		FileDescA WinDrive::Stat(String Path) {
			if (Path.Length() + 2 > MAX_PATH)
			{
				FileDesc RtnTmp = Stat(wString(Path.GetData(), Path.Length()));
				FileDescA Rtn;
				Rtn.Attr = RtnTmp.Attr;
				Rtn.CreateTime = RtnTmp.CreateTime;
				Rtn.LastAccessTime = RtnTmp.LastAccessTime;
				Rtn.LastWriteTime = RtnTmp.LastWriteTime;
				Rtn.Size = RtnTmp.Size;
				Rtn.fName = String(RtnTmp.fName.GetData(), RtnTmp.fName.Length());
				return Rtn;
			}
			MkFullPath(Path);
			char *cPath = Path.GetCString();
			FileDescA Rtn;
			Rtn.CreateTime = 0;
			Rtn.LastAccessTime = 0;
			Rtn.LastWriteTime = 0;
			Rtn.Attr = 0xFFFFFFFF;
			WIN32_FILE_ATTRIBUTE_DATA Dat;
			if (GetFileAttributesExA(cPath, GetFileExInfoStandard, &Dat))
			{
				Rtn.Attr = Dat.dwFileAttributes;
				Rtn.fName = Path;
				Rtn.CreateTime = Dat.ftCreationTime.dwLowDateTime | (Dat.ftCreationTime.dwHighDateTime << 32);
				Rtn.LastAccessTime = Dat.ftLastAccessTime.dwLowDateTime | (Dat.ftLastAccessTime.dwHighDateTime << 32);
				Rtn.LastWriteTime = Dat.ftLastWriteTime.dwLowDateTime | (Dat.ftLastWriteTime.dwHighDateTime << 32);
				Rtn.Size = Dat.nFileSizeLow | (Dat.nFileSizeHigh << 32);
			}
			delete[] cPath;
			return Rtn;
		}
		Array<FileDesc> WinDrive::ListDirSt(wString Path) {
			MkFullPath(Path);
			Array<FileDesc> Rtn;
			WIN32_FIND_DATAW fdFile;
			HANDLE hFind = NULL;
			Path += "/*.*";
			wchar_t *cPath = Path.GetCString();
			if ((hFind = FindFirstFileW(cPath, &fdFile)) == INVALID_HANDLE_VALUE)
			{
				delete[] cPath;
				Rtn += {"<INVALID SEARCH PATH>", 0, 0, 0, 0, 0};
				return Rtn;
			}
			do {
				if ((fdFile.cFileName != L".") && (fdFile.cFileName != L".."))
				{
					FileDesc Add;
					Add.Attr = fdFile.dwFileAttributes;
					Add.fName = fdFile.cFileName;
					Add.CreateTime = fdFile.ftCreationTime.dwLowDateTime | (fdFile.ftCreationTime.dwHighDateTime << 32);
					Add.LastAccessTime = fdFile.ftLastAccessTime.dwLowDateTime | (fdFile.ftLastAccessTime.dwHighDateTime << 32);
					Add.LastWriteTime = fdFile.ftLastWriteTime.dwLowDateTime | (fdFile.ftLastWriteTime.dwHighDateTime << 32);
					Add.Size = fdFile.nFileSizeLow | (fdFile.nFileSizeHigh << 32);
					Rtn += Add;
				}
			} while (FindNextFileW(hFind, &fdFile));
			FindClose(hFind);
			return Rtn;
		}
		Array<FileDescA> WinDrive::ListDirSt(String Path) {
			if (Path.Length() + 2 > MAX_PATH)
			{
				Array<FileDesc> RtnTmp = ListDirSt(wString(Path.GetData(), Path.Length()));
				Array<FileDescA> Rtn;
				Rtn.SetLength(RtnTmp.Length());
				for (unsigned long c = 0; c < Rtn.Length(); ++c) {
					Rtn[c].Attr = RtnTmp[c].Attr;
					Rtn[c].CreateTime = RtnTmp[c].CreateTime;
					Rtn[c].LastAccessTime = RtnTmp[c].LastAccessTime;
					Rtn[c].LastWriteTime = RtnTmp[c].LastWriteTime;
					Rtn[c].Size = RtnTmp[c].Size;
					Rtn[c].fName = String(RtnTmp[c].fName.GetData(), RtnTmp[c].fName.Length());
				}
				return Rtn;
			}
			MkFullPath(Path);
			Array<FileDescA> Rtn;
			WIN32_FIND_DATAA fdFile;
			HANDLE hFind = NULL;
			Path += "/*.*";
			char *cPath = Path.GetCString();
			if ((hFind = FindFirstFileA(cPath, &fdFile)) == INVALID_HANDLE_VALUE)
			{
				delete[] cPath;
				Rtn += {"<INVALID SEARCH PATH>", 0, 0, 0, 0, 0};
				return Rtn;
			}
			do {
				if ((fdFile.cFileName != ".") && (fdFile.cFileName != ".."))
				{
					FileDescA Add;
					Add.Attr = fdFile.dwFileAttributes;
					Add.fName = fdFile.cFileName;
					Add.CreateTime = fdFile.ftCreationTime.dwLowDateTime | (fdFile.ftCreationTime.dwHighDateTime << 32);
					Add.LastAccessTime = fdFile.ftLastAccessTime.dwLowDateTime | (fdFile.ftLastAccessTime.dwHighDateTime << 32);
					Add.LastWriteTime = fdFile.ftLastWriteTime.dwLowDateTime | (fdFile.ftLastWriteTime.dwHighDateTime << 32);
					Add.Size = fdFile.nFileSizeLow | (fdFile.nFileSizeHigh << 32);
					Rtn += Add;
				}
			} while (FindNextFileA(hFind, &fdFile));
			FindClose(hFind);
			return Rtn;
		}
		Array<wString> WinDrive::GetFileExt(wString Path, wString Ext) {
			MkFullPath(Path);
			Ext.ToLower();
			Array<wString> Rtn;
			Array<wString> CurrSearchDir("", 1);//blank so that all that happens is it becomes Path
			Array<wString> NewSearchDir;
			while (CurrSearchDir.Length() > 0) {
				for (wString CurPath : CurrSearchDir) {
					WIN32_FIND_DATAW fdFile;
					HANDLE hFind = NULL;
					wString Path1 = Path + CurPath + "/*." + Ext;
					wchar_t *cPath = Path1.GetCString();
					if ((hFind = FindFirstFileW(cPath, &fdFile)) == INVALID_HANDLE_VALUE)
					{
						delete[] cPath;
						Rtn += "<INVALID SEARCH PATH>";
						return Rtn;
					}
					delete[] cPath;
					do {
						if ((fdFile.cFileName != L".") && (fdFile.cFileName != L".."))
						{
							wString Tmp = fdFile.cFileName;
							unsigned long Pos = 0;
							if (fdFile.dwFileAttributes & FILE_ATTR_DIRECTORY) NewSearchDir += CurPath + "/" + fdFile.cFileName;
							else Rtn += CurPath + "/" + fdFile.cFileName;
						}
					} while (FindNextFileW(hFind, &fdFile));
					FindClose(hFind);
				}
			}
			return Rtn;
		}
		Array<String> WinDrive::GetFileExt(String Path, String Ext) {
			if (Path.Length() + 2 > MAX_PATH)
			{
				Array<wString> RtnTmp = GetFileExt(wString(Path.GetData(), Path.Length()), wString(Ext.GetData(), Ext.Length()));
				Array<String> Rtn;
				Rtn.SetLength(RtnTmp.Length());
				for (unsigned long c = 0; c < Rtn.Length(); ++c) {
					Rtn[c] = String(RtnTmp[c].GetData(), RtnTmp[c].Length());
				}
				return Rtn;
			}
			MkFullPath(Path);
			Ext.ToLower();
			Array<String> Rtn;
			Array<String> CurrSearchDir("", 1);//blank so that all that happens is it becomes Path
			Array<String> NewSearchDir;
			while (CurrSearchDir.Length() > 0) {
				for (String CurPath : CurrSearchDir) {
					WIN32_FIND_DATAA fdFile;
					HANDLE hFind = NULL;
					String Path1 = Path + CurPath + "/*." + Ext;
					char *cPath = Path1.GetCString();
					if ((hFind = FindFirstFileA(cPath, &fdFile)) == INVALID_HANDLE_VALUE)
					{
						delete[] cPath;
						Rtn += "<INVALID SEARCH PATH>";
						return Rtn;
					}
					delete[] cPath;
					do {
						if ((fdFile.cFileName != ".") && (fdFile.cFileName != ".."))
						{
							wString Tmp = fdFile.cFileName;
							unsigned long Pos = 0;
							if (fdFile.dwFileAttributes & FILE_ATTR_DIRECTORY) NewSearchDir += CurPath + "/" + fdFile.cFileName;
							else Rtn += CurPath + "/" + fdFile.cFileName;
						}
					} while (FindNextFileA(hFind, &fdFile));
					FindClose(hFind);
				}
			}
			return Rtn;
		}
		WinFile::WinFile(wString Path, unsigned long Mode){
			fName = Path;
			wchar_t * wStr = fName.GetCString();
			unsigned long Generic = 0;
			if (Mode & F_IN > 0) Generic |= GENERIC_READ;
			if (Mode & F_OUT > 0) Generic |= GENERIC_WRITE;
			SECURITY_ATTRIBUTES SecAttr;
			SecAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
			SecAttr.lpSecurityDescriptor = NULL;
			SecAttr.bInheritHandle = FALSE;
			unsigned long Create = 0;
			if (Mode & F_OUT)
			{
				if (Mode & F_TRUNC > 0)
				{
					if (Mode & F_NOREPLACE > 0) Create |= TRUNCATE_EXISTING;
					else Create |= CREATE_NEW;
				}
				else
				{
					if (Mode & F_NOREPLACE == 0) Create |= OPEN_ALWAYS;
					else Create |= OPEN_EXISTING;
				}
			}
			else Create |= OPEN_EXISTING;
			hFile = (HFILE)CreateFileW(wStr, Generic, FILE_SHARE_READ, &SecAttr, Create, FILE_ATTRIBUTE_NORMAL, NULL);
			delete[] wStr;
			if (hFile == NULL)
			{
				wchar_t *BuffErr = 0;
				FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), NULL, (LPWSTR)&BuffErr, 2, NULL);
				FileError Err("OpenFileFailed", wString(BuffErr));
				LocalFree(BuffErr);
				throw Err;
			}
			Md = Mode;
		}
		WinFile::WinFile(String Path, unsigned long Mode) {
			fName = wString(Path.GetData(), Path.Length());
			wchar_t * wStr = fName.GetCString();
			unsigned long Generic = 0;
			if (Mode & F_IN > 0) Generic |= GENERIC_READ;
			if (Mode & F_OUT > 0) Generic |= GENERIC_WRITE;
			SECURITY_ATTRIBUTES SecAttr;
			SecAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
			SecAttr.lpSecurityDescriptor = NULL;
			SecAttr.bInheritHandle = FALSE;
			unsigned long Create = 0;
			if (Mode & F_OUT)
			{
				if (Mode & F_TRUNC > 0)
				{
					if (Mode & F_NOREPLACE > 0) Create |= TRUNCATE_EXISTING;
					else Create |= CREATE_NEW;
				}
				else
				{
					if (Mode & F_NOREPLACE == 0) Create |= OPEN_ALWAYS;
					else Create |= OPEN_EXISTING;
				}
			}
			else Create |= OPEN_EXISTING;
			hFile = (HFILE)CreateFileW(wStr, Generic, FILE_SHARE_READ, &SecAttr, Create, FILE_ATTRIBUTE_NORMAL, NULL);
			delete[] wStr;
			if (hFile == NULL)
			{
				wchar_t *BuffErr = 0;
				FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), NULL, (LPWSTR)&BuffErr, 2, NULL);
				FileError Err("OpenFileFailed", wString(BuffErr));
				LocalFree(BuffErr);
				throw Err;
			}
			Md = Mode;
		}
		Array<Byte> WinFile::Read() {
			Array<Byte> Rtn;
			long long Sz = 0;
			GetFileSizeEx((HANDLE)hFile, (PLARGE_INTEGER)Sz);
			long long Fp = 0;
			LARGE_INTEGER Zero;
			Zero.u.HighPart = 0;
			Zero.u.LowPart = 0;
			SetFilePointerEx((HANDLE)hFile, Zero, (PLARGE_INTEGER)&Fp, FILE_CURRENT);
			if (Sz - Fp > 0xFFFFFFFF) throw FileError("Too much to read", "since one cannot easily read more than 2^32 bytes without running into major memory issues");
			Rtn.SetLength(Sz - Fp);
			unsigned long NumHasRead = 0;
			ReadFile((HANDLE)hFile, (LPVOID)Rtn.GetData(), (unsigned long)(Sz - Fp), &NumHasRead, NULL);
			Rtn.SetLength(NumHasRead);
			return Rtn;
		}
		Array<Byte> WinFile::Read(unsigned long Num) {
			Array<Byte> Rtn;
			Rtn.SetLength(Num);
			unsigned long NumHasRead = 0;
			ReadFile((HANDLE)hFile, (LPVOID)Rtn.GetData(), Num, &NumHasRead, NULL);
			Rtn.SetLength(NumHasRead);
			return Rtn;
		}
		bool WinFile::Seek(long long Pos, int From) {
			LARGE_INTEGER ToPos;
			ToPos.QuadPart = Pos;
			if (From > 2) From = 0;
			return SetFilePointerEx((HANDLE)hFile, ToPos, NULL, From);
		}
		long long WinFile::Tell() {
			long long Rtn = 0;
			LARGE_INTEGER Zero;
			Zero.QuadPart = 0;
			SetFilePointerEx((HANDLE)hFile, Zero, (PLARGE_INTEGER)&Rtn, FILE_CURRENT);
			return Rtn;
		}
		unsigned long WinFile::Write(Array<Byte> Data) {
			unsigned long Rtn = 0;
			WriteFile((HANDLE)hFile, (LPCVOID)Data.GetData(), Data.Length(), &Rtn, NULL);
			return Rtn;
		}
		void WinFile::Close() {
			CloseHandle((HANDLE)hFile);
		}
		wString WinFile::GetName() {
			return fName;
		}
		unsigned long WinFile::GetMode() {
			return Md;
		}
		wString GetFullPath(wString Path) {
			if (Path.Length() > MAX_PATH) Path.Insert(0, ExtPath);
			wchar_t *cPath = Path.GetCString();
			wString Rtn = Path;
			unsigned long Len = 0;
			if ((Len = GetFullPathNameW(cPath, Rtn.Length(), (LPWSTR)Rtn.GetData(), NULL)) > Rtn.Length())
			{
				Rtn = wString(wchar_t(0), SizeL(Len));
				GetFullPathNameW(cPath, Rtn.Length(), (LPWSTR)Rtn.GetData(), NULL);
				Rtn.Remove(Rtn.Length() - 1);
			}
			if (Rtn.SubStr(0, ExtPath.Length()) == ExtPath) return Rtn.SubStr(ExtPath.Length());
			else return Rtn;
		}
	}
	void ShowError(wString Caption, wString Text){
		wchar_t *Cap = Caption.GetCString(), *Txt = Text.GetCString();
		MessageBoxW(NULL, Txt, Cap, MB_ICONERROR | MB_OK);
		delete[]Cap;
		delete[]Txt;
	}
	long long Clock::Tps = 0;
	void OsInit(){
		QueryPerformanceFrequency((LARGE_INTEGER *)&Clock::Tps);
		Array<bool> DrvAvail(false, (SizeL)26);
		unsigned long Drvs = GetLogicalDrives();
		for (bool &Avail : DrvAvail) {
			Avail = Drvs % 2;
			Drvs >>= 1;
		}

	}
	void Clock::StartTime(){
		++NumTimes;
		QueryPerformanceCounter((LARGE_INTEGER *)&CurClk);
	}
	void Clock::EndTime(){
		long long Later = 0;
		QueryPerformanceCounter((LARGE_INTEGER *)&Later);
		Later -= CurClk;
		double Add = Later;
		Add /= Tps;
		TotTime += Add;
	}
}
