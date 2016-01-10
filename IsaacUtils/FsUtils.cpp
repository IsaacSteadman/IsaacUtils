#include "stdafx.h"
#include "Utils.h"
namespace Utils {
	namespace fs {
		FileError::FileError() {}
//		DriveBase::DriveBase() {}
		HashMap<wString, DriveBase *> DriveMap;
		FileBase::~FileBase() {}
//		FileBase::FileBase() {}

		signed long GetDrvNPath(String &Path, DriveBase *&Drv) {
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
		signed long GetDrvNPath(wString &Path, DriveBase *&Drv) {
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
		FileBase *GetFileObj(String Path, unsigned long Mode) {
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
		FileBase *GetFileObj(wString Path, unsigned long Mode) {
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
		FileError::FileError(wString Cap, wString Txt) {
			Msg = Txt;
			Type = Cap;
		}
		unsigned long ParseModeStr(const String &ModeStr) {
			unsigned long Rtn = 0;
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
		String ParseModeLong(unsigned long Md) {
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
	}
	unsigned long fRdBuff::BuffWorker(void *hThread, unsigned long Id, void *Params) {
		UtilsThread CurThrd(Id);
		fRdBuff *ThisObj = (fRdBuff *)Params;
		CondVar *TheCond = ThisObj->TheCond;
		TheCond->GetInternLock()->Acquire();
		while (ThisObj->BlkLen > 0) {
			ThisObj->Buff.PutBytes(ThisObj->FlObj->Read(ThisObj->BlkLen));
			if (ThisObj->Buff.Length() >= ThisObj->MinMax[1] ||
				(ThisObj->BlkLen & 0x80000000 > 0) ||
				ThisObj->Buff.Length() + ThisObj->Pos >= ThisObj->FlLen)
				TheCond->wait();
		}
		TheCond->IsLockRef = false;
		DestroyCond(TheCond);
		ThisObj->TheCond = 0;
		ThisObj->FlObj->Close();
		ThisObj->FlObj = 0;
		ThisObj->TheCond = 0;
		TheCond->GetInternLock()->Release();
		return 0;
	}
	void fRdBuff::BuffGetFunc(void *Obj, SizeL NumBytes) {
		((fRdBuff *)Obj)->Pos += NumBytes;
	}
	fRdBuff::fRdBuff(fs::FileBase *Fl, unsigned long Min, unsigned long Max, unsigned long InBlkLen) {
		Pos = Fl->Tell();
		Fl->Seek(0, fs::SK_END);
		FlLen = Fl->Tell();
		Fl->Seek(Pos);
		Buff.GetFunc = BuffGetFunc;
		Buff.CbObj = this;
		BlkLen = InBlkLen;
		MinMax[0] = Min;
		MinMax[1] = Max;
		TheCond = GetCondVar(GetSingleMutex());
		Thrd.Init(BuffWorker, this);
	}
	bool fRdBuff::Seek(long long SkPos, int From) {
		BlkLen |= 0x80000000;
		TheCond->GetInternLock()->Acquire();
		FlObj->Seek(SkPos, From);
		if (From == fs::SK_SET) SkPos -= Pos;
		else if (From == fs::SK_END) SkPos = FlLen - Pos;
		else if (From != fs::SK_CUR) SkPos = FlObj->Tell() - Pos;
		Buff.Clear(SkPos);
		TheCond->notify();
		BlkLen &= 0x7FFFFFFF;
		TheCond->GetInternLock()->Release();
		return true;
	}
	long long fRdBuff::Tell() {
		return Pos;
	}
	unsigned long fRdBuff::Write(const ByteArray &Data) {
		throw fs::FileError("NI", "NotImplemented: Write method is not defined for read only buffer");
	}
	void fRdBuff::Close() {
		TheCond->GetInternLock()->Acquire();
		BlkLen = 0;
		TheCond->notify();
		TheCond->GetInternLock()->Release();
	}
	void fRdBuff::Flush() {}
	fRdBuff::~fRdBuff() {
		Close();
		Thrd.Join();
	}
	wString fRdBuff::GetName() {
		return FlObj->GetName();
	}
	unsigned long fRdBuff::GetMode() {
		return FlObj->GetMode();
	}
	ByteArray fRdBuff::Read(unsigned long Num) {
		if (Pos + Num > FlLen) Num = FlLen - Pos;
		SizeL CurPos = 0;
		ByteArray Rtn(Byte(0), Num);
		while (Num > MinMax[1]) {
			if (TheCond->GetInternLock()->TryAcquire())
			{
				if (Buff.Length() < MinMax[0]) TheCond->notify();
				TheCond->GetInternLock()->Release();
			}
			Buff.GetBytes(Rtn, MinMax[1], CurPos);
			CurPos += MinMax[1];
			Num -= MinMax[1];
		}
		if (TheCond->GetInternLock()->TryAcquire())
		{
			if (Buff.Length() < MinMax[0]) TheCond->notify();
			TheCond->GetInternLock()->Release();
		}
		Buff.GetBytes(Rtn, Num, CurPos);
		return Rtn;
	}
	ByteArray fRdBuff::Read() {
		if (FlLen - Pos <= MAX_INT32) return Read(FlLen - Pos);

	}
}