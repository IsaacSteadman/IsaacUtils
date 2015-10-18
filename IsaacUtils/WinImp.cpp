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
		Array<wString> ListDir(wString Path){
			if (Path.Length() > MAX_PATH) Path.Insert(0, ExtPath);
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
			do{
				if ((fdFile.cFileName != L".") && (fdFile.cFileName != L".."))
					Rtn += fdFile.cFileName;
			} while (FindNextFileW(hFind, &fdFile));
			FindClose(hFind);
		}
		bool Exists(wString Path){
			if (Path.Length() > MAX_PATH) Path.Insert(0, ExtPath);
			wchar_t *cPath = Path.GetCString();
			bool Rtn = GetFileAttributesW(cPath) == INVALID_FILE_ATTRIBUTES;
			delete[] cPath;
			return Rtn;
		}
		bool IsFile(wString Path){
			if (Path.Length() > MAX_PATH) Path.Insert(0, ExtPath);
			wchar_t *cPath = Path.GetCString();
			unsigned long Tmp = GetFileAttributesW(cPath);
			bool Rtn = Tmp == INVALID_FILE_ATTRIBUTES || (Tmp & FILE_ATTR_DIRECTORY) == 0;
			delete[] cPath;
			return Rtn;
		}
		bool IsDir(wString Path){
			if (Path.Length() > MAX_PATH) Path.Insert(0, ExtPath);
			wchar_t *cPath = Path.GetCString();
			unsigned long Tmp = GetFileAttributesW(cPath);
			bool Rtn = (Tmp != INVALID_FILE_ATTRIBUTES) && (Tmp & FILE_ATTR_DIRECTORY);
			delete[] cPath;
			return Rtn;
		}
		/**
		 * RETURNS: an array of wString filenames. to get the absolute path of each filename prepend the string with the [search path followed by a '/']
		 *   This function will search through all the files in the path and in any sub-directories and produce the filenames
		 *    with the extension Ext
		 * EXAMPLE: To find all mp3 files in C:/Users/John.Doe/Music
		 *   Array<wString> Mp3Files = GetFileExt("C:/Users/John.Doe/Music", "mp3");
		 *   
		*/
		Array<wString> GetFileExt(wString Path, wString Ext){
			Ext.ToLower();
			if (Path.Length() > MAX_PATH) Path.Insert(0, ExtPath);
			Array<wString> Rtn;
			Array<wString> CurrSearchDir("", 1);//blank so that all that happens is it becomes Path
			Array<wString> NewSearchDir;
			while (CurrSearchDir.Length() > 0){
				for (wString CurPath : CurrSearchDir){
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
					do{
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
		FileDesc Stat(wString Path){
			if (Path.Length() > MAX_PATH) Path.Insert(0, ExtPath);
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
		Array<FileDesc> ListDirStats(wString Path){
			if (Path.Length() > MAX_PATH) Path.Insert(0, ExtPath);
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
			do{
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
		}
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
		Array<String> ListDir(String Path){
			if (Path.Length() > MAX_PATH) Path.Insert(0, ExtPathA);
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
			do{
				if ((fdFile.cFileName != ".") && (fdFile.cFileName != ".."))
					Rtn += fdFile.cFileName;
			} while (FindNextFileA(hFind, &fdFile));
			FindClose(hFind);
		}
		bool Exists(String Path){
			if (Path.Length() > MAX_PATH) Path.Insert(0, ExtPathA);
			char *cPath = Path.GetCString();
			bool Rtn = GetFileAttributesA(cPath) == INVALID_FILE_ATTRIBUTES;
			delete[] cPath;
			return Rtn;
		}
		bool IsFile(String Path){
			if (Path.Length() > MAX_PATH) Path.Insert(0, ExtPathA);
			char *cPath = Path.GetCString();
			unsigned long Tmp = GetFileAttributesA(cPath);
			bool Rtn = Tmp == INVALID_FILE_ATTRIBUTES || (Tmp & FILE_ATTR_DIRECTORY) == 0;
			delete[] cPath;
			return Rtn;
		}
		bool IsDir(String Path){
			if (Path.Length() > MAX_PATH) Path.Insert(0, ExtPathA);
			char *cPath = Path.GetCString();
			unsigned long Tmp = GetFileAttributesA(cPath);
			bool Rtn = (Tmp != INVALID_FILE_ATTRIBUTES) && (Tmp & FILE_ATTR_DIRECTORY);
			delete[] cPath;
			return Rtn;
		}
		/**
		* RETURNS: an array of wString filenames. to get the absolute path of each filename prepend the search path with a '/'
		*   This function will search through all the files in the path and in any sub-directories and produce the filenames
		*    with the extension Ext
		* EXAMPLE: To find all mp3 files in C:/Users/John.Doe/Music
		*   Array<String> Mp3Files = GetFileExt("C:/Users/John.Doe/Music", "mp3");
		*
		*/
		Array<String> GetFileExt(String Path, String Ext){
			Ext.ToLower();
			if (Path.Length() > MAX_PATH) Path.Insert(0, ExtPathA);
			Array<String> Rtn;
			Array<String> CurrSearchDir("", 1);//blank so that all that happens is it becomes Path
			Array<String> NewSearchDir;
			while (CurrSearchDir.Length() > 0){
				for (String CurPath : CurrSearchDir){
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
					do{
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
		FileDescA Stat(String Path){
			if (Path.Length() > MAX_PATH) Path.Insert(0, ExtPathA);
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
		Array<FileDescA> ListDirStats(String Path){
			if (Path.Length() > MAX_PATH) Path.Insert(0, ExtPathA);
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
			do{
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
			virtual Array<Byte> Read();
			virtual Array<Byte> Read(unsigned long Num);
			virtual bool Seek(long long Pos, int From = SEEK_SET);
			virtual long long Tell();
			virtual unsigned long Write(Array<Byte> Data);
			virtual void Close();
			virtual wString GetName();
			virtual unsigned long GetMode();
		};
		WinFile::WinFile(wString fName, unsigned long Mode){
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
		Array<Byte> WinFile::Read()
		{
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
		Array<Byte> WinFile::Read(unsigned long Num)
		{
			Array<Byte> Rtn;
			Rtn.SetLength(Num);
			unsigned long NumHasRead = 0;
			ReadFile((HANDLE)hFile, (LPVOID)Rtn.GetData(), Num, &NumHasRead, NULL);
			Rtn.SetLength(NumHasRead);
			return Rtn;
		}
		bool WinFile::Seek(long long Pos, int From)
		{
			LARGE_INTEGER ToPos;
			ToPos.QuadPart = Pos;
			if (From > 2) From = 0;
			return SetFilePointerEx((HANDLE)hFile, ToPos, NULL, From);
		}
		long long WinFile::Tell()
		{
			long long Rtn = 0;
			LARGE_INTEGER Zero;
			Zero.QuadPart = 0;
			SetFilePointerEx((HANDLE)hFile, Zero, (PLARGE_INTEGER)&Rtn, FILE_CURRENT);
			return Rtn;
		}
		unsigned long WinFile::Write(Array<Byte> Data)
		{
			unsigned long Rtn = 0;
			WriteFile((HANDLE)hFile, (LPCVOID)Data.GetData(), Data.Length(), &Rtn, NULL);
			return Rtn;
		}
		void WinFile::Close()
		{
			CloseHandle((HANDLE)hFile);
		}
		wString WinFile::GetName()
		{
			return fName;
		}
		unsigned long WinFile::GetMode()
		{
			return Md;
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
