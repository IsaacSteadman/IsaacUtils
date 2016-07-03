#if defined(_WIN32) || defined(_WIN64)
#include "stdafx.h"
#endif
#include "Utils.h"
namespace Utils {
	namespace sock {
		HashMap<SizeL, String> ErrCodes;
		bool Usable = false;
		SockErr::SockErr() {}
		SockErr::SockErr(int SockErrCode) {
			ErrCode = SockErrCode;
			if (ErrCodes.HasKey(ErrCode)) Msg = ErrCodes[ErrCode].wStr();
		}
		void Socket::setblocking(bool IsBlk) {
			if (IsBlk) settimeout(-1);
			else settimeout(0);
		}
		SizeL Socket::send(const ByteArray &Bytes, int Flags) {
			return sendBase((const char *)Bytes.GetData(), Bytes.Length(), Flags);
		}
		SizeL Socket::send(const String &Bytes, int Flags) {
			return sendBase(Bytes.GetData(), Bytes.Length(), Flags);
		}
		SizeL Socket::sendto(const ByteArray &Bytes, const SockAddr &Addr, int Flags) {
			return sendtoBase((const char *)Bytes.GetData(), Bytes.Length(), Addr, Flags);
		}
		SizeL Socket::sendto(const String &Bytes, const SockAddr &Addr, int Flags) {
			return sendtoBase(Bytes.GetData(), Bytes.Length(), Addr, Flags);
		}
		ByteArray Socket::recv(SizeL Num, int Flags) {
			ByteArray Rtn(Byte(0), Num);
			Rtn.SetLength(recvBase((char *)Rtn.GetData(), Num, Flags));
			return Rtn;
		}
		String Socket::recvS(SizeL Num, int Flags) {
			String Rtn(char(0), Num);
			Rtn.SetLength(recvBase((char *)Rtn.GetData(), Num, Flags));
			return Rtn;
		}
		ByteArray Socket::recvfrom(SockAddr &Addr, SizeL Num, int Flags) {
			ByteArray Rtn(Byte(0), Num);
			Rtn.SetLength(recvfromBase(Addr, (char *)Rtn.GetData(), Num, Flags));
			return Rtn;
		}
		String Socket::recvfromS(SockAddr &Addr, SizeL Num, int Flags) {
			String Rtn(char(0), Num);
			Rtn.SetLength(recvfromBase(Addr, (char *)Rtn.GetData(), Num, Flags));
			return Rtn;
		}
		SockAddr Socket::getsockname() {
			return SockName;
		}
		SockAddr Socket::getpeername() {
			return PeerName;
		}
		double Socket::gettimeout() {
			if (TmOuts[0] == MAX_INT32) return -1;
			double Rtn = TmOuts[1];
			Rtn /= 1000000;
			Rtn += TmOuts[0];
			return Rtn;
		}
		SockAddr::SockAddr() {
			Data = 0;
		}
		SockAddr::SockAddr(SockAddr &&Cpy) {
			Data = Cpy.Data;
			Cpy.Data = 0;
		}
		SockAddr::~SockAddr() {
			DeInitData();
		}
		SockAddr &SockAddr::operator=(SockAddr &&Cpy) {
			void *Tmp = Data;
			Data = Cpy.Data;
			Cpy.Data = Tmp;
			return *this;
		}
		void *SockAddr::GetData() const {
			return Data;
		}
	}
	void PadForEnc(SizeL KeyLen, ByteArray &Data) {
		SizeL CurLen = Data.Length();
		if (CurLen % KeyLen != 0) Data.SetLength(CurLen + (KeyLen - CurLen % KeyLen));
		else return;
		for (; CurLen < Data.Length(); ++CurLen) {
			Data[CurLen] = 0;
		}
	}
	EncProt::EncProt(MidEncSt *EncSt, sock::Socket *Conn, double TmOut) {
		Enc = EncSt;
		Sock = Conn;
		Sock->InitMngd(8 * 16777216);
		Timeout = TmOut;
		BlkLen = 4096;
	}
	void EncProt::Recv(AbsFile NoEnc, AbsFile EncDat) {
		Byte NumLen = Sock->recv(1)[0];
		BigLong NoEncLen;
		BigLong EncLen;
		{
			ByteArray Tmp = Sock->recv(NumLen % 16 + 1);
			NoEncLen = BigLong(Tmp.GetData(), Tmp.Length());
			Tmp = Sock->recv(NumLen / 16 + 1);
			EncLen = BigLong(Tmp.GetData(), Tmp.Length());
		}
		NoEnc.PreAlloc(NoEncLen);
		EncDat.PreAlloc(EncLen);
		double TmRtn = Sock->gettimeout();
		try {
			long long PartLen = (NoEncLen / BlkLen).ToLL();
			while (PartLen-- > 0) {
				NoEnc.Write(Sock->recv(BlkLen));
			}
			PartLen = (NoEncLen % BlkLen).ToLL();
			if (PartLen > 0) NoEnc.Write(Sock->recv(PartLen));
			PartLen = (EncLen / BlkLen).ToLL();
			while (PartLen-- > 0) {
				ByteArray Data = Sock->recv(BlkLen);
				if (Enc != 0) Enc->Decrypt(Data);
				EncDat.Write(Data);
			}
			PartLen = (EncLen % BlkLen).ToLL();
			if (PartLen > 0)
			{
				SizeL NumChunks = (PartLen + Enc->Key.Length() - 1) / Enc->Key.Length();
				ByteArray Data = Sock->recv(Enc->Key.Length() * NumChunks);
				if (Enc != 0) Enc->Decrypt(Data);
				EncDat.Write(Data.SubArr(0, PartLen));
			}
		}
		catch (sock::SockErr &Exc) {
			Sock->settimeout(TmRtn);
			throw Exc;
		}
		Sock->settimeout(TmRtn);
	}
	void EncProt::Send(const AbsFile NoEnc, const AbsFile EncDat) {
		ByteArray NoEncHead = NoEnc.GetLen().ToByteArray();
		if (Enc == 0 && EncDat.GetLen() > (unsigned long)0) throw sock::SockErr(11001);
		SizeL KeyLen = bool(Enc) ? Enc->Key.Length() : 0;
		BigLong BlKeyLen = KeyLen;
		ByteArray EncHead = EncDat.GetLen().ToByteArray();
		SizeL Pos = MAX_INT;
		SizeL BegZeros = NoEncHead.Length();
		while (BegZeros > 1) {
			--BegZeros;
			if (NoEncHead[BegZeros] != 0)
			{
				++BegZeros;
				break;
			}
		}
		NoEncHead.SetLength(BegZeros);
		BegZeros = EncHead.Length();
		while (BegZeros > 1) {
			--BegZeros;
			if (EncHead[BegZeros] != 0)
			{
				++BegZeros;
				break;
			}
		}
		EncHead.SetLength(BegZeros);
		ByteArray Header(Byte(0), 1 + NoEncHead.Length() + EncHead.Length());
		SizeL c = 1;
		for (SizeL NoEncPos = 0; NoEncPos < NoEncHead.Length(); ++NoEncPos, ++c)
			Header[c] = NoEncHead[NoEncPos];
		for (SizeL EncPos = 0; EncPos < EncHead.Length(); ++EncPos, ++c)
			Header[c] = EncHead[EncPos];
		Header[0] = Byte(NoEncHead.Length() - 1 + 16 * (EncHead.Length() - 1));
		Sock->send(Header);
		double TmRtn = Sock->gettimeout();
		try {
			Sock->settimeout(Timeout);
			ByteArray Data;
			NoEnc.Read(Data, BlkLen);
			while (Data.Length() == BlkLen) {
				Sock->send(Data);
				NoEnc.Read(Data, BlkLen);
			}
			if (Data.Length() > 0) Sock->send(Data);
			if (Enc == 0)
			{
				Sock->settimeout(TmRtn);
				return;
			}
			EncDat.Read(Data, BlkLen);
			while (Data.Length() == BlkLen) {
				Enc->Encrypt(Data);
				Sock->send(Data);
				EncDat.Read(Data, BlkLen);
			}
			if (Data.Length() > 0)
			{
				PadForEnc(KeyLen, Data);
				Enc->Encrypt(Data);
				Sock->send(Data);
			}
		}
		catch (sock::SockErr &Exc) {
			Sock->settimeout(TmRtn);
			throw Exc;
		}
		Sock->settimeout(TmRtn);
	}
	void RfsDrv::UniStrNi() {
		ErrIsRead = false;
		Err.Type = "UNISTR_NI";
		Err.Msg = "Unicode support not available";
	}
	void RfsDrv::NoImp() {
		ErrIsRead = false;
		Err.Type = "NI";
		Err.Msg = "Not Implemented or Not Available";
	}
	RfsDrv::RfsDrv(EncProt *Serv, const String &Username) {
		Prot = Serv;
		Name = Username;
		FsLock = GetSingleMutex();
	}
	wString RfsDrv::GetName() {
		return Name.wStr();
	}
	String RfsDrv::GetNameA() {
		return Name;
	}
	fs::FileBase *RfsDrv::OpenFile(const wString &Path, unsigned long Mode) {
		UniStrNi();
		return 0;
	}
	//#GC-CHECK delete
	fs::FileBase *RfsDrv::OpenFile(const String &Path, unsigned long Mode) {
		Lock Lk(FsLock);
		return new RfsFile(Prot, Path, fs::ParseModeLong(Mode), FsLock);
	}
	bool RfsDrv::IsFile(const wString &Path) {
		UniStrNi();
		return 0;
	}
	bool RfsDrv::IsFile(const String &Path) {
		ByteArray Rtn(Byte(FL_ISFILE), 1);
		Prot->Send(Rtn, (String &)Path);
		Prot->Recv(Rtn, AbsFile());
		return Rtn[1] != 0;
	}
	bool RfsDrv::Exists(const wString &Path) {
		UniStrNi();
		return 0;
	}
	bool RfsDrv::Exists(const String &Path) {
		ByteArray Rtn(Byte(FL_EXIST), 1);
		Prot->Send(Rtn, (String &)Path);
		Prot->Recv(Rtn, AbsFile());
		return Rtn[1] != 0;
	}
	bool RfsDrv::IsDir(const wString &Path) {
		UniStrNi();
		return 0;
	}
	bool RfsDrv::IsDir(const String &Path) {
		ByteArray Rtn(Byte(FL_ISDIR), 1);
		Prot->Send(Rtn, (String &)Path);
		Prot->Recv(Rtn, AbsFile());
		return Rtn[1] != 0;
	}
	Array<wString> RfsDrv::ListDir(const wString &Path) {
		UniStrNi();
		return Array<wString>();
	}
	Array<String> RfsDrv::ListDir(const String &Path) {
		ByteArray Rtn(Byte(FL_LIST), 1);
		Prot->Send(Rtn, (String &)Path);
		ByteArray EncDat;
		Prot->Recv(Rtn, EncDat);
		if (Rtn[0] != 0)
		{
			ErrIsRead = false;
			Err.Type = FromNumber(Rtn[0], 16);
			Err.Msg = wString((char *)Rtn.GetData() + 1, Rtn.Length() - 1);
			return Array<String>();
		}
		else return SplitStr((String &)EncDat, 2);
	}
	fs::FileDesc RfsDrv::Stat(const wString &Path) {
		UniStrNi();
		return{ "Error", 0, 0, 0, 0, 0 };
	}
	fs::FileDescA RfsDrv::Stat(const String &Path) {
		NoImp();
		return{ "Error", 0, 0, 0, 0, 0 };
	}
	Array<fs::FileDesc> RfsDrv::ListDirSt(const wString &Path) {
		UniStrNi();
		return Array<fs::FileDesc>();
	}
	Array<fs::FileDescA> RfsDrv::ListDirSt(const String &Path) {
		NoImp();
		return Array<fs::FileDescA>();
	}
	Array<wString> RfsDrv::GetFileExt(const wString &Path, const Array<wString> &Exts, bool Invert, bool RtnBegDots) {
		UniStrNi();
		return Array<wString>();
	}
	Array<String> RfsDrv::GetFileExt(const String &Path, const Array<String> &Exts, bool Invert, bool RtnBegDots) {//TODO
		Byte Opts = 0;
		if (Invert) Opts |= FL_LS_OPT_NOT;
		if (RtnBegDots) Opts |= FL_LS_OPT_BEGDOTS;
		String Rtn(char(FL_LIST_EXTS), 2);
		Rtn[1] = Opts;
		SizeL Pos = 2;
		PackListStr(Rtn, Pos, Exts, 1, 1);
		ByteArray NoEnc;
		Rtn.SetLength(0);
		Prot->Recv(NoEnc, Rtn);
		return UnpackListStr(Rtn, 2, 2);
	}

