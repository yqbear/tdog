//---------------------------------------------------------------------------
// PROJECT      : TDOG
// FILENAME     : runner.hpp
// COPYRIGHT    : Andy Thomas (c) 2016
// WEBSITE      : bigangrydog.com
// LICENSE      : Apache 2.0
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// HEADER GUARD
//---------------------------------------------------------------------------
#ifndef TDOG_RUNNER_H
#define TDOG_RUNNER_H

//---------------------------------------------------------------------------
// INCLUDES
//---------------------------------------------------------------------------
#include "types.hpp"
#include "private/basic_test.hpp"

#include <string>
#include <vector>
#include <map>
#include <iostream>

//---------------------------------------------------------------------------
// DOCUMENTATION
//---------------------------------------------------------------------------
/**
 * \file
 * \brief This file provides the tdog::runner class.
 * \details Typically, there is no need to use the runner class directly as macros,
 * which access the runner::global() singleton, provide for this. The runner class
 * can be used directly to access lower level functionality, or to have multiple
 * runners. However, there will be a greater degree of support for backward
 * compatibility with macros in the event of future updates.
 * \sa tdog::runner, TDOG_GET_TCPTR()
*/

//---------------------------------------------------------------------------
// DECLARATIONS
//---------------------------------------------------------------------------

/**
 * \brief Namespace for TDOG implementation machinery.
 */
namespace tdog {

// Forward declarations
class basic_test;
class basic_reporter;
class test_list;

//---------------------------------------------------------------------------
// CLASS runner
//---------------------------------------------------------------------------
/**
 * \class runner
 * \brief The runner class is used to run test cases, collate results and create
 * reports.
 * \details Typically, there is no need to use the runner class directly as macros,
 * which access the runner::global() singleton, provide for this. The runner class
 * can be used directly to access lower level functionality, or to have multiple
 * runners. However, there will be a greater degree of support for backward
 * compatibility with macros in the event of future updates.
  * \sa runner::global(), TDOG_GET_TCPTR()
* */
class runner
{
  private:

  std::string m_platos;
  std::string m_project_name;
  std::string m_project_version;
  std::string m_project_desc;
  report_style_t m_report_style;
  std::string m_report_charset;
  std::string m_html_stylesheet;
  bool m_sorted;
  int m_text_report_break_width;
  tdog::i64_t m_start_time;
  tdog::i64_t m_end_time;
  bool m_has_suites;
  tdog::i64_t m_global_time_limit;
  tdog::i64_t m_global_time_warn;
  int m_run_rslt;
  bool m_immutable;
  mutable bool m_decl_flag;
  std::vector<std::string> m_decl_errors;

  // Use pointer so we don't need to
  // expose test_list include file.
  test_list* m_list_ptr;

  struct auto_report
  {
    report_style_t style;
    std::string fileout;
  };
  std::vector<auto_report> m_reports_out;

  bool _exists(const std::string& s, const std::vector<std::string>& v) const;
  std::vector<std::string> _split_names(std::string names) const;
  basic_reporter* _create_reporter(report_style_t style) const;

  // Allowed to create new, but not copy.
  runner(runner const&);
  runner& operator=(runner const&);

  protected:

  // Make this protected, because we
  // want to unit test it in a fixture
  template <typename T> bool _run_parse(int argc, const T* argv[],
    std::string& run_out, std::string& dis_out) const;

