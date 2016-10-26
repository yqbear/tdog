//---------------------------------------------------------------------------
// PROJECT      : TDOG
// FILENAME     : xml_reporter.cpp
// COPYRIGHT    : Kuiper (c) 2016
// WEBSITE      : kuiper.zone
// LICENSE      : Apache 2.0
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// INCLUDES
//---------------------------------------------------------------------------
#include "xml_reporter.hpp"
#include "tdog.hpp"
#include "util.hpp"

#include <sstream>

//---------------------------------------------------------------------------
// NON-CLASS
//---------------------------------------------------------------------------
using namespace tdog;

namespace tdog {

// XML Elements
const char* const TEST_ROOT_ELEM = "testsuites";
const char* const TEST_DECLERR_ELEM = "declerr";
const char* const TEST_SUITE_ELEM = "testsuite";
const char* const TEST_CASE_ELEM = "testcase";
const char* const TEST_SKIPPED_ELEM = "skipped";
const char* const TEST_ERROR_ELEM = "error";
const char* const TEST_FAILURE_ELEM = "failure";
const char* const TEST_SYSTEMOUT_ELEM = "system-out";

// XML Attributes
const char* const NAME_ATTRIB = "name";
const char* const VERSION_ATTRIB = "version";
const char* const HOSTNAME_ATTRIB = "hostname";
const char* const TESTS_ATTRIB = "tests";
const char* const ERRORS_ATTRIB = "errors";
const char* const FAILURES_ATTRIB = "failures";
const char* const DISABLED_ATTRIB = "disabled";
const char* const SKIPPED_ATTRIB = "skipped";
const char* const SKIP_ATTRIB = "skip";
const char* const TIME_ATTRIB = "time";
const char* const TIMESTAMP_ATTRIB = "timestamp";
const char* const ID_ATTRIB = "id";
const char* const CLASS_ATTRIB = "classname";
const char* const ASSERTIONS_ATTRIB = "assertions";
const char* const STATUS_ATTRIB = "status";
const char* const MESSAGE_ATTRIB = "message";
const char* const TYPE_ATTRIB = "type";

// Others
const char* const DEFAULT_SUITE = "DEFAULT";

}
//---------------------------------------------------------------------------
// CLASS xml_reporter : PRIVATE MEMBERS
//---------------------------------------------------------------------------
void xml_reporter::_impl_clear()
{
  // Internal clear as public clear is virtual.
  m_current_suite.clear();
  m_root_output.clear();
  m_suite_output.clear();

  m_decl_flag = false;
  m_comment_flag = false;

  m_suite_tests = 0;
  m_suite_errors = 0;
  m_suite_failures = 0;
  m_suite_skipped = 0;
  m_suite_disabled = 0;
  m_suite_time = 0;
}
//---------------------------------------------------------------------------
void xml_reporter::_close_suite(std::string& ref)
{
  // Close testsuite and decrement indent
  ref += _indent(-1) + "</";
  ref += TEST_SUITE_ELEM;
  ref += ">\n";
}
//---------------------------------------------------------------------------
void xml_reporter::_write_stats(std::string& ref, bool is_root)
{
  // Replaces placeholders with know values
  const std::string PFX = "$";

  if (is_root)
  {
    // Root
    ref = str_replace(ref, PFX + TESTS_ATTRIB, int_to_str(
      owner().statistic_count(CNT_TOTAL, "*")));
    ref = str_replace( ref, PFX + FAILURES_ATTRIB, int_to_str(
      owner().statistic_count(CNT_FAILED, "*")));
    ref = str_replace(ref, PFX + ERRORS_ATTRIB, int_to_str(
      owner().statistic_count(CNT_ERRORS, "*")));
    ref = str_replace(ref, PFX + SKIP_ATTRIB, int_to_str(
      owner().statistic_count(CNT_SKIPPED, "*")));
    ref = str_replace(ref, PFX + DISABLED_ATTRIB, int_to_str(
      owner().statistic_count(CNT_DISABLED, "*")));

    // Total duration
    tdog::i64_t dur = (owner().end_time() - owner().start_time()) / 1000;
    ref = str_replace(ref, PFX + TIME_ATTRIB, int_to_str(dur));
  }
  else
  {
    // Suite
    ref = str_replace(ref, PFX + TESTS_ATTRIB, int_to_str(m_suite_tests));
    ref = str_replace(ref, PFX + FAILURES_ATTRIB, int_to_str(m_suite_failures));
    ref = str_replace(ref, PFX + ERRORS_ATTRIB, int_to_str(m_suite_errors));
    ref = str_replace(ref, PFX + SKIPPED_ATTRIB, int_to_str(m_suite_skipped));
    ref = str_replace(ref, PFX + DISABLED_ATTRIB, int_to_str(m_suite_disabled));
    ref = str_replace(ref, PFX + TIME_ATTRIB, int_to_str(m_suite_time));
  }
}
//---------------------------------------------------------------------------
// CLASS xml_reporter : PUBLIC MEMBERS
//---------------------------------------------------------------------------
xml_reporter::xml_reporter(const runner* owner, bool verbose)
  : basic_reporter(owner, verbose)
{
  // Call non-virtual clear routine.
  _impl_clear();
}
//---------------------------------------------------------------------------
void xml_reporter::clear()
{
  // Clear internal data
  basic_reporter::clear();
  _impl_clear();
}
//---------------------------------------------------------------------------
void xml_reporter::gen_start(std::ostream&)
{
  // Write header.
  clear();

  // We ignore the ostream here, but write to an internal
  // buffer as we must update the stats at the end of the run.
  std::stringstream os;

  // Write XML log header to output stream
  std::string cs_str = owner().report_charset();
  os << _indent() << "<?xml version=\"1.0\" ";
  if (!cs_str.empty()) os << "encoding=\"" << cs_str << "\"";
  os << "?>\n";

  // Comment
  os << _indent() << "<!-- Test report generated by: "  << TDOG_LIB_NAME << " ";
  os << TDOG_LIB_VERSION << " -->\n";

  // Root test run element. NOTE - we insert placeholders
  // prefixed with "$". We will replace this in gen_end();
  os << _indent(+1) << "<" << TEST_ROOT_ELEM;
  os << " " << NAME_ATTRIB << "=\"" << xml_esc(owner().project_name()) << "\"";
  os << " " << VERSION_ATTRIB << "=\"" << xml_esc(owner().project_version()) << "\"";
  os << " " << TESTS_ATTRIB << "=\"$" << TESTS_ATTRIB << "\"";
  os << " " << FAILURES_ATTRIB << "=\"$" << FAILURES_ATTRIB << "\"";
  os << " " << ERRORS_ATTRIB << "=\"$" << ERRORS_ATTRIB << "\"";
  os << " " << SKIP_ATTRIB << "=\"$" << SKIP_ATTRIB << "\"";
  os << " " << DISABLED_ATTRIB << "=\"$" << DISABLED_ATTRIB << "\"";
  os << " " << TIME_ATTRIB << "=\"$" << TIME_ATTRIB << "\"";
  os << ">\n";

  // Check declaration errors
  std::vector<std::string> decl_errors = owner().declaration_errors(false);
  m_decl_flag = (decl_errors.size() != 0);

  // Build comment
  os << "\n";
  os << _indent() << "<!-- The '" << TEST_DECLERR_ELEM << "' element contains test case"
  " declaration error messages. Test run will be aborted if errors are present.\n";
  os << _indent() << "Example: <" << TEST_DECLERR_ELEM << " " << ERRORS_ATTRIB << "=\"1\">";
  os << "<" << TEST_ERROR_ELEM << ">Some message.</" << TEST_ERROR_ELEM << ">";
  os << "</" << TEST_DECLERR_ELEM << "> -->\n";

  // Actual element
  int estep = 0;
  if (m_decl_flag) estep = 1;
  os << _indent(estep) << "<" << TEST_DECLERR_ELEM;
  os << " " << ERRORS_ATTRIB << "=\"" << decl_errors.size() << "\">";

  for(std::size_t n = 0; n < decl_errors.size(); ++n)
  {
    if (n == 0) os << "\n";
    os << _indent() << "<" << TEST_ERROR_ELEM << ">";
    os << xml_esc(decl_errors[n]);
    os << "</" << TEST_ERROR_ELEM << ">\n";
  }

  // Close errors
  if (m_decl_flag) os << _indent(-estep);
  os << "</" << TEST_DECLERR_ELEM << ">\n";

  // Spacer
  os << "\n";

  // Hold for later
  m_root_output = os.str();
}
//---------------------------------------------------------------------------
void xml_reporter::gen_test(std::ostream&, const basic_test* tc)
{
  // Write test case to stream. tc cannot be null.
  // We ignore the ostream here, but write to an
  // internal buffer as we must update the stats at the end of the run.
  const run_helper& helper = tc->helper();

  // Declaration error - abort
  if (m_decl_flag) return;

  if (helper.status() != TS_READY)
  {
    // We ignore the ostream here, but write to an internal
    // buffer as we must update the stats at the end of the run.
    std::stringstream os;

    // Suite ID starts at 0
    std::size_t suite_id = m_suite_output.size();

    if (tc->suite_name() != m_current_suite || suite_id == 0)
    {
      if (suite_id != 0) // <- suite exists
      {
        // Close old suite
        std::string& ref = m_suite_output.back();

        // Insert stats and add closing tag
        _write_stats(ref, false);
        _close_suite(ref);
      }

      // New suite
      m_suite_output.push_back( std::string() );
      m_current_suite = tc->suite_name();

      // Zero suite stats
      m_suite_tests = 0;
      m_suite_errors = 0;
      m_suite_failures = 0;
      m_suite_skipped = 0;
      m_suite_disabled = 0;
      m_suite_time = 0;

      // Rename default suite
      std::string suite_temp = xml_esc(m_current_suite);
      if (suite_temp.empty()) suite_temp = DEFAULT_SUITE;

      // Push on suite element
      os << _indent(+1) << "<" << TEST_SUITE_ELEM;
      os << " " << NAME_ATTRIB << "=\"" << suite_temp << "\"";
      os << " " << ID_ATTRIB << "=\"" << suite_id << "\"";
      os << " " << TESTS_ATTRIB << "=\"$" << TESTS_ATTRIB << "\"";
      os << " " << ERRORS_ATTRIB << "=\"$" << ERRORS_ATTRIB << "\"";
      os << " " << FAILURES_ATTRIB << "=\"$" << FAILURES_ATTRIB << "\"";
      os << " " << SKIPPED_ATTRIB << "=\"$" << DISABLED_ATTRIB << "\"";
      os << " " << DISABLED_ATTRIB << "=\"$" << SKIPPED_ATTRIB << "\"";
      os << " " << TIME_ATTRIB << "=\"$" << TIME_ATTRIB << "\"";
      os << " " << TIMESTAMP_ATTRIB << "=\"" << iso_time(helper.start_time()) << "\"";
      os << " " << HOSTNAME_ATTRIB << "=\"" << tdog::hostname() << "\"";
      os << ">\n";
    }

    // Increment test stats
    ++m_suite_tests;
    m_suite_time += (helper.duration() / 1000);

    // Add test case
    os << _indent(+1) << "<" << TEST_CASE_ELEM;
    os << " " << NAME_ATTRIB << "=\"" << xml_esc(tc->test_name()) << "\"";
    os << " " << CLASS_ATTRIB << "=\"" << xml_esc(tc->full_name()) << "\"";
    os << " " << ASSERTIONS_ATTRIB << "=\"" << helper.assert_cnt() << "\"";
    os << " " << STATUS_ATTRIB << "=\"" << status_str(helper.status()) << "\"";
    os << " " << TIME_ATTRIB << "=\"" << (helper.duration() / 1000) << "\"";
    os << ">\n";

    if (helper.has_ran())
    {
      // Output failures, errors and messages
      bool sysout_need_close = false;
      std::size_t esz = helper.event_log().size();

      for(std::size_t n = 0; n < esz; ++n)
      {
        event_item eit = helper.event_log()[n];
        std::string msg = xml_esc(eit.msg_str);

        // Determine if message, and hold value
        bool is_sysout_msg = (verbose() || (eit.item_type != ET_PASS && eit.item_type != ET_INFO));

        if (!is_sysout_msg && sysout_need_close)
        {
          // Close system-out
          os << "</" << TEST_SYSTEMOUT_ELEM << ">\n";
          sysout_need_close = false;
        }

        if (eit.line_num > 0)
        {
          // i.e. "fail message [368]"
          msg += " [" + int_to_str(eit.line_num) + "]";
        }

        if (eit.item_type == ET_ERROR)
        {
          // Should only ever be one error, as test
          // execution would cease when raised.
          os << _indent() << "<" << TEST_ERROR_ELEM;
          os << " " << MESSAGE_ATTRIB << "=\"" << msg << "\"";
          os << " " << TYPE_ATTRIB << "=\"" << xml_esc(helper.error_name()) << "\"";
          os << "/>\n";
        }
        else
        if (eit.item_type == ET_FAIL)
        {
          os << _indent() << "<" << TEST_FAILURE_ELEM;
          os << " " << MESSAGE_ATTRIB << "=\"" << msg << "\"";
          os << " " << TYPE_ATTRIB << "=\"\""; // <- empty, as we don't log assert types
          os << "/>\n";
        }
        else
        if (is_sysout_msg)
        {
          // System-out
          // Contents will be escaped, so we don't need CDATA.
          if (!m_comment_flag)
          {
            os << _indent() << "<!-- The '" << TEST_SYSTEMOUT_ELEM << "' element contains assertion and "
            "informational messages rather than actual stdout. -->\n";
            m_comment_flag = true;
          }

          if (eit.item_type == ET_WARN)
          {
            // Add warning leader
            msg = event_str(ET_WARN) + ": " + msg;
          }

          if (!sysout_need_close)
          {
            // Element leader
            os << _indent() << "<" << TEST_SYSTEMOUT_ELEM << ">";
          }
          else
          {
            // Add escaped LF followed by
            // direct LF for XML readability.
            os << "&#xD\n";
          }

          // Output the message.
          os << msg;

          // Flag that we need closure.
          sysout_need_close = true;
        }
      }

      if (sysout_need_close)
      {
        // Close trailing system-out
        os << "</" << TEST_SYSTEMOUT_ELEM << ">\n";
      }

    }
    else
    {
      // We are including skipped element for both
      // disabled and skipped status.
      os << _indent() << "<" << TEST_SKIPPED_ELEM << "/>\n";
    }

    // Keep count
    switch( helper.status() )
    {
      case TS_FAILED:
        ++m_suite_failures;
        break;
      case TS_ERROR:
        ++m_suite_errors;
        break;
      case TS_SKIPPED:
        ++m_suite_skipped;
        break;
      case TS_DISABLED:
        ++m_suite_disabled;
        break;
      default:
        break;
    }

    // End testcase element. NB. we do not put closing
    // tag for testsuite. This is done at the start of
    // this method and in gen_end().
    os << _indent(-1) << "</" << TEST_CASE_ELEM << ">\n";

    // Store in array
    m_suite_output.back() += os.str();
  }
}
//---------------------------------------------------------------------------
void xml_reporter::gen_end(std::ostream& os)
{
  // Write footer to stream. In this we write cached
  // root and suite data. Followed by final closing tag.
  std::size_t suite_sz = m_suite_output.size();

  if (suite_sz != 0)
  {
    // Close old suite
    std::string& ref = m_suite_output.back();

    // Insert stats and add closing tag
    _write_stats(ref, false);
    _close_suite(ref);
  }

  // Finalize stats
  _write_stats(m_root_output, true);

  // Write "cached" output
  os << m_root_output;

  for(std::size_t n = 0; n < suite_sz; ++n)
  {
    os << m_suite_output[n];
  }

  // Close test run
  os << _indent(-1) << "</" << TEST_ROOT_ELEM << ">\n";
}
//---------------------------------------------------------------------------