	RfsFile::RfsFile(EncProt *Serv, const String &fName, const String &Mode, Mutex *RfsLock) {
		Md = Mode;
		Md.ToLower();
		ThisfName = fName;
		FsLock = RfsLock;
		Pos = 0;
		Prot = Serv;
		ByteArray NoEnc(Byte(FL_OPEN), 1);
		NoEnc += (ByteArray &)Mode;
		if (FsLock) FsLock->Acquire();
		Prot->Send(NoEnc, ThisfName);
		Prot->Recv(NoEnc, AbsFile());
		if (FsLock) FsLock->Release();
		if (NoEnc[0] != 0)
			throw fs::FileError(wString("Error Code ") + FromNumber(NoEnc[0]),
				wString((char *)NoEnc.GetData() + 1, NoEnc.Length() - 1));
		NoEnc.RemBeg(1);
		IdStr = (ByteArray&&)NoEnc;
	}
	ByteArray RfsFile::Read() {
		ByteArray Rtn;
		if (FsLock)
		{
			Lock Lk(FsLock);
			InternRead(Rtn, InternGetLen());
		}
		else InternRead(Rtn, InternGetLen());
		return Rtn;
	}
	ByteArray RfsFile::Read(unsigned long Num) {
		ByteArray Rtn;
		if (FsLock)
		{
			Lock Lk(FsLock);
			InternRead(Rtn, Num);
		}
		else InternRead(Rtn, Num);
		return Rtn;
	}
	unsigned long RfsFile::Read(ByteArray &Data) {
		ByteArray Rtn;
		if (FsLock)
		{
			Lock Lk(FsLock);
			InternRead(Rtn, Data.Length());
		}
		else InternRead(Rtn, Data.Length());
		Data.WriteFromAt(Rtn);
		return Rtn.Length();
	}
	bool RfsFile::Seek(long long PosIn, int From) {
		if (FsLock)
		{
			Lock Lk(FsLock);
			return InternSeek(PosIn, From);
		}
		else return InternSeek(PosIn, From);
	}
	long long RfsFile::Tell() {
		if (FsLock)
		{
			Lock Lk(FsLock);
			return InternTell();
		}
		else return InternTell();
	}
	unsigned long RfsFile::Write(const ByteArray &Data) {
		if (FsLock)
		{
			Lock Lk(FsLock);
			return InternWrite(Data);
		}
		else return InternWrite(Data);
	}
	void RfsFile::Close() {
		if (FsLock)
		{
			Lock Lk(FsLock);
			InternClose();
		}
		else InternClose();
	}
	void RfsFile::Flush() {}
	wString RfsFile::GetName() {
		return ThisfName.wStr();
	}
	unsigned long RfsFile::GetMode() {
		return fs::ParseModeStr(Md);
	}
	unsigned long long RfsFile::InternGetLen() {
		long long Prev = InternTell();
		InternSeek(0, fs::SK_END);
		long long Rtn = InternTell();
		InternSeek(Prev, fs::SK_SET);
		return Rtn;
	}
	void RfsFile::InternRead(ByteArray &Data, unsigned long Num) {
		bool IsEnc = Md[0] >= 'A' && Md[0] <= 'Z';
		ByteArray NoEnc = IdStr;
		NoEnc.Insert(0, FL_READ);
		NoEnc.SetLength(10);
		NoEnc.AtEnd() = IsEnc;
		NoEnc[5] = Num & 0xFF;
		NoEnc[6] = (Num >> 8) & 0xFF;
		NoEnc[7] = (Num >> 16) & 0xFF;
		NoEnc[8] = (Num >> 24) & 0xFF;
		Prot->Send(NoEnc, AbsFile());
		Data.SetLength(0);
		if (IsEnc)
		{
			ByteArray Rtn;
			Prot->Recv(Rtn, Data);
			if (Rtn[0] != 0)
				throw fs::FileError(wString("Rfs:") + FromNumber(Rtn[0], 10), wString((char *)&Rtn[1], Rtn.Length() - 1));
			//If no error then data is filled with the bytes it was supposed to have
		}
		else
		{
			Prot->Recv(Data, AbsFile());
			if (Data.Length() > 0 && Data[0] != 0)
				throw fs::FileError(wString("Rfs:") + FromNumber(Data[0], 10), wString((char *)&Data[1], Data.Length() - 1));
			else if (Data.Length() == 0) throw fs::FileError("Rfs:-1", "Unknown error occured");
			Data.RemBeg(1);
		}
		if (Pos != -1) Pos += Data.Length();
	}
	unsigned long RfsFile::InternWrite(const ByteArray &Data) {
		bool IsEnc = Md[0] >= 'A' && Md[0] <= 'Z';
		ByteArray NoEnc = IdStr;
		NoEnc.Insert(0, FL_WRITE);
		NoEnc.SetLength(6);
		NoEnc.AtEnd() = IsEnc;
		if (IsEnc)
		{
			ByteArray TheDat = Data;
			Prot->Send(NoEnc, TheDat);
		}
		else
		{
			NoEnc += Data;
			Prot->Send(NoEnc, AbsFile());
		}
		NoEnc.SetLength(0);
		Prot->Recv(NoEnc, AbsFile());
		if (NoEnc.Length() > 0 && NoEnc[0] != 0)
			throw fs::FileError(wString("Rfs:") + FromNumber(NoEnc[0], 10), wString((char *)&NoEnc[1], NoEnc.Length() - 1));
		else if (NoEnc.Length() == 0) throw fs::FileError("Rfs:-1", "Unknown error occured");
		if (Pos != -1) Pos += Data.Length();
		return Data.Length();
	}
	bool RfsFile::InternSeek(long long PosIn, int From) {
		Pos = -1;
		if (PosIn < 0)
		{
			From |= 0x80;
			PosIn = -PosIn;
		}
		ByteArray NoEnc = IdStr;
		NoEnc.Insert(0, FL_SEEK);
		NoEnc += Byte(From);
		NoEnc += BigLong((unsigned long long)PosIn).ToByteArray();
		Prot->Send(NoEnc, AbsFile());
		NoEnc.SetLength(0);
		Prot->Recv(NoEnc, AbsFile());
		if (NoEnc.Length() > 0 && NoEnc[0] != 0)
			throw fs::FileError(wString("Rfs:") + FromNumber(NoEnc[0], 10), wString((char *)&NoEnc[1], NoEnc.Length() - 1));
		else if (NoEnc.Length() == 0) throw fs::FileError("Rfs:-1", "Unknown error occured");
	}
	long long RfsFile::InternTell() {
		if (Pos != -1) return Pos;
		ByteArray NoEnc = IdStr;
		NoEnc.Insert(0, FL_TELL);
		Prot->Send(NoEnc, AbsFile());
		NoEnc.SetLength(0);
		Prot->Recv(NoEnc, AbsFile());
		if (NoEnc.Length() > 0 && NoEnc[0] != 0)
			throw fs::FileError(wString("Rfs:") + FromNumber(NoEnc[0], 10), wString((char *)&NoEnc[1], NoEnc.Length() - 1));
		else if (NoEnc.Length() == 0) throw fs::FileError("Rfs:-1", "Unknown error occured");
		return Pos = BigLong(NoEnc.GetData() + 1, NoEnc.Length() - 1).ToLL();
	}
	void RfsFile::InternClose() {
		ByteArray NoEnc = IdStr;
		NoEnc.Insert(0, FL_CLOSE);
		Prot->Send(NoEnc, AbsFile());
		NoEnc.SetLength(0);
		Prot->Recv(NoEnc, AbsFile());
		if (NoEnc.Length() > 0 && NoEnc[0] != 0)
			throw fs::FileError(wString("Rfs:") + FromNumber(NoEnc[0], 10), wString((char *)&NoEnc[1], NoEnc.Length() - 1));
		else if (NoEnc.Length() == 0) throw fs::FileError("Rfs:-1", "Unknown error occured");
	}
}
