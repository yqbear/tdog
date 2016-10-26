//---------------------------------------------------------------------------
// PROJECT      : TDOG
// FILENAME     : xml_reporter.cpp
// COPYRIGHT    : Kuiper (c) 2016
// LICENSE      : Apache 2.0
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// HEADER GUARD
//---------------------------------------------------------------------------
#ifndef TDOG_XML_REPORTER_H
#define TDOG_XML_REPORTER_H

//---------------------------------------------------------------------------
// INCLUDES
//---------------------------------------------------------------------------
#include "basic_reporter.hpp"
#include "tdog/types.hpp"

#include <string>
#include <ostream>
#include <vector>

//---------------------------------------------------------------------------
// DOCUMENTATION
//---------------------------------------------------------------------------

/* JUnit: http://llg.cubic.org/docs/junit/
 *
 * xUnit: https://nose.readthedocs.org/en/latest/plugins/xunit.html
 * <?xml version="1.0" encoding="UTF-8"?>
 * <testsuite name="nosetests" tests="1" errors="1" failures="0" skip="0">
 *     <testcase classname="path_to_test_suite.TestSomething"
 *               name="test_it" time="0">
 *         <error type="exceptions.TypeError" message="oops, wrong type">
 *         Traceback (most recent call last):
 *         ...
 *         TypeError: oops, wrong type
 *         </error>
 *     </testcase>
 * </testsuite>
 */

//---------------------------------------------------------------------------
// DECLARATIONS
//---------------------------------------------------------------------------

// Namespace
namespace tdog {

//---------------------------------------------------------------------------
// CLASS xml_reporter
//---------------------------------------------------------------------------
class xml_reporter : public basic_reporter
{
  private:

  bool m_decl_flag;
  bool m_comment_flag;
  std::string m_root_output;
  std::string m_current_suite;
  std::vector<std::string> m_suite_output;

  // Keep running stats for each suite
  int m_suite_tests, m_suite_errors, m_suite_failures,
    m_suite_skipped, m_suite_disabled;
  tdog::i64_t m_suite_time;

  void _impl_clear();
  void _close_suite(std::string& ref);
  void _write_stats(std::string& ref, bool is_root);
  static std::string _rslt_str(const basic_test* tc);

  public:

  // Constructor
  xml_reporter(const runner* owner, bool verbose = true);

  // Report generator methods
  virtual void clear();
  virtual void gen_start(std::ostream& os);
  virtual void gen_test(std::ostream& os, const basic_test* tc);
  virtual void gen_end(std::ostream& os);
};

} // namespace

//---------------------------------------------------------------------------
#endif // HEADER GUARD
//---------------------------------------------------------------------------

