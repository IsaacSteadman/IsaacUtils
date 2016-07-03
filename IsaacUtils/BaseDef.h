/* Defines common identifiers based on information gathered about
 *   the compiler (and its host platform and architecture) that is building
 *   this library.
 * Acronyms:
 *   TRA - To Represent Host Architecture
 *     used to convey that the size of the value/type is defined as
 *     (32bit value)/(64bit value) -> UInt32/UInt64 TRHA
 * Guaranteed defines/typedefs (for platforms this library is supported on)
 *   typedef ... UInt8; // unsigned 8 bit integer
 *   typedef ... SInt8; // signed 8 bit integer
 *   typedef ... UInt16; // unsigned 16 bit integer
 *   typedef ... SInt16; // signed 16 bit integer
 *   typedef ... UInt32; // unsigned 32 bit integer
 *   typedef ... SInt32; // signed 32 bit integer
 *   typedef ... UInt64; // unsigned 64 bit integer
 *   typedef ... SInt64; // signed 64 bit integer
 *   typedef ... SizeL; // defined as UInt32/UInt64 TRHA
 *   typedef ... SnzL; // signed version of SizeL. defined as SInt32/SInt64 TRHA
 *   #define MAX_INT32 0xFFFFFFFF
 *   #define MAX_INT64 0xFFFFFFFFFFFFFFFF
 *   #define MAX_INT ... // defined as MAX_INT32/MAX_INT64 TRHA
 *   #define MAX_HEX_DIGIT // defined as 8/16 TRA
 *   #define ISAACUTILS_API ... // used to import/export on windows but on linux is blank
 *   #define OS_CALL ... // used to refer to the specific calling convention of the OS (__stdcall on windows)
*/
#if !defined(INC_KYDEF_H)
#define INC_KYDEF_H

#define MAX_INT64 0xFFFFFFFFFFFFFFFF
#define MAX_INT32 0xFFFFFFFF

typedef unsigned char UInt8;
typedef signed char SInt8;
typedef unsigned short UInt16;
typedef signed short SInt16;
typedef unsigned int UInt32;
typedef signed int SInt32;
typedef unsigned long long UInt64;
typedef signed long long SInt64;

#if defined(_M_X64) || defined(_M_AMD64) || defined(__amd64__) || defined(__amd64)\
	|| defined(__x86_64__) || defined(__x86_64) || defined(__aarch64__)
	typedef UInt64 SizeL;
	typedef SInt64 SnzL;
	#define MAX_INT 0xFFFFFFFFFFFFFFFF
	#define MAX_HEX_DIGIT 16
#else
	typedef UInt32 SizeL;
	typedef SInt32 SnzL;
	#define MAX_INT 0xFFFFFFFF
	#define MAX_HEX_DIGIT 8
#endif
#if defined(__linux__)
	#define OS_CALL
	#define ISAACUTILS_API
#elif defined(_WIN32) || defined(_WIN64) || defined(__WIN32__) ||\
	defined(__TOS_WIN__) || defined(__WINDOWS__)
	#define OS_CALL __stdcall
	#if defined(ISAACUTILS_EXPORTS)
	#define ISAACUTILS_API __declspec(dllexport)
	#else
	#define ISAACUTILS_API __declspec(dllimport)
	#endif
#endif
#if defined(__GNUC__) // GNU Compiler Collection (GCC)
#elif defined(__clang__) // Clang compiler
#elif defined(_MSC_BUILD) || defined(_MSC_FULL_VER) || defined(_MSC_VER) // MSVC
#elif defined(__DMC__) // Digital Mars Compiler
#endif

#endif
