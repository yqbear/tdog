//---------------------------------------------------------------------------
// PROJECT      : TDOG
// FILENAME     : compat.hpp
// COPYRIGHT    : Andy Thomas (c) 2016
// WEBSITE      : bigangrydog.com
// LICENSE      : Apache 2.0
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// HEADER GUARD
//---------------------------------------------------------------------------
#ifndef TDOG_COMPAT_H
#define TDOG_COMPAT_H

//---------------------------------------------------------------------------
// INCLUDES
//---------------------------------------------------------------------------
#if __cplusplus >= 201103L
#define TDOG_CSTDINT
#include <cstdint>
#endif

//---------------------------------------------------------------------------
// DOCUMENTATION
//---------------------------------------------------------------------------

/**
* \brief Defines basic sized integer types. On C++11, we use "stdints". On other
* platforms, we define our own.
*/

//---------------------------------------------------------------------------
// DECLARATIONS
//---------------------------------------------------------------------------

namespace tdog {

#if defined(TDOG_CSTDINT) || defined(DOXYGEN)

  // Modern
  typedef std::int8_t i8_t;     //!< Signed 8-bit integer.
  typedef std::int16_t i16_t;   //!< Signed 16-bit integer.
  typedef std::int32_t i32_t;   //!< Signed 32-bit integer.
  typedef std::int64_t i64_t;   //!< Signed 64-bit integer.

  typedef std::uint8_t u8_t;    //!< Unsigned 8-bit integer.
  typedef std::uint16_t u16_t;  //!< Unsigned 16-bit integer.
  typedef std::uint32_t u32_t;  //!< Unsigned 32-bit integer.
  typedef std::uint64_t u64_t;  //!< Unsigned 64-bit integer.

  typedef std::int64_t xint_t;  //!< Maximum integer size supported by TDOG.

#elif defined(_MSC_VER) || defined(__BORLANDC__)

  // Use MS built-in types
  typedef __int8 i8_t;
  typedef __int16 i16_t;
  typedef __int32 i32_t;
  typedef __int64 i64_t;

  typedef unsigned __int8 u8_t;
  typedef unsigned __int16 u16_t;
  typedef unsigned __int32 u32_t;
  typedef unsigned __int64 u64_t;

  typedef __int64 xint_t;

#else

  // Legacy
  // Datatype   LP64  ILP64 LLP64 ILP32 LP32
  // char       8     8     8     8     8
  // short      16    16    16    16    16
  // _int32           32
  // int        32    64    32    32    16
  // long       64    64    32    32    32
  // long long              64
  typedef signed char i8_t;
  typedef unsigned char u8_t;
  typedef short i16_t;
  typedef unsigned short u16_t;

  #if defined(_ILP64) || defined (__ILP64) || defined(__ILP64__)
    typedef _int32 i32_t;
    typedef unsigned _int32 u32_t;
    typedef int i64_t;
    typedef unsigned int u64_t;
    typedef int xint_t;
  #else
    typedef int i32_t;
    typedef unsigned int u32_t;
    typedef long long i64_t;
    typedef unsigned long long u64_t;
    typedef long long xint_t;
  #endif // defined(_ILP64) ...

#endif

} // namespace

//---------------------------------------------------------------------------
#endif // HEADER GUARD
//---------------------------------------------------------------------------
