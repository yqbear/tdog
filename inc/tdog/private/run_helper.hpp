//---------------------------------------------------------------------------
// PROJECT      : TDOG
// FILENAME     : run_helper.hpp
// COPYRIGHT    : Andy Thomas (c) 2016
// WEBSITE      : bigangrydog.com
// LICENSE      : Apache 2.0
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// HEADER GUARD
//---------------------------------------------------------------------------
#ifndef TDOG_RUN_HELPER_H
#define TDOG_RUN_HELPER_H

//---------------------------------------------------------------------------
// INCLUDES
//---------------------------------------------------------------------------
#include "tdog/compat.hpp"
#include "tdog/types.hpp"

#include <string>
#include <vector>

//---------------------------------------------------------------------------
// DOCUMENTATION
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// DECLARATIONS
//---------------------------------------------------------------------------
namespace tdog {

enum event_type_t
{
  ET_INFO,      // <- print statement
  ET_PASS,      // <- pass
  ET_FAIL,      // <- assert fail
  ET_ERROR,     // <- exception
  ET_WARN       // <- user warning
};

struct event_item
{
  std::string msg_str;    // <- message string
  event_type_t item_type; // <- type of item
  int line_num;           // <- line no. where msg was generated

  // Constructor
  event_item(const std::string& mstr = std::string(), event_type_t t = ET_PASS,
    int lnum = 0) : msg_str(mstr), item_type(t), line_num(lnum) {}
};

typedef std::vector<event_item> event_vector;

//---------------------------------------------------------------------------
// CLASS run_helper
//---------------------------------------------------------------------------
class run_helper
{
  private:

  status_t m_status;
  std::string m_error_name;
  int m_assert_cnt;
  int m_assert_fails;
  tdog::i64_t m_start_time;
  tdog::i64_t m_duration;
  tdog::i64_t m_time_limit;
  tdog::i64_t m_global_limit;
  tdog::i64_t m_warning_limit;
  bool m_time_exceeded;
  bool m_time_warn_flag;
  std::string m_author;

  // Hold list of events
  event_vector m_event_log;

  // We use this to hold a list
  // mapping our own integer tag
  // specifiers to standard printf ones
  struct itag
  {
    std::string tag;
    std::string spec;
    itag(const std::string& t, const std::string& s)
      : tag(t), spec(s) {}
  };

  std::vector<itag> m_itag_vect;

  // Event state with message
  void _raise_event(std::string s, event_type_t t, int lnum = 0,
    const std::string& ename = std::string());

  // Underlying common print statement implementations.
  void _printf_int(const std::string& s, tdog::i64_t val, int lnum = 0);
  void _printf_double(const std::string& s, double val, int lnum = 0);
  void _printf_str(const std::string& s, const std::string& val, int lnum = 0);
  void _printf_str(const std::string& s, const std::wstring& val, int lnum = 0);
  void _printf_ptr(const std::string& s, const void* val, int lnum = 0);

  // Internal assert - has return value (derivatives
  // do not). Derived asserts should call this.
  bool _assert_impl(bool condition, bool eq, const std::string& s, int lnum);

  // Compare used by assert_stric
  template <typename T>
  bool _ascii_compic(const T& s1, const T& s2) const;

  public:

  run_helper();
  virtual ~run_helper();

  // Test run properties
  void clear();
  bool start_test(tdog::i64_t glimit = 0, tdog::i64_t gwarn = 0);
  void stop_test();
  status_t status() const;
  std::string error_name() const;
  int assert_cnt() const;
  int fail_cnt() const;
  tdog::i64_t start_time() const;
  tdog::i64_t duration() const;
  bool has_ran() const;
  bool has_failed() const;
  bool has_passed() const;
  std::string author() const;

  // Access to test log
  const event_vector& event_log() const;

  // ---------------------------
  // TEST CASE CALLS
  // ---------------------------
  void set_author(const std::string& a);
  void set_skipped();
  bool set_enabled(bool flag);
  void set_time_limit(tdog::i64_t ms);
  void set_global_limit_exempt();
  void sleep(int ms);

  // Check time not execeed.
  // Will log error if true.
  bool check_time_limit(int lnum = 0);

  // Test fail calls
  void raise_failure(const std::string& s, int lnum);
  void raise_warning(const std::string& s, int lnum);
  void raise_error(const std::string& s, int lnum,
    const std::string& ename = std::string());

  // ---------------------------
  // TEST PRINT STATEMENTS
  // ---------------------------

