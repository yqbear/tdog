//---------------------------------------------------------------------------
// PROJECT      : TDOG
// FILENAME     : text_reporter.cpp
// COPYRIGHT    : Andy Thomas (c) 2016
// WEBSITE      : bigangrydog.com
// LICENSE      : Apache 2.0
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// INCLUDES
//---------------------------------------------------------------------------
#include "text_reporter.hpp"
#include "tdog/runner.hpp"
#include "tdog.hpp"
#include "util.hpp"

//---------------------------------------------------------------------------
// NON-CLASS
//---------------------------------------------------------------------------
using namespace tdog;

namespace tdog {

// Report tag names
const char* const PROJECT_NAME_LEADER = "PROJECT: ";
const char* const PROJECT_VERSION_LEADER = "Version: ";
const char* const PROJECT_HOST_LEADER = "Hostname: ";
const char* const PROJECT_PLATFORM_LEADER = "Platform: ";
const char* const REG_DECL_ERRORS = "TEST DECLARATION ERRORS";
const char* const START_TIME_LEADER = "Start Time: ";
const char* const TC_OUTPUT_SEP = " : ";
const char* const SUITE_LEADER = "SUITE : ";
const char* const DEF_SUITE_NAME = "DEFAULT SUITE";
const char* const TEST_STARTED_LEADER = "TEST STARTED: ";
const char* const AUTHOR_LEADER = "AUTHOR: ";
const char* const FILENAME_LEADER = "LOCATION: ";
const char* const TEST_TYPE_LEADER = "TEST TYPE: ";
const char* const REPEAT_TYPE_LEADER = "REPEAT TYPE: ";
const char* const USER_TYPE_LEADER = "USER TYPE: ";
const char* const TEST_END_LEADER = "Completed in ";
const char* const ASSERT_COUNT = "asserts ";
const char* const FAIL_COUNT = "failures ";
const char* const SKIP_SECTION = "SKIPPED/DISABLED TESTS";
const char* const STAT_SECTION = "STATISTICAL RESULTS";
const char* const STAT_TOTAL_LEADER = "TOTAL: ";
const char* const STAT_RAN_LEADER = "Ran: ";
const char* const STAT_PASSED_LEADER = "Passed: ";
const char* const STAT_WARNING_LEADER = "Warnings: ";
const char* const STAT_FAILED_LEADER = "Failed: ";
const char* const STAT_ERRORS_LEADER = "Errors: ";
const char* const STAT_SKIPPED_LEADER = "Skipped: ";
const char* const STAT_DISABLED_LEADER = "Disabled: ";
const char* const STAT_PERC_LEADER = "Pass Rate: ";
const char* const ABORTED_MSG = "TEST RUN ABORTED";
const char* const END_TIME_LEADER = "END TIME: ";
const char* const DURATION_LEADER = "RUN DURATION: ";
const char* const TEARDOWN_SUFFIX = " (teardown)";
const char* const NONE_STR = "NONE";

}
//---------------------------------------------------------------------------
// CLASS text_reporter : PRIVATE MEMBERS
//---------------------------------------------------------------------------
void text_reporter::_impl_clear()
{
  // Internal clear as public clear is virtual.
  m_decl_flag = false;
  m_current_suite.clear();
  m_skipped_tests.clear();
}
//---------------------------------------------------------------------------
void text_reporter::_write_txt_break(std::ostream& os) const
{
  // Write text breaker (if any)
  if (m_break_text_width > 0)
  {
    os << std::string(m_break_text_width, '-');
    os << "\n";
  }
}
//---------------------------------------------------------------------------
// CLASS text_reporter : PUBLIC MEMBERS
//---------------------------------------------------------------------------
text_reporter::text_reporter(const runner* owner, bool verbose)
  : basic_reporter(owner, verbose)
{
  // Set default values
  m_break_text_width = owner->text_report_break_width();
  m_testname_leader = true;
  m_suite_breaks = true;

  // Call non-virtual clear routine.
  _impl_clear();
}
//---------------------------------------------------------------------------
void text_reporter::clear()
{
  // Clear internal data
  basic_reporter::clear();
  _impl_clear();
}
//---------------------------------------------------------------------------
int text_reporter::break_text_width() const
{
  // Accessor
  return m_break_text_width;
}
//---------------------------------------------------------------------------
void text_reporter::set_break_text_width(int bw)
{
  // Mutator
  if (bw < 0) bw = 0;
  m_break_text_width = bw;
}
//---------------------------------------------------------------------------
bool text_reporter::testname_leader() const
{
  // Accessor
  return m_testname_leader;
}
//---------------------------------------------------------------------------
void text_reporter::set_testname_leader(bool tn)
{
  // Mutator
  m_testname_leader = tn;
}
//---------------------------------------------------------------------------
bool text_reporter::suite_breaks() const
{
  // Accessor
  return m_suite_breaks;
}
//---------------------------------------------------------------------------
void text_reporter::set_suite_breaks(bool sb)
{
  // Mutator
  m_suite_breaks = sb;
}
//---------------------------------------------------------------------------
void text_reporter::gen_start(std::ostream& os)
{
  // Write header to stream
  clear();

  _write_txt_break(os);

  // Name
  std::string temp_str = owner().project_name();
  if (!temp_str.empty()) os << PROJECT_NAME_LEADER << temp_str << "\n";

  // Version
  temp_str = owner().project_version();
  if (!temp_str.empty()) os << PROJECT_VERSION_LEADER << temp_str << "\n";

  // Name and platform OS
  os << PROJECT_HOST_LEADER << tdog::hostname() << "\n";
  os << PROJECT_PLATFORM_LEADER << TDOG_PLATOS << "\n";

  // Start time
  os << START_TIME_LEADER << iso_time(owner().start_time()) << "\n";

  _write_txt_break(os);
  os << "\n";

  // Project description
  temp_str = owner().project_desc();
  if (!temp_str.empty())
  {
    os << _indent() << temp_str << "\n";
    os << "\n";
  }

  // Check declaration errors
  std::vector<std::string> decl_errors = owner().declaration_errors(true);
  m_decl_flag = (decl_errors.size() != 0);

  if (m_decl_flag)
  {
    _write_txt_break(os);
    os << REG_DECL_ERRORS << "\n";
    _write_txt_break(os);
    os << "\n";

    for(std::size_t n = 0; n < decl_errors.size(); ++n)
    {
      os << decl_errors[n] << "\n";
      os << "\n";
    }
  }

}
//---------------------------------------------------------------------------
void text_reporter::gen_test(std::ostream& os, const basic_test* tc)
{
  // Write test case to stream. tc cannot be null.

  // Example verbose...
  // TEST STARTED
  // FILENAME - ucd.cpp
  // Beginning test [451]
  // OK - condition [463]
  // FAIL - mismatch [489]
  // ERROR - unhandled std::exception "ioerror"
  // TEST DURATION - 5 sec
  // TEST FAILED(2)

  // Non-verbose...
  // TEST PASSED OK
  //
  // or...
  // FILENAME - ucd.cpp
  // FAIL - mismatch [489]
  // ERROR - unhandled std::exception "ioerror"
  // TEST FAILED(2)
  const run_helper& helper = tc->helper();

  // Declaration error - abort
  if (m_decl_flag) return;

  if (helper.has_ran())
  {
    // Initialize
    event_item eit;
    std::string line_str, type_str, result_str;

    std::size_t lsz = helper.event_log().size();

    // Test name leader
    std::string tn_str;
    if (m_testname_leader) tn_str = tc->full_name() + TC_OUTPUT_SEP;

    // Build a condition result string
    for(std::size_t n = 0; n < lsz; ++n)
    {
      eit = helper.event_log()[n];

      // Write failures only or everything if verbose is true
      if (verbose() || (eit.item_type != ET_PASS && eit.item_type != ET_INFO))
      {
        // Build upto "ucd_tests::surrogates : FAIL"
        type_str = event_str(eit.item_type);
        line_str = type_str;

        if (!type_str.empty() && !eit.msg_str.empty())
        {
          // Build upto "FAIL: "
          line_str += ": ";
        }

        if (!eit.msg_str.empty())
        {
          // Build upto "FAIL: fail message"
          line_str += eit.msg_str;
        }

        if (eit.line_num > 0)
        {
          // Build upto "FAIL: fail message [368]"
          if (!eit.msg_str.empty() || !type_str.empty())
          {
            line_str += " ";
          }

          line_str += "[" + int_to_str(eit.line_num) + "]";
        }

        if (!line_str.empty())
        {
          // EOL
          result_str += tn_str + line_str + "\n";
        }
      }
    }

    // Write suite header (verbose only)
    std::string tc_suite = SUITE_LEADER + tc->suite_name();
    if (owner().has_suites() && tc->suite_name().empty()) tc_suite = DEF_SUITE_NAME;

    if (m_current_suite != tc_suite)
    {
      // Suite breaks
      m_current_suite = tc_suite;

      if (verbose() && m_suite_breaks)
      {
        _write_txt_break(os);

        os << tc_suite;
        if (tc->is_teardown()) os << TEARDOWN_SUFFIX;

        os << "\n";
        _write_txt_break(os);
      }
    }

    if (verbose())
    {
      // Write test case header (verbose only)
      os << tn_str << TEST_STARTED_LEADER << iso_time(helper.start_time()) << "\n";

      // Write filename
      os << tn_str << FILENAME_LEADER << tc->file_location() << "\n";

      if (!helper.author().empty())
      {
        os << tn_str << AUTHOR_LEADER << helper.author() << "\n";
      }

      // Test and data type info
      os << tn_str << TEST_TYPE_LEADER << test_type_str(tc->test_type()) << "\n";

      if (!tc->repeat_typename().empty())
      {
        os << tn_str << REPEAT_TYPE_LEADER << tc->repeat_typename() << "\n";
      }

      if (!tc->user_typename().empty())
      {
        os << tn_str << USER_TYPE_LEADER << tc->user_typename() << "\n";
      }
    }

    // Write main result section
    os << result_str;

    if (verbose())
    {
      // Test stats
      os << tn_str << TEST_END_LEADER;
      os << duration_str(helper.duration()) << ", ";
      os << ASSERT_COUNT << helper.assert_cnt() << ", ";
      os << FAIL_COUNT << helper.fail_cnt();
      os << "\n";
    }

    // Final result
    os << tn_str << status_str(helper.status()) << "\n";

    // Separate tests if verbose
    if (verbose()) os << "\n";
  }
  else
  {
    // Record to skipped list
    m_skipped_tests.push_back(tc->full_name());
  }

}
//---------------------------------------------------------------------------
void text_reporter::gen_end(std::ostream& os)
{
  // Write footer to stream
  std::size_t sk_sz = m_skipped_tests.size();

  // Build list of skipped tests
  os << "\n";
  _write_txt_break(os);
  os << SKIP_SECTION << "\n";
  _write_txt_break(os);

  if (sk_sz > 0)
  {
    for(std::size_t n = 0; n < sk_sz; ++n)
    {
      os << m_skipped_tests[n] << "\n";
    }
  }
  else
  {
    os << NONE_STR << "\n";
  }

  // Spacer
  os << "\n";

  // Statistics
  _write_txt_break(os);
  os << STAT_SECTION << "\n";
  _write_txt_break(os);

  os << STAT_TOTAL_LEADER << owner().statistic_count(CNT_TOTAL) << "\n";
  os << STAT_RAN_LEADER << owner().statistic_count(CNT_RAN) << "\n";
  os << STAT_PASSED_LEADER << owner().statistic_count(CNT_PASSED) << "\n";
  os << STAT_WARNING_LEADER << owner().statistic_count(CNT_WARNINGS) << "\n";
  os << STAT_FAILED_LEADER << owner().statistic_count(CNT_FAILED) << "\n";
  os << STAT_ERRORS_LEADER << owner().statistic_count(CNT_ERRORS) << "\n";
  os << STAT_SKIPPED_LEADER << owner().statistic_count(CNT_SKIPPED) << "\n";
  os << STAT_DISABLED_LEADER << owner().statistic_count(CNT_DISABLED) << "\n";
  os << STAT_PERC_LEADER << owner().statistic_count(CNT_PASS_RATE) << "%\n";
  os << "\n";

  if (m_decl_flag)
  {
    os << ABORTED_MSG << "\n";
    os << "\n";
  }

  _write_txt_break(os);

  os << END_TIME_LEADER << iso_time(owner().end_time()) << "\n";
  os << DURATION_LEADER << duration_str(owner().duration()) << "\n";
  _write_txt_break(os);
  os << "\n";
}
//---------------------------------------------------------------------------