  public:

/**
 * @name Class instantiation
 * @{
 */

/**
 * \brief Default constructor.
 * \details The "platos" parameter can be used to specify a custom "platform OS" value,
 * i.e. "Fedora". If omitted, it will default to the TDOG_PLATOS value with which the
 * library was built. This would normally be defined automatically.
 * \param[in] platos Custom "platform OS" value.
 */
  runner(const std::string& platos = std::string());

/**
 * \brief Constructor with default report setting.
 * \param[in] def_style Report style written to STDOUT.
 * \param[in] platos Custom "platform OS" value.
 */
  runner(report_style_t def_style, const std::string& platos = std::string());

/**
 * \brief Destructor.
 */
  virtual ~runner();

/**
 * \brief Singleton access.
 * \details Returns a reference to a global runner instance.
 *
 * Example:
 *
 * \code
 * // Run all tests
 * tdog::runner::global().run();
 * \endcode
 *
 * \return Reference to global instance
 */
  static runner& global()
  {
  // Singleton access
#if defined(TDOG_PLATOS)
    static runner storage(TDOG_PLATOS); // <- must be a string literal
#else
    static runner storage;
#endif
    return storage;
  }

/**
 * @}
 * @name Project information
 * @{
*/

/**
 * \brief Returns the user supplied name of the project under test.
 * \details The project name is an optional string which is displayed
 * in test report outputs.
 * \return Project name string
 * \sa set_project_name(), project_version(), project_desc()
 */
  std::string project_name() const;

/**
 * \brief Sets the name of the project under test.
 * \details The project name is an optional string which is displayed
 * in test report outputs.
 *
 * Example:
 * \code
 * tdog::runner().global().set_project_name("smppd");
 * \endcode
 * \param[in] name Project name
 * \sa project_name(), set_project_version(), set_project_desc()
 */
  void set_project_name(const std::string& name);

/**
 * \brief Returns the user supplied version string for the test project.
 * \details The project version is an optional string which is displayed
 * in test report outputs.
 * \return Project version string
 * \sa set_project_version(), project_name(), project_desc()
 */
  std::string project_version() const;

/**
 * \brief Sets the version string of the project under test.
 * \details The project version is an optional string which is displayed
 * in test report outputs.
 *
 * Example:
 * /code
 * tdog::runner().global().set_project_version("3.1.6");
 * /endcode
 * \param[in] version Project version string
 * \sa project_version(), set_project_name(), set_project_desc()
 */
  void set_project_version(const std::string& version);

/**
 * \brief Returns the user supplied description string for the test project.
 * \details The project description is an optional string which is displayed
 * in test report outputs.
 * \return Project description string
 * \sa set_project_desc(), project_name(), project_version()
 */
  std::string project_desc() const;

/**
 * \brief Sets a descriptive string of the project under test.
 * \details The project description is an optional string which is displayed
 * in test report outputs.
 * \param[in] desc Project description string
 * \sa project_desc(), set_project_name(), set_project_version()
 */
  void set_project_desc(const std::string& desc);

/**
 * \brief Clears results, and optionally the project name, version,
 * reporting settings and all tests.
 * \details Clears test results. If "all" is true,
 * the test list is emptied, and all settings are set back to their default values.
 * \param[in] all Clear all
 */
  void clear(bool all = true);

/**
 * @}
 * @name Managing tests
 * @{
 */

/**
 * \brief Explicitly registers a test case instance with the runner.
 * \details Normally there is no need to call this method, as test cases
 * automatically register themselves with the global runner object
 * when they are instantiated.
 *
 * \details When you declare a test case with TDOG_TEST_CASE() or TDOG_TEST_FIXTURE(),
 * for example, a C++ class is instantiated and your assert statements will
 * form part of the class implementation. You would not normally need to access
 * the underlying class instance directly, however, as these objects "self-register"
 * with a global tdog::runner object which provides the means to run the
 * tests. However, if you wish to register test cases explicitly with a runner
 * yourself (rather than have them register themselves), then you will need
 * a mechanism by which to access to the underlying test instance, and
 * the TDOG_GET_TCPTR() macro provides this. The TDOG library uses this
 * in order to test itself, but it is not generally needed by client
 * applications.
 *
 * Below is an example of how to register a test case explicitly with the
 * runner using this macro. Note that in order to make tests non self-registering,
 * you must declare them inside the scope of TDOG_EXPLICIT_SUITE().
 *
 * \code
 * #include <tdog.hpp>
 *
 * TDOG_EXPLICIT_SUITE(suite_name)
 * {
 *   // Define test
 *   TDOG_TEST_CASE(some_test)
 *   {
 *     ...
 *   }
 *
 *   // Must close the suite
 *   TDOG_CLOSE_SUITE
 * }
 *
 *
 * int main(int argc, char **argv)
 * {
 *   // Explicitly register with non-global runner
 *   tdog::runner tr;
 *   tr.register_test(TDOG_GET_TCPTR(suite_name::some_test));
 *   ...
 * }
 * \endcode
 *
 * Tests within sub-suites of the TDOG_EXPLICIT_SUITE() will also
 * require explicit registration with a runner, irrespective of
 * whether the sub-suite is declared using TDOG_SUITE() or
 * TDOG_EXPLICIT_SUITE(). Tests in the global suite namespace cannot
 * be made "explicit", and will always self-register.
 *
 * The test case instance must exist for the lifetime of the runner class.
 * \param[in] tc Underlying test instance
 * \return true on success
 * \sa TDOG_GET_TCPTR(), declaration_errors()
 */
  bool register_test(basic_test* tc);

/**
 * \brief Returns a list of test registration declaration errors.
 * \details This is used by the reporters to detect any problems
 * with test declaration statements, such as misplaced TDOG_SUITE_CLOSE()
 * statements or conflicting test names. Note that misplaced suite closure
 * statements cannot be detected prior to test execution.
 *
 * The run will be aborted, and the test_status() will return TS_DECL_ERROR
 * if there are declaration errors. The list of declaration errors will be
 * displayed in test reports.
 *
 * The value is a vector of strings, which will have zero size if
 * no errors are detected. If "lf" is true, the messages may contain
 * formatting line-feed characters. If false, each message will
 * be a single line of text.
 * \param[in] lf Messages to contain line-feeds
 * \return const vector of strings
 * \sa register_test(), test_status()
 */
std::vector<std::string> declaration_errors(bool lf = false) const;

/**
 * \brief Returns whether a test case or suite with the
 * given name exists.
 * \details To discover whether a test exists, specify the
 * fully qualified test name. To determine if a suite exists,
 * append the suffix "::" to the suite name. Empty suites, i.e.
 * a suite declaration which contains no tests, is not considered
 * to exist.
 *
 * The name is case sensitive, and multiple name inputs are not supported
 * with this routine.
 * \param[in] name Fully qualified test case name
 * \return Boolean result
 * \sa enumerate_test_names(), enumerate_suite_names()
 */
  bool exists(const std::string& name) const;

/**
 * \brief Returns true if one or more tests were declared within a
 * named test suites.
 * \details This method will return false if all tests we declared outside
 * of TDOG_SUITE() scopes.
 * \return Boolean result
 * \sa exists()
 */
  bool has_suites() const;

/**
 * \brief Allows test cases and suites of tests matching the given name list
 * to be disabled (or re-enabled).
 * \details Disabled tests will not be executed by the test run.
 *
 * The input string may contain multiple names separated with a space or
 * comma (or both). Test case names should be fully qualified with its suite name.
 * If a test case is not part of a suite (i.e. it's in the default suite), then
 * the name may optionally be prefixed with the default namespace "::".
 *
 * Both test case and suite names may be specified, and will match according to the
 * following rules:
 *
 * - To match a single test, provide the fully qualified test name exactly
 * (i.e. "SUITE1::SUITE2::TESTNAME").
 * - To match all tests directly in suite, but not in any of its sub-suites,
 * use the suite name suffixed with "::". For example, "SUITE1::".
 * - To match all tests in a suite, including its sub-suites, suffix the name
 * input with "::*". For example, "SUITE1::*".
 * - To match everything, use "*" (or "::*").
 * .
 *
 * Example:
 *
 * \code
 * tdog::runner::global().set_enabled("TEST1, SUITE1::, SUITE2::TEST2", false);
 * \endcode
 *
 * Here, "TEST1" is a test case declared outside of a suite, and will be disabled.
 * Other tests in the default suite will not be affected. All tests directly under
 * "SUITE1" will be disabled, but not those under sub-suites of "SUITE1". Finally,
 * "TEST2" will be disabled in "SUITE2".
 *
 * To refer to the default suite, use "::" as the input. For example, the
 * following will disable all tests declared outside of any suite:
 *
 * \code
 * tdog::runner::global().set_enabled("::", false);
 * \endcode
 *
 * Names are case sensitive, and any name which does not exist will be ignored.
 * The result is the total number of test cases updated. Use test_status() to
 * discover the enabled state of a test.
 *
 * The diffence between specifically disabling a test and omitting it when calling
 * the run() method is that disabled tests will be noted in the test report,
 * whereas omitted tests will simply be ignored by the run call.
 * \param[in] names Seperated test names string
 * \param[in] flag Enabled flag
 * \return Number of test cases updated
 * \sa exists(), test_status()
 */
  int set_enabled(const std::string& names, bool flag);

/**
 * \brief Enumerates the fully qualified names of all test cases registered
 * with the runner.
 * \details The results are returned to "rslt_out", which
 * supplies a reference to a vector of std::string. The rslt_out array will
 * first be emptied before being re-populated with test case names.
 *
 * Test names are provided in the order in which they are executed.
 * \param[out] rslt_out Reference to a vector of strings
 * \return Reference to rslt_out
 * \sa enumerate_suite_names(), exists()
 */
  std::vector<std::string>& enumerate_test_names(
    std::vector<std::string>& rslt_out) const;

/**
 * \brief Enumerates the names of all test suites in the runner.
 * \details The results are returned to "rslt_out", which
 * supplies a reference to a vector of std::string. The rslt_out array will
 * first be emptied before being re-populated with test suite names.
 *
 * An empty string will be used to designate the default suit, i.e.
 * where test cases are declared outside of a test suite. Suite names
 * are provided in the order in which they will be executed.
 * \param[out] rslt_out Reference to vector of strings
 * \return Reference to rslt_out
 * \sa enumerate_test_names(), exists()
 */
  std::vector<std::string>& enumerate_suite_names(
    std::vector<std::string>& rslt_out) const;

/**
 * @}
 * @name Running tests
 * @{
 */

/**
 * \brief Returns whether the run order is fully sorted.
 * \details When running tests, suite names will always be sorted, with the default
 * suite first. Names are sorted according to an ordinal* comparison between their ASCII
 * lower case values, and if equal, their case sensitive values. This will sort
 * names with case variations together in a consistent way.
 *
 * By default, test names within suites will be executed in the order they are
 * registered with the runner, with a few exceptions:
 *
 * - Test names beginning with "setup" or "_setup" (case insensitive) will always
 * be executed at the beginning of the suite.
 * - Test names beginning with "teardown" or "_teardown" (case insensitive) will
 * always be executed at the end of the suite -- after any nested suites.
 * .
 *
 * If the sorted option is true prior to execution, test cases inside suites will
 * also be sorted during execution. It is good practice to ensure that tests do not
 * depend on the run order, other than for explicitly declared setup() and teardown()
 * operations.
 *
 * *In an ordinal sort, "setup2" will come after "setup10". Use additional zeros
 * to ensure numerical order, i.e. "setup02" and "setup10", if required.
 * \return Returns true for a fully sorted run order (default is false)
 * \sa set_sorted()
 */
  bool sorted() const;

/**
 * \brief Sets whether the run order is fully sorted.
 * \details When running tests, suite names will always be sorted, with the default
 * suite first. Names are sorted according to an ordinal* comparison between their ASCII
 * lower case values, and if equal, their case sensitive values. This will sort
 * names with case variations together in a consistent way.
 *
 * By default, test names within suites will be executed in the order they are
 * registered with the runner, with a few exceptions:
 *
 * - Test names beginning with "setup" or "_setup" (case insensitive) will always
 * be executed at the beginning of the suite.
 * - Test names beginning with "teardown" or "_teardown" (case insensitive) will
 * always be executed at the end of the suite -- after any nested suites.
 * .
 *
 * If the "full_sort" option is true prior to execution, test cases inside suites will
 * also be sorted during execution. It is good practice to ensure that tests do not
 * depend on the run order, other than for explicitly declared setup() and teardown()
 * operations.
 *
 * *In an ordinal sort, "setup2" will come after "setup10". Use additional zeros
 * to ensure numerical order, i.e. "setup02" and "setup10", if required.
 * \param[in] full_sort Set true for a fully sorted run order (default is false)
 * \sa sorted()
 */
  void set_sorted(bool full_sort);

/**
 * \brief Run the tests matching the given list of test case and suite name(s).
 * \details The input string may contain multiple names separated with a space or
 * comma (or both). Test case names should be qualified with the suite name, and
 * are case sensitive. If a test case is not part of a suite (i.e. it's in the
 * default suite), then the name may optionally be prefixed with the default
 * namespace "::". Any name referring to a test or suite which does not exist
 * will be ignored.
 *
 * Both test case and suite names may be specified, and will match according to the
 * following rules:
 *
 * - To match a single test, provide the fully qualified test name exactly
 * (i.e. "SUITE1::SUITE2::TESTNAME").
 * - To match all tests directly in suite, but not in any of its sub-suites,
 * use the suite name suffixed with "::". For example, "SUITE1::".
 * - To match all tests in a suite, including its sub-suites, suffix the name
 * input with "::*". For example, "SUITE1::*".
 * - To match everything, use "*" (or "::*").
 * .
 *
 * To execute all tests (in all suites), specify "*" as the input, as follows:
 *
 * \code
 * // Run all tests
 * tdog::runner::global().run("*");
 * \endcode
 *
 * To run a single test case in the default suite, for example, call:
 *
 * \code
 * // Run single test
 * tdog::runner::global().run("TESTNAME1");
 * \endcode
 *
 * To specify all tests in the default test suite (i.e. those declared outside of
 * a suite) use "::". For example, the following will run the default suite,
 * plus a single test in another suite:
 *
 * \code
 * // Run single test
 * tdog::runner::global().run("::, SUITE1::TESTNAME2");
 * \endcode
 *
 * Test cases which have been disabled will not be executed, even if they
 * are explicity specified in the "names" string.  By default, tests are run
 * in the order they are registered with the runner, which will usually be
 * the order in which they are declared and included, with a few exceptions:
 *
 * - The default suite (i.e. tests declared outside of TDOG_SUITE()), will
 * always be executed before other suites.
 * - A test name beginning with "setup" or "_setup" (case insensitive) will always
 * be executed at the beginning of the suite.
 * - A test name beginning with "teardown" or "_teardown" (case insensitive) will
 * always be executed at the end of the suite.
 * .
 *
 * The run order may (optionally) be fully sorted. To do this, call
 * set_sorted(true) prior to execution. In this case, both suites and tests
 * will be executed according to their ordinal sort order, except for
 * setup/teardown named "tests" as described above.
 *
 * The return value gives the number of test cases which either failed or raised
 * an error. The result will be 0 if all test cases ran successfully without
 * asserting failures or test errors. If, however, no tests were run,
 * the return value will be tdog::RAN_NONE(-1), rather than 0. This distinguishes
 * a successful test run from one that doesn't actually test anything.
 * This may occur, for example, if all test cases are disabled, if no
 * tests have been registered with the runner, or the name input refers
 * to non-existent tests.
 * \param[in] names Separated test or suite names
 * \param[in] throw_declerr Throws std::logic_error if declaration errors detected
 * \return Number of test failures, or RUN_NONE (-1) if no tests performed
 * \sa run_cmdline(), test_status(), statistic_count(), declaration_errors()
 */
  int run(const std::string& names = "*", bool throw_declerr = false);

