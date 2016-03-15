//---------------------------------------------------------------------------
// PROJECT      : TDOG
// FILENAME     : run_helper.cpp
// COPYRIGHT    : Andy Thomas (c) 2016
// WEBSITE      : bigangrydog.com
// LICENSE      : Apache 2.0
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// INCLUDES
//---------------------------------------------------------------------------
#include "tdog/private/run_helper.hpp"
#include "util.hpp"

//---------------------------------------------------------------------------
// NON-CLASS
//---------------------------------------------------------------------------
using namespace tdog;

// Use with m_duration
const tdog::i64_t TDUR_NOT_RUN = -2;
const tdog::i64_t TDUR_RUNNING = -1;

//---------------------------------------------------------------------------
// CLASS run_helper : PRIVATE MEMBERS
//---------------------------------------------------------------------------
void run_helper::_raise_event(std::string s, event_type_t t, int lnum,
  const std::string& ename)
{
  // Underlying event, such as assert pass, fail or error.
  // If error, "ename" can supply additional error type string,
  // otherwise it is ignored. Line number of zero is taken to
  // mean unknown or N/A.
  s = trim_str(s);

  if (t == ET_ERROR)
  {
    m_status = TS_ERROR;
    m_error_name = ename;
    if (m_error_name.empty()) m_error_name = "unknown";
  }

  if (has_passed())
  {
    if (t == ET_FAIL)
    {
      m_status = TS_FAILED;
    }
    else
    if (t == ET_WARN)
    {
      m_status = TS_PASS_WARN;
    }
  }

  if (!s.empty() || (t != ET_INFO && t != ET_PASS))
  {
    // Output to event log
    m_event_log.push_back( event_item(s, t, lnum) );
  }

  check_time_limit(lnum);
}
//---------------------------------------------------------------------------
void run_helper::_printf_int(const std::string& s, tdog::i64_t val, int lnum)
{
  // Writes an integer integer value to the log.

  if (m_itag_vect.size() == 0)
  {
    // Mappings to sprintf()
    // Only have to do this once
    m_itag_vect.push_back(itag("%g", "%lld"));
    m_itag_vect.push_back(itag("%i", "%lld"));
    m_itag_vect.push_back(itag("%d", "%lld"));
    m_itag_vect.push_back(itag("%u", "%llu"));
    m_itag_vect.push_back(itag("%c", "%c"));
    m_itag_vect.push_back(itag("%o", "%llo"));
    m_itag_vect.push_back(itag("%x", "%llx"));
    m_itag_vect.push_back(itag("%02x", "%02llx"));
    m_itag_vect.push_back(itag("%04x", "%04llx"));
    m_itag_vect.push_back(itag("%08x", "%08llx"));
    m_itag_vect.push_back(itag("%16x", "%16llx"));
    m_itag_vect.push_back(itag("%X", "%llX"));
    m_itag_vect.push_back(itag("%02X", "%02llX"));
    m_itag_vect.push_back(itag("%04X", "%04llX"));
    m_itag_vect.push_back(itag("%08X", "%08llX"));
    m_itag_vect.push_back(itag("%16X", "%16llX"));
  }

  // Remove literal temporarily
  std::string fs = str_replace(s, "\\%", "$#&");

  // Enumerate through array
  for(std::size_t n = 0; n < m_itag_vect.size(); ++n)
  {
    // Translate if required
    fs = str_replace(fs, m_itag_vect[n].tag, int_to_str(val, m_itag_vect[n].spec));
  }

  // Special "boolean" tag
  if (val != 0) fs = str_replace(fs, "%b", "true");
  else fs = str_replace(fs, "%b", "false");

  // Put back literal
  fs = str_replace(fs, "$#&", "%");

  // Output to event log
  print(fs, lnum);
}
//---------------------------------------------------------------------------
void run_helper::_printf_double(const std::string& s, double val, int lnum)
{
  // Remove literal temporarily
  std::string fs = str_replace(s, "\\%", "$#&");

  // Replace general
  fs = str_replace(fs, "%g", "%f");

  // Replace
  fs = str_replace(fs, "%f", double_to_str(val));

  // Put back literal
  fs = str_replace(fs, "$#&", "%");

  // Output to event log
  print(fs, lnum);
}
//---------------------------------------------------------------------------
void run_helper::_printf_str(const std::string& s, const std::string& val, int lnum)
{
  // Push value string onto the log vector
  // Remove literal temporarily
  std::string fs = str_replace(s, "\\%", "$#&");

  // Replace general
  fs = str_replace(fs, "%g", "%s");

  // Replace strings
  fs = str_replace(fs, "%s", val);

  if (fs.find("%k") != std::string::npos) // < search in s, not fs
  {
    // Replace
    fs = str_replace(fs, "%k", encode_esc(val));
  }

  // Put back literal
  fs = str_replace(fs, "$#&", "%");

   // Output to event log
  print(fs, lnum);
}
//---------------------------------------------------------------------------
void run_helper::_printf_str(const std::string& s, const std::wstring& val, int lnum)
{
   // Output to event log
  _printf_str(s, wide_to_str(val), lnum);
}
//---------------------------------------------------------------------------
void run_helper::_printf_ptr(const std::string& s, const void* val, int lnum)
{
  // Print pointer value
  tdog::i64_t d = reinterpret_cast<tdog::i64_t>(val);

  if (s.find("%s") != s.npos || s.find("%g") != s.npos)
  {
    if (val == 0) _printf_str(s, "<nullptr>", lnum);
    else _printf_str(s, "addr:0x" + int_to_str(d, "%08x"), lnum);
  }
  else
  {
    _printf_int(s, d, lnum);
  }
}
//---------------------------------------------------------------------------
bool run_helper::_assert_impl(bool condition, bool eq, const std::string& s, int lnum)
{
  // Internal assert
  ++m_assert_cnt; // <- increment

  if (condition == eq)
  {
    _raise_event(s, ET_PASS, lnum);
  }
  else
  {
    ++m_assert_fails;
    _raise_event(s, ET_FAIL, lnum);
  }

  return (condition == eq);
}
//---------------------------------------------------------------------------
template <typename T>
bool run_helper::_ascii_compic(const T& s1, const T& s2) const
{
  // Insensitive case compare function (ASCII only)
  bool rslt = (s1 == s2);
  std::size_t sz = s2.size();

  if (!rslt && s1.size() == sz)
  {
    rslt = true;
    wchar_t c1, c2;
    for(std::size_t n = 0; n < sz; ++n)
      {
      c1 = s1[n]; c2 = s2[n];
      if (c1 >= 'A' && c1 <= 'Z') c1 += 0x20;
      if (c2 >= 'A' && c2 <= 'Z') c2 += 0x20;
      if (c1 != c2) return false;
    }
  }

  return rslt;
}
//---------------------------------------------------------------------------
// CLASS run_helper : PUBLIC MEMBERS
//---------------------------------------------------------------------------
run_helper::run_helper()
{
  m_status = TS_READY;
  clear();
}
//---------------------------------------------------------------------------
run_helper::~run_helper()
{
  // Does nothing at moment
}
//---------------------------------------------------------------------------
void run_helper::clear()
{
  // Clear results
  if (m_status != TS_DISABLED)
  {
    m_status = TS_READY;
    m_assert_cnt = 0;
    m_assert_fails = 0;
    m_start_time = 0;
    m_duration = TDUR_NOT_RUN;
    m_time_exceeded = false;
    m_time_warn_flag = false;
    m_time_limit = 0;
    m_global_limit = 0;
    m_warning_limit = 0;
    m_author.clear();
    m_error_name.clear();
    m_event_log.clear();
  }
}
//---------------------------------------------------------------------------
bool run_helper::start_test(tdog::i64_t glimit, tdog::i64_t gwarn)
{
  // Start timer. True if not disabled.
  if (m_status != TS_DISABLED && m_status != TS_SKIPPED)
  {
    clear();

    m_status = TS_PASS_OK; //<- default
    m_global_limit = glimit;
    m_warning_limit = gwarn;
    m_duration = TDUR_RUNNING;
    m_start_time = msec_time();

    return true;
  }

  return false;
}
//---------------------------------------------------------------------------
void run_helper::stop_test()
{
  if (m_duration == TDUR_RUNNING)
  {
    m_duration = msec_time() - m_start_time;
    check_time_limit(0);
  }
}
//---------------------------------------------------------------------------
status_t run_helper::status() const
{
  // Test state
  return m_status;
}
//---------------------------------------------------------------------------
std::string run_helper::error_name() const
{
  // Error (exception) name
  return m_error_name;
}
//---------------------------------------------------------------------------
int run_helper::assert_cnt() const
{
  // Number of asserts called
  return m_assert_cnt;
}
//---------------------------------------------------------------------------
int run_helper::fail_cnt() const
{
  // The number of failed assertions
  return m_assert_fails;
}
//---------------------------------------------------------------------------
tdog::i64_t run_helper::start_time() const
{
  // Test start time
  return m_start_time;
}
//---------------------------------------------------------------------------
tdog::i64_t run_helper::duration() const
{
  // Test duration in ms
  if (m_duration == TDUR_NOT_RUN) return 0;
  if (m_duration == TDUR_RUNNING) return msec_time() - m_start_time;

  return m_duration;
}
//---------------------------------------------------------------------------
bool run_helper::has_ran() const
{
  // True if test ran, excluding skipped.
  bool end_state = (m_status == TS_PASS_OK ||
    m_status == TS_PASS_WARN ||
    m_status == TS_FAILED ||
    m_status == TS_ERROR);

  return (m_duration >= 0 && end_state);
}
//---------------------------------------------------------------------------
bool run_helper::has_failed() const
{
  // Failed or error
  return (m_status == TS_FAILED || m_status == TS_ERROR);
}
//---------------------------------------------------------------------------
bool run_helper::has_passed() const
{
  // Have we passed?
  return (m_status == TS_PASS_OK || m_status == TS_PASS_WARN);
}
//---------------------------------------------------------------------------
std::string run_helper::author() const
{
  return m_author;
}
//---------------------------------------------------------------------------
const event_vector& run_helper::event_log() const
{
  // Access to test log
  return m_event_log;
}
//---------------------------------------------------------------------------
void run_helper::set_author(const std::string& a)
{
  // Set by test case
  m_author = a;
}
//---------------------------------------------------------------------------
void run_helper::set_skipped()
{
  if (m_status != TS_DISABLED)
  {
    m_status = TS_SKIPPED;

    // Clear relevant data, but
    // not author and duration.
    m_assert_cnt = 0;
    m_assert_fails = 0;
    m_error_name.clear();
    m_event_log.clear();
  }
}
//---------------------------------------------------------------------------
bool run_helper::set_enabled(bool flag)
{
  // Set disabled status (or reset)
  if (!flag && m_status != TS_DISABLED)
  {
    clear();
    m_status = TS_DISABLED;
    return true;
  }
  else
  if (flag && m_status == TS_DISABLED)
  {
    m_status = TS_READY;
    return true;
  }

  return false;
}
//---------------------------------------------------------------------------
void run_helper::set_time_limit(tdog::i64_t ms)
{
  m_time_limit = ms;
}
//---------------------------------------------------------------------------
void run_helper::set_global_limit_exempt()
{
  m_global_limit = 0;
}
//---------------------------------------------------------------------------
void run_helper::sleep(int ms)
{
  // Sleep for mininum number of milliseconds.
  // We have a multi-platform sleep call.
  msleep(ms);
}
//---------------------------------------------------------------------------
bool run_helper::check_time_limit(int lnum)
{
  // Check time not exceeded. Return true
  // or throw tdog::test_time error.
  bool check_this = (m_time_limit > 0 || m_global_limit > 0 || m_warning_limit > 0);

  if (check_this && !m_time_exceeded &&
    m_duration != TDUR_NOT_RUN &&
    m_status != TS_DISABLED)
  {
    tdog::i64_t dur = duration();

    if (m_time_limit > 0 && dur > m_time_limit)
    {
      // Local time constraint failure
      m_time_exceeded = true;
      _raise_event("local time constraint of " + int_to_str(m_time_limit) + " ms exceeded", ET_FAIL);
    }
    else
    if (m_global_limit > 0 && dur > m_global_limit)
    {
      // Global time constraint failure
      m_time_exceeded = true;
      _raise_event("global time constraint of " + int_to_str(m_global_limit) + " ms exceeded", ET_FAIL);
    }
    else
    if (!m_time_warn_flag && m_warning_limit > 0 && dur > m_warning_limit)
    {
      // Global time warning
      m_time_warn_flag = true;
      _raise_event("time warning threshold of " + int_to_str(m_warning_limit) + " ms exceeded", ET_WARN);
    }
  }

  return m_time_exceeded;
}
//---------------------------------------------------------------------------
void run_helper::raise_failure(const std::string& s, int lnum)
{
  _raise_event(s, ET_FAIL, lnum);
}
//---------------------------------------------------------------------------
void run_helper::raise_warning(const std::string& s, int lnum)
{
  _raise_event(s, ET_WARN, lnum);
}
//---------------------------------------------------------------------------
void run_helper::raise_error(const std::string& s, int lnum, const std::string& ename)
{
  _raise_event(s, ET_ERROR, lnum, ename);
}
//---------------------------------------------------------------------------
void run_helper::print(const std::string& s, int lnum)
{
  // Underlying print method
  // This is pushed onto an event log storage container.
  // Calls raise event, which will also check the time.
  _raise_event(s, ET_INFO, lnum);
}
//---------------------------------------------------------------------------
void run_helper::printf(const std::string& s, bool val, int lnum)
{
  _printf_int(s, val, lnum);
}
//---------------------------------------------------------------------------
void run_helper::printf(const std::string& s, tdog::i8_t val, int lnum)
{
  _printf_int(s, val, lnum);
}
//---------------------------------------------------------------------------
void run_helper::printf(const std::string& s, tdog::u8_t val, int lnum)
{
  _printf_int(s, val, lnum);
}
//---------------------------------------------------------------------------
void run_helper::printf(const std::string& s, tdog::i16_t val, int lnum)
{
  _printf_int(s, val, lnum);
}
//---------------------------------------------------------------------------
void run_helper::printf(const std::string& s, tdog::u16_t val, int lnum)
{
  _printf_int(s, val, lnum);
}
//---------------------------------------------------------------------------
void run_helper::printf(const std::string& s, tdog::i32_t val, int lnum)
{
  _printf_int(s, val, lnum);
}
//---------------------------------------------------------------------------
void run_helper::printf(const std::string& s, tdog::u32_t val, int lnum)
{
  _printf_int(s, val, lnum);
}
//---------------------------------------------------------------------------
void run_helper::printf(const std::string& s, tdog::i64_t val, int lnum)
{
  _printf_int(s, val, lnum);
}
//---------------------------------------------------------------------------
void run_helper::printf(const std::string& s, tdog::u64_t val, int lnum)
{
  _printf_int(s, val, lnum);
}
//---------------------------------------------------------------------------
void run_helper::printf(const std::string& s, double val, int lnum)
{
  _printf_double(s, val, lnum);
}
//---------------------------------------------------------------------------
void run_helper::printf(const std::string& s, const char* const val, int lnum)
{
  if (val == 0) _printf_str(s, "<nullptr>", lnum);
  else  _printf_str(s, static_cast<std::string>(val), lnum);
}
//---------------------------------------------------------------------------
void run_helper::printf(const std::string& s, const wchar_t* val, int lnum)
{
  if (val == 0) _printf_str(s, L"<nullptr>", lnum);
  else _printf_str(s, static_cast<std::wstring>(val), lnum);
}
//---------------------------------------------------------------------------
void run_helper::printf(const std::string& s, char* val, int lnum)
{
  this->printf(s, const_cast<const char*>(val), lnum);
}
//---------------------------------------------------------------------------
void run_helper::printf(const std::string& s, wchar_t* val, int lnum)
{
  this->printf(s, const_cast<const wchar_t*>(val), lnum);
}
//---------------------------------------------------------------------------
void run_helper::printf(const std::string& s, std::string val, int lnum)
{
  _printf_str(s, val, lnum);
}
//---------------------------------------------------------------------------
void run_helper::printf(const std::string& s, std::wstring val, int lnum)
{
  _printf_str(s, val, lnum);
}
//---------------------------------------------------------------------------
void run_helper::assert(bool condition, bool eq, const std::string& s, int lnum)
{
  if ( !_assert_impl(condition, eq, s, lnum) )
  {
    this->printf("-cond = %b", condition, 0);
  }
}
//---------------------------------------------------------------------------
void run_helper::assert_double_equal(const double& exp, const double& act,
  bool eq, const double& delta, const std::string& s, int lnum)
{
  if ( !_assert_impl( ((act >= (exp - delta)) && (act <= (exp + delta))), eq, s, lnum ) )
  {
    if (eq) this->printf("-exp = %f", exp, 0);
    else this->printf("-nxp = %f", exp, 0);

    this->printf("-act = %f", act, 0);
  }
}
//---------------------------------------------------------------------------
void run_helper::assert_stric_equal(const std::string& exp, const std::string& act,
  bool eq, const std::string& s, int lnum)
{
  if ( !_assert_impl( _ascii_compic(exp, act), eq, s, lnum ) )
  {
    if (eq) this->printf("-exp = %s", exp, 0);
    else this->printf("-nxp = %s", exp, 0);
    this->printf("-act = %s", act, 0);
  }
}
//---------------------------------------------------------------------------
void run_helper::assert_stric_equal(const std::wstring& exp, const std::wstring& act,
  bool eq, const std::string& s, int lnum)
{
  if ( !_assert_impl( _ascii_compic(exp, act), eq, s, lnum ) )
  {
    if (eq) this->printf("-exp = %s", exp, 0);
    else this->printf("-nxp = %s", exp, 0);

    this->printf("-act = %s", act, 0);
  }
}
//---------------------------------------------------------------------------
