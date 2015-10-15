#define DECLSPEC_IMPORT __declspec(dllimport)
#define WINADVAPI DECLSPEC_IMPORT
#define WINBASEAPI DECLSPEC_IMPORT
typedef int BOOL;
typedef unsigned long DWORD;
typedef unsigned char BYTE;
typedef long LONG;
typedef long long LONGLONG;
typedef wchar_t * LPWSTR;
typedef const wchar_t *LPCWSTR;

#if defined(_WIN64)
typedef unsigned long long HCRYPTPROV;
#else
typedef unsigned long HCRYPTPROV;
#endif
#define WINAPI __stdcall
#define PROV_RSA_FULL 1
#define CRYPT_VERIFYCONTEXT 0xF0000000
/*#if defined(MIDL_PASS)
typedef struct _LARGE_INTEGER {
#else // MIDL_PASS
typedef union _LARGE_INTEGER {
	struct {
		DWORD LowPart;
		LONG HighPart;
	} DUMMYSTRUCTNAME;
	struct {
		DWORD LowPart;
		LONG HighPart;
	} u;
#endif //MIDL_PASS
	LONGLONG QuadPart;
} LARGE_INTEGER;*/
#ifdef __cplusplus
extern "C" {
#endif
	WINBASEAPI BOOL WINAPI QueryPerformanceCounter(LARGE_INTEGER * lpPerformanceCount);
	WINBASEAPI BOOL WINAPI QueryPerformanceFrequency(LARGE_INTEGER * lpPerformanceCount);
	WINADVAPI BOOL WINAPI CryptReleaseContext(HCRYPTPROV  hProv, DWORD dwFlags);
	WINADVAPI BOOL WINAPI CryptGenRandom(HCRYPTPROV hProv, DWORD dwLen, BYTE *pbBuffer);
	WINADVAPI BOOL WINAPI CryptAcquireContextW(HCRYPTPROV *phProv, LPCWSTR szContainer, LPCWSTR szProvider, DWORD dwProvType, DWORD dwFlags);
#ifdef __cplusplus
}
#endif