  // NB. We need catch all overloaded integer types explicitly
  // to avoid conflict with double and template fallback.
  // We also catch and check for null pointer types.
  void print(const std::string& s, int lnum);
  void printf(const std::string& s, bool val, int lnum);
  void printf(const std::string& s, tdog::i8_t val, int lnum);
  void printf(const std::string& s, tdog::u8_t val, int lnum);
  void printf(const std::string& s, tdog::i16_t val, int lnum);
  void printf(const std::string& s, tdog::u16_t val, int lnum);
  void printf(const std::string& s, tdog::i32_t val, int lnum);
  void printf(const std::string& s, tdog::u32_t val, int lnum);
  void printf(const std::string& s, tdog::i64_t val, int lnum);
  void printf(const std::string& s, tdog::u64_t val, int lnum);
  void printf(const std::string& s, double val, int lnum);
  void printf(const std::string& s, const char* val, int lnum);
  void printf(const std::string& s, const wchar_t* val, int lnum);
  void printf(const std::string& s, char* val, int lnum);
  void printf(const std::string& s, wchar_t* val, int lnum);
  void printf(const std::string& s, std::string val, int lnum);   // <- non-const
  void printf(const std::string& s, std::wstring val, int lnum);  // <- non-const

  // Generic types need to be defined in header
  template<typename T>
  void printf(const std::string& s, const T* val, int lnum)
    { _printf_ptr(s, val, lnum); }
  template<typename T>
  void printf(const std::string& s, T* val, int lnum)
    { _printf_ptr(s, val, lnum); }
  template<typename T>
  void printf(const std::string& s, T, int lnum)
    { _printf_str(s, "<unknown type>", lnum); }

  // ---------------------------
  // TEST ASSERT STATEMENTS
  // ---------------------------

  void assert_true(bool condition, bool eq, const std::string& s, int lnum);
  void assert_double_equal(const double& exp, const double& act,
    bool eq, const double& delta, const std::string& s, int lnum);
  void assert_stric_equal(const std::string& exp, const std::string& act,
    bool eq, const std::string& s, int lnum);
  void assert_stric_equal(const std::wstring& exp, const std::wstring& act,
    bool eq, const std::string& s, int lnum);

  // Generic types need to be defined in header
  template<typename TA, typename TB>
  void assert_equal(TA exp, TB act, bool eq, const std::string& s, int lnum) {
    if ( !_assert_impl( (exp == act), eq, s, lnum ) ) {
      if (eq) printf("-exp = %g", exp, 0);
      else printf("-nxp = %g", exp, 0);
      printf("-act = %g", act, 0);
    }
  }
  template<typename TA, typename TB>
  void assert_less_than(TA act, TB lim, bool eq, const std::string& s, int lnum) {
    if ( !_assert_impl( ((act < lim) || (eq && act == lim)), true, s, lnum ) ) {
      printf("-act = %g", act, 0);
      printf("-lim = %g", lim, 0);
    }
  }
  template<typename TA, typename TB>
  void assert_greater_than(TA act, TB lim, bool eq, const std::string& s, int lnum) {
    if ( !_assert_impl( ((act > lim) || (eq && act == lim)), true, s, lnum ) ) {
      printf("-act = %g", act, 0);
      printf("-lim = %g", lim, 0);
    }
  }

  // Array assert equal
  // We can do some pointer checking with this
  template<typename TA, typename TB>
  void assert_array_equal(TA* const exp, TB* const act, bool eq,
    std::size_t cnt, const std::string& s, int lnum = 0) {

    int rslt = 0;
    if ((exp != act) && (exp == 0 || act == 0)) rslt = -1;
    if (cnt != 0 && (exp == 0 || act == 0)) rslt = -1;

    for(std::size_t n = 0; (n < cnt && rslt == 0); ++n)
      if (exp[n] != act[n]) rslt = 1;

     if (rslt == -1) {
       _assert_impl(false, true, s, lnum); // <- force fail irrespective of eq
       _printf_str("-error: invalid null pointer", "", 0);
     } else {
       _assert_impl((rslt == 0), eq, s, lnum);
     }
  }

  // Container assert equal
  template<typename TA, typename TB>
  void assert_array_equal(const TA& exp, const TB& act, bool eq, std::size_t cnt,
    const std::string& s, int lnum = 0) {
    bool rslt = true;
    for(std::size_t n = 0; (n < cnt && rslt); ++n)
      if (exp[n] != act[n]) rslt = false;
    _assert_impl(rslt, eq, s, lnum);
  }

};

} // namespace

//---------------------------------------------------------------------------
#endif // HEADER GUARD
//---------------------------------------------------------------------------
