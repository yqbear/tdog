//---------------------------------------------------------------------------
// PROJECT      : TDOG
// FILENAME     : runner.cpp
// COPYRIGHT    : Kuiper (c) 2016
// WEBSITE      : kuiper.zone
// LICENSE      : Apache 2.0
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// INCLUDES
//---------------------------------------------------------------------------
#include "tdog/runner.hpp"
#include "tdog/private/basic_test.hpp"
#include "tdog/private/suite_manager.hpp"
#include "basic_reporter.hpp"
#include "null_reporter.hpp"
#include "text_reporter.hpp"
#include "html_reporter.hpp"
#include "xml_reporter.hpp"
#include "test_list.hpp"
#include "util.hpp"

#include <iostream>
#include <fstream>
#include <stdexcept>

//---------------------------------------------------------------------------
// NON-CLASS
//---------------------------------------------------------------------------
using namespace tdog;

const char* IMMUTABLE_ERROR = "Cannot modify own runner within tests cases";

//---------------------------------------------------------------------------
// CLASS runner : PRIVATE MEMBERS
//---------------------------------------------------------------------------
bool runner::_exists(const std::string& s, const std::vector<std::string>& v) const
{
  // True if s in v
  for(std::size_t n = 0; n < v.size(); ++n)
  {
    if (v[n] == s) return true;
  }

  return false;
}
//---------------------------------------------------------------------------
std::vector<std::string> runner::_split_names(std::string names) const
{
  // Split separated name list. Will also
  // remove any default namespace prefix.
  const std::size_t NSZ = std::string(NSSEP).size();

  names = trim_str(names);
  names = str_replace(names, " ", ",");
  names = str_replace(names, ",,", ",");

  std::size_t pa = 0;
  std::size_t sz = names.size();

  std::string temp;
  std::vector<std::string> rslt;

  while(pa < sz)
  {
    std::size_t pb = names.find(",", pa);

    if (pb == std::string::npos)
    {
      pb = sz;
    }

    // Remove default namespace
    temp = names.substr(pa, pb - pa);
    if (temp != NSSEP && temp.substr(0, NSZ) == NSSEP)
    {
      temp.erase(0, NSZ);
    }

    rslt.push_back(temp);
    pa = pb + 1;
  }

  return rslt;
}
//---------------------------------------------------------------------------
basic_reporter* runner::_create_reporter(report_style_t style) const
{
  // Creates an appropriate concrete instance of a
  // report generator class. Should be destroyed with
  // a call to delete when finished with it. Note
  // that if reporting is disabled, with return
  // a silent dummy instance.
  switch(style)
  {
    case RS_TEXT_SUMMARY: return new text_reporter(this, false);
    case RS_TEXT_VERBOSE: return new text_reporter(this, true);
    case RS_HTML_SUMMARY: return new html_reporter(this, false);
    case RS_HTML_VERBOSE: return new html_reporter(this, true);
    case RS_XML: return new xml_reporter(this, true);
    default: return new null_reporter(this);
  }
}
//---------------------------------------------------------------------------
// CLASS runner : PROTECTED MEMBERS
//---------------------------------------------------------------------------
template <typename T>
bool runner::_run_parse(int argc, const T* argv[],
  std::string& run_out, std::string& dis_out) const
{
  // Parse, and provide out as std::string.
  // Returns true if should run.
  run_out.clear();
  dis_out.clear();
  bool run_flag = false;
  bool dis_flag = false;

  for(int n = 0; n < argc; ++n)
  {
    if (argv[n] != 0)
    {
      std::string arg = trim_str(wide_to_str(argv[n]));

      if (arg == "--trun" || arg == "/trun" || arg == "/trun:")
      {
        run_flag = true;
        dis_flag = false;
        continue;
      }
      if (arg == "--trall" || arg == "/trall")
      {
        run_out = "*";
        run_flag = true;
        dis_flag = false;
        continue;
      }
      else
      if (arg == "--tdis" || arg == "/tdis")
      {
        dis_flag = true;
        run_flag = false;
        continue;
      }
      else
      if (arg.substr(0, 6) == "/trun:")
      {
        // Special case - no coninue
        run_flag = true;
        dis_flag = false;
        arg.erase(0, 6);
      }
      else
      if (arg.substr(0, 6) == "/tdis:")
      {
        // Special case
        run_flag = false;
        dis_flag = true;
        arg.erase(0, 6);
      }

      if (!arg.empty())
      {
        if (arg.substr(0, 1) == "-" || arg.substr(0, 1) == "/")
        {
          // Ignore unknown parameters
          run_flag = false;
          dis_flag = false;
        }
        else
        if (run_flag)
        {
          if (!run_out.empty()) run_out += " ";
          run_out += arg;
        }
        else
        if (dis_flag)
        {
          if (!dis_out.empty()) dis_out += " ";
          dis_out += arg;
        }
      }
    }
  }

  // Tidy up by pushing result into list
  // maker and rebuilding. Not strictly
  // necessary but it will make testing easier.
  std::vector<std::string> temp;

  if (!dis_out.empty())
  {
    temp = _split_names(dis_out);
    dis_out.clear();

    for(std::size_t n = 0; n < temp.size(); ++n)
    {
      if (n != 0) dis_out += ",";
      dis_out += temp[n];
    }
  }

  if (!run_out.empty())
  {
    temp = _split_names(run_out);
    run_out.clear();

    for(std::size_t n = 0; n < temp.size(); ++n)
    {
      if (n != 0) run_out += ",";
      run_out += temp[n];
    }

    return true;
  }

  return false;
}
//---------------------------------------------------------------------------
// CLASS runner : PUBLIC MEMBERS
//---------------------------------------------------------------------------
runner::runner(const std::string& platos)
{
  // Default
  m_immutable = false;
  m_list_ptr = new test_list();

  clear(true);

  if (!platos.empty())
  {
    m_platos = platos;
  }
#if defined(TDOG_PLATOS)
  else
  {
    m_platos = TDOG_PLATOS;
  }
#endif
}
//---------------------------------------------------------------------------
runner::runner(report_style_t def_style, const std::string& platos)
{
  // Default
  m_immutable = false;
  m_list_ptr = new test_list();

  clear(true);
  m_report_style = def_style;

  if (!platos.empty())
  {
    m_platos = platos;
  }
#if defined(TDOG_PLATOS)
  else
  {
    m_platos = TDOG_PLATOS;
  }
#endif
}
//---------------------------------------------------------------------------
runner::~runner()
{
  delete m_list_ptr;
}
//---------------------------------------------------------------------------
std::string runner::project_name() const
{
  // Returns the test project name.
  return m_project_name;
}
//---------------------------------------------------------------------------
void runner::set_project_name(const std::string& name)
{
  // Allows the setting of a name for the test project.
  if (m_immutable) throw std::logic_error(IMMUTABLE_ERROR);

  m_project_name = trim_str(name);
}
//---------------------------------------------------------------------------
std::string runner::project_version() const
{
  // Returns the user supplied version string for the test project.
  return m_project_version;
}
//---------------------------------------------------------------------------
void runner::set_project_version(const std::string& version)
{
  // Sets the version string of the project under test.
  if (m_immutable) throw std::logic_error(IMMUTABLE_ERROR);

  m_project_version = trim_str(version);
}
//---------------------------------------------------------------------------
std::string runner::project_desc() const
{
  // Returns the test project description.
  return m_project_desc;
}
//---------------------------------------------------------------------------
void runner::set_project_desc(const std::string& desc)
{
  // Sets the description string of the project under test.
  if (m_immutable) throw std::logic_error(IMMUTABLE_ERROR);

  m_project_desc = trim_str(desc);
}
//---------------------------------------------------------------------------
void runner::clear(bool all)
{
  // Clears all tests registered with the runner.
  // We do not clear platos
  if (m_immutable) throw std::logic_error(IMMUTABLE_ERROR);

  m_start_time = 0;
  m_end_time = 0;
  m_run_rslt = RAN_NONE;

  if (all)
  {
    // Clear everything
    m_list_ptr->clear();
    m_has_suites = false;
    m_decl_flag = false;
    m_decl_errors.clear();
    m_project_name.clear();
    m_project_version.clear();
    m_project_desc.clear();
    m_report_charset.clear();
    m_html_stylesheet.clear();
    m_text_report_break_width = 65;
    m_report_style = RS_TEXT_SUMMARY;
    m_sorted = false;
    m_reports_out.clear();
    m_global_time_limit = 0;
    m_global_time_warn = 0;
  }
  else
  {
    // Clear only last results
    for(std::size_t n = 0; n < m_list_ptr->size(); ++n)
    {
      m_list_ptr->get(n)->clear_results();
    }
  }
}
//---------------------------------------------------------------------------
bool runner::register_test(basic_test* tc)
{
  // Register the test case with this class.
  if (m_immutable) throw std::logic_error(IMMUTABLE_ERROR);

  bool rslt = true;
  std::string name = tc->full_name();
  std::string suite = tc->suite_name();

  if (name.empty())
  {
    // This should never happen,
    // but we can't allow it anyway.
    rslt = false;
    m_decl_flag = true;
    std::string temp = "Cannot register a TDOG test case with no name";

    if (!tc->file_location().empty())
    {
      temp += "\nin file: " + tc->file_location();
    }

    m_decl_errors.push_back(temp);
  }

  if (rslt && exists(name))
  {
    // Can only occur with if register_test()
    // explicitly called twice with same test.
    rslt = false;
    m_decl_flag = true;

    std::string temp = "Naming conflict.\n";
    temp += "A TDOG test case of name '" + name + "' already exists.";

    if (!tc->file_location().empty())
    {
      temp += "\nTest is re-declared in file: " + tc->file_location();
    }

    m_decl_errors.push_back(temp);
  }

  if (rslt)
  {
    m_list_ptr->add(tc);

    // Unlikely TC may have been disabled by another runner,
    // but happens in self-test. Make sure it is initially enabled.
    tc->set_enabled(true);

    // Flag if suites are used
    m_has_suites |= !suite.empty();
  }

  return rslt;
}
//---------------------------------------------------------------------------
std::vector<std::string> runner::declaration_errors(bool lf) const
{
  // Returns a list of test declaration erros.

  // Make copy, as we may need to add to it here.
  std::vector<std::string> rslt = m_decl_errors;

  // Start time will be non-zero if either the test
  // is running, or has been run. This means the test
  // registration phase has finished, and we can check
  // matching suite open/closure statements.
  if (m_start_time > 0)
  {
    if (suite_manager::counter() > 0)
    {
      // Missing closure statement
      m_decl_flag = true; // <- mutable
      rslt.push_back("Mismatching TDOG suite declaration and closure \
statement(s).\nThe suite name '" + suite_manager::current() + "' has \
no correctly\nmatching TDOG_CLOSE_SUITE statement.");
    }
    else
    if (suite_manager::counter() < 0)
    {
      // Suite closure statement without matching open
      m_decl_flag = true; // <- mutable
      rslt.push_back("Mismatching TDOG suite declaration and closure \
statement(s).\nThere appears to be an extra TDOG_CLOSE_SUITE \
statement somewhere.");
    }
  }

  // Get rid of line-feeds
  for(std::size_t n = 0; n < rslt.size() && !lf; ++n)
  {
    rslt[n] = str_replace(rslt[n], "\n", " ");
  }

  return rslt;
}
//---------------------------------------------------------------------------
bool runner::exists(const std::string& name) const
{
  // Returns whether the test case with the given name exists.
  return (m_list_ptr->find(name, false) != 0);
}
//---------------------------------------------------------------------------
bool runner::has_suites() const
{
  // Returns true if there are more than one test suites in the runner.
  return m_has_suites;
}
//---------------------------------------------------------------------------
int runner::set_enabled(const std::string& names, bool flag)
{
  // Allows list of tests with the given name list
  // to be disabled (or re-enabled) in the test run.
  if (m_immutable) throw std::logic_error(IMMUTABLE_ERROR);

  int rslt = 0;
  std::vector<std::string> list = _split_names(names);

  std::size_t l_sz = list.size();
  std::size_t t_sz = m_list_ptr->size();

  for(std::size_t t = 0; t < t_sz; ++t)
  {
    basic_test* tc = m_list_ptr->get(t);

    for(std::size_t l = 0; l < l_sz; ++l)
    {
      if (tc->name_equals(list[l], false))
      {
        if (tc->set_enabled(flag)) ++rslt;
      }
    }
  }

  return rslt;
}
//---------------------------------------------------------------------------
std::vector<std::string>& runner::enumerate_test_names(
  std::vector<std::string>& rslt_out) const
{
  // Enumerates the names of all test cases registered in the runner.
  rslt_out.clear();

  // Sort
  test_list temp = *m_list_ptr;
  temp.sort(m_sorted);

  for(std::size_t n = 0; n < temp.size(); ++n)
  {
    rslt_out.push_back(temp.get(n)->full_name());
  }

  return rslt_out;
}
//---------------------------------------------------------------------------
std::vector<std::string>& runner::enumerate_suite_names(
  std::vector<std::string>& rslt_out) const
{
  // Enumerates the names of all test suites registered in the runner.
  rslt_out.clear();

  // Sort
  std::string s;
  test_list temp = *m_list_ptr;
  temp.sort(m_sorted);

  for(std::size_t n = 0; n < temp.size(); ++n)
  {
    s = temp.get(n)->suite_name();

    if (!_exists(s, rslt_out))
    {
      rslt_out.push_back(s);
    }
  }

  return rslt_out;
}
//---------------------------------------------------------------------------
bool runner::sorted() const
{
  // Returns whether the run order is sorted or not.
  return m_sorted;
}
//---------------------------------------------------------------------------
void runner::set_sorted(bool sort)
{
  // Sets whether the run order is sorted or not.
  if (m_immutable) throw std::logic_error(IMMUTABLE_ERROR);

  m_sorted = sort;
}
//---------------------------------------------------------------------------
int runner::run(const std::string& names, bool throw_declerr)
{
  // Run tests
  if (m_immutable) throw std::logic_error(IMMUTABLE_ERROR);

  // Do we have an error?
  // Mast call after set start time, otherwise the
  // result might not be complete. See implementation.
  std::vector<std::string> declerrs = declaration_errors();
  bool decl_ok = (declerrs.size() == 0);

  if (throw_declerr && !decl_ok)
  {
    // Throw first message
    throw std::logic_error(declerrs[0].c_str());
  }

  int run_cnt = 0;
  int fail_cnt = 0;
  std::vector<std::string> name_list = _split_names(names);

  // Create reporter
  basic_reporter* reporter = _create_reporter(m_report_style);

  // Zero results
  clear(false);

  // Initialize timer
  m_start_time = msec_time();

  try
  {
    // Copy into temporary sorted test list
    test_list tlist = *m_list_ptr;
    tlist.sort(m_sorted);

    std::size_t t_sz = tlist.size();
    std::size_t l_sz = name_list.size();

    // Write report header
    reporter->gen_start(std::cout);

    // Loop through all tests if no error
    for(std::size_t t = 0; t < t_sz && decl_ok; ++t)
    {
      bool listed = false;
      basic_test* tc = tlist.get(t);

      // Determine whether this test is in the list
      for(std::size_t n = 0; n < l_sz; ++n)
      {
        listed = (tc->name_equals(name_list[n], false));
        if (listed) break;
      }

      // Run this test?
      if (listed)
      {
        // Set immutable flag.
        // This prevent tests modifying the
        // global runner (accidentally).
        m_immutable = true;

        // RUN TEST
        tc->run(m_global_time_limit, m_global_time_warn);

        // Clear immutable
        m_immutable = false;

        // Increment run counter
        if (tc->helper().has_ran()) ++run_cnt;

        // Failed or error
        if (tc->helper().has_failed()) ++fail_cnt;

        // Write report
        reporter->gen_test(std::cout, tc);

        // Skip tests in suite if setup() failed
        if (tc->helper().has_failed() && tc->is_setup())
        {
          std::string sname = tc->suite_name() + NSSEP + "*";

          for(std::size_t k = t + 1; k < t_sz; ++k)
          {
            if (tlist.get(k)->name_equals(sname, false))
            {
              tlist.get(k)->set_skipped();
            }
          }
        }
      }
    }

    // Mark end time
    m_end_time = msec_time();

    // Write report footer
    reporter->gen_end(std::cout);

    // Generate additional reports
    for(std::size_t n = 0; n < m_reports_out.size(); ++n)
    {
      std::ofstream fs;
      fs.open(m_reports_out[n].fileout.c_str());

      if (fs.is_open())
      {
        generate_report(fs, m_reports_out[n].style);
        fs.close();
      }
    }

  }
  catch(...)
  {
    // Unexpected exception
    // Shouldn't happen, as the basic_test::run() method
    // will catch. It's a problem in our code if occurs.
    m_immutable = false;
    delete reporter;
    std::cout << "\nABORTED - Unexpected exception in test library. Sorry.\n";
    throw;
  }

  // Delete reporter
  delete reporter;

  // Set result value
  if (run_cnt == 0) m_run_rslt = RAN_NONE;
  else m_run_rslt = fail_cnt;

  return m_run_rslt;
}
//---------------------------------------------------------------------------
int runner::run_cmdline(int argc, char* argv[], bool def_run)
{
  // Run the test according to the given command line arguments.
  if (m_immutable) throw std::logic_error(IMMUTABLE_ERROR);

  std::string run_list, dis_list;

  bool parse_ok = _run_parse<char>(argc,
    const_cast<const char**>(argv),
    run_list, dis_list);

  if (parse_ok || def_run)
  {
    if (!dis_list.empty()) set_enabled(dis_list, false);

    if (parse_ok) return run(run_list);
    else if (def_run) return run("*");
  }

  return RAN_NONE;
}
//---------------------------------------------------------------------------
int runner::run_cmdline(int argc, wchar_t* argv[], bool def_run)
{
  // Overload of above
  if (m_immutable) throw std::logic_error(IMMUTABLE_ERROR);

  std::string run_list, dis_list;

  bool parse_ok = _run_parse<wchar_t>(argc,
    const_cast<const wchar_t**>(argv),
    run_list, dis_list);

  if (parse_ok || def_run)
  {
    if (!dis_list.empty()) set_enabled(dis_list, false);

    if (parse_ok) return run(run_list);
    else if (def_run) return run("*");
  }

  return RAN_NONE;
}
//---------------------------------------------------------------------------
tdog::i64_t runner::global_time_limit() const
{
  // Returns the global time limit for test cases in ms.
  return m_global_time_limit;
}
//---------------------------------------------------------------------------
void runner::set_global_time_limit(tdog::i64_t ms)
{
  // Sets the global time limit for test cases in ms.
  if (m_immutable) throw std::logic_error(IMMUTABLE_ERROR);

  if (ms > 0) m_global_time_limit = ms;
  else m_global_time_limit = 0;
}
//---------------------------------------------------------------------------
tdog::i64_t runner::global_time_warning() const
{
  // Returns the global time limit for test cases in ms.
  return m_global_time_limit;
}
//---------------------------------------------------------------------------
void runner::set_global_time_warning(tdog::i64_t ms)
{
  // Sets the global time limit for test cases in ms.
  if (m_immutable) throw std::logic_error(IMMUTABLE_ERROR);

  if (ms > 0) m_global_time_warn = ms;
  else m_global_time_warn = 0;
}
//---------------------------------------------------------------------------
report_style_t runner::default_report() const
{
  // Returns the report style written to STDOUT during the test run.
  return m_report_style;
}
//---------------------------------------------------------------------------
void runner::set_default_report(report_style_t style)
{
  // Sets the report style written to STDOUT during the test run.
  if (m_immutable) throw std::logic_error(IMMUTABLE_ERROR);

  m_report_style = style;
}
//---------------------------------------------------------------------------
std::ostream& runner::generate_report(std::ostream& ro, report_style_t style) const
{
  // Generates a test report pertaining to the last test run and writes
  // it to the supplied output stream.
  if (m_immutable) throw std::logic_error(IMMUTABLE_ERROR);

  // Create reporter
  basic_reporter* reporter = _create_reporter(style);

  test_list temp = *m_list_ptr;
  temp.sort(m_sorted);

  try
  {
    // Write report header
    reporter->gen_start(ro);

    // Loop through all tests
    for(std::size_t n = 0; n < temp.size(); ++n)
    {
      reporter->gen_test(ro, temp.get(n));
    }

    // Write report footer
    reporter->gen_end(ro);
  }
  catch(...)
  {
    // Unexpected exception
    delete reporter;
    std::cout << "\nABORTED - Unknown exception in test library.\n";
    throw;
  }

  // Delete reporter
  delete reporter;

  return ro;
}
//---------------------------------------------------------------------------
void runner::add_report(report_style_t style, const std::string& filename)
{
  // Add report to list to be generated after the run.
  if (m_immutable) throw std::logic_error(IMMUTABLE_ERROR);

  if (style != RS_NONE && !filename.empty())
  {
    auto_report rep;
    rep.style = style;
    rep.fileout = filename;
    m_reports_out.push_back(rep);
  }
}
//---------------------------------------------------------------------------
std::string runner::report_charset() const
{
  // Returns the test report character set encoding.
  return m_report_charset;
}
//---------------------------------------------------------------------------
void runner::set_report_charset(const std::string& cs)
{
  // Sets the test report character set encoding.
  if (m_immutable) throw std::logic_error(IMMUTABLE_ERROR);

  m_report_charset = trim_str(cs);
}
//---------------------------------------------------------------------------
int runner::text_report_break_width() const
{
  // Returns the number of '-' characters used to create a 'break bar'
  // in the text style of a test log output.
  return m_text_report_break_width;
}
//---------------------------------------------------------------------------
void runner::set_text_report_break_width(int bw)
{
  // Sets the number of '-' characters used to create a 'break bar'
  // in the text style of a test log output.
  if (m_immutable) throw std::logic_error(IMMUTABLE_ERROR);

  if (bw < 0) bw = 0;
  if (bw > 500) bw = 500;
  m_text_report_break_width = bw;
}
//---------------------------------------------------------------------------
std::string runner::html_stylesheet() const
{
  // Gets the stylesheet name for HTML test reports.
  return m_html_stylesheet;
}
//---------------------------------------------------------------------------
void runner::set_html_stylesheet(const std::string& ss)
{
  // Sets the stylesheet name for HTML test reports.
  if (m_immutable) throw std::logic_error(IMMUTABLE_ERROR);

  m_html_stylesheet = trim_str(ss);
}
//---------------------------------------------------------------------------
tdog::i64_t runner::statistic_count(stat_count_t type, const std::string& name) const
{
  // Returns a statistical result for the last test run.

  // If declaration error, short cut to provide
  // error count, otherwise result would be zero.
  if (m_decl_flag && type == CNT_ERRORS)
  {
    return static_cast<int>(declaration_errors().size());
  }

  // Loop through tests and perform count
  int run_cnt = 0;
  tdog::i64_t rslt = 0;

  for(std::size_t n = 0; n < m_list_ptr->size(); ++n)
  {
    basic_test* tc = m_list_ptr->get(n);

    if (tc->name_equals(name, false))
    {
      // Get status
      status_t status = tc->helper().status();

      bool has_ran = tc->helper().has_ran();
      if (has_ran) ++run_cnt;

      switch(type)
      {
        case CNT_TOTAL:
          ++rslt;
          break;
        case CNT_RAN:
          if (has_ran) ++rslt;
          break;
        case CNT_PASSED:
        case CNT_PASS_RATE:
          if (tc->helper().has_passed()) ++rslt;
          break;
        case CNT_FAILED:
          if (status == TS_FAILED) ++rslt;
          break;
        case CNT_ERRORS:
          if (status == TS_ERROR) ++rslt;
          break;
        case CNT_SKIPPED:
          if (status == TS_SKIPPED) ++rslt;
          break;
        case CNT_DISABLED:
          if (status == TS_DISABLED) ++rslt;
          break;
        case CNT_ASSERT_TOTAL:
          rslt += tc->helper().assert_cnt();
          break;
        case CNT_ASSERT_FAILED:
          rslt += tc->helper().fail_cnt();
          break;
        case CNT_WARNINGS:
          if (status == TS_PASS_WARN) ++rslt;
          break;
        case CNT_DURATION:
          rslt += tc->helper().duration(); // <- why result needs to be 64 bit
          break;
        default:
          // Error
          return -1;
      }
    }
  }

  // Calculate pass rate
  if (type == CNT_PASS_RATE)
  {
    if (run_cnt > 0) return (100 * rslt) / run_cnt;
    else return 0;
  }

  return rslt;
}
//---------------------------------------------------------------------------
status_t runner::test_status(const std::string& name) const
{
  // Returns the result state of a given test case.
  if (m_decl_flag || declaration_errors().size() != 0)
  {
    // Test aborted
    return TS_DECL_ERROR;
  }

  // Otherwise
  basic_test* tc = m_list_ptr->find(name, true);

  if (tc != 0)
  {
    return tc->helper().status();
  }

  return TS_NOT_EXIST;
}
//---------------------------------------------------------------------------
tdog::i64_t runner::start_time() const
{
  // Returns the date/time when the test run was started.
  return m_start_time;
}
//---------------------------------------------------------------------------
tdog::i64_t runner::end_time() const
{
  // Returns the date/time when the last test run completed.
  return m_end_time;
}
//---------------------------------------------------------------------------
tdog::i64_t runner::duration() const
{
  // Returns the total duration of the test run in seconds.
  if (m_start_time == 0) return -1;
  else if (m_end_time == 0) return msec_time() - m_start_time;
  return m_end_time - m_start_time;
}
//---------------------------------------------------------------------------

