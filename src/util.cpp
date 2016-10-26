//---------------------------------------------------------------------------
// PROJECT      : TDOG
// FILENAME     : util.cpp
// COPYRIGHT    : Kuiper (c) 2016
// WEBSITE      : kuiper.zone
// LICENSE      : Apache 2.0
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// INCLUDES
//---------------------------------------------------------------------------
#include "util.hpp"

#include <cstdlib>
#include <cstdio>
#include <ctime>

#if defined(_MSC_VER) || defined(WINDOWS) || defined(_WINDOWS) || \
  defined(WIN32) || defined(WIN64) || defined(_WIN32) || defined(_WIN64)
  #define TDOG_WINDOWS
  #include <windows.h>
#else
  #include <errno.h>
  #include <unistd.h>
  #include <sys/time.h>
  #include <signal.h>
  #ifdef _POSIX_PRIORITY_SCHEDULING
    // sched_yield()
    #include <sched.h>
  #endif // _POSIX_PRIORITY_SCHEDULING

#endif // defined(_MSC_VER) ...

//---------------------------------------------------------------------------
// NON-CLASS
//---------------------------------------------------------------------------
using namespace tdog;

//---------------------------------------------------------------------------
// INTERNAL ROUTINES
//---------------------------------------------------------------------------
template <class T>
static std::string _sprintf(const char* format, T v)
{
  // INTERNAL STATIC ROUTINE
  // Wrapper for sprintf
  // Convert T to string, according to format_str,
  // where format_str has default "%d". Uses sprintf for
  // the conversion. For example, _sprintf("0x%08X", 5)
  // will convert to 8 character hex string.

  // Format should be
  // %[flags][width][.precision][length]specifier
  // Where specifier is the most significant one and defines the type and
  // the interpretation of the value of the coresponding argument:

  // Specifier Output {Example}
  // d or i    Signed decimal integer {392}
  // e         Scientific notation (mantise/exponent) using e character {3.9265e+2}
  // E         Scientific notation (mantise/exponent) using E character {3.9265E+2}
  // f         Decimal floating point {392.65}
  // g         Use the shorter of %e or %f {392.65}
  // G         Use the shorter of %E or %f {392.65}
  // o         Signed octal {610}
  // u         Unsigned decimal integer {7235}
  // x         Unsigned hexadecimal integer {7fa}
  // X         Unsigned hexadecimal integer (capital letters) {7FA}
  // %         A % followed by another % character will write % to the string.
  static const std::size_t SPRINTF_BUF_SZ = 100;

  int sz;
  char buf[SPRINTF_BUF_SZ];

#if defined(_MSC_VER)
  // MS version
  sz = sprintf_s(buf, SPRINTF_BUF_SZ, format, v);
#elif (__STDC_VERSION__ >= 199901L || __GNUC__ >= 3)
  // Safe version if we have it
  sz = std::snprintf(buf, SPRINTF_BUF_SZ, format, v);
#else
  sz = std::sprintf(buf, format, v);
#endif

  if (sz > 0 && sz < static_cast<int>(SPRINTF_BUF_SZ))
  {
    return std::string(buf, sz);
  }

  return std::string();
}
//---------------------------------------------------------------------------
// ROUTINES
//---------------------------------------------------------------------------
std::string tdog::ascii_lower(std::string s)
{
  // ASCII Lowercase
  for(std::size_t n = 0; n < s.size(); ++n) {
    if (s[n] >= 'A' && s[n] <= 'Z') s[n] += 0x20;
  }
  return s;
}
//---------------------------------------------------------------------------
std::string tdog::trim_str(std::string s)
{
  // Trim left and right of spaces/control characters.
  std::size_t len = s.size();

  if (len > 0)
  {
    // Scan left
    for(std::size_t n = 0; ; ++n)
    {
      if (static_cast<unsigned char>(s[n]) > 0x20)
      {
        s.erase(0, n);
        break;
      }
      else
      if (n == len - 1)
      {
        s.clear();
        break;
      }
    }

    len = s.size();

    if (len > 0)
    {
      // Scan right
      for(std::size_t n = len - 1; ; --n)
      {
        if (static_cast<unsigned char>(s[n]) > 0x20)
        {
          if (n < len - 1)
          {
            s.erase(n + 1);
          }

          break;
        }
        else
        if (n == 0)
        {
          s.clear();
          break;
        }
      }
    }
  }

  return s;
}
//---------------------------------------------------------------------------
std::string tdog::int_to_str(tdog::xint_t i, const std::string& format_str)
{
  // Convert integer to string, according to format_str.
  return _sprintf<tdog::xint_t>(format_str.c_str(), i);
}
//---------------------------------------------------------------------------
std::string tdog::double_to_str(double f, const std::string& format_str)
{
  // Convert integer to string
  return _sprintf<double>(format_str.c_str(), f);
}
//---------------------------------------------------------------------------
std::string tdog::str_replace(std::string src, const std::string& find,
  const std::string& rep)
{
  // Search & replace on src string
  if (!find.empty() && find.length() <= src.length() && find != rep)
  {
    std::size_t p = 0;
    std::size_t rl = rep.length();
    std::size_t fl = find.length();

    // True if replace contains find
    bool flag = true;
    bool changed = false;
    bool no_recurse = (rep.find(find) != std::string::npos);

    do
    {
      do
      {
        if (flag) p = src.find(find, p);

        if (p != std::string::npos)
        {
          src.replace(p, fl, rep);
          p += rl;
          flag = true;
          changed = true;
        }

      } while(p != std::string::npos);

      // Must search again, as overlapping find-replace
      // can cause search string to remain, i.e. replacing
      // ",," with ",". We don't do this if replace string
      // contains find, otherwise would loop forever.
      if (changed) p = src.find(find);
      flag = false; // <- save repeating find

    } while(!no_recurse && p != std::string::npos);
  }

  return src;
}
//---------------------------------------------------------------------------
std::string tdog::wide_to_str(const std::wstring& s)
{
  // Wide to narrow
  std::size_t sz = s.size();

  std::string rslt;
  rslt.reserve(sz);

  for(std::size_t n = 0; n < sz; ++n)
  {
    unsigned int c = static_cast<unsigned int>(s[n]);

    if (c < 0x80)
    {
      rslt += static_cast<char>(c);
    }
    else
    if (c <= 0xFFFF)
    {
      rslt += "\\u" + int_to_str(c, "%04X");
    }
    else
    {
      rslt += "\\U" + int_to_str(c, "%08X");
    }

  }

  return rslt;
}
//---------------------------------------------------------------------------
std::string tdog::wide_to_str(const std::string& s)
{
  // Provides compatibility with template routines
  return s;
}
//---------------------------------------------------------------------------
std::string tdog::encode_esc(const std::string& s)
{
  // Replaces common character values with escape
  // sequences and returns the result.
  std::size_t sz = s.size();

  std::string rslt;
  rslt.reserve(sz);

  for(std::size_t n = 0; n < sz; ++n)
  {
    unsigned char c = static_cast<unsigned char>(s[n]);

    if (c > 0x1F)
    {
      rslt += static_cast<char>(c);
    }
    else
    {
      switch(c)
      {
        case 0x09: rslt += "\\t"; break;
        case 0x0A: rslt += "\\n"; break;
        case 0x0B: rslt += "\\v"; break;
        case 0x0C: rslt += "\\f"; break;
        case 0x0D: rslt += "\\r"; break;
        default: rslt += "\\x" + int_to_str(c, "%02X"); break;
      }
    }
  }

  return rslt;
}
//---------------------------------------------------------------------------
std::string tdog::xml_esc(std::string s)
{
  // Escapse XML characters
  s = str_replace(s, "&", "&amp;");
  s = str_replace(s, "<", "&lt;");
  s = str_replace(s, ">", "&gt;");
  s = str_replace(s, "'", "&apos;");
  s = str_replace(s, "\"", "&quot;");
  return s;
}
//---------------------------------------------------------------------------
bool tdog::is_suffix(const std::string& s, const std::string& sfx)
{
  // True if sfx is a suffix of s.
  std::size_t s_sz = s.size();
  std::size_t x_sz = sfx.size();

  if (s_sz >= x_sz && x_sz != 0)
  {
    std::size_t x = 0;
    for(std::size_t n = s_sz - x_sz; n < s_sz; ++n)
    {
      if (s[n] != sfx[x++]) return false;
    }

    return true;
  }

  return false;
}
//---------------------------------------------------------------------------
std::string tdog::hostname()
{
  // Host name
  char* rslt = std::getenv("TDOG_HOSTNAME");
  if (rslt == 0) rslt = std::getenv("HOSTNAME");

  if (rslt != 0) return rslt;
  else return "localhost";
}
//---------------------------------------------------------------------------
tdog::i64_t tdog::msec_time()
{
  // Returns the time in milliseconds from 1970.
  tdog::i64_t rslt = static_cast<tdog::i64_t>(std::time(0));
  rslt *= 1000;

  // Get the millisecond component
#if defined(TDOG_WINDOWS)
  SYSTEMTIME st;
  st.wYear = 0;
  st.wMilliseconds = 0;
  GetSystemTime(&st);

  if (st.wYear != 0)
  {
    rslt += st.wMilliseconds;
  }
#elif _POSIX_VERSION >= 200112L
  timeval tv;
  tv.tv_usec = 0;
  if (gettimeofday(&tv, 0) == 0)
  {
    rslt += tv.tv_usec / 1000;
  }
#endif

  return rslt;
}
//---------------------------------------------------------------------------
void tdog::msleep(int m)
{
  // Sleep for number of milli seconds.
  if (m >= 0)
  {
#if defined(TDOG_WINDOWS)

    // Windows
    Sleep(m);

#else
    if (m == 0)
    {
#if defined(_POSIX_PRIORITY_SCHEDULING)
      sched_yield();
#endif // _POSIX_PRIORITY_SCHEDULING
    }
    else
    {
#if _POSIX_C_SOURCE >= 199309L
      // Nanosleep
      timespec req, rem;
      req.tv_sec = m / 1000;
      req.tv_nsec = 1000000 * (m % 1000);
      bool sleep_again = false;

      do
      {
        errno = 0;
        if (nanosleep(&req, &rem) < 0)
        {
          sleep_again = (errno == EINTR);
        }
      } while(sleep_again);
#else
      // Nanosleep not supported
      sleep(m / 1000);
      int us = 1000 * (m % 1000);
      if (us != 0) usleep(us);
#endif // _POSIX_C_SOURCE >= 199309L
    }
#endif // defined(TDOG_WINDOWS)
  }
}
//---------------------------------------------------------------------------
std::string tdog::iso_time(tdog::i64_t t, bool utc)
{
  // Returns ISO8601 local date/time as string of form: YYYY-MM-DD HH:MM:SS
  // Output is UTC if utc is true, or local otherwise. Input is ms from 1970.
  bool tm_ok;
  std::tm tm_rslt;

  tm_rslt.tm_year = 0;
  tm_rslt.tm_mon = 0;
  tm_rslt.tm_mday = 0;
  tm_rslt.tm_hour = 0;
  tm_rslt.tm_min = 0;
  tm_rslt.tm_sec = 0;

  // Make seconds
  std::time_t ct = t / 1000;

#if defined(_MSC_VER)

  // MSVC
  if (utc) tm_ok = (gmtime_s(&tm_rslt, &ct) == 0);
  else tm_ok = (localtime_s(&tm_rslt, &ct) == 0);

#elif _POSIX_C_SOURCE >= 1 || _XOPEN_SOURCE || _BSD_SOURCE || _SVID_SOURCE || _POSIX_SOURCE

  if (utc) tm_ok = (gmtime_r(&ct, &tm_rslt) != 0);
  else tm_ok = (localtime_r(&ct, &tm_rslt) != 0);

#else

  // Default (Used on MinGW)
  // NB. Generally, localtime()/gmtime() are not thread-safe.
  // Their results are a pointer to a static structure,
  // which means there won't be memory allocation
  // issues, but data could be overwritten at the same time
  // by concurrent threads. However, MinGW calls MSVC runtime,
  // and it appears this is threadsafe because the results point
  // to thread specific local buffers.
  // http://sources.redhat.com/ml/pthreads-win32/2005/msg00011.html
  std::tm* t_ptr;

  if (utc) t_ptr = std::gmtime(&ct);
  else t_ptr = std::localtime(&ct);

  tm_ok = (t_ptr != 0);
  if (tm_ok) tm_rslt = *t_ptr;

#endif

  // Build result (or error)
  if (tm_ok)
  {
    return int_to_str(tm_rslt.tm_year + 1900, "%04d-") +
    int_to_str(tm_rslt.tm_mon + 1, "%02d-") +
    int_to_str(tm_rslt.tm_mday, "%02d ") +
    int_to_str(tm_rslt.tm_hour, "%02d:") +
    int_to_str(tm_rslt.tm_min, "%02d:") +
    int_to_str(tm_rslt.tm_sec, "%02d");
  }
  else
  {
    return std::string();
  }
}
//---------------------------------------------------------------------------
std::string tdog::duration_str(i64_t t)
{
  // Duration string, i.e. "5 min, 37 sec". Input in ms since 1970.
  std::string rslt;

  if (t >= 0)
  {
    int ms = t % 1000; t /= 1000;
    int sc = t % 60; t /= 60;
    int mn = t % 60; t /= 60;
    int hr = t % 24; t /= 24;
    int dy = static_cast<int>(t);

    if (dy > 0) rslt += int_to_str(dy) + "day(s), ";
    if (dy > 0 || hr > 0) rslt += int_to_str(hr) + " hrs, ";
    if (dy > 0 || hr > 0 || mn > 0) rslt += int_to_str(mn) + " min, ";

    rslt += int_to_str(sc) + " sec";
    if (sc < 60) rslt += ", " + int_to_str(ms) + " ms";
  }

  return rslt;
}
//---------------------------------------------------------------------------