 /**
  * \brief Run the tests according to the given command line arguments.
  * \details This method takes the command line arguments provided by a main()
  * function, and runs the tests according to specification:
  *
  * \code
  * --trun <run test list> [--tdis <disable test list>]
  * \endcode
  *
  * To run all tests, the command "--trun *" should be given or,
  * alternatively "--trall". The use of "--tdis" to disable tests is optional.
  *
  * For example, the following will run all with two tests disabled:
  *
  * \code
  * --trun * --tdis test1,suite2::test2
  * \endcode
  *
  * Windows style input, with a command prefix of "/", is also accepted
  * and the following command is equivalent to that above:
  *
  * \code
  * /trun:* /tdis:test1,suite2::test2
  * \endcode
  *
  * This method is also overloaded with the wchar_t type for use with a
  * Windows main function.
  *
  * If "def_run" is true, all tests will execute by default if no "--trun"
  * or "--trall" command is found. In this case, tests may optionally be
  * disabled with the "--tdis" command alone.
  *
  * The return value gives the number of test cases which either failed or raised
  * an error. The result will be 0 if all test cases ran successfully without
  * asserting failures or test errors. If, however, no tests were run,
  * the return value will be tdog::RAN_NONE (-1), rather than 0. This distinguishes
  * a successful test run from one that doesn't actually test anything. In the event
  * that the argument list does not contain "-trun" or "-trall", the result will also
  * be tdog::RAN_NONE (-1) unless "def_run" is true.
  * \param[in] argc Argument count
  * \param[in] argv Pointer to array of c-strings from main()
  * \param[in] def_run Boolean flag, run tests by default.
  * \return Number of test failures, or RUN_NONE (-1) if no tests performed
  * \sa run()
  */
  int run_cmdline(int argc, char* argv[], bool def_run = false);

/**
  * \overload
  * \param[in] argc Argument count
  * \param[in] argv Pointer to array of c-strings from main()
  * \param[in] def_run Boolean flag, run tests by default.
  * \return Number of test failures, or RUN_NONE (-1) if no tests performed
  * \sa run()
  */
  int run_cmdline(int argc, wchar_t* argv[], bool def_run = false);

/**
 * \brief Returns the global time limit for test cases in milliseconds.
 * \details This is a global constraint on the maximum amount of time
 * that each test case can take to complete.
 *
 * If a test takes longer than this value, it will be deemed to
 * have failed. A value of 0, the default, disables the global time
 * constaint.
 * \return Time in milliseconds
 * \sa set_global_time_limit(), global_time_warning()
 */
  tdog::i64_t global_time_limit() const;

/**
 * \brief Sets the global time constaints for tests in milliseconds.
 * \details This is a global constraint on the maximum amount of time
 * that each test case may take to complete. If any test case takes
 * longer than this value, it will be deemed to have failed. This
 * macro should be called prior to running the tests. A value
 * of 0, the default, disables the global time constaint.
 *
 * If a test takes longer than this value, it will be deemed to
 * have failed. A value of 0, the default, disables the global time
 * constaint.
 * \param[in] ms Time in milliseconds
 * \sa global_time_limit(), set_global_time_warning()
 */
  void set_global_time_limit(tdog::i64_t ms);

/**
 * \brief Returns the global warning time for test cases in milliseconds.
 * \details  If a test takes longer than this value, a warning message
 * will be generated in the test reports. However, the test will not fail.
 *
 * It is possible to set both a warning time, and a longer global
 * time limit, for example.
 * \return Time in milliseconds
 * \sa set_global_time_warning(), global_time_limit()
 */
  tdog::i64_t global_time_warning() const;

/**
 * \brief Sets the global warning time for test cases in milliseconds.
 * \details  If a test takes longer than this value, a warning message
 * will be generated in the test reports. However, the test will not fail.
 *
 * It is possible to set both a warning time, and a longer global
 * time limit, for example.
 * \param[in] ms Time in milliseconds
 * \sa global_time_warning(), set_global_time_limit()
 */
  void set_global_time_warning(tdog::i64_t ms);

/**
 * @}
 * @name Generating test reports
 * @{
 */

/**
 * \brief Returns the report style written to STDOUT during the test run.
 * \details The result value will be one of:
 * - tdog::RS_NONE - Does nothing here
 * - tdog::RS_TEXT_SUMMARY - Human readable test summary
 * - tdog::RS_TEXT_VERBOSE - Human readable verbose test report
 * - tdog::RS_HTML_SUMMARY - HTML summary report
 * - tdog::RS_HTML_VERBOSE - HTML verbose report
 * - tdog::RS_XML - JUnit compatible XML report
 * .
 * The default value is RS_TEXT_SUMMARY.
 *
 * Non-verbose reports contain only summary information pertaining to tests which pass,
 * but further details are provided for those tests which fail. Verbose reports, on the
 * other hand, contain maximum trace information. For verbose XML, the report will
 * contain test case output information within the "<system-out>" element. HTML reports
 * are suitable for viewing in a browser.
 * \return Stdout reporting style
 * \sa set_default_report(), add_report()
 */
  report_style_t default_report() const;

/**
 * \brief Sets the report style written to STDOUT during the test run.
 * \details If required, this method should called prior to running the tests.
 * The report "style" parameter can be one of the following tdog::report_style_t
 * values:
 * - tdog::RS_NONE - Does nothing here
 * - tdog::RS_TEXT_SUMMARY - Human readable test summary
 * - tdog::RS_TEXT_VERBOSE - Human readable verbose test report
 * - tdog::RS_HTML_SUMMARY - HTML summary report
 * - tdog::RS_HTML_VERBOSE - HTML verbose report
 * - tdog::RS_XML - JUnit compatible XML report
 * .
 * The default value is RS_TEXT_SUMMARY.
 *
 * Non-verbose reports contain only summary information pertaining to tests which pass,
 * but further details are provided for those tests which fail. Verbose reports, on the
 * other hand, contain maximum trace information. For verbose XML, the report will
 * contain test case output information within the "<system-out>" element. HTML reports
 * are suitable for viewing in a browser.
 *
 * Example:
 *
 * \code
 * #include <tdog.hpp>
 * ...
 *
 * int main(int argc, char **argv)
 * {
 *   // Use verbose report
 *   tdog::runner::global().set_default_report(tdog::RS_TEXT_VERBOSE);
 *
 *   // Run all tests
 *   return tdog::runner::global().run();
 * }
 * \endcode
 *
 * You can use the tdog::RS_NONE to silence TDOG's own output to STDOUT, but this
 * won't affect output generated by any printf() or std::cout statements within
 * test implementation code. It you specify a non-text report style, you would typically
 * want to re-direct the process output to file, but see also the add_report() method.
 * \param[in] style Report style
 * \sa default_report(), add_report()
 */
  void set_default_report(report_style_t style);

/**
 * \brief Generates a test report pertaining to the test run performed and
 * writes it to the supplied output stream.
 * \details The report is generated according to the style given, and
 * is useful in writing test reports to a stream, other than STDOUT or a
 * a file.
 *
 * The report style parameter can be one of:
 * - tdog::RS_NONE - Does nothing here
 * - tdog::RS_TEXT_SUMMARY - Human readable test summary
 * - tdog::RS_TEXT_VERBOSE - Human readable verbose test report
 * - tdog::RS_HTML_SUMMARY - HTML summary report
 * - tdog::RS_HTML_VERBOSE - HTML verbose report
 * - tdog::RS_XML - JUnit compatible XML report
 * .
 * Non-verbose reports contain only summary information pertaining to tests which pass,
 * but further details are provided for those tests which fail. Verbose reports, on the
 * other hand, contain maximum trace information. For verbose XML, the report will
 * contain test case output information within the "<system-out>" element. HTML reports
 * are suitable for viewing in a browser.
 *
 * Unlike add_report(), which should be called prior to running the tests, this
 * method should be used after the test run to extract data.
 *
 * Here's an example of where a text style report is written to the STDOUT as
 * the tests are executed, and an additional HTML report is written to a string
 * stream on completion.
 *
 * \code
 * #include <tdog.hpp>
 * #include <sstream>
 * ...
 *
 * int main(int argc, char **argv)
 * {
 *   // Set verbose STDOUT report
 *   tdog::runner::global().set_default_report(tdog::RS_TEXT_VERBOSE);
 *
 *   // Run all tests
 *   int rslt = tdog::runner::global().run();
 *
 *   // Generate HTML report file
 *   std::stringstream s;
 *   tdog::runner::global().generate_report(s, tdog::RS_HTML_SUMMARY);
 *   ...
 *   return rslt;
 * }
 * \endcode
 *
 * If the style RS_NONE is used, this method does nothing.
 * \param[in,out] ro Output stream
 * \param[in] style Report style
 * \return Reference to output stream
 * \sa set_default_report(), add_report()
 */
  std::ostream& generate_report(std::ostream& ro, report_style_t style) const;

/**
 * \brief Causes an additional report to be written to file automatically
 * at the conclusion of the test run.
 * \details During the test run, a text summary report is normally written to
 * STDOUT as tests are performed. This method can be used to specify that an additional
 * report should be written to file when the test run has finished. The "filename"
 * parameter specifies the name of the file to write, while the report style parameter
 * can be one of the following tdog::report_style_t values:
 * - tdog::RS_NONE - Does nothing here
 * - tdog::RS_TEXT_SUMMARY - Human readable test summary
 * - tdog::RS_TEXT_VERBOSE - Human readable verbose test report
 * - tdog::RS_HTML_SUMMARY - HTML summary report
 * - tdog::RS_HTML_VERBOSE - HTML verbose report
 * - tdog::RS_XML - JUnit compatible XML report
 * .
 * Non-verbose reports contain only summary information pertaining to tests which pass,
 * but further details are provided for those tests which fail. Verbose reports, on the
 * other hand, contain maximum trace information. For verbose XML, the report will
 * contain test case output information within the "<system-out>" element. HTML reports
 * are suitable for viewing in a browser.
 *
 * By default, no additional reports are written to file.
 *
 * Here's an example of where the default reporting to STDOUT is silenced,
 * but verbose TEXT, HTML and XML reports are written to file after the test run:
 *
 * \code
 * #include <tdog.hpp>
 * ...
 *
 * int main(int argc, char **argv)
 * {
 *   // Set default STDOUT report
 *   tdog::runner::global().set_default_report(tdog::RS_NONE);
 *
 *   // Add HTML report
 *   tdog::runner::global().add_report(tdog::RS_TEXT_VERBOSE, "./test-report.txt");
 *
 *   // Add HTML report
 *   tdog::runner::global().add_report(tdog::RS_HTML_VERBOSE, "./test-report.html");
 *
 *   // Add XML report
 *   tdog::runner::global().add_report(tdog::RS_XML, "./test-report.xml");
 *
 *   // Run all tests
 *   return tdog::runner::global().run();
 * }
 * \endcode
 *
 * At the end of the test run, the generate_report() method will
 * be called automatically to perform the actual report generation.
 * If the style RS_NONE is used, this method does nothing.
 * \param[in] style Report style
 * \param[out] filename The output filename
 * \sa set_default_report(), generate_report()
 */
  void add_report(report_style_t style, const std::string& filename);

/**
 * \brief Returns the test report character set encoding.
 * \details This is a short string specifying a valid character set encoding
 * for use in generating HTML test reports. The value supplied is
 * used simply to populate the "charset" field of HTML documents.
 * Valid examples include, "utf-8" and "iso-8859-1".
 *
 * The default value is an empty string which leaves the field unspecified.
 * \return Report character set encoding
 * \sa set_report_charset(), html_stylesheet(), default_report()
 */
  std::string report_charset() const;

/**
 * \brief Sets the test report character set encoding.
 * \details This is a short string specifying a valid character set encoding
 * for use in generating HTML test reports. The value supplied is
 * used simply to populate the "charset" field of HTML documents.
 * Valid examples include, "utf-8" and "iso-8859-1".
 *
 * The default value is an empty string which leaves the field unspecified.
 * \param[in] cs Report character set encoding
 * \sa report_charset(), set_html_stylesheet(), set_default_report()
 */
  void set_report_charset(const std::string& cs);

/**
 * \brief Returns the number of "-" characters used to create a
 * "breaker line" in the text reporting style.
 * \details A breaker line is a sequence of "-" characters used to create
 * a visual separator in text report outputs. This setting applies only
 * to the RS_TEXT_SUMMARY and RS_TEXT_VERBOSE report styles.
 *
 * A value of 0 will disable the appearence of breaker lines in the output.
 * \return Number of characters
 * \sa set_text_report_break_width(), default_report()
 */
  int text_report_break_width() const;

/**
 * \brief Sets the number of "-" characters used to create a
 * "breaker line" in the text reporting style.
 * \details A breaker line is a sequence of "-" characters used to create
 * a visual separator in text report outputs. This setting applies only
 * to the RS_TEXT_SUMMARY and RS_TEXT_VERBOSE report styles.
 *
 * A value of 0 will disable the appearence of breaker lines in the output.
 * \param[in] bw Break width (num of chars)
 * \sa text_report_break_width(), set_default_report()
 */
  void set_text_report_break_width(int bw);

/**
 * \brief Gets the stylesheet name for HTML test reports.
 * \details The field is used to populate the stylesheet field when
 * generating HTML test reports. It is not used for other reports.
 *
 * The default value is an empty string which means that HTML reports do
 * not use a stylesheet.
 * \return Stylesheet name
 * \sa set_html_stylesheet(), report_charset(), default_report()
 */
  std::string html_stylesheet() const;

/**
 * \brief Sets the external stylesheet filename for HTML test reports.
 * \details The field is used to populate the stylesheet field when
 * generating HTML test reports. It is not used for other reports.
 *
 * Example:
 *
 * \code
 * // Supply the path of a local stylesheet
 * tdog::runner::global().set_stylesheet("./styles.css");
 * \endcode
 *
 * This can be used to override the document's styles.
 *
 * The default value is an empty string which means that HTML reports do
 * not use an external stylesheet.
 * \param[in] ss Stylesheet filename
 * \sa html_stylesheet(), set_report_charset(), set_default_report()
 */
  void set_html_stylesheet(const std::string& ss);

/**
 * @}
 * @name Accessing statistics
 * @{
 */

/**
 * \brief Returns a statistical result for the test case, suite or entire test run.
 * \details This method should be called after the executing the tests, and
 * the result will pertain to the following statistic type:
 * - tdog::CNT_TOTAL - Total number of registered tests matching the name
 * - tdog::CNT_RAN - Number of test cases executed
 * - tdog::CNT_PASSED - Number of tests which passed (inc. warnings)
 * - tdog::CNT_FAILED - Number of test cases which failed
 * - tdog::CNT_ERRORS - Number of test cases with implementation errors
 * - tdog::CNT_SKIPPED - Number of test cases skipped (exc. disabled)
 * - tdog::CNT_DISABLED - Number of disabled test cases (exc. skipped)
 * - tdog::CNT_WARNINGS - Number of tests with warnings
 * - tdog::CNT_PASS_RATE - Percentage of tests which passed
 * - tdog::CNT_ASSERT_TOTAL - Total number of assert statements
 * - tdog::CNT_ASSERT_FAILED - Number of assert failures
 * - tdog::CNT_DURATION - Duration in milliseconds
 * .
 * Either a test case or suite name may be specified, and will match tests
 * according to the following rules:
 *
 * - To match a single test, provide the fully qualified test name exactly
 * (i.e. "SUITE1::SUITE2::TESTNAME").
 * - To match all tests directly in suite, but not in any of its sub-suites,
 * use the suite name suffixed with "::". For example, "SUITE1::".
 * - To match all tests in a suite, including its sub-suites, suffix the name
 * input with "::*". For example, "SUITE1::*".
 * - To match everything, use "*" (or "::*").
 * .
 *
 * Note that this routine does not support multiple name input. Provide only a
 * single name according to the rules above.
 *
 * Example:
 *
 * \code
 * // Get count of all passed tests
 * std::int64_t pass = TDOG_STATISTIC_COUNT(tdog::CNT_PASSED, "*");
 * \endcode
 *
 * Note also that the "ASSERT" result types pertain explicitly to counts of
 * TDOG_ASSERT() statements (and variants of). They are not incremented, for
 * example, if the test fails because of a timeout error, or if TDOG_TEST_FAIL()
 * is called.
 *
 * All return values will be zero if no test run was performed prior to calling
 * this method, otherwise they will be an applicable count value or duration.
 * \param[in] type Required statistic of type
 * \param[in] name Test case or suite name, or "*" for all
 * \return Statistical value
 * \sa test_status()
 */
  tdog::i64_t statistic_count(stat_count_t type, const std::string& name = "*") const;

/**
 * \brief Returns the state of a given test case.
 * \details Given a fully qualified test name, this method returns one of the
 * following values:
 * - tdog::TS_NOT_EXIST - Test case does not exist
 * - tdog::TS_READY - Test is ready to run
 * - tdog::TS_PASS_OK - Test passed OK
 * - tdog::TS_PASS_WARN - Test passed with warning
 * - tdog::TS_FAILED - Test failed
 * - tdog::TS_ERROR - Test failed because of a possible test case implementation error
 * - tdog::TS_SKIPPED - Test is enabled, but was skipped because by the author
 * - tdog::TS_DISABLED - Test is disabled
 * - tdog::TS_DECL_ERROR - Test run aborted because of test declaration errors
 * .
 * If the test case was declared within a test suite, if should be qualified
 * with the suite name using the "::" as the separator. If a test case is not
 * part of a suite (i.e. it's in the default suite), then the name may optionally
 * be prefixed with the default namespace "::".
 *
 * This method pertains to individual test cases only, and will not return data
 * for suites. Test names are case sensitive, and multiple names are not
 * supported.
 * \param[in] name Test case name
 * \return Test result state
 * \sa statistic_count()
 */
  status_t test_status(const std::string& name) const;

/**
 * \brief Returns the time when the last test run started.
 * \details The result is a signed 64 bit integer, in milliseconds since 1970.
 *
 * If no tests have been run, the return value is zero.
 * \return Test run start time in ms since 1970
 * \sa end_time(), duration()
 */
  tdog::i64_t start_time() const;

/**
 * \brief Returns the date/time when the last test run completed.
 * \details The result is a signed 64 bit integer, in milliseconds since 1970.
 *
 * If no tests have been run, the return value is zero.
 * \return Test run end time in ms since 1970
 * \sa start_time(), duration()
 */
  tdog::i64_t end_time() const;

/**
 * \brief Returns the total duration of the last test run in milli seconds.
 * \details The result is a signed 64 bit integer.
 *
 * If no tests have been run, the return value is -1, rather than zero.
 * \return Duration of the test run in milliseconds
 * \sa start_time(), end_time()
 */
  tdog::i64_t duration() const;

/**
 * @}
 */

};

} // namespace

//---------------------------------------------------------------------------
#endif // HEADER GUARD
//---------------------------------------------------------------------------
