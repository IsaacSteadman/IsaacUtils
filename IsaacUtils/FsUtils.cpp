#include "stdafx.h"
#include "Utils.h"
namespace Utils {
	namespace fs {
		FileError::FileError() {}
		DriveBase::DriveBase() {}
		HashMap<wString, DriveBase *> DriveMap;
		FileBase::~FileBase() {}
		FileBase::FileBase() {}

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
				if (!Path.Find(Pos, ":/")) TmpPath = GetFullPath(Path.wStr()).Str();
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
		Array<String> GetFileExt(String Path, String Ext) {
			Array<String> Rtn;
			DriveBase *Drv = 0;
			if (GetDrvNPath(Path, Drv) != 0) return Rtn;
			ErrorFuncId = FUNC_GETF_EXT;
			try {
				Rtn = Drv->GetFileExt(Path, Ext);
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
		Array<wString> GetFileExt(wString Path, wString Ext) {
			Array<wString> Rtn;
			DriveBase *Drv = 0;
			if (GetDrvNPath(Path, Drv) != 0) return Rtn;
			ErrorFuncId = FUNC_GETF_EXT;
			try {
				Rtn = Drv->GetFileExt(Path, Ext);
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
					Rtn |= F_TRUNC;
					break;
				case 'a':
					Rtn |= F_IN;
					break;
				}
			}
			return Rtn;
		}
	}
}