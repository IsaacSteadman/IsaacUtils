// The following ifdef block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the ISAACUTILS_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see
// ISAACUTILS_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.

#include "Utils.h"

extern "C"{
	//Warning: This CRT interface is not completely thread safe (Actually now it is)  (making new variables and deallocating existing ones)
	//  The Concurrent Queue can be used safely by a writing thread that does not make use of this interface but instead uses the C++ interface
	//    and a reading thread that can use this CRT interface
	// Initializes IsaacUtils and the IsaacUtils C Runtime
	ISAACUTILS_API void Init();
	// Deallocates the object [Obj]
	ISAACUTILS_API bool DelObj(void *Obj);
	// Deallocates all objects (BigLong, ByteArray, Socket, wString) created by this runtime
	ISAACUTILS_API void CleanHeap();
	// DeInitializes IsaacUtils and the IsaacUtils C Runtime
	ISAACUTILS_API void DeInit();
	// Constructs a new wString from an array of wchar_t pointed to by [Str] of length [Len]
	ISAACUTILS_API void *wStr_newWLen(wchar_t *Str, SizeL Len);
	// Constructs a new wString from a null terminated array of wchar_t pointed to by [Str]
	ISAACUTILS_API void *wStr_newW(wchar_t *Str);
	// Constructs a new wString from an array of char pointed to by [Str] of length [Len]
	ISAACUTILS_API void *wStr_newALen(char *Str, SizeL Len);
	// Constructs a new wString from a null terminated array of char pointed to by [Str]
	ISAACUTILS_API void *wStr_newA(char *Str);
	// Constructs a new empty wString
	ISAACUTILS_API void *wStr_new();
	// Retreive the length of the wString [wStr]
	ISAACUTILS_API unsigned long wStr_Len(void *wStr);
	// Retreives the underlying array pointer for wString [wStr]
	ISAACUTILS_API void *wStr_DataPtr(void *wStr);
	// Constructs a new empty ByteArray
	ISAACUTILS_API void *ByteArray_new();
	// Constructs a new ByteArray from a null terminated array of char pointed to by [Str]
	ISAACUTILS_API void *ByteArray_newA(char *Str);
	// Constructs a new ByteArray from an array of char pointed to by [Str] of length [Len]
	ISAACUTILS_API void *ByteArray_newALen(char *Str, SizeL Len);
	// Constructs a new ByteArray from a null terminated array of wchar_t pointed to by [Str] according to [ChOpt]
	ISAACUTILS_API void *ByteArray_newW(wchar_t *Str, Utils::Byte ChOpt);
	// Constructs a new ByteArray from a wString pointed to by [wStr] according to [ChOpt]
	ISAACUTILS_API void *ByteArray_newWStr(void *wStr, Utils::Byte ChOpt);
	// Constructs a Big Endian ByteArray that represents the BigLong [BLong]
	ISAACUTILS_API void *ByteArray_newBigLong(void *BLong);
	// Retreives the length of the ByteArray [bArray]
	ISAACUTILS_API SizeL ByteArray_Len(void *bArray);
	// Retreives the underlying array pointer for ByteArray [bArray]
	ISAACUTILS_API void *ByteArray_DataPtr(void *bArray);
	// Retreives the underlying array pointer of native endian 32 bit unsigned integers of BigLong [Bl]
	ISAACUTILS_API void *BigLong_DataPtr(void *Bl);
	// Retreives the length of the unsigned long array for [Bl] in Bytes
	ISAACUTILS_API SizeL BigLong_Len(void *Bl);
	// Constructs a new BigLong set to 0
	ISAACUTILS_API void *BigLong_new();
	// Constructs a new BigLong initialized to long [L]
	ISAACUTILS_API void *BigLong_newLong(long L);
	// Constructs a new BigLong from a Big Endian ByteArray [bArray]
	ISAACUTILS_API void *BigLong_newByteArray(void *bArray);
	// Constructs a new BigLong from a Big Endian string [Str] of length [Len]
	ISAACUTILS_API void *BigLong_newALen(char *Str, SizeL Len);
	// Makes the BigLong [Bl] represent the human readable wString [wStr] number in base [base]
	ISAACUTILS_API bool BigLong_FromwString(void *Bl, void *wStr, Utils::Byte base);
	// Makes the human readable wString [wStr] from the BigLong [Bl] number in base [base]
	ISAACUTILS_API bool BigLong_TowString(void *Bl, void *wStr, Utils::Byte base);
	// Copies [BlThat] to [BlThis] and returns the [BlThis] pointer
	ISAACUTILS_API void *BigLong_Assign(void *BlThis, void *BlThat);
	// Adds [BlThat] to [BlThis] and returns new BigLong pointer result
	ISAACUTILS_API void *BigLong_Add(void *BlThis, void *BlThat);
	// Adds [BlThat] to [BlThis] and returns [BlThis]
	ISAACUTILS_API void *BigLong_IAdd(void *BlThis, void *BlThat);
	// Subtracts [BlThat] from [BlThis] and returns new BigLong pointer result
	ISAACUTILS_API void *BigLong_Sub(void *BlThis, void *BlThat);
	// Subtracts [BlThat] from [BlThis] and returns [BlThis]
	ISAACUTILS_API void *BigLong_ISub(void *BlThis, void *BlThat);
	// Multiplies [BlThat] by [BlThis] and returns new BigLong pointer result
	ISAACUTILS_API void *BigLong_Mul(void *BlThis, void *BlThat);
	// Multiplies [BlThat] by [BlThis] and returns [BlThis]
	ISAACUTILS_API void *BigLong_IMul(void *BlThis, void *BlThat);
	// Multiplies [BlThat] by [BlThis] and returns [BlThis] max of (LimNum + 3) / 4 bytes
	ISAACUTILS_API void *BigLong_IMulLim(void *BlThis, void *BlThat, SizeL LimNum);
	// Divides [BlThat] by [BlThis] and returns new BigLong pointer result
	ISAACUTILS_API void *BigLong_Div(void *BlThis, void *BlThat);
	// Divides [BlThat] by [BlThis] and returns [BlThis]
	ISAACUTILS_API void *BigLong_IDiv(void *BlThis, void *BlThat);
	// Modulos [BlThat] by [BlThis] and returns new BigLong pointer result
	ISAACUTILS_API void *BigLong_Mod(void *BlThis, void *BlThat);
	// Modulos [BlThat] by [BlThis] and returns [BlThis]
	ISAACUTILS_API void *BigLong_IMod(void *BlThis, void *BlThat);
	// Left Shifts the bits of a new copy of [BlTHis] by [That] many places and returns that copy
	ISAACUTILS_API void *BigLong_LShift(void *BlThis, unsigned long That);
	// Left Shifts [BlTHis] by [That] many places and returns [BlThis]
	ISAACUTILS_API void *BigLong_ILShift(void *BlThis, unsigned long That);
	// Right Shifts the bits of a new copy of [BlTHis] by [That] many places and returns that copy
	ISAACUTILS_API void *BigLong_RShift(void *BlThis, unsigned long That);
	// Right Shifts [BlTHis] by [That] many places and returns [BlThis]
	ISAACUTILS_API void *BigLong_IRShift(void *BlThis, unsigned long That);
	// Bitwise And result of [BlThis] and [BlThat] as a new BigLong
	ISAACUTILS_API void *BigLong_And(void *BlThis, void *BlThat);
	// Bitwise And result of [BlThis] and [BlThat] in [BlThis]
	ISAACUTILS_API void *BigLong_IAnd(void *BlThis, void *BlThat);
	// Bitwise Or result of [BlThis] and [BlThat] as a new BigLong
	ISAACUTILS_API void *BigLong_Or(void *BlThis, void *BlThat);
	// Bitwise Or result of [BlThis] and [BlThat] in [BlThis]
	ISAACUTILS_API void *BigLong_IOr(void *BlThis, void *BlThat);
	// Bitwise Xor result of [BlThis] and [BlThat] as a new BigLong
	ISAACUTILS_API void *BigLong_Xor(void *BlThis, void *BlThat);
	// Bitwise Xor result of [BlThis] and [BlThat] in [BlThis]
	ISAACUTILS_API void *BigLong_IXor(void *BlThis, void *BlThat);
	// Bool Is Equal: *((BigLong *)BlThis) == *((BigLong *)BlThat)
	ISAACUTILS_API bool BigLong_IsEq(void *BlThis, void *BlThat);
	// Bool Is not Equal: *((BigLong *)BlThis) != *((BigLong *)BlThat)
	ISAACUTILS_API bool BigLong_IsNe(void *BlThis, void *BlThat);
	// Bool Is not Equal: *((BigLong *)BlThis) < *((BigLong *)BlThat)
	ISAACUTILS_API bool BigLong_IsLt(void *BlThis, void *BlThat);
	// Bool Is not Equal: *((BigLong *)BlThis) <= *((BigLong *)BlThat)
	ISAACUTILS_API bool BigLong_IsLe(void *BlThis, void *BlThat);
	// Bool Is not Equal: *((BigLong *)BlThis) > *((BigLong *)BlThat)
	ISAACUTILS_API bool BigLong_IsGt(void *BlThis, void *BlThat);
	// Bool Is not Equal: *((BigLong *)BlThis) >= *((BigLong *)BlThat)
	ISAACUTILS_API bool BigLong_IsGe(void *BlThis, void *BlThat);
	// Retreives the bit length of [Bl]
	ISAACUTILS_API unsigned long long BigLong_BitLen(void *Bl);
	// Modular Exponentiation: [Base] ** [Exp] % [Mod]
	ISAACUTILS_API void *BigLong_ModPow(void *Base, void *Exp, void *Mod);
	// Gets the last error pointer to a cstring of wchar_t
	ISAACUTILS_API void *LastErrorDataPtrW();
	// Gets the Length of the last error
	ISAACUTILS_API SizeL LastErrorLenW();
	// Gets the last error code, or 0 if there was no error
	ISAACUTILS_API unsigned long UlLastError();
	// Constructs a new Socket with the address family, type and protocol specified
	ISAACUTILS_API void *Socket_newAfTpProt(int af, int type, int prot);
	// Binds [Sock] to the address [Addr], returns true if successful
	ISAACUTILS_API bool Socket_bind(void *Sock, void *Addr);
	// Connects [Sock] to the address [Addr], returns true if successful
	ISAACUTILS_API bool Socket_connect(void *Sock, void *Addr);
	// Sets the socket option values to the value in the ByteArray [bArray]
	ISAACUTILS_API bool Socket_setsockopt(void *Sock, int Lvl, int OptName, void *bArray);
	// Accepts a connection request to the bound Socket [Sock] and places the connection in [Conn]
	ISAACUTILS_API bool Socket_accept(void *Sock, void *Conn);
	// Sets the allowed maximum number of pending connections
	ISAACUTILS_API bool Socket_listen(void *Sock, int Backlog);
	// Closes [Sock]
	ISAACUTILS_API bool Socket_close(void *Sock);
	// Sends the data in ByteArray [bArray] to the peer of [Sock], if successful LastError.Length() == 0
	ISAACUTILS_API SizeL Socket_send(void *Sock, void *bArray, int Flags);
	// Sends the data in ByteArray [bArray] to the address [Addr], if successful LastError.Length() == 0
	ISAACUTILS_API SizeL Socket_sendto(void *Sock, void *Addr, void *bArray, int Flags);
	// Receives [Num] Bytes from the peer of [Sock] and returns it as a ByteArray
	ISAACUTILS_API void *Socket_recv(void *Sock, SizeL Num, int Flags);
	// Receives [Num] Bytes sent to [Sock] from a single address that is placed in Addr
	ISAACUTILS_API void *Socket_recvfrom(void *Sock, void *Addr, SizeL Num, int Flags);
	// Retreives the address of [Sock]
	ISAACUTILS_API void *Socket_getsockname(void *Sock);
	// Retreives the address of the peer of [Sock]
	ISAACUTILS_API void *Socket_getpeername(void *Sock);
	// Sets the timeout [Time] in seconds for [Sock]
	ISAACUTILS_API bool Socket_settimeout(void *Sock, double Time);
	// Retreives the timeout in seconds for [Sock]
	ISAACUTILS_API double Socket_gettimeout(void *Sock);
	// Initializes the socket for managed mode
	ISAACUTILS_API bool Socket_InitMngd(void *Sock, unsigned long MidTmOut);
	// Constructs a new EncProt from the parameters, If [Key] == nullptr then you can initialize later
	ISAACUTILS_API void *EncProt_newS(void *Sock, void *Key, void *InitVec, unsigned long CipherNum, unsigned long ModeNum);
	// Initialize the Enc part of EncProt [Prot]
	ISAACUTILS_API bool EncProt_Init(void *Prot, void *Key, void *InitVec, unsigned long CipherNum, unsigned long ModeNum);
	// Sends Data using [Prot] with [NoEncDat] not encrypted and [EncDat] encrypted
	ISAACUTILS_API bool EncProt_Send(void *Prot, void *NoEncDat, void *EncDat);
	// Receives Data using [Prot] with [NoEncDat] to hold non-encrypted and [EncDat] to hold encrypted
	ISAACUTILS_API bool EncProt_Recv(void *Prot, void *NoEncDat, void *EncDat);
	// Constructs a new File given a filename [Str] and a mode [Mode] and a file system lock [FsLock]
	ISAACUTILS_API void *RfsFile_newA(void *Prot, char *Str, char *Mode, void *FsLock);
	// Constructs a new File that acts as a read buffer for [Fl], Initially not buffered
	ISAACUTILS_API void *RdBuffFile_newF(void *Fl, unsigned long Min, unsigned long Max, unsigned long BlkLen = 8192);
	// Sets the the buffering mode of [FlObj] to (Direct ? Non-Buffered : Buffered)
	ISAACUTILS_API bool RdBuffFile_SetMode(void *FlObj, bool Direct = false);
	// Constructs a host file system File object
	ISAACUTILS_API void *File_newA(char *fName, char *Mode);
	// Flushes the write buffers for the File [FlObj]
	ISAACUTILS_API bool File_flush(void *FlObj);
	// Constructs a host file system File object
	//ISAACUTILS_API void *File_newW(wchar_t *fName, wchar_t *Mode);//TODO add
	// Seeks a file [FlObj] to position [Pos] from [From]
	ISAACUTILS_API bool File_seek(void *FlObj, long long Pos, int From);
	// Tells the position the file [FlObj] is writing at or reading from
	ISAACUTILS_API long long File_tell(void *FlObj);
	// Reads a max of [Num] Bytes from File [FlObj] and returns it in a bytearray
	ISAACUTILS_API void *File_read(void *FlObj, unsigned long Num);
	// Writes the ByteArray [bArray] to File [FlObj] and returns the number of bytes written
	ISAACUTILS_API unsigned long File_write(void *FlObj, void *bArray);
	// Closes the File [FlObj]
	ISAACUTILS_API bool File_close(void *FlObj);
	// Constructs a new SingleMutex
	ISAACUTILS_API void *SingleMutex_new();
	// Constructs a new RWMutex
	ISAACUTILS_API void *RWMutex_new();
	// Trys to Acquire the Lock [Lk] without blocking and returns if the lock was acquired
	ISAACUTILS_API bool Lock_TryAcquire(void *Lk, bool Access = false);
	// Acquires the Lock [Lk] or blocks untils it is acquired
	ISAACUTILS_API bool Lock_acquire(void *Lk, bool Access = false);
	// Releases the Lock [Lk]
	ISAACUTILS_API bool Lock_release(void *Lk, bool Access = false);
	// Constructs a new empty Concurrent Queue
	//ISAACUTILS_API void *ConQueue_new();//NextTime
	// Constructs a new SockAddr from a human readable address [Str], a port [Port] and, for Ipv6 [FlowInf] and [ScopeId]
	ISAACUTILS_API void *SockAddr_newA(char *Str, unsigned short Port, unsigned long FlowInf, unsigned long ScopeId);
	// Register a prime modulus for a bit length of operation for MyHash_10 and MyHash_11
	ISAACUTILS_API void *RegMyHash_10(unsigned long BitLen);
	// MyHash_11 from Cryptic
	ISAACUTILS_API void *MyHash_11(void *StrBytes, unsigned long BitLen);
	//ISAACUTILS_API void ByteArray_del(void *ByteArray);
	//ISAACUTILS_API void wStr_del(void *Str);
	//ISAACUTILS_API void BigLong_del(void * Bl);
}
