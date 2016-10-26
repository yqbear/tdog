//---------------------------------------------------------------------------
// PROJECT      : TDOG
// FILENAME     : basic_test.cpp
// COPYRIGHT    : Kuiper (c) 2016
// WEBSITE      : kuiper.zone
// LICENSE      : Apache 2.0
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// INCLUDES
//---------------------------------------------------------------------------
#include "tdog/private/basic_test.hpp"
#include "tdog/private/suite_manager.hpp"
#include "tdog/runner.hpp"
#include "tdog_exception.hpp"
#include "util.hpp"

#include <stdexcept>
#include <typeinfo>

//---------------------------------------------------------------------------
// NON-CLASS
//---------------------------------------------------------------------------
using namespace tdog;

// Convenience only
const std::size_t NSZ = std::string(NSSEP).size();

//---------------------------------------------------------------------------
// CLASS basic_test : PUBLIC MEMBERS
//---------------------------------------------------------------------------
basic_test::basic_test(const std::string& tname, // <- test name
    test_type_t ttype,  // <- test type
    const std::string& fname,   // <- filename
    int lnum,   // <- line number
    const std::string& utype,   // <- user typename
    const std::string& rtype)   // <- repeated test typename
{
  // Initialize members
  m_test_name = trim_str(tname);
  m_suite_name = suite_manager::current();
  m_filename = trim_str(fname);
  m_test_type = ttype;
  m_line_num = lnum;
  m_user_typename = utype;
  m_repeat_typename = rtype;

  // Clean up leading separator
  if (m_suite_name.substr(0, NSZ) == NSSEP)
  {
    m_suite_name.erase(0, NSZ);
  }

  // Assign final. The string should not
  // change after this, otherwise we need
  // re-assign pointer member.
  if (!m_suite_name.empty())
  {
    m_full_name = m_suite_name + NSSEP + m_test_name;
  }
  else
  {
    m_full_name = m_test_name;
  }

  m_full_name_ptr = m_full_name.c_str();

  // Temporary debug
  // printf("FULL NAME = %s\n", m_full_name_ptr);

  if (suite_manager::auto_reg())
  {
    // Register with test_runner if reg flag is true.
    runner::global().register_test(this);
  }
}
//---------------------------------------------------------------------------
basic_test::~basic_test()
{
  // Destructor
}
//---------------------------------------------------------------------------
std::string basic_test::test_name() const
{
  // Accessor to test name
  return m_test_name;
}
//---------------------------------------------------------------------------
std::string basic_test::suite_name() const
{
  // Accessor to suite name
  return m_suite_name;
}
//---------------------------------------------------------------------------
std::string basic_test::full_name() const
{
  // The suite::test_name as a c-style string.
  return m_full_name;
}
//---------------------------------------------------------------------------
const char* basic_test::full_name_cstr()
{
  // The suite::test_name as a c-style string.
  // Points to array maintaind for life of object.
  return m_full_name_ptr;
}
//---------------------------------------------------------------------------
bool basic_test::is_setup() const
{
  // True if a setup() test
  if (m_test_name.size() > 4)
  {
    std::string temp = ascii_lower(m_test_name);
    while (temp.substr(0, 1) == "_") temp.erase(0, 1);
    return (temp.substr(0, 5) == "setup");
  }

  return false;
}
//---------------------------------------------------------------------------
bool basic_test::is_teardown() const
{
  // True if a teardown() test
  if (m_test_name.size() > 7)
  {
    std::string temp = ascii_lower(m_test_name);
    while (temp.substr(0, 1) == "_") temp.erase(0, 1);
    return (temp.substr(0, 8) == "teardown");
  }

  return false;
}
//---------------------------------------------------------------------------
std::string basic_test::file_location() const
{
  // File name and line number where test is declared
  std::string rslt;

  if (!m_filename.empty())
  {
    rslt += m_filename;
    if (m_line_num > 0) rslt  += " [" + int_to_str(m_line_num) + "]";
  }

  return rslt;
}
//---------------------------------------------------------------------------
test_type_t basic_test::test_type() const
{
  return m_test_type;
}
//---------------------------------------------------------------------------
std::string basic_test::user_typename() const
{
  return m_user_typename;
}
//---------------------------------------------------------------------------
std::string basic_test::repeat_typename() const
{
  return m_repeat_typename;
}
//---------------------------------------------------------------------------
void basic_test::set_skipped()
{
  // Handled by the helper
  return m_helper.set_skipped();
}
//---------------------------------------------------------------------------
bool basic_test::set_enabled(bool flag)
{
  // Handled by the helper
  return m_helper.set_enabled(flag);
}
//---------------------------------------------------------------------------
void basic_test::clear_results()
{
  m_helper.clear();
}
//---------------------------------------------------------------------------
const run_helper& basic_test::helper() const
{
  // Access the helper object results
  return m_helper;
}
//---------------------------------------------------------------------------
bool basic_test::name_equals(const std::string& name, bool precise) const
{
  // True if name matches the test full name, or...
  //  - name is "*" (match anything),
  //  - "::" matches empty (default) suite,
  //  - "suite::" matches all directly in suite, but not sub-suites,
  //  - "suite::*" matches everything under "suite",
  //  - "::*" also matches everthing.
  // If pricise is true, name must match full test name.
  const std::string XSFX = std::string(NSSEP) + "*";

  std::string tn = trim_str(name);

  // The easy bit
  if (tn.empty()) return false;
  if (tn == "*" || tn == XSFX) return !precise;

  if (tn.size() > NSZ && tn.substr(0, NSZ) == NSSEP)
  {
    // Remove leading "::"
    tn.erase(0, NSZ);
  }

  if (!precise)
  {
    // Determine suffixes
    if (is_suffix(tn, NSSEP))
    {
      // Full suite name match
      tn.erase(tn.size() - NSZ, NSZ);
      return (m_suite_name == tn);
    }
    else
    if (is_suffix(tn, XSFX))
    {
      // Partial suite name match - remove "*"
      tn.erase(tn.size() - 1, 1);
      return ((m_suite_name.size() > tn.size() &&
        m_suite_name.substr(0, tn.size()) == tn) ||
        m_suite_name + std::string(NSSEP) == tn);
    }
  }

  // Full test name match
  return (m_full_name == tn);
}
//---------------------------------------------------------------------------
void basic_test::run(tdog::i64_t glimit, tdog::i64_t gwarn)
{
  // Run the test, wrapping it into try-catch block.
  try
  {
    if (m_helper.start_test(glimit, gwarn))
    {
      // Call the wrapper of the wapper
      // of the thing that does the thing.
      _run_unprotected();
    }
  }
  catch(const tdog_exception&)
  {
    // Do nothing.
    // Information already written to log.
  }
  catch(const std::bad_alloc&)
  {
    m_helper.raise_error("unhandled std::bad_alloc thrown in test", 0,
      "std::bad_alloc");
  }
  catch(const std::bad_cast&)
  {
    m_helper.raise_error("unhandled std::bad_cast thrown in test", 0,
      "std::bad_cast");
  }
  catch(const std::bad_typeid&)
  {
    m_helper.raise_error("unhandled std::bad_typeid thrown in test", 0,
      "std::bad_typeid");
  }
  catch(const std::domain_error& e)
  {
    m_helper.raise_error("unhandled std::domain_error ('" +
      std::string(e.what()) + "') thrown in test", 0, "std::domain_error");
  }
  catch(const std::invalid_argument& e)
  {
    m_helper.raise_error("unhandled std::invalid_argument ('" +
      std::string(e.what()) + "') thrown in test", 0, "std::invalid_argument");
  }
  catch(const std::length_error& e)
  {
    m_helper.raise_error("unhandled std::length_error ('" +
      std::string(e.what()) + "') thrown in test", 0, "std::length_error");
  }
  catch(const std::out_of_range& e)
  {
    m_helper.raise_error("unhandled std::out_of_range ('" +
      std::string(e.what()) + "') thrown in test", 0, "std::out_of_range");
  }
  catch(const std::overflow_error& e)
  {
    m_helper.raise_error("unhandled std::overflow_error ('" +
      std::string(e.what()) + "') thrown in test", 0, "std::overflow_error");
  }
  catch(const std::range_error& e)
  {
    m_helper.raise_error("unhandled std::range_error ('" +
      std::string(e.what()) + "') thrown in test", 0, "std::range_error");
  }
  catch(const std::underflow_error& e)
  {
    m_helper.raise_error("unhandled std::underflow_error ('" +
      std::string(e.what()) + "') thrown in test", 0, "std::underflow_error");
  }
  catch(const std::logic_error& e)
  {
    m_helper.raise_error("unhandled std::logic_error ('" +
      std::string(e.what()) + "') thrown in test", 0, "std::logic_error");
  }
  catch(const std::runtime_error& e)
  {
    m_helper.raise_error("unhandled std::runtime_error ('" +
      std::string(e.what()) + "') thrown in test", 0, "std::runtime_error");
  }
  catch(const std::exception& e)
  {
    m_helper.raise_error("unhandled std::exception ('" +
      std::string(e.what()) + "') thrown in test", 0, "std::exception");
  }
  catch(const void*)
  {
    m_helper.raise_error("unhandled pointer thrown in test", 0, "pointer");
  }
  catch(const int&)
  {
    m_helper.raise_error("unhandled 'int' thrown in test", 0, "int");
  }
  catch(...)
  {
    m_helper.raise_error("unknown exception thrown in test", 0);
  }

   m_helper.stop_test();
}
//---------------------------------------------------------------------------
