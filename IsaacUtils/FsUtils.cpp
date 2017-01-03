#include "stdafx.h"
#include "Utils.h"
namespace Utils {
	namespace fs {
		FileError::FileError() {}
		FileError::FileError(wString Cap, wString Txt) {
			Msg = Txt;
			Type = Cap;
		}
		FileErrorN::FileErrorN() {}
		FileErrorN::FileErrorN(UInt32 Code, wString Txt) {
			Msg = Txt;
			ErrCode = Code;
		}
		FileTime::FileTime() {
			tv_sec = 0;
			tv_nsec = 0;
		}
		FileTime::FileTime(SnzL sec, SInt32 nsec) {
			tv_sec = sec;
			tv_nsec = nsec;
			Normalize();
		}
		FileTime::FileTime(const FileTime &Other) {
			tv_sec = Other.tv_sec;
			tv_nsec = Other.tv_nsec;
		}
		FileTime::FileTime(const float Other) {
			tv_sec = Other;
			tv_nsec = int(Other * 1e9) % 1000000000;
		}
		FileTime::FileTime(const double Other) {
			tv_sec = Other;
			tv_nsec = int(Other * 1e9) % 1000000000;
		}
		FileTime::FileTime(const SInt32 Other) {
			tv_sec = Other;
			tv_nsec = 0;
		}
		bool FileTime::operator==(const FileTime &Cmp) const {
			return tv_sec == Cmp.tv_sec && tv_nsec == Cmp.tv_nsec;
		}
		bool FileTime::operator!=(const FileTime &Cmp) const {
			return tv_sec != Cmp.tv_sec || tv_nsec != Cmp.tv_nsec;
		}
		bool FileTime::operator<=(const FileTime &Cmp) const {
			if (tv_sec > Cmp.tv_sec) return false;
			else if (tv_sec < Cmp.tv_sec) return true;
			else if (tv_nsec > Cmp.tv_nsec) return false;
			return true;
		}
		bool FileTime::operator>=(const FileTime &Cmp) const {
			if (tv_sec < Cmp.tv_sec) return false;
			else if (tv_sec > Cmp.tv_sec) return true;
			else if (tv_nsec < Cmp.tv_nsec) return false;
			return true;
		}
		bool FileTime::operator<(const FileTime &Cmp) const {
			if (tv_sec > Cmp.tv_sec) return false;
			else if (tv_sec < Cmp.tv_sec) return true;
			else if (tv_nsec >= Cmp.tv_nsec) return false;
			return true;
		}
		bool FileTime::operator>(const FileTime &Cmp) const {
			if (tv_sec < Cmp.tv_sec) return false;
			else if (tv_sec > Cmp.tv_sec) return true;
			else if (tv_nsec <= Cmp.tv_nsec) return false;
			return true;
		}
		FileTime &FileTime::operator-=(const FileTime &Other) {
			tv_sec -= Other.tv_sec;
			tv_nsec -= Other.tv_nsec;
			Normalize();
			return *this;
		}
		FileTime &FileTime::operator+=(const FileTime &Other) {
			tv_sec += Other.tv_sec;
			tv_nsec += Other.tv_nsec;
			Normalize();
			return *this;
		}
		FileTime FileTime::operator-(const FileTime &Other) const {
			return FileTime(tv_sec - Other.tv_sec, tv_nsec - Other.tv_nsec);
		}
		FileTime FileTime::operator+(const FileTime &Other) const {
			return FileTime(tv_sec + Other.tv_sec, tv_nsec + Other.tv_nsec);
		}
		FileTime &FileTime::operator=(const FileTime &Other) {
			tv_sec = Other.tv_sec;
			tv_nsec = Other.tv_nsec;
			Normalize();
			return *this;
		}
		FileTime::operator float() const {
			return float(tv_nsec / 1e9) + tv_sec;
		}
		FileTime::operator double() const {
			return double(tv_nsec / 1e9) + tv_sec;
		}
		FileTime::operator SnzL() const {
			return tv_sec;
		}
		void FileTime::Normalize() {
			if (tv_nsec < 0)
			{
				tv_nsec += 1000000000;
				tv_sec -= 1;
			}
			else if (tv_nsec >= 1000000000)
			{
				tv_nsec -= 1000000000;
				tv_sec += 1;
			}
		}
		//		DriveBase::DriveBase() {}
		HashMap<wString, DriveBase *> DriveMap;
		FileBase::~FileBase() {}
		//		FileBase::FileBase() {}

