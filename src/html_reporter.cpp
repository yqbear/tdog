//---------------------------------------------------------------------------
// PROJECT      : TDOG
// FILENAME     : html_reporter.cpp
// COPYRIGHT    : Andy Thomas (c) 2016
// WEBSITE      : bigangrydog.com
// LICENSE      : Apache 2.0
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// INCLUDES
//---------------------------------------------------------------------------
#include "html_reporter.hpp"
#include "text_reporter.hpp"
#include "tdog.hpp"
#include "util.hpp"

#include <sstream>

//---------------------------------------------------------------------------
// NON-CLASS
//---------------------------------------------------------------------------
using namespace tdog;

namespace tdog {

// Report tag names

// Updating these can be used to form the basis for internationalization
const char* const REPORT_TITLE = "TEST REPORT";
const char* const PROJECT_NAME_LEADER = "PROJECT: ";
const char* const PROJECT_VERSION_LEADER = "Version: ";
const char* const PROJECT_HOST_LEADER = "Hostname: ";
const char* const PROJECT_PLATFORM_LEADER = "Platform: ";
const char* const REG_DECL_ERRORS = "Test Declaration Errors";
const char* const START_TIME_LEADER = "Start Time: ";
const char* const SUITE_LEADER = "SUITE: ";
const char* const DEF_SUITE_NAME = "DEFAULT SUITE";
const char* const REPORT_CONTENTS = "Report Contents";
const char* const TEST_RESULTS = "Test Run Results";
const char* const TEST_STATS = "Statistical Results";
const char* const FAIL_DETAILS = "Output: Test Failures";
const char* const ERROR_DETAILS = "Output: Test Errors";
const char* const PASS_DETAILS = "Output: Test Passes";
const char* const SKIPPED_TESTS = "Skipped or Disabled Tests";
const char* const SKIPPED_NOTES = "The following tests were not executed:";
const char* const END_TIME_LEADER = "End Time: ";
const char* const DURATION_LEADER = "Run Duration: ";
const char* const TAB_TEST_NAME = "Test Name";
const char* const TAB_TEST_TYPE = "Type";
const char* const TAB_AUTHOR = "Author";
const char* const TAB_START_TIME = "Start Time";
const char* const TAB_DURATION = "Duration";
const char* const TAB_RESULT = "Result";
const char* const STAT_TAB_METRIC = "Statistic";
const char* const STAT_TAB_VALUE = "Value";
const char* const STAT_TAB_RAN = "Ran";
const char* const STAT_TAB_SKIPPED = "Skipped";
const char* const STAT_TAB_DISABLED = "Disabled";
const char* const STAT_TAB_PASSED = "Passed";
const char* const STAT_TAB_WARNINGS = "Warnings";
const char* const STAT_TAB_FAILED = "Failed";
const char* const STAT_TAB_ERRORS = "Errors";
const char* const STAT_TAB_TOTAL = "TOTAL";
const char* const STAT_TAB_PERC = "Pass Rate";
const char* const ABORTED_MSG = "TEST RUN ABORTED";
const char* const TEARDOWN_SUFFIX = " (teardown)";
const char* const NONE_STR = "NONE";
}
//---------------------------------------------------------------------------
// CLASS html_reporter : PRIVATE MEMBERS
//---------------------------------------------------------------------------
void html_reporter::_impl_clear()
{
  // Internal clear as public clear is virtual.
  m_decl_flag = false;
  m_suite_added = false;
  m_current_suite.clear();
  m_text_reporter_ptr->clear();

  m_pass_trace.clear();
  m_fail_trace.clear();
  m_error_trace.clear();
  m_skipped_tests.clear();
}
//---------------------------------------------------------------------------
std::ostream& html_reporter::_generate_css(std::ostream& os)
{
  // Generate interal CCS, or specify external sheet
  std::string temp_str = owner().html_stylesheet();

  // Default styles
  os << _indent(+1) << "<style type=\"text/css\">\n";

  os << _indent() << "th {background-color:silver; font-weight:bold;";
  os << " border-style:solid; border-width:thin}\n";

  os << _indent() << "td {border-style:solid; border-width:thin}\n";

  os << _indent() << ".pass-cell-col {background-color:lightgreen}\n";
  os << _indent() << ".warn-cell-col {background-color:yellow}\n";
  os << _indent() << ".fail-cell-col {background-color:red}\n";

  os << _indent(-1) << "</style>\n";

  if (!temp_str.empty())
  {
    os << _indent() << "<link rel=\"stylesheet\" href=\"";
    os << temp_str;
    os << "\" type=\"text/css\" />\n";
  }

  return os;
}
//---------------------------------------------------------------------------
std::string html_reporter::_clean_id(const std::string& id_str)
{
  // Clean up id_str and return result
  std::string rslt;

  std::size_t sz = id_str.size();
  for(std::size_t n = 0; n < sz; ++n)
  {
    if ((id_str[n] >= 'a' && id_str[n] <= 'z') ||
      (id_str[n] >= '0' && id_str[n] <= '9') ||
      id_str[n] == '_' || id_str[n] == ':' ||
      id_str[n] == '.' || id_str[n] == '-')
    {
      rslt += id_str[n];
    }
    else
    if (id_str[n] == 0x20)
    {
      rslt += '_';
    }
    else
    if (id_str[n] >= 'A' && id_str[n] <= 'Z')
    {
      // Lowercase
      rslt += static_cast<char>(id_str[n] + 0x20);
    }
  }

  return rslt;
}
//---------------------------------------------------------------------------
std::string html_reporter::_page_link(const std::string& id_str,
  const std::string& name_str)
{
  // Make "id_str" a link to an anchor.
  // Example: _link("suite", "UCD_TESTS");
  // returns: <a href=\"#anchor_suite">UCD_TESTS</a>
  return "<a href=\"#anchor_" + _clean_id(id_str) + "\">" + name_str + "</a>";
}
//---------------------------------------------------------------------------
std::string html_reporter::_page_anchor(const std::string& id_str)
{
  // Make "cat_str" an (empty) anchor.
  // Example: _anchor("suite");
  // returns: <span id=\"anchor_suite" />
  return "<p id=\"anchor_" + _clean_id(id_str) + "\"></p>";
}
//---------------------------------------------------------------------------
std::string html_reporter::_th_str(const std::string& content,
  const std::string& width)
{
  // Make table header cell
  if (!width.empty())
  {
    return "<th style=\"width:" + width + "\">" + content + "</th>";
  }
  else
  {
    return "<th>" + content + "</th>";
  }
}
//---------------------------------------------------------------------------
std::string html_reporter::_td_str(const std::string& content, bool bold,
    const std::string& class_id)
{
  // Make table data cell
  std::string rslt = "<td";

  if (!class_id.empty()) rslt += " class=\"" + class_id + "\"";
  rslt += ">";

  if (bold) rslt += "<strong>" + content + "</strong>";
  else rslt += content;

  return rslt + "</td>";
}
//---------------------------------------------------------------------------
std::string html_reporter::_cond_classid(status_t s)
{
  // Condition color
  switch(s)
  {
    case TS_PASS_OK: return "pass-cell-col";
    case TS_PASS_WARN: return "warn-cell-col";
    case TS_ERROR:
    case TS_FAILED: return "fail-cell-col";
    default: return "";
  }
}
//---------------------------------------------------------------------------
void html_reporter::_build_trace(std::ostream& os,
  const std::vector<std::string>& trace_vec, const std::string& title_elem)
{
  // During test case generation, trace output if buffered to a vector
  // array for adding to the report later. In this call, we take the vector
  // an build the trace sections of the report. Each test case is stored as
  // two items in the array. The first is the test name, and section is the
  // text formatted trace. Therefore, even numbered array indexes always
  // hold the test name for the trace that follows in the next item.

  // Start local indent - we expect 2 steps instead <html> & <body> tags
  std::size_t sz = trace_vec.size();

  if (sz > 0 && sz % 2 == 0)
  {
    std::string t_name, t_cont;

    for(std::size_t n = 0; n < sz; n += 2)
    {
      // Extract test name and trace contents
      t_name = trace_vec[n];
      t_cont = trace_vec[n+1];

      if (t_cont.empty()) t_cont = NONE_STR;

      if (!t_name.empty())
      {
        // Write title anchor
        os << _indent() << _page_anchor(t_name) + "\n";

        // Make title, i.e. "....<h3>Suite::TestName</h3>"
        os << _indent() << "<" << title_elem << ">";
        os << t_name << "</" + title_elem << ">\n";

        // Stream <ul> start
        os << _indent(+1) << "<ul>\n";

        // Stream trace content, replacing newlines with <li> pairs
        os << _indent() << "<li>";
        os << str_replace(t_cont, "\n", "</li>\n" + _indent() + "<li>");
        os << "</li>\n";

        // Stream <ul> end
        os << _indent(-1) << "</ul>\n";

      }
    }
  }
  else
  {
    // No tests
    os << _indent() << "<p>" << NONE_STR << "</p>\n";
  }
}
//---------------------------------------------------------------------------
// CLASS html_reporter : PUBLIC MEMBERS
//---------------------------------------------------------------------------
html_reporter::html_reporter(const runner* owner, bool verbose)
  : basic_reporter(owner, verbose)
{
  // Create internal text reporter for test details
  // NB. We disable the test name leader as each output
  // block will have the test name as the heading.
  m_text_reporter_ptr = new text_reporter(owner, verbose);
  m_text_reporter_ptr->set_testname_leader(false);
  m_text_reporter_ptr->set_suite_breaks(false);

  // Call non-virtual clear routine.
  _impl_clear();
}
//---------------------------------------------------------------------------
html_reporter::~html_reporter()
{
  try
  {
    // Destructor
    delete m_text_reporter_ptr;
  }
  catch(...)
  {
  }
}
//---------------------------------------------------------------------------
void html_reporter::clear()
{
  // Clear internal data
  basic_reporter::clear();
  _impl_clear();
}
//---------------------------------------------------------------------------
void html_reporter::gen_start(std::ostream& os)
{
  // Write header to stream
  clear();

  // DOCTYPE
  os << "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.1//EN\" ";
  os << "\"http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd\">\n";

  // HTML element
  os << "<html xmlns=\"http://www.w3.org/1999/xhtml\">\n";

  // Head
  os << _indent(+1) << "<head>\n";

  // Content-Type
  std::string temp_str = owner().report_charset();
  os << _indent() << "<meta http-equiv=\"Content-Type\" ";
  os << "content=\"text/html;";
  if (!temp_str.empty()) os << " charset=" << temp_str;
  os << "\" />\n";

  // Title
  temp_str = xml_esc(owner().project_name());
  os << _indent() << "<title>" << REPORT_TITLE;
  if (!temp_str.empty()) os << " | " << temp_str;
  os << "</title>\n";

  // Stylesheet
  _generate_css(os);

  // Head closure
  os << _indent(-1) << "</head>\n";

  // Body
  os << _indent(+1) << "<body>\n";

  // Content - Title
  os << _indent() << "<h1 style=\"margin-bottom:0;\">" << REPORT_TITLE << "</h1>\n";

  // Generator name
  os << _indent() << "<p style=\"margin-top:0;\">";
  os << TDOG_LIB_NAME << " " << TDOG_LIB_VERSION;
  os << "</p>\n";

  // Project information
  os << _indent(+1) << "<p>\n";

  temp_str = xml_esc(owner().project_name());
  if (!temp_str.empty())
  {
    os << _indent() << "<strong>" << PROJECT_NAME_LEADER << "</strong>";
    os << temp_str << "<br/>\n";
  }

  // Project version
  temp_str = xml_esc(owner().project_version());
  if (!temp_str.empty())
  {
    os << _indent() << "<strong>" << PROJECT_VERSION_LEADER << "</strong>";
    os << temp_str << "<br/>\n";
  }

  // Name and platform OS
  temp_str = xml_esc(tdog::hostname());
  os << _indent() << "<strong>" << PROJECT_HOST_LEADER << "</strong>";
  os << temp_str << "<br/>\n";

  temp_str = xml_esc(TDOG_PLATOS);
  os << _indent() << "<strong>" << PROJECT_PLATFORM_LEADER << "</strong>";
  os << temp_str << "<br/>\n";

  // Start time
  os << _indent() << "<strong>" << START_TIME_LEADER << "</strong>";
  os << iso_time(owner().start_time()) << "<br/>\n";

  os << _indent(-1) << "</p>\n";

  // Project description
  temp_str = xml_esc(owner().project_desc());
  if (!temp_str.empty())
  {
    temp_str = str_replace(temp_str, "\n", "<br/>");
    os << _indent() << "<p>" << temp_str << "</p>";
  }

  // Check declaration errors
  std::vector<std::string> decl_errors = owner().declaration_errors(false);
  m_decl_flag = (decl_errors.size() != 0);

  if (!m_decl_flag)
  {
    // Content - Title
    os << _indent() << "<h2>" << REPORT_CONTENTS << "</h2>\n";

    // Build navigation list
    if (owner().has_suites())
    {
      // Suite navigation
      // List
      os << _indent(+1) << "<ul>\n";

      os << _indent() << "<li>";
      os << _page_link("main", "<strong>" +
        std::string(TEST_RESULTS) + "</strong>");
      os << "</li>\n";

      // Get suite names
      std::vector<std::string> suite_array;
      owner().enumerate_suite_names(suite_array);
      std::size_t sz = suite_array.size();

      for(std::size_t n = 0; n < sz; ++n)
      {
        std::string name = suite_array[n];

        if (name.empty())
        {
          // Sub default name
          name = DEF_SUITE_NAME;
        }

        // Escape
        suite_array[n] = xml_esc(suite_array[n]);

        os << _indent() << "<li>";
        os << _page_link("suite_" + suite_array[n], name) << "</li>\n";
      }

      // List closure
      os << _indent(-1) << "</ul>\n";
    }
    else
    {
      // List
      os << _indent(+1) << "<ul>\n";

      os << _indent() << "<li>";
      os << _page_link("main", "<strong>" +
        std::string(TEST_RESULTS) + "</strong>");
      os << "</li>\n";

      // List closure
      os << _indent(-1) << "</ul>\n";
    }

    // Additional info

    // List
    os << _indent(+1) << "<ul>\n";

    os << _indent() << "<li>";
    os << _page_link("test_stats", "<strong>" +
      std::string(TEST_STATS) + "</strong>");
    os << "</li>\n";

    // Additional content
    os << _indent() << "<li>";
    os << _page_link("fail_details", FAIL_DETAILS);
    os << "</li>\n";

    os << _indent() << "<li>";
    os << _page_link("error_details", ERROR_DETAILS);
    os << "</li>\n";

    if (verbose())
    {
      // Verbose only content
      os << _indent() << "<li>";
      os << _page_link("pass_details", PASS_DETAILS);
      os << "</li>\n";
    }

    os << _indent() << "<li>";
    os << _page_link("skipped_tests", SKIPPED_TESTS);
    os << "</li>\n";

    // List closure
    os << _indent(-1) << "</ul>\n";

    // Main section
    os << _indent() << _page_anchor("main") << "\n";
    os << _indent() << "<h2>" << TEST_RESULTS << "</h2>\n";
  }
  else
  {
    // Error title
    os << _indent() << "<h2>" << REG_DECL_ERRORS << "</h2>\n";

    // List
    os << _indent(+1) << "<ul>\n";

    for(std::size_t n = 0; n < decl_errors.size(); ++n)
    {
      os << _indent() << "<li>";
      os << xml_esc(decl_errors[n]);
      os << "</li>\n";
    }

    // List closure
    os << _indent(-1) << "</ul>\n";
  }

}
//---------------------------------------------------------------------------
void html_reporter::gen_test(std::ostream& os, const basic_test* tc)
{
  // Write test case to stream. tc cannot be null.
  const run_helper& helper = tc->helper();

  if (m_decl_flag)
  {
    // Declaration error - abort
    return;
  }
  else
  if (helper.has_ran())
  {
    // Set temp_str to be suite name
    std::string s_name = xml_esc(tc->suite_name());

    if (s_name != m_current_suite || !m_suite_added)
    {
      // New test suite
      if (m_suite_added)
      {
        // Close old suite
        os << _indent(-1) << "</table>\n";
      }

      // Add new suite
      m_suite_added = true;
      m_current_suite = s_name;

      // Apply default name
      if (owner().has_suites())
      {
        // Add suite header
        std::string title = DEF_SUITE_NAME;
        if (!s_name.empty()) title = SUITE_LEADER + s_name;
        if (tc->is_teardown()) title += TEARDOWN_SUFFIX;

        os << _indent() << _page_anchor("suite_" + s_name) << "\n";
        os << _indent() << "<h3>" << title << "</h3>\n";
      }

      // Open table (defines width properties)
      os << _indent(+1) << "<table style=\"width:75%; max-width:100em\">\n";

      // Header row
      os << _indent(+1) << "<tr>\n";

      os << _indent() << _th_str(TAB_TEST_NAME) << "\n";
      os << _indent() << _th_str(TAB_AUTHOR, "15%") << "\n";
      os << _indent() << _th_str(TAB_TEST_TYPE, "8%") << "\n";
      os << _indent() << _th_str(TAB_START_TIME, "15%") << "\n";
      os << _indent() << _th_str(TAB_DURATION, "16%") << "\n";
      os << _indent() << _th_str(TAB_RESULT, "12%") << "\n";

      // Close row
      os << _indent(-1) << "</tr>\n";
    }

    // Data row
    os << _indent(+1) << "<tr>\n";

    // Set temp_str to be full test name
    std::string t_name = xml_esc(tc->full_name());

    if (verbose() || !helper.has_passed())
    {
      // Add event to corresponding vector - these
      // will be appended to the report later. We append
      // the test name, followed by the trace text, so
      // that even number elements always content the
      // test name, and trace contents will follow.
      std::stringstream ss;
      m_text_reporter_ptr->gen_test(ss, tc);
      std::string t_cont = xml_esc(trim_str(ss.str()));
      t_cont = str_replace(t_cont, "\n", "<br/>");

      // Append to applicable vector
      if (helper.has_passed())
      {
        m_pass_trace.push_back(t_name);
        m_pass_trace.push_back(t_cont);
      }
      else
      if (helper.status() == TS_FAILED)
      {
        m_fail_trace.push_back(t_name);
        m_fail_trace.push_back(t_cont);
      }
      else
      if (helper.status() == TS_ERROR)
      {
        m_error_trace.push_back(t_name);
        m_error_trace.push_back(t_cont);
      }

      // Linked test name
      t_name = _page_link(t_name, t_name);
    }

    os << _indent() << _td_str(t_name) << "\n";
    os << _indent() << _td_str(xml_esc(helper.author())) << "\n";
    os << _indent() << _td_str(test_type_str(tc->test_type())) << "\n";
    os << _indent() << _td_str(iso_time(helper.start_time())) << "\n";
    os << _indent() << _td_str(duration_str(helper.duration())) << "\n";

    // Color pass/fail value
    status_t stat = helper.status();
    os << _indent() << _td_str(status_str(stat), !helper.has_passed(), _cond_classid(stat));
    os << "\n";

    // Close row
    os << _indent(-1) << "</tr>\n";
  }
  else
  {
    // Record to skipped list
    m_skipped_tests.push_back(tc->full_name());
  }

}
//---------------------------------------------------------------------------
void html_reporter::gen_end(std::ostream& os)
{
  // Write footer to stream
  if (m_suite_added)
  {
    // Close last table
    os << _indent(-1) << "</table>\n";
  }
  else
  if (owner().statistic_count(CNT_RAN) == 0)
  {
    os << _indent() << "<p>" << NONE_STR << "</p>\n";
  }

  // Statistics
  os << _indent() << _page_anchor("test_stats") << "\n";
  os << _indent() << "<h2>" << TEST_STATS << "</h2>\n";

  // Define colors
  std::string pass_id, fail_id, err_id;

  if (owner().statistic_count(CNT_FAILED) > 0)
  {
    fail_id = _cond_classid(TS_FAILED);
  }

  if (owner().statistic_count(CNT_ERRORS) > 0)
  {
    err_id = _cond_classid(TS_ERROR);
  }

  tdog::i64_t ran_cnt = owner().statistic_count(CNT_RAN);

  if (ran_cnt > 0 && ran_cnt == owner().statistic_count(CNT_PASSED))
  {
    // All passed
    pass_id = _cond_classid(TS_PASS_OK);
  }

  if (m_decl_flag)
  {
    // Yikes!
    pass_id = _cond_classid(TS_ERROR);
  }


  // Open table
  os << _indent(+1) << "<table style=\"width:20em\">\n";

  // Header row
  os << _indent(+1) << "<tr>\n";

  os << _indent() << _th_str(STAT_TAB_METRIC) << "\n";
  os << _indent() << _th_str(STAT_TAB_VALUE) << "\n";

  // Close row
  os << _indent(-1) << "</tr>\n";

  os << _indent(+1) << "<tr>\n";
  os << _indent() << _td_str(STAT_TAB_TOTAL, true) << "\n";
  os << _indent() << _td_str(int_to_str(
    owner().statistic_count(CNT_TOTAL))) << "\n";
  os << _indent(-1) << "</tr>\n";

  os << _indent(+1) << "<tr>\n";
  os << _indent() << _td_str(STAT_TAB_RAN, true) << "\n";
  os << _indent() << _td_str(int_to_str(
    owner().statistic_count(CNT_RAN))) << "\n";
  os << _indent(-1) << "</tr>\n";

  os << _indent(+1) << "<tr>\n";
  os << _indent() << _td_str(STAT_TAB_PASSED, true) << "\n";
  os << _indent() << _td_str(int_to_str(
    owner().statistic_count(CNT_PASSED))) << "\n";
  os << _indent(-1) << "</tr>\n";

  os << _indent(+1) << "<tr>\n";
  os << _indent() << _td_str(STAT_TAB_WARNINGS, true) << "\n";
  os << _indent() << _td_str(int_to_str(
    owner().statistic_count(CNT_WARNINGS))) << "\n";
  os << _indent(-1) << "</tr>\n";

  os << _indent(+1) << "<tr>\n";
  os << _indent() << _td_str(STAT_TAB_FAILED, true) << "\n";
  os << _indent() << _td_str(int_to_str(
    owner().statistic_count(CNT_FAILED)), false, fail_id) << "\n";
  os << _indent(-1) << "</tr>\n";

  os << _indent(+1) << "<tr>\n";
  os << _indent() << _td_str(STAT_TAB_ERRORS, true) << "\n";
  os << _indent() << _td_str(int_to_str(
    owner().statistic_count(CNT_ERRORS)), false, err_id) << "\n";
  os << _indent(-1) << "</tr>\n";

  os << _indent(+1) << "<tr>\n";
  os << _indent() << _td_str(STAT_TAB_SKIPPED, true) << "\n";
  os << _indent() << _td_str(int_to_str(
    owner().statistic_count(CNT_SKIPPED))) << "\n";
  os << _indent(-1) << "</tr>\n";

  os << _indent(+1) << "<tr>\n";
  os << _indent() << _td_str(STAT_TAB_DISABLED, true) << "\n";
  os << _indent() << _td_str(int_to_str(
    owner().statistic_count(CNT_DISABLED))) << "\n";
  os << _indent(-1) << "</tr>\n";

  os << _indent(+1) << "<tr>\n";
  os << _indent() << _td_str(STAT_TAB_PERC, true) << "\n";
  os << _indent() << _td_str(int_to_str(
    owner().statistic_count(CNT_PASS_RATE)) + "%", false, pass_id) << "\n";
  os << _indent(-1) << "</tr>\n";

  os << _indent(-1) << "</table>\n";

  // Test end time
  os << _indent(+1) << "<p>\n";

  if (m_decl_flag)
  {
    os << _indent() << "<strong>" << ABORTED_MSG << "</strong>";
    os << "<br/>\n";
    os << "<br/>\n";
  }

  os << _indent() << "<strong>" << END_TIME_LEADER << "</strong>";
  os << iso_time(owner().end_time()) << "<br/>\n";

  // Duration
  os << _indent() << "<strong>" << DURATION_LEADER << "</strong>";
  os << duration_str(owner().duration()) << "<br/>\n";

  os << _indent(-1) << "</p>\n";

  if (!m_decl_flag)
  {
    // Additional content
    os << _indent() << _page_anchor("fail_details") << "\n";
    os << _indent() << "<h2>" << FAIL_DETAILS << "</h2>\n";
    _build_trace(os, m_fail_trace, "h3");

    os << _indent() << _page_anchor("error_details") << "\n";
    os << _indent() << "<h2>" << ERROR_DETAILS << "</h2>\n";
    _build_trace(os, m_error_trace, "h3");

    if (verbose())
    {
      // Verbose only content
      os << _indent() << _page_anchor("pass_details") << "\n";
      os << _indent() << "<h2>" << PASS_DETAILS << "</h2>\n";
      _build_trace(os, m_pass_trace, "h3");
    }

    // Skipped/disabled tests
    os << _indent() << _page_anchor("skipped_tests") << "\n";
    os << _indent() << "<h2>" << SKIPPED_TESTS << "</h2>\n";

    // Build list of disabled tests
    std::size_t sk_sz = m_skipped_tests.size();

    if (sk_sz > 0)
    {
      os << _indent() << "<p>" << SKIPPED_NOTES << "</p>\n";

      os << _indent(+1) << "<ul>\n";

      for(std::size_t n = 0; n < sk_sz; ++n)
      {
        os << _indent() << "<li>" << m_skipped_tests[n] << "</li>\n";
      }

      os << _indent(-1) << "</ul>\n";
    }
    else
    {
      os << _indent() << "<p>" << NONE_STR << "</p>\n";
    }
  }

  os << _indent() << "<p style=\"font-size:smaller\">";
  os << "For information on " << TDOG_LIB_NAME << ", see: ";
  os << "<a href=\"" << TDOG_WEBSITE_URL << "\">" << TDOG_WEBSITE_NAME << "</a>";
  os << "</p>\n";

  // Close body
  os << _indent(-1) << "</body>\n";

  // Close html
  os << _indent(-1) << "</html>\n";
}
//---------------------------------------------------------------------------
