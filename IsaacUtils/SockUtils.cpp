#include "stdafx.h"
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
		double Socket::gettimeout() {
			if (TmOuts[0] == MAX_INT32) return -1;
			double Rtn = TmOuts[1];
			Rtn /= 1000000;
			Rtn += TmOuts[0];
			return Rtn;
		}

	}
}