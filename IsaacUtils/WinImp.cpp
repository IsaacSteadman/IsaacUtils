#include "stdafx.h"
#include "WinImp.h"
#include "Utils.h"
#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")
namespace Utils {
	//=====================================================BEGIN RANDOM============================================================
	class CryptRandom : public Random {
	protected:
		bool IsValid;
		HCRYPTPROV Data;
	public:
		CryptRandom();
		//NoLessBitLen if true means that no numbers of lesser bit length are produced
		BigLong RandBit(SizeL BitLen, bool NoLessBitLen = false);
		BigLong GetRand(SizeL ByteLen);
		BigLong GetRand(BigLong a, BigLong b);
		double GetRand();
		void Seed(BigLong Val);
		~CryptRandom();
	};
	Random *GetCryptoRand() {
		return new CryptRandom();
	}
	void DestroyRand(Random *Obj) {
		delete Obj;
	}
	CryptRandom::CryptRandom() {
		Data = 0;
		IsValid = CryptAcquireContextW(&Data, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT);
	}
	BigLong CryptRandom::GetRand(SizeL ByteLen) {
		if (!IsValid) return (unsigned long)0;
		BigLong Rtn;
		Rtn.GetLongs() = Array<unsigned long>((unsigned long)0, (ByteLen + 3) / 4);
		CryptGenRandom(Data, ByteLen, (Byte*)Rtn.GetLongs().GetData());
		return Rtn;
	}
	BigLong CryptRandom::RandBit(SizeL BitLen, bool NoLessBitLen) {
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
	BigLong CryptRandom::GetRand(BigLong a, BigLong b) {
		if (a == b) return a;
		else if (a > b) return (unsigned long)0;
		BigLong Off = b - a;
		BigLong Rtn = GetRand((Off.BitLength() + 7) / 8);
		while (Rtn > Off) {
			Rtn >>= 1;
		}
		Rtn += a;
		return Rtn;
	}
	double CryptRandom::GetRand() {
		unsigned long long Rnd = 0;
		BigLong Tmp = GetRand(8);
		Utils::Array<unsigned long> &Longs = Tmp.GetLongs();
		for (unsigned char c = 0; c < 2; ++c) {
			if (c < Longs.Length()) Rnd |= Longs[c];
			Rnd <<= 32;
		}
		unsigned long long Denom = 0xFFFFFFFFFFFFFFFF;
		double Rtn = Rnd;
		Rtn /= Denom;
		return Rtn;
	}
	void CryptRandom::Seed(BigLong Val) {}
	CryptRandom::~CryptRandom() {
		if (IsValid) CryptReleaseContext(Data, 0);
	}
	//======================================================END RANDOM=============================================================

	//=======================================================BEGIN FS==============================================================

	namespace fs {
		wString ExtPath = "\\\\?\\";
		String ExtPathA = ExtPath.Str();
		wString Getcwd() {
			unsigned long Len = GetCurrentDirectoryW(0, NULL);
			wString Rtn(wchar_t(0), Len);
			GetCurrentDirectoryW(Len, (wchar_t *)Rtn.GetData());
			Rtn.Remove(Rtn.Length() - 1);
			return Rtn;
		}
		bool Setcwd(wString Path) {
			Path.Insert(Path.Length(), 0);
			return SetCurrentDirectoryW(Path.GetData());
		}
		String GetcwdA() {
			unsigned long Len = GetCurrentDirectoryA(0, NULL);
			String Rtn(char(0), Len);
			GetCurrentDirectoryA(Len, (char *)Rtn.GetData());
			Rtn.Remove(Rtn.Length() - 1);
			return Rtn;
		}
		bool Setcwd(String Path) {
			Path.Insert(Path.Length(), 0);
			return SetCurrentDirectoryA(Path.GetData());
		}
		class WinFile : public FileBase {
		private:
			HFILE hFile;
			wString fName;
			unsigned long Md;
		public:
			WinFile(wString fName, unsigned long Mode);
			WinFile(String fName, unsigned long Mode);
			virtual ByteArray Read();
			virtual ByteArray Read(unsigned long Num);
			virtual bool Seek(long long Pos, int From = SK_SET);
			virtual long long Tell();
			virtual unsigned long Write(ByteArray Data);
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
			PathPart = wString(fLabel, (SizeL)2);
			PathPart[1] = ':';
			Name = FullName;
			PathPartA = PathPart.Str();
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
				Array<wString> RtnTmp = ListDir(Path.wStr());
				Array<String> Rtn;
				Rtn.SetLength(RtnTmp.Length());
				for (unsigned long c = 0; c < Rtn.Length(); ++c) {
					Rtn[c] = RtnTmp[c].Str();
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
				FileDesc RtnTmp = Stat(Path.wStr());
				FileDescA Rtn;
				Rtn.Attr = RtnTmp.Attr;
				Rtn.CreateTime = RtnTmp.CreateTime;
				Rtn.LastAccessTime = RtnTmp.LastAccessTime;
				Rtn.LastWriteTime = RtnTmp.LastWriteTime;
				Rtn.Size = RtnTmp.Size;
				Rtn.fName = RtnTmp.fName.Str();
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
				Array<FileDesc> RtnTmp = ListDirSt(Path.wStr());
				Array<FileDescA> Rtn;
				Rtn.SetLength(RtnTmp.Length());
				for (unsigned long c = 0; c < Rtn.Length(); ++c) {
					Rtn[c].Attr = RtnTmp[c].Attr;
					Rtn[c].CreateTime = RtnTmp[c].CreateTime;
					Rtn[c].LastAccessTime = RtnTmp[c].LastAccessTime;
					Rtn[c].LastWriteTime = RtnTmp[c].LastWriteTime;
					Rtn[c].Size = RtnTmp[c].Size;
					Rtn[c].fName = RtnTmp[c].fName.Str();
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
		WinFile::WinFile(wString Path, unsigned long Mode) {
			if (Mode & F_APP > 0 && Mode & F_TRUNC > 0)
				throw FileError("OpenModeError", "Cannot open file with append and truncate");
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
				else if (Mode & F_APP > 0)
				{
					Create |= OPEN_EXISTING;
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
			if (Mode & F_APP > 0 && Mode & F_TRUNC > 0)
				throw FileError("OpenModeError", "Cannot open file with append and truncate");
			fName = Path.wStr();
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
				else if (Mode & F_APP > 0)
				{
					Create |= OPEN_EXISTING;
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
		ByteArray WinFile::Read() {
			ByteArray Rtn;
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
		ByteArray WinFile::Read(unsigned long Num) {
			ByteArray Rtn;
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
		unsigned long WinFile::Write(ByteArray Data) {
			unsigned long Rtn = 0;
			LARGE_INTEGER Off;
			Off.QuadPart = 0;
			if (Md & F_APP) SetFilePointerEx((HANDLE)hFile, Off, NULL, FILE_END);
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
	//========================================================END FS===============================================================

	void ShowError(wString Caption, wString Text) {
		wchar_t *Cap = Caption.GetCString(), *Txt = Text.GetCString();
		MessageBoxW(NULL, Txt, Cap, MB_ICONERROR | MB_OK);
		delete[]Cap;
		delete[]Txt;
	}
	long long Clock::Tps = 0;
	WSADATA WinSockData;
	void OsInit() {
		sock::ErrCodes.SetHashFunc(NumHash);
		sock::ErrCodes.Put(21, "Invalid address format.");
		sock::ErrCodes.Put(10004, "The operation was interrupted.");
		sock::ErrCodes.Put(10009, "A bad file handle was passed.");
		sock::ErrCodes.Put(10013, "Permission denied.");
		sock::ErrCodes.Put(10014, "A fault occurred on the network??");
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
		QueryPerformanceFrequency((LARGE_INTEGER *)&Clock::Tps);
		Array<bool> DrvAvail(false, (SizeL)26);
		unsigned long Drvs = GetLogicalDrives();
		for (bool &Avail : DrvAvail) {
			Avail = Drvs % 2;
			Drvs >>= 1;
		}
		WORD VerReq = MAKEWORD(2, 2);
		sock::Usable = WSAStartup(VerReq, &WinSockData) == 0 && WinSockData.wVersion == VerReq;
	}
	void OsDeInit() {
		if (sock::Usable) WSACleanup();
	}
	void Clock::StartTime() {
		++NumTimes;
		QueryPerformanceCounter((LARGE_INTEGER *)&CurClk);
	}
	void Clock::EndTime() {
		long long Later = 0;
		QueryPerformanceCounter((LARGE_INTEGER *)&Later);
		Later -= CurClk;
		double Add = Later;
		Add /= Tps;
		TotTime += Add;
	}

	//=====================================================BEGIN THREAD============================================================

	unsigned long __stdcall UtilsThread::ThreadProc(void *Params) {
		unsigned long Rtn = 0;
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
	UtilsThread::UtilsThread(unsigned long(*ThreadFunc)(void *, unsigned long, void *), void *FunctParams) {
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
	UtilsThread::UtilsThread(unsigned long Id) {
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
	bool UtilsThread::ExitCurrentThread(unsigned long ExitCode) {
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
	bool UtilsThread::Terminate(unsigned long ExitCode) {
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
	UtilsThread::~UtilsThread() {
		if (hThread != 0) CloseHandle((HANDLE)hThread);
	}
	bool UtilsThread::IsCallingThread() {
		return ThreadId == GetCurrentThreadId();
	}
	void UtilsThread::SetThread(unsigned long Id) {
		if (hThread != 0) CloseHandle((HANDLE)hThread);
		ThreadId = Id;
		if (Id != 0) hThread = OpenThread(THREAD_ALL_ACCESS, TRUE, Id);
		else hThread = 0;
	}
	bool UtilsThread::Join() {
		if (GetCurrentThreadId() == ThreadId) return false;
		return WaitForSingleObject(hThread, INFINITE) != 0xFFFFFFFF;
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
	Lock::Lock(Mutex *Obj, bool Access) {
		LockObj = Obj;
		Attr = Access;
		LockObj->Acquire(Attr);
	}
	Lock::~Lock() {
		LockObj->Release(Attr);
	}
	Mutex *GetSingleMutex() {
		return new SingleMutex();
	}
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
		void wait(unsigned long Timeout = MAX_INT32, bool Access = false);
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
	void RWCondVar::wait(unsigned long Timeout, bool Access) {
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
		void wait(unsigned long Timeout = MAX_INT32, bool Access = false);
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
		if (PreWtNtfy) NumAllowWake += 1;
		else if (NumWaiting > 0 && NumAllowWake < NumWaiting) NumAllowWake += 1;
		WakeConditionVariable(&Data);
	}
	void CSCondVar::notifyAll() {
		NumAllowWake = NumWaiting;
		WakeAllConditionVariable(&Data);
	}
	void CSCondVar::wait(unsigned long Timeout, bool Access) {
		++NumWaiting;
		while (NumAllowWake == 0) SleepConditionVariableCS(&Data, &CSLock->Data, Timeout);
		--NumAllowWake;
		--NumWaiting;
	}
	Mutex *CSCondVar::GetInternLock() {
		return CSLock;
	}
	CSCondVar::~CSCondVar() {
		if (!IsLockRef) DestroyMutex(CSLock);
	}
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
	}/*
	AsyncTask::~AsyncTask() {}
	class AsyncAddLock : public AsyncTask {
	private:
		Mutex *VarLock;
	public:
		AsyncAddLock(Mutex *TheLock);
		bool TryRun();
		void WaitOnce();
		~AsyncAddLock();
	};
	AsyncAddLock::AsyncAddLock(Mutex *TheLock) {
		VarLock = TheLock;
		TheCond = new CSCondVar(new SingleMutex());
		TheCond->IsLockRef = false;
		Done = false;
	}
	bool AsyncAddLock::TryRun() {

	}
	void AsyncAddLock::WaitOnce() {

	}
	AsyncAddLock::~AsyncAddLock() {

	}*/
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
	namespace sock {
		Socket::Socket() {
			Data.Ptr = INVALID_SOCKET;
			TmOuts[0] = MAX_INT32;
			TmOuts[1] = 0xFFFFFFFF;
		}
		void Socket::Init(int af, int Type, int Prot) {
			if ((Data.Ptr = socket(af, Type, Prot)) == INVALID_SOCKET)
				throw SockErr(WSAGetLastError());
		}
		void Socket::bind(const SockAddr &Addr) {
			if (Data.Ptr == INVALID_SOCKET) throw SockErr(10038);
			if (::bind(Data.Ptr, (sockaddr *)Addr.GetData(), Addr.GetSize()) == SOCKET_ERROR)
				throw SockErr(WSAGetLastError());
			SockName = Addr;
		}
		void Socket::connect(const SockAddr &Addr) {
			if (Data.Ptr == INVALID_SOCKET) throw SockErr(10038);
			if (TmOuts[0] != MAX_INT32)
			{
				fd_set Tmp;
				FD_ZERO(&Tmp);
				FD_SET(Data.Ptr, &Tmp);
				timeval TimeOut;
				TimeOut.tv_sec = TmOuts[0];
				TimeOut.tv_usec = TmOuts[1];
				int Rtn = select(0, 0, &Tmp, 0, &TimeOut);
				if (Rtn == 0) throw SockErr(10060);
				else if (Rtn == SOCKET_ERROR) throw SockErr(WSAGetLastError());
			}
			if (::connect(Data.Ptr, (sockaddr *)Addr.GetData(), Addr.GetSize()) == SOCKET_ERROR)
				throw SockErr(WSAGetLastError());
			SOCKADDR_STORAGE Mine;
			int MineLen = sizeof(Mine);
			if (::getsockname(Data.Ptr, (sockaddr *)&Mine, &MineLen) == 0) SockName.Init(&Mine);
			PeerName = Addr;
		}
		void Socket::setsockopt(int Lvl, int OptName, void *OptData, unsigned long DataLen) {
			if (Data.Ptr == INVALID_SOCKET) throw SockErr(10038);
			if (::setsockopt(Data.Ptr, Lvl, OptName, (char*)OptData, DataLen) == SOCKET_ERROR)
				throw SockErr(WSAGetLastError());
		}
		void Socket::accept(Socket &Sock) {
			if (Data.Ptr == INVALID_SOCKET) throw SockErr(10038);
			if (TmOuts[0] != MAX_INT32)
			{
				fd_set Tmp;
				FD_ZERO(&Tmp);
				FD_SET(Data.Ptr, &Tmp);
				timeval TimeOut;
				TimeOut.tv_sec = TmOuts[0];
				TimeOut.tv_usec = TmOuts[1];
				int Rtn = select(0, &Tmp, 0, 0, &TimeOut);
				if (Rtn == 0) throw SockErr(10060);
				else if (Rtn == SOCKET_ERROR) throw SockErr(WSAGetLastError());
			}
			SOCKADDR_STORAGE Tmp;
			int AddrLen = sizeof(Tmp);
			if ((Sock.Data.Ptr = ::accept(Data.Ptr, (sockaddr *)&Tmp, &AddrLen)) == INVALID_SOCKET)
				throw SockErr(WSAGetLastError());
			Sock.SockName = SockName;
			Sock.PeerName.Init(&Tmp);
		}
		void Socket::listen(int AllowBuff) {
			if (Data.Ptr == INVALID_SOCKET) throw SockErr(10038);
			if (::listen(Data.Ptr, AllowBuff) == SOCKET_ERROR)
				throw SockErr(WSAGetLastError());
		}
		void Socket::close() {
			if (Data.Ptr == INVALID_SOCKET) throw SockErr(10038);
			if (::closesocket(Data.Ptr) == SOCKET_ERROR)
				throw SockErr(WSAGetLastError());
		}
		SizeL Socket::send(const ByteArray &Bytes, int Flags) {
			if (Data.Ptr == INVALID_SOCKET) throw SockErr(10038);
			if (TmOuts[0] != MAX_INT32)
			{
				fd_set Tmp;
				FD_ZERO(&Tmp);
				FD_SET(Data.Ptr, &Tmp);
				timeval TimeOut;
				TimeOut.tv_sec = TmOuts[0];
				TimeOut.tv_usec = TmOuts[1];
				int Rtn = select(0, 0, &Tmp, 0, &TimeOut);
				if (Rtn == 0) throw SockErr(10060);
				else if (Rtn == SOCKET_ERROR) throw SockErr(WSAGetLastError());
			}
			SizeL Rtn = 0;
			if ((Rtn = ::send(Data.Ptr, (char *)Bytes.GetData(), Bytes.Length(), Flags)) == SOCKET_ERROR)
				throw SockErr(WSAGetLastError());
			return Rtn;
		}
		SizeL Socket::send(const String &Bytes, int Flags) {
			if (Data.Ptr == INVALID_SOCKET) throw SockErr(10038);
			if (TmOuts[0] != MAX_INT32)
			{
				fd_set Tmp;
				FD_ZERO(&Tmp);
				FD_SET(Data.Ptr, &Tmp);
				timeval TimeOut;
				TimeOut.tv_sec = TmOuts[0];
				TimeOut.tv_usec = TmOuts[1];
				int Rtn = select(0, 0, &Tmp, 0, &TimeOut);
				if (Rtn == 0) throw SockErr(10060);
				else if (Rtn == SOCKET_ERROR) throw SockErr(WSAGetLastError());
			}
			SizeL Rtn = 0;
			if ((Rtn = ::send(Data.Ptr, Bytes.GetData(), Bytes.Length(), Flags)) == SOCKET_ERROR)
				throw SockErr(WSAGetLastError());
			return Rtn;
		}
		SizeL Socket::sendto(const ByteArray &Bytes, const SockAddr &Addr, int Flags) {
			if (Data.Ptr == INVALID_SOCKET) throw SockErr(10038);
			if (TmOuts[0] != MAX_INT32)
			{
				fd_set Tmp;
				FD_ZERO(&Tmp);
				FD_SET(Data.Ptr, &Tmp);
				timeval TimeOut;
				TimeOut.tv_sec = TmOuts[0];
				TimeOut.tv_usec = TmOuts[1];
				int Rtn = select(0, 0, &Tmp, 0, &TimeOut);
				if (Rtn == 0) throw SockErr(10060);
				else if (Rtn == SOCKET_ERROR) throw SockErr(WSAGetLastError());
			}
			SizeL Rtn = 0;
			if ((Rtn = ::sendto(Data.Ptr, (char *)Bytes.GetData(), Bytes.Length(), Flags, (sockaddr *)Addr.GetData(), Addr.GetSize())) == SOCKET_ERROR)
				throw SockErr(WSAGetLastError());
			return Rtn;
		}
		SizeL Socket::sendto(const String &Bytes, const SockAddr &Addr, int Flags) {
			if (Data.Ptr == INVALID_SOCKET) throw SockErr(10038);
			if (TmOuts[0] != MAX_INT32)
			{
				fd_set Tmp;
				FD_ZERO(&Tmp);
				FD_SET(Data.Ptr, &Tmp);
				timeval TimeOut;
				TimeOut.tv_sec = TmOuts[0];
				TimeOut.tv_usec = TmOuts[1];
				int Rtn = select(0, 0, &Tmp, 0, &TimeOut);
				if (Rtn == 0) throw SockErr(10060);
				else if (Rtn == SOCKET_ERROR) throw SockErr(WSAGetLastError());
			}
			SizeL Rtn = 0;
			if ((Rtn = ::sendto(Data.Ptr, Bytes.GetData(), Bytes.Length(), Flags, (sockaddr *)Addr.GetData(), Addr.GetSize())) == SOCKET_ERROR)
				throw SockErr(WSAGetLastError());
			return Rtn;
		}
		ByteArray Socket::recv(SizeL Num, int Flags) {
			if (Data.Ptr == INVALID_SOCKET) throw SockErr(10038);
			if (TmOuts[0] != MAX_INT32)
			{
				fd_set Tmp;
				FD_ZERO(&Tmp);
				FD_SET(Data.Ptr, &Tmp);
				timeval TimeOut;
				TimeOut.tv_sec = TmOuts[0];
				TimeOut.tv_usec = TmOuts[1];
				int Rtn = select(0, &Tmp, 0, 0, &TimeOut);
				if (Rtn == 0) throw SockErr(10060);
				else if (Rtn == SOCKET_ERROR) throw SockErr(WSAGetLastError());
			}
			ByteArray Rtn(Byte(0), Num);
			int LenRecv = ::recv(Data.Ptr, (char *)Rtn.GetData(), Rtn.Length(), Flags);
			if (LenRecv == SOCKET_ERROR) throw SockErr(WSAGetLastError());
			Rtn.SetLength(LenRecv);
			return Rtn;
		}
		String Socket::recvS(SizeL Num, int Flags) {
			if (Data.Ptr == INVALID_SOCKET) throw SockErr(10038);
			if (TmOuts[0] != MAX_INT32)
			{
				fd_set Tmp;
				FD_ZERO(&Tmp);
				FD_SET(Data.Ptr, &Tmp);
				timeval TimeOut;
				TimeOut.tv_sec = TmOuts[0];
				TimeOut.tv_usec = TmOuts[1];
				int Rtn = select(0, &Tmp, 0, 0, &TimeOut);
				if (Rtn == 0) throw SockErr(10060);
				else if (Rtn == SOCKET_ERROR) throw SockErr(WSAGetLastError());
			}
			String Rtn(char(0), Num);
			int LenRecv = ::recv(Data.Ptr, (char *)Rtn.GetData(), Rtn.Length(), Flags);
			if (LenRecv == SOCKET_ERROR) throw SockErr(WSAGetLastError());
			return Rtn.SubStr(0, LenRecv);
		}
		ByteArray Socket::recvfrom(SockAddr &Addr, SizeL Num, int Flags) {
			if (Data.Ptr == INVALID_SOCKET) throw SockErr(10038);
			if (TmOuts[0] != MAX_INT32)
			{
				fd_set Tmp;
				FD_ZERO(&Tmp);
				FD_SET(Data.Ptr, &Tmp);
				timeval TimeOut;
				TimeOut.tv_sec = TmOuts[0];
				TimeOut.tv_usec = TmOuts[1];
				int Rtn = select(0, &Tmp, 0, 0, &TimeOut);
				if (Rtn == 0) throw SockErr(10060);
				else if (Rtn == SOCKET_ERROR) throw SockErr(WSAGetLastError());
			}
			ByteArray Rtn(Byte(0), Num);
			SOCKADDR_STORAGE Tmp;
			int TmpLen = sizeof(Tmp);
			int LenRecv = ::recvfrom(Data.Ptr, (char *)Rtn.GetData(), Rtn.Length(), Flags, (sockaddr *)&Tmp, &TmpLen);
			if (LenRecv == SOCKET_ERROR) throw SockErr(WSAGetLastError());
			Rtn.SetLength(LenRecv);
			Addr.Init(&Tmp);
			return Rtn;
		}
		String Socket::recvfromS(SockAddr &Addr, SizeL Num, int Flags) {
			if (Data.Ptr == INVALID_SOCKET) throw SockErr(10038);
			if (TmOuts[0] != MAX_INT32)
			{
				fd_set Tmp;
				FD_ZERO(&Tmp);
				FD_SET(Data.Ptr, &Tmp);
				timeval TimeOut;
				TimeOut.tv_sec = TmOuts[0];
				TimeOut.tv_usec = TmOuts[1];
				int Rtn = select(0, &Tmp, 0, 0, &TimeOut);
				if (Rtn == 0) throw SockErr(10060);
				else if (Rtn == SOCKET_ERROR) throw SockErr(WSAGetLastError());
			}
			String Rtn(char(0), Num);
			SOCKADDR_STORAGE Tmp;
			int TmpLen = sizeof(Tmp);
			int LenRecv = ::recvfrom(Data.Ptr, (char *)Rtn.GetData(), Rtn.Length(), Flags, (sockaddr *)&Tmp, &TmpLen);
			if (LenRecv == SOCKET_ERROR) throw SockErr(WSAGetLastError());
			Addr.Init(&Tmp);
			return Rtn.SubStr(0, LenRecv);
		}
		SockAddr Socket::getsockname() {
			return SockName;
		}
		SockAddr Socket::getpeername() {
			return PeerName;
		}
		void Socket::settimeout(double Sec) {
			if (Data.Ptr == INVALID_SOCKET) throw SockErr(10038);
			int Rtn = 0;
			if (Sec == -1)
			{
				TmOuts[0] = 0;
				Rtn = ioctlsocket(Data.Ptr, FIONBIO, &TmOuts[0]);
				TmOuts[0] = MAX_INT32;
				TmOuts[1] = 0;
			}
			else
			{
				TmOuts[0] = Sec;
				TmOuts[1] = Sec * 100000;
				TmOuts[1] %= 1000000;
				unsigned long Tmp = 1;
				Rtn = ioctlsocket(Data.Ptr, FIONBIO, &Tmp);
			}
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
		SockAddr::SockAddr() {
			Data = 0;
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
		void SockAddr::Init(const wString &AddrStr, unsigned short Port, unsigned long FlowInf, unsigned long ScopeId) {
			DeInitData();
			if (AddrStr.Contains('.'))// we are going to say it is AF_INET (IPV4)
			{
				sockaddr_in *Tmp = new sockaddr_in;
				wchar_t *TheStr = AddrStr.GetCString();
				int Rtn = InetPtonW(AF_INET, TheStr, &Tmp->sin_addr);
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
				int Rtn = InetPtonW(AF_INET6, TheStr, &Tmp->sin6_addr);
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
		void SockAddr::Init(const String &AddrStr, unsigned short Port, unsigned long FlowInf, unsigned long ScopeId) {
			DeInitData();
			if (AddrStr.Contains('.'))// we are going to say it is AF_INET (IPV4)
			{
				sockaddr_in *Tmp = new sockaddr_in;
				char *TheStr = AddrStr.GetCString();
				int Rtn = InetPtonA(AF_INET, TheStr, &Tmp->sin_addr);
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
				int Rtn = InetPtonA(AF_INET6, TheStr, &Tmp->sin6_addr);
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
		SockAddr::SockAddr(SockAddr &&Cpy) {
			Data = Cpy.Data;
			Cpy.Data = 0;
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
		SockAddr &SockAddr::operator=(SockAddr &&Cpy) {
			void *Tmp = Data;
			Data = Cpy.Data;
			Cpy.Data = Tmp;
			return *this;
		}
		int SockAddr::GetSize() const{
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
		void *SockAddr::GetData() const{
			return Data;
		}
		SockAddr::~SockAddr() {
			DeInitData();
		}
	}
	//=======================================================END SOCK==============================================================
}
