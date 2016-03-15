//---------------------------------------------------------------------------
// PROJECT      : TDOG
// FILENAME     : basic_test.hpp
// COPYRIGHT    : Andy Thomas (c) 2016
// WEBSITE      : bigangrydog.com
// LICENSE      : Apache 2.0
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// HEADER GUARD
//---------------------------------------------------------------------------
#ifndef TDOG_BASIC_TEST_H
#define TDOG_BASIC_TEST_H

//---------------------------------------------------------------------------
// INCLUDES
//---------------------------------------------------------------------------
#include "tdog/types.hpp"
#include "run_helper.hpp"

#include <string>

//---------------------------------------------------------------------------
// DOCUMENTATION
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// DECLARATIONS
//---------------------------------------------------------------------------

// Namespace
namespace tdog {

//---------------------------------------------------------------------------
// CLASS basic_test
//---------------------------------------------------------------------------
class basic_test
{
  private:

  std::string m_test_name;
  std::string m_suite_name;
  std::string m_full_name;
  const char* m_full_name_ptr;
  std::string m_filename;
  int m_line_num;
  test_type_t m_test_type;
  std::string m_user_typename;
  std::string m_repeat_typename;

  protected:

  run_helper m_helper;

  // Must be implemented in typed concrete class.
  virtual void _run_unprotected() = 0;

  public:

  // Construction
  basic_test(const std::string& tname, // <- test name
    test_type_t ttype,  // <- test type
    const std::string& fname, // <- filename
    int lnum,   // <- line number
    const std::string& utype = std::string(),   // <- user typename
    const std::string& rtype = std::string());  // <- repeated test typename
  virtual ~basic_test();

  // Properties
  std::string test_name() const;
  std::string suite_name() const;
  std::string full_name() const;
  const char* full_name_cstr();
  bool is_setup() const;
  bool is_teardown() const;
  std::string file_location() const;
  test_type_t test_type() const;
  std::string user_typename() const;
  std::string repeat_typename() const;
  void set_skipped();
  bool set_enabled(bool flag);
  void clear_results();

  // Get the helper
  const run_helper& helper() const;

  // Name equality
  bool name_equals(const std::string& name, bool precise) const;

  // The public run method
  void run(tdog::i64_t glimit = 0, tdog::i64_t gwarn = 0);
};

} // namespace

//---------------------------------------------------------------------------
#endif // HEADER GUARD
//---------------------------------------------------------------------------