		SInt32 GetDrvNPath(String &Path, DriveBase *&Drv) {
			ErrorFuncId = FUNC_GETDRVPATH;
			Path.Replace('\\', '/');
			if (Path[0] == '/')
			{
				Drv = DriveMap["/"];
				return 0;
			}
			else
			{
				SizeL Pos = 0;
				String TmpPath;
				if (!Path.Find(Pos, ':')) TmpPath = GetFullPath(Path.wStr()).Str();
				else TmpPath = Path;
				if (TmpPath[0] == '/')
				{
					Path = TmpPath;
					Drv = DriveMap["/"];
					return 0;
				}
				if (!TmpPath.Find(Pos, ':'))
				{
					UtilsSetError("Path Name could not be resolved. The Path Name must either be a relative directory or must include the drive name");
					return -2;
				}
				String DrvName = TmpPath.SubStr(0, Pos);
				DriveBase **Tmp = DriveMap.GetPtrVal(DrvName.wStr());
				if (Tmp == 0)
				{
					UtilsSetError("Drive Name specified was not valid or refered to a removed/non-existent drive");
					return -1;
				}
				Path = TmpPath.SubStr(Pos + 1);
				Drv = *Tmp;
				return 0;
			}
		}
		SInt32 GetDrvNPath(wString &Path, DriveBase *&Drv) {
			ErrorFuncId = FUNC_GETDRVPATH;
			Path.Replace('\\', '/');
			if (Path[0] == '/')
			{
				Drv = DriveMap["/"];
				return 0;
			}
			else
			{
				SizeL Pos = 0;
				wString TmpPath;
				if (!Path.Find(Pos, ":/")) TmpPath = GetFullPath(Path);
				else TmpPath = Path;
				if (TmpPath[0] == '/')
				{
					Path = TmpPath;
					Drv = DriveMap["/"];
					return 0;
				}
				if (!TmpPath.Find(Pos, ":/"))
				{
					UtilsSetError("Path Name could not be resolved. The Path Name must either be a relative directory or must include the drive name");
					return -2;
				}
				wString DrvName = TmpPath.SubStr(0, Pos);
				DriveBase **Tmp = DriveMap.GetPtrVal(DrvName);
				if (Tmp == 0)
				{
					UtilsSetError("Drive Name specified was not valid or refered to a removed/non-existent drive");
					return -1;
				}
				Path = TmpPath.SubStr(Pos + 1);
				Drv = *Tmp;
				return 0;
			}
		}
		bool GetIsError(DriveBase *Drv) {
			bool Rtn = Drv->ErrNotRead;
			Drv->ErrNotRead = false;
			return Rtn;
		}
		Array<String> GetFileExt(String Path, const Array<String> &Exts) {
			Array<String> Rtn;
			DriveBase *Drv = 0;
			if (GetDrvNPath(Path, Drv) != 0) return Rtn;
			ErrorFuncId = FUNC_GETF_EXT;
			try {
				Rtn = Drv->GetFileExt(Path, Exts);
				if (GetIsError(Drv)) throw Drv->Err;
			}
			catch (FileError Msg) {
				UtilsSetError(Msg.Type + ": " + Msg.Msg);
			}
			return Rtn;
		}
		FileDescA Stat(String Path) {
			FileDescA Rtn;
			DriveBase *Drv = 0;
			if (GetDrvNPath(Path, Drv) != 0) return Rtn;
			ErrorFuncId = FUNC_FSTAT;
			try {
				Rtn = Drv->Stat(Path);
				if (GetIsError(Drv)) throw Drv->Err;
			}
			catch (FileError Msg) {
				UtilsSetError(Msg.Type + ": " + Msg.Msg);
			}
			return Rtn;
		}
		Array<FileDescA> ListDirStats(String Path) {
			Array<FileDescA> Rtn;
			DriveBase *Drv = 0;
			if (GetDrvNPath(Path, Drv) != 0) return Rtn;
			ErrorFuncId = FUNC_LS_FSTAT;
			try {
				Rtn = Drv->ListDirSt(Path);
				if (GetIsError(Drv)) throw Drv->Err;
			}
			catch (FileError Msg) {
				UtilsSetError(Msg.Type + ": " + Msg.Msg);
			}
			return Rtn;
		}
		Array<String> ListDir(String Path) {
			Array<String> Rtn;
			DriveBase *Drv = 0;
			if (GetDrvNPath(Path, Drv) != 0) return Rtn;
			ErrorFuncId = FUNC_LSDIR;
			try {
				Rtn = Drv->ListDir(Path);
				if (GetIsError(Drv)) throw Drv->Err;
			}
			catch (FileError Msg) {
				UtilsSetError(Msg.Type + ": " + Msg.Msg);
			}
			return Rtn;
		}
		bool Exists(String Path) {
			DriveBase *Drv = 0;
			if (GetDrvNPath(Path, Drv) != 0) return false;
			return Drv->Exists(Path);
		}
		bool IsFile(String Path) {
			DriveBase *Drv = 0;
			if (GetDrvNPath(Path, Drv) != 0) return false;
			return Drv->IsFile(Path);
		}
		bool IsDir(String Path) {
			DriveBase *Drv = 0;
			if (GetDrvNPath(Path, Drv) != 0) return false;
			return Drv->IsDir(Path);
		}
		Array<wString> GetFileExt(wString Path, const Array<wString> &Exts) {
			Array<wString> Rtn;
			DriveBase *Drv = 0;
			if (GetDrvNPath(Path, Drv) != 0) return Rtn;
			ErrorFuncId = FUNC_GETF_EXT;
			try {
				Rtn = Drv->GetFileExt(Path, Exts);
				if (GetIsError(Drv)) throw Drv->Err;
			}
			catch (FileError Msg) {
				UtilsSetError(Msg.Type + ": " + Msg.Msg);
			}
			return Rtn;
		}
		FileDesc Stat(wString Path) {
			FileDesc Rtn;
			DriveBase *Drv = 0;
			if (GetDrvNPath(Path, Drv) != 0) return Rtn;
			ErrorFuncId = FUNC_FSTAT;
			try {
				Rtn = Drv->Stat(Path);
				if (GetIsError(Drv)) throw Drv->Err;
			}
			catch (FileError Msg) {
				UtilsSetError(Msg.Type + ": " + Msg.Msg);
			}
			return Rtn;
		}
		Array<FileDesc> ListDirStats(wString Path) {
			Array<FileDesc> Rtn;
			DriveBase *Drv = 0;
			if (GetDrvNPath(Path, Drv) != 0) return Rtn;
			ErrorFuncId = FUNC_LS_FSTAT;
			try {
				Rtn = Drv->ListDirSt(Path);
				if (GetIsError(Drv)) throw Drv->Err;
			}
			catch (FileError Msg) {
				UtilsSetError(Msg.Type + ": " + Msg.Msg);
			}
			return Rtn;
		}
		Array<wString> ListDir(wString Path) {
			Array<wString> Rtn;
			DriveBase *Drv = 0;
			if (GetDrvNPath(Path, Drv) != 0) return Rtn;
			ErrorFuncId = FUNC_LSDIR;
			try {
				Rtn = Drv->ListDir(Path);
				if (GetIsError(Drv)) throw Drv->Err;
			}
			catch (FileError Msg) {
				UtilsSetError(Msg.Type + ": " + Msg.Msg);
			}
			return Rtn;
		}
		bool Exists(wString Path) {
			DriveBase *Drv = 0;
			if (GetDrvNPath(Path, Drv) != 0) return false;
			return Drv->Exists(Path);
		}
		bool IsFile(wString Path) {
			DriveBase *Drv = 0;
			if (GetDrvNPath(Path, Drv) != 0) return false;
			return Drv->IsFile(Path);
		}
		bool IsDir(wString Path) {
			DriveBase *Drv = 0;
			if (GetDrvNPath(Path, Drv) != 0) return false;
			return Drv->IsDir(Path);
		}
		FileBase *GetFileObj(String Path, UInt32 Mode) {
			DriveBase *Drv = 0;
			if (GetDrvNPath(Path, Drv) != 0) return 0;
			ErrorFuncId = FUNC_OPENFILE;
			try {
				FileBase *Tmp = Drv->OpenFile(Path, Mode);
				if (Tmp == 0) throw Drv->Err;
				else return Tmp;
			}
			catch (FileError Msg) {
				UtilsSetError(Msg.Type + ": " + Msg.Msg);
				return 0;
			}
		}
		FileBase *GetFileObj(wString Path, UInt32 Mode) {
			DriveBase *Drv = 0;
			if (GetDrvNPath(Path, Drv) != 0) return 0;
			ErrorFuncId = FUNC_OPENFILE;
			try {
				FileBase *Tmp = Drv->OpenFile(Path, Mode);
				if (Tmp == 0) throw Drv->Err;
				else return Tmp;
			}
			catch (FileError Msg) {
				UtilsSetError(Msg.Type + ": " + Msg.Msg);
				return 0;
			}
		}
		UInt32 ParseModeStr(const String &ModeStr) {
			UInt32 Rtn = 0;
			if (ModeStr.Length() == 0 || ModeStr.Length() > 3) return MAX_INT32;
			char First = ModeStr[0];
			if (First >= 'A' && First <= 'Z') First += 32;
			switch (First) {
			case 'r':
				Rtn |= F_IN;
				break;
			case 'w':
				Rtn |= F_TRUNC | F_OUT;
				break;
			case 'a':
				Rtn |= F_APP | F_OUT;
				break;
			default:
				return MAX_INT32;
			}
			if (ModeStr.Length() == 1) return Rtn;
			if (ModeStr.Contains('b') || ModeStr.Contains('B')) Rtn |= F_BIN;
			if (ModeStr.Contains('+'))
			{
				switch (First) {
				case 'r':
					Rtn |= F_OUT | F_NOREPLACE;
					break;
				case 'w':
				case 'a':
					Rtn |= F_IN;
					break;
				}
			}
			return Rtn;
		}
		String ParseModeLong(UInt32 Md) {
			String Rtn;
			if (Md & F_IN && Md && F_OUT)
			{
				if (Md & F_TRUNC) Rtn = "w+";
				else if (Md & F_NOREPLACE) Rtn = "r+";
				else if (Md & F_APP) Rtn = "a+";
			}
			else if (Md & F_IN) Rtn = "r";
			else if (Md & F_OUT && Md & F_APP) Rtn = "a";
			else if (Md & F_OUT) Rtn = "w";
			if (Md & F_BIN) Rtn += 'b';
			return Rtn;
		}
		wString DefUniDrive::GetName() {
			return GetNameA().wStr();
		}
		FileBase *DefUniDrive::OpenFile(const wString &Path, UInt32 Mode) {
			return OpenFile(Path.Str(), Mode);
		}
		bool DefUniDrive::IsFile(const wString &Path) {
			return IsFile(Path.Str());
		}
		bool DefUniDrive::IsFile(const String &Path) {
			try {
				return Stat(Path).Mode & FTYP_MASK == FTYP_REG;
			} catch (...) {
				return false;
			}
		}
		bool DefUniDrive::Exists(const wString &Path) {
			return Exists(Path.Str());
		}
		bool DefUniDrive::Exists(const String &Path) {
			try {
				Stat(Path);
			} catch (...) {
				return false;
			}
		}
		bool DefUniDrive::IsDir(const wString &Path) {
			return IsDir(Path.Str());
		}
		bool DefUniDrive::IsDir(const String &Path) {
			try {
				return Stat(Path).Mode & FTYP_MASK == FTYP_DIR;
			} catch (...) {
				return false;
			}
		}
		Array<wString> DefUniDrive::ListDir(const wString &Path) {
			Array<String> Tmp = ListDir(Path.Str());
			Array<wString> Rtn;
			Rtn.SetLength(Tmp.Length());
			for (SizeL c = 0; c < Tmp.Length(); ++c) {
				Rtn[c] = Tmp[c].wStr();
			}
			return Rtn;
		}
		FileDesc DefUniDrive::Stat(const wString &Path) {
			FileDescA Tmp = Stat(Path.Str());
			return{
				Tmp.fName.wStr(), Tmp.Mode, Tmp.Attr, Tmp.Size,
				Tmp.CreateTime, Tmp.LastWriteTime, Tmp.LastAccessTime };
		}
		Array<FileDesc> DefUniDrive::ListDirSt(const wString &Path) {
			Array<FileDescA> Tmp = ListDirSt(Path.Str());
			Array<FileDesc> Rtn;
			Rtn.SetLength(Tmp.Length());
			for (SizeL c = 0; c < Tmp.Length(); ++c) {
				FileDescA &Cur = Tmp[c];
				Rtn[c] = {
					Cur.fName.wStr(), Cur.Mode, Cur.Attr, Cur.Size,
					Cur.CreateTime, Cur.LastWriteTime, Cur.LastAccessTime };
			}
			return Rtn;
		}
		Array<FileDescA> DefUniDrive::ListDirSt(const String &Path) {
			Array<FileDescA> Rtn;
			Array<String> Names = ListDir(Path);
			Rtn.SetLength(Names.Length());
			for (SizeL c = 0; c < Names.Length(); ++c) {
				Rtn[c] = Stat(Names[c]);
			}
			return Rtn;
		}
		Array<wString> DefUniDrive::GetFileExt(const wString &Path, const Array<wString> &Ext, bool Invert, bool RtnBegDots) {
			Array<wString> Cur;
			Array<wString> NextDir("/", 1);
			Array<wString> Rtn;
			SizeL ChunkSz = 64;
			SizeL RtnPos = 0;
			while (NextDir.Length() > 0) {
				Cur.SetLength(0);
				Cur = (Array<wString> &&)NextDir;
				for (wString &Dir : Cur) {
					if (RtnPos >= Rtn.Length()) Rtn.SetLength(Rtn.Length() + ChunkSz);
					for (FileDesc &Comp : ListDirSt(Path+Dir)) {
						wString CompName = Dir + "/" + Comp.fName;
						switch (Comp.Mode & FTYP_MASK) {
						case FTYP_REG:
						{
							wString CurExt;
							SizeL Pos = 0;
							if (Comp.fName.RFind(Pos, '.')) CurExt = Comp.fName.SubStr(Pos + 1);
							if (!(Ext.Contains(CurExt) ^ Invert)) break;
							if (!RtnBegDots && Comp.fName[0] == '.') break;
						}
							Rtn[RtnPos++] = CompName;
							break;
						case FTYP_DIR:
							NextDir += CompName;
							break;
						}
					}
				}
			}
			Rtn.SetLength(RtnPos);
			return Rtn;
		}
		Array<String> DefUniDrive::GetFileExt(const String &Path, const Array<String> &Ext, bool Invert, bool RtnBegDots) {
			Array<wString> TmpExt;
			TmpExt.SetLength(Ext.Length());
			for (SizeL c = 0; c < Ext.Length(); ++c) {
				TmpExt[c] = Ext[c].wStr();
			}
			Array<wString> Tmp = GetFileExt(Path.wStr(), TmpExt, Invert, RtnBegDots);
			Array<String> Rtn;
			Rtn.SetLength(Tmp.Length());
			for (SizeL c = 0; c < Tmp.Length(); ++c) {
				Rtn[c] = Tmp[c].Str();
			}
			return Rtn;
		}
		
	}
	bool DefDelFl(void *Fl) {
		delete (fs::FileBase *)Fl;
		return true;
	}
	UInt32 fRdBuff::BuffWorker(SizeL Id, void *Params) {
		UtilsThread CurThrd(Id);
		fRdBuff *ThisObj = (fRdBuff *)Params;
		CondVar *TheCond = ThisObj->TheCond;
		TheCond->GetInternLock()->Acquire();
		if (ThisObj->BlkLen & 0x80000000) TheCond->wait();
		while (ThisObj->BlkLen > 0) {
			try {
				ThisObj->Buff.PutBytes(ThisObj->FlObj->Read(ThisObj->BlkLen & 0x7FFFFFFF));
			}
			catch (fs::FileError &Exc) {
				ThisObj->ThrdErr = new fs::FileError(Exc);
				ThisObj->BlkLen = 0;
				break;
			}
			catch (sock::SockErr &Exc) {
				ThisObj->ThrdErr = new fs::FileError(wString("Error: ") + FromNumber(Exc.ErrCode), Exc.Msg);
				ThisObj->BlkLen = 0;
				break;
			}
			if (ThisObj->Buff.Length() >= ThisObj->MinMax[1] ||
				(ThisObj->BlkLen & 0x80000000) ||
				ThisObj->FlObj->Tell() >= ThisObj->FlLen)
				TheCond->wait();
		}
		TheCond->GetInternLock()->Release();
		return 0;
	}
	void fRdBuff::BuffGetFunc(void *Obj, SizeL NumBytes) {
		fRdBuff *ThisObj = (fRdBuff *)Obj;
		if (ThisObj->ThrdErr) throw *ThisObj->ThrdErr;
		ThisObj->Pos += NumBytes;
		//ThisObj->IsAtEnd = ThisObj->Buff.Length() + ThisObj->Pos >= ThisObj->FlLen;//Do We Need this or can we use Fl->Tell()
		if (ThisObj->Buff.Length() < ThisObj->MinMax[0]
			&& ThisObj->TheCond->GetInternLock()->TryAcquire())
		{
			ThisObj->TheCond->notify();
			ThisObj->TheCond->GetInternLock()->Release();
		}
	}
	fRdBuff::fRdBuff(fs::FileBase *Fl, UInt32 Min, UInt32 Max, UInt32 InBlkLen, bool Direct) {
		ThrdErr = 0;
		Pos = Fl->Tell();
		Fl->Seek(0, fs::SK_END);
		FlLen = Fl->Tell();
		Fl->Seek(Pos);
		FlObj = Fl;
		Buff.GetFunc = BuffGetFunc;
		Buff.CbObj = this;
		FreeFlCb = DefDelFl;
		BlkLen = InBlkLen;
		MinMax[0] = Min;
		MinMax[1] = Max;
		TheCond = GetCondVar(GetSingleMutex());
		if (Direct) BlkLen |= 0x80000000;
		Thrd.Init(BuffWorker, this);
	}
	void fRdBuff::SetFlDelFunc(bool(*DelFunc)(void *)) {
		FreeFlCb = DelFunc;
	}
	bool fRdBuff::Seek(SInt64 SkPos, SInt32 From) {
		if (BlkLen & 0x80000000)
		{
			if (!FlObj->Seek(SkPos, From)) return false;
			Pos = FlObj->Tell();
			return true;
		}
		BlkLen |= 0x80000000;
		if (ThrdErr) throw *ThrdErr;
		TheCond->GetInternLock()->Acquire();
		try {
			if (!FlObj->Seek(SkPos, From))
			{
				TheCond->notify();
				BlkLen &= 0x7FFFFFFF;
				TheCond->GetInternLock()->Release();
				return false;
			}
		}
		catch (...) {
			TheCond->notify();
			BlkLen &= 0x7FFFFFFF;
			TheCond->GetInternLock()->Release();
			throw;
		}
		if (From == fs::SK_SET) SkPos -= Pos;
		else if (From == fs::SK_END) SkPos = (FlLen + SkPos) - Pos;
		else if (From != fs::SK_CUR) SkPos = FlObj->Tell() - Pos;
		if (SkPos < 0) Buff.Clear(Buff.Length());
		else Buff.Clear(SkPos);
		TheCond->notify();
		BlkLen &= 0x7FFFFFFF;
		Pos = FlObj->Tell();
		TheCond->GetInternLock()->Release();
		return true;
	}
	SInt64 fRdBuff::Tell() {
		return Pos;
	}
	UInt32 fRdBuff::Write(const ByteArray &Data) {
		throw fs::FileError("NI", "NotImplemented: Write method is not defined for read only buffer");
	}
	void fRdBuff::Close() {
		if (!BlkLen) return;
		TheCond->GetInternLock()->Acquire();
		BlkLen = 0;
		TheCond->notify();
		TheCond->GetInternLock()->Release();
	}
	void fRdBuff::Flush() {}
	void fRdBuff::SetBuffMode(bool Direct) {
		if (Direct && (BlkLen & 0x80000000) > 0) return;
		else if (!Direct && (BlkLen & 0x80000000) == 0) return;
		if (Direct)
		{
			BlkLen |= 0x80000000;
			if (ThrdErr) throw *ThrdErr;
			TheCond->GetInternLock()->Acquire();
			Buff.Clear(Buff.Length());
			FlObj->Seek(Pos);
			TheCond->GetInternLock()->Release();
		}
		else
		{
			if (ThrdErr) throw *ThrdErr;
			TheCond->GetInternLock()->Acquire();
			BlkLen &= 0x7FFFFFFF;
			TheCond->notify();
			TheCond->GetInternLock()->Release();
		}
	}
	fRdBuff::~fRdBuff() {
		Close();
		Thrd.Join();
		TheCond->IsLockRef = false;
		DestroyCond(TheCond);
		TheCond = 0;
		try {
			FlObj->Close();
		}
		catch (...) {}
		if (FreeFlCb) FreeFlCb(FlObj);
		FlObj = 0;
		if (ThrdErr) delete ThrdErr;
	}
	wString fRdBuff::GetName() {
		return FlObj->GetName();
	}
	UInt32 fRdBuff::GetMode() {
		return FlObj->GetMode();
	}
	ByteArray fRdBuff::Read(UInt32 Num) {
		if (BlkLen & 0x80000000)
		{
			ByteArray Rtn = FlObj->Read(Num);
			Pos += Rtn.Length();
			return Rtn;
		}
		if (Pos + Num > FlLen) Num = FlLen - Pos;
		SizeL CurPos = 0;
		ByteArray Rtn(Byte(0), Num);
		Buff.GetBytes(Rtn, Num);
		return Rtn;
	}
	ByteArray fRdBuff::Read() {
		if (FlLen - Pos <= MAX_INT32) return Read(FlLen - Pos);
		else throw fs::FileError("DataOverflow", "Too much data to read");
	}
	UInt32 fRdBuff::Read(ByteArray &Data) {
		if (BlkLen & 0x80000000)
		{
			UInt32 Rtn = FlObj->Read(Data);
			Pos += Rtn;
			return Rtn;
		}
		UInt32 Num = Data.Length();
		if (Pos + Num > FlLen) Num = FlLen - Pos;
		Buff.GetBytes(Data, Num);
		return Num;
	}
}